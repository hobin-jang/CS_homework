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

struct buffer fifolist;

//cache_buffer 초기화
void init()
{
	// 전역으로 주어진 buffer fifolist를 이용하여 초기화 
	(&fifolist)->blkno = NULL;
	(&fifolist)->next = NULL;
	(&fifolist)->prev = NULL;
	*cache_buffer = fifolist;
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

	if (fp != NULL) {
		char str[128]; // 한 줄씩 불러올 때, string 길이를 넉넉하게 128로 잡았다.
		char* stop;

		while (!feof(fp)) {

			fgets(str, sizeof(str), fp);
			blkno = strtoul(str, &stop, 10); // string을 10진수 unsigned long으로 변환

			if (search(blkno) != NULL) { // blkno 값 메모리에 존재하는 경우 (hit 발생)
				
				// 메모리가 다 차있는지 여부와 상관없이 
				// fifo에서는 hit 발생 시 해당 data를 insert 하지 않아도 되므로
				// ret와 total 값만 +1한다.
				ret++;
				total++;
			}
			else { // hit 발생하지 않은 경우
				if (len < MAX_CACHE_SIZE) { //cache_buffer 메모리 남은 경우

					// hit가 발생하지 않았지만 메모리가 남았으므로
					// 해당 data insert와 len와 total을 +1 한다.
					len++;
					total++;
					dinsert(blkno);
				}
				else { //cache_buffer 메모리 다 찬 경우
					
					// hit 발생하지 않고 메모리 다 찬 경우
					// FIFO 이므로 head 부분 노드 제거 후 blkno 값 insert한다.
					total++;
					ddelete(cache_buffer->prev);
					dinsert(blkno);
				}
			}
		}
		fclose(fp);
	} 
	// len = MAX_CACHE_SIZE 인 지점부터 메모리가 다 찬 상황이다. 
	// 메모리가 꽉 찬 경우, 새로운 노드 삽입되더라도 맨 처음 삽입된 노드가 삭제되므로 
	// 전체 메모리 수에는 변화가 없으므로 len 값을 이용하여 구분하였다.

	double ratio = (double)ret / total;

	printf("FIFO hit count : %d\n", ret);
	printf("total count : %d\n", total);
	printf("FIFO cache hit ratio : %.2f %%", ratio * 100);

	return 0;
}

