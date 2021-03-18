#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

#define	MAX_CACHE_SIZE		8192

// 데이터 참조 스트림이 저장된 파일 
#define TRACE_FILE_NAME		"ref_stream.txt"


// 이중연결리스트의 노드 타입
struct buffer {
	unsigned long blkno;
	struct buffer* next, * prev;
	struct buffer* hash_next, * hash_prev;
};

// 데이터를 저장할 공간을 한 번에 할당 받음
// 이후 추가적인 메모리 할당은 없음
// 할당 받은 노드들을 이용하여 LRU 리스트 또는 FIFO 리스트를 유지해야 함 
struct buffer cache_buffer[MAX_CACHE_SIZE];

// LRU 시뮬레이터 인 경우 lrulist 아니면 fifolist를 유지하기 위한 헤드 노드 선언
#if 1
struct buffer lrulist;
#else
struct buffer fifolist;
#endif

long curtime, hit, miss;

//LRU에서 buffer *bp를 삽입
void lruin(struct buffer* bp)
{
	struct buffer* dp = &lrulist;
	bp->next = dp->next;
	bp->prev = dp;
	(dp->next)->prev = bp;
	dp->next = bp;
}

//가장 덜 참조된 것 삭제
struct buffer* lruout()
{
	struct buffer* bp;
	bp = lrulist.prev;
	(bp->prev)->next = bp->next;
	(bp->next)->prev = bp->prev;

	return bp;
}

//bp 업데이트
void reorder(struct buffer* bp)
{
	(bp->prev)->next = bp->next;
	(bp->next)->prev = bp->prev;

	lruin(bp);
}

// LRU 초기화
void init()
{
	int i;

	lrulist.next = lrulist.prev = &lrulist;

	for (i = 0; i < MAX_CACHE_SIZE; i++)
	{
		cache_buffer[i].blkno = ~0;
		cache_buffer[i].next = cache_buffer[i].prev = NULL;

		lruin(&cache_buffer[i]);
	}

	return;
}

//LRU 이용하여 blkno 탐색 (기존 방법)
struct buffer* findblk(unsigned long blkno) {
	struct buffer* dp, * bp;

	dp = &lrulist;
	for (bp = dp->next; bp != dp; bp = bp->next)
	{
		if (bp->blkno == blkno)
			return bp;
	}

	return NULL;
}

// blkno에 관한 것 업데이트 
void pgref(unsigned long blkno)
{
	struct buffer* bufp;

	bufp = findblk(blkno);
	if (bufp)
	{
		hit++;

		// 참조되었으므로 빼서 새로 삽입
		reorder(bufp);
	}
	else
	{
		miss++;

		// 없으므로 blkno 삽입
		bufp = lruout();
		bufp->blkno = blkno;
		lruin(bufp);
	}
}



// hash_table
// HASH_SIZE = MAX_CACHE_SIZE * 0.75 = 6144
#define HASH_SIZE	6144 
struct buffer hash_table[HASH_SIZE];

// blkno의 hash 값
int Hash(unsigned long blkno)
{
	unsigned long mod = HASH_SIZE;
	return (int)(blkno % mod);
}

//hash table[hash]에 buffer *bp를 삽입
void hash_in(struct buffer* bp, unsigned long blkno)
{
	int hash = Hash(blkno);
	struct buffer* dp = &hash_table[hash];
	
	bp->hash_next = dp->hash_next;
	bp->hash_prev = dp;
	(dp->hash_next)->hash_prev = bp;
	dp->hash_next = bp;

}

//가장 덜 참조된 것 삭제
void hash_out(struct buffer * bp)
{
	(bp->hash_prev)->hash_next = bp->hash_next;
	(bp->hash_next)->hash_prev = bp->hash_prev;
}

//bp 업데이트
void hash_reorder(struct buffer* bp, unsigned long blkno)
{
	(bp->hash_prev)->hash_next = bp->hash_next;
	(bp->hash_next)->hash_prev = bp->hash_prev;

	hash_in(bp, blkno);
}

// hash table 초기화
void init_hash()
{
	for (int i = 0; i < HASH_SIZE; i++)
	{
		hash_table[i].blkno = ~0;
		hash_table[i].hash_next = hash_table[i].hash_prev = &hash_table[i];
	}

	for (int i = 0; i < MAX_CACHE_SIZE; i++)
	{
		cache_buffer[i].hash_next = cache_buffer[i].hash_prev = NULL;
		hash_in(&cache_buffer[i], cache_buffer[i].blkno);
	}
	return;
}

// hash 이용해 탐색
struct buffer* hash_findblk(unsigned long blkno)
{
	int hash = Hash(blkno);
	struct buffer* dp, * bp;
	
	dp = &hash_table[hash];

	for (bp = dp->hash_next; bp != dp; bp = bp->hash_next) {
		if (bp->blkno == blkno)
			return bp;
	}

	return NULL;
}

// blkno에 대한 것 업데이트
void hash_pgref(unsigned long blkno)
{
	struct buffer* bufp;

	bufp = hash_findblk(blkno);
	if (bufp)
	{
		hit++;

		// hit 발생했으므로 cache_buffer와 hash_table 업데이트

		// cache_buffer에서 순서 변경
		reorder(bufp);

		// hash_table에서 순서 변경
		hash_reorder(bufp, blkno);
	}
	else
	{
		miss++;

		// cache_buffer에 일치하는 값 없으므로 blkno 삽입 과정

		// cache_buffer에서 맨 끝 노드 삭제
		bufp = lruout();

		// hash_table에서 해당 노드 삭제
		hash_out(bufp);

		// cache_buffer와 hash_table에 blkno 삽입
		bufp->blkno = blkno;
		lruin(bufp);
		hash_in(bufp, blkno);
	}
}

int main(int argc, char* argv[])
{
	int	ret;
	unsigned long blkno;
	FILE* fp = NULL;

	time_t start, end;
	
	init();

	curtime = hit = miss = 0;

	if ((fp = fopen(TRACE_FILE_NAME, "r")) == NULL) {
		printf("%s trace file open fail.\n", TRACE_FILE_NAME);

		return 0;
	}
	else {
		printf("start simulation!\n");
	}

	////////////////////////////////////////////////
	// 시뮬레이션 시작 및 결과 출력을 위한 코드 (기존 방법)
	////////////////////////////////////////////////
	
	long time1; // LRU 이용했을 때 수행시간
	
	// 기존의 방법으로 LRU 이용했을 때 시간 측정 시작
	start = time(NULL);
	while ((ret = fscanf(fp, "%lu\n", &blkno)) != EOF) {
		curtime++;
		pgref(blkno);
	}
	fclose(fp);
	end = time(NULL);
	// 시간 측정 끝
	time1 = (end - start);

	printf("time using LRU = %lu s\n", time1);
	printf("total access = %lu, hit = %lu, miss = %lu\n", curtime, hit, miss);
	printf("hit ratio = %f, miss ratio = %f\n", (float)hit / (float)curtime, (float)miss / (float)curtime);

	//////////////////////////////////////////////////////
	// hash 이용해 시간 측정 
	//////////////////////////////////////////////////////

	init();
	init_hash();

	curtime = hit = miss = 0;
	
	if ((fp = fopen(TRACE_FILE_NAME, "r")) == NULL) {
		printf("%s trace file open fail.\n", TRACE_FILE_NAME);

		return 0;
	}
	else {
		printf("start simulation!\n");
	}
	
	long time2; // hash table 이용했을 때 수행시간

	// hash table 이용했을 때 시간 측정 시작
	start = time(NULL);
	while ((ret = fscanf(fp, "%lu\n", &blkno)) != EOF) {
		curtime++;
		hash_pgref(blkno);
	}
	fclose(fp);
	end = time(NULL);
	// 시간 측정 끝
	time2 = (end - start);

	printf("time using hash table = %lu s\n", time2);
	printf("total access = %lu, hit = %lu, miss = %lu\n", curtime, hit, miss);
	printf("hit ratio = %f, miss ratio = %f\n", (float)hit / (float)curtime, (float)miss / (float)curtime);
	return 0;
}

