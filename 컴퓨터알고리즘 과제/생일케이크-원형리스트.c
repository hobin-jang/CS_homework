#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

// 원형연결리스트 노드
typedef struct ListNode
{
	int elem;
	struct ListNode* next;
}ListNode;

// 노드 추가
ListNode* getnode()
{
	ListNode* node = (ListNode*)malloc(sizeof(ListNode));
	return node;
}

// 노드 삭제
void putnode(ListNode* L)
{
	free(L);
}

// 원형연결리스트 출력
void printNodes(ListNode* L)
{
	ListNode* first = L;
	printf("->");
	while (first != L->next)
	{
		printf("[%d]->", L->elem);
		L = L->next;
	}
	printf("[%d]->", L->elem);
	printf("\n");
}

// 전체리스트 만들기
ListNode* buildList(int n)
{
	ListNode* p = getnode();
	ListNode* L = p;
	p->elem = 1;
	for (int i = 2; i <= n; i++)
	{
		p->next = getnode();
		p = p->next;
		p->elem = i;
	}
	p->next = L;
	return L;
}

// 계산 실행
int runSimulation(ListNode* L, int n, int k)
{
	ListNode* p = L;
	printNodes(p);

	while (p != p->next) // 노드 1개 남을 때까지 반복
	{
		for (int i = 1; i < k; i++)
			p = p->next;
		ListNode* pnext = p->next;
		p->next = (p->next)->next;
		putnode(pnext);
		p = p->next;
		printNodes(p);
	}
	return p->elem;
}

int candle(int n, int k)
{
	ListNode* L = buildList(n);
	return runSimulation(L, n, k);
}

// 에러 처리
void error(char* message)
{
	printf("%s\n", message);
	exit(1);
}

void main()
{
	int time;
	int n, k;
	printf("몇 번 실행하시겠습니까? ");
	scanf("%d", &time);
	while (time--)
	{
		printf("n과 k를 입력하세요.\n");
		printf("n = ");
		scanf("%d", &n);
		printf("k = ");
		scanf("%d", &k);

		if (n <= 0 || k <= 0)
			error("올바르지 않은 값입니다.\n");

		printf("[%d]번째 양초가 정답입니다.\n", candle(n, k));
		printf("==========실행완료=========\n\n");
	}
}