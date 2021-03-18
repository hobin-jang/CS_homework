#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	MAX_CACHE_SIZE		8192

// 데이터 참조 스트림이 저장된 파일 
#define TRACE_FILE_NAME		"ref_stream.txt"


// 이중연결리스트의 노드 타입
struct buffer {
	unsigned long blkno;
	struct buffer* next, * prev;
};

// 데이터를 저장할 공간을 한 번에 할당 받음
// 이후 추가적인 메모리 할당은 없음
// 할당 받은 노드들을 이용하여 LRU 리스트 또는 FIFO 리스트를 유지해야 함 
struct buffer cache_buffer[MAX_CACHE_SIZE];

// LRU 시뮬레이터 인 경우 lrulist 아니면 fifolist를 유지하기 위한 헤드 노드 선언

struct buffer lrulist;

//cache_buffer 초기화
void init()
{
	// 주어진 buffer lrulist를 이용하여 초기화
	(&lrulist)->blkno = NULL;
	(&lrulist)->next = NULL;
	(&lrulist)->prev = NULL;
	*cache_buffer = lrulist;
	cache_buffer->next = cache_buffer;
	cache_buffer->prev = cache_buffer;
}

//cache_buffer에 data insert
void dinsert(unsigned long data)
{
	struct buffer* newnode;
	newnode = (struct buffer*)malloc(sizeof(struct buffer));

	// 새로 삽입될 노드의 포인터 변경
	newnode->blkno = data;
	newnode->prev = cache_buffer;
	newnode->next = cache_buffer->next;

	// 새로 삽입되는 노드를 가리키는 두 포인터 변경
	cache_buffer->next->prev = newnode;
	cache_buffer->next = newnode;
}

void ddelete(struct buffer* removed)
{
	if (removed == cache_buffer)
		return;

	// 삭제될 노드를 가리키고 있던 두 포인터 변경
	removed->prev->next = removed->next;
	removed->next->prev = removed->prev;

	free(removed);
}

struct buffer* search(unsigned long data)
{
	struct buffer* p;

	// 특정 값 가지고 있는 노드 검색하여 해당 노드의 포인터 반환
	for (p = cache_buffer->next; p != cache_buffer; p = p->next)
	{
		if (p->blkno == data)
			return p;
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	// ret: hit 횟수, total: 전체 data 수, len: MAX_CACHE_SIZE 확인용
	int	ret, total, len;
	unsigned long blkno;
	FILE* fp = NULL;

	init();

	if ((fp = fopen(TRACE_FILE_NAME, "r")) == NULL) {
		printf("%s trace file open fail.\n", TRACE_FILE_NAME);

		return 0;
	}
	else {
		printf("start simulation!\n");
	}

	////////////////////////////////////////////////
	// 시뮬레이션 시작 및 결과 출력을 위한 코드
	////////////////////////////////////////////////

	ret = 0;
	total = 0;
	len = 0;

	// ref_stream에 저장된 값들을 한 줄씩 읽기
	if (fp != NULL) {
		char str[128]; // 한 줄씩 불러올 때 string 최대 길이를 128로 잡았다.
		char* stop;

		while (!feof(fp)) {

			fgets(str, sizeof(str), fp);
			blkno = strtoul(str, &stop, 10); // string을 10진수 unsigned long으로 변환

			if (search(blkno) != NULL) { // blkno 값 메모리에 존재하는 경우 (hit 발생)

				// LRU 원리에 의해 hit 발생하는 경우 메모리가 꽉 찬 상태인지와 상관없이 
				// 해당 노드를 가장 최신 노드로 업데이트하므로 전체 노드 수는 변화가 없다.
				// blkno값 가지는 노드를 기존 삭제 후 가장 최신으로 옮긴다.
				ret++;
				total++;
				ddelete(search(blkno));
				dinsert(blkno);
			}
			else { // hit 발생하지 않은 경우

				if (len < MAX_CACHE_SIZE) { //cache_buffer 메모리 남은 경우

					// hit발생하지 않았지만 blkno 값을 가지는 노드가 들어갈 메모리가 남았으므로
					// len, total을 +1하고, 해당 노드를 insert한다.
					len++;
					total++;
					dinsert(blkno);
				}

				else { //cache_buffer 메모리 다 찬 경우

					// LRU 원리에 의해 head에 가장 가까운 것이 가장 덜 참조된 노드이므로 
					// 이를 제거 후 blkno 값을 가지는 노드를 insert한다.
					total++;
					ddelete(cache_buffer->prev);
					dinsert(blkno);
				}
			}
		}
		fclose(fp);
	}

	// len = MAX_CACHE_SIZE 인 지점부터 메모리가 다 찬 상황이다. 
	// len < MAX_CACHE_SIZE 일 때, blkno가 참조된 값이라도 해당 노드를 삭제 후 삽입하는 것이므로
	// 값이 존재하는 메모리에는 변화가 없으므로
	// hit가 발생하지 않은 경우에 대해서만 len을 이용하여 구분하였다.

	double ratio = (double)ret / total;

	printf("LRU hit count : %d\n", ret);
	printf("total count : %d\n", total);
	printf("LRU cache hit ratio : %.2f %%", ratio * 100);

	return 0;
}

