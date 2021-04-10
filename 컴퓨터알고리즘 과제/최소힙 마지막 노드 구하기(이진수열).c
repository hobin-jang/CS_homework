#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAX_SIZE 100

// 힙 구조체
typedef struct
{
    int heap[MAX_SIZE];
    int heap_size;
}HeapType;

// 스택 구조체
typedef struct
{
    int stack[MAX_SIZE];
    int top;
}Stack;

// 힙 초기화, 첫번째 index는 1
void init(HeapType* h)
{
    h->heap_size = 0;
}

// 최소힙
void upHeap(HeapType* h) 
{
    int i = h->heap_size;
    int key = h->heap[i];

    // 루트 아니고 부모보다 key 작은 동안 위로 올라감
    while ((i != 1) && (key < h->heap[i / 2]))
    {
        h->heap[i] = h->heap[i / 2];
        i /= 2;
    }
    h->heap[i] = key;
}

// key 삽입
void insertItem(HeapType* h, int key) 
{
    h->heap_size += 1;
    h->heap[h->heap_size] = key;
    upHeap(h);
}

// 힙 출력
void printHeap(HeapType* h)
{
    for (int i = 1; i <= h->heap_size; i++)
        printf("[%d] ", h->heap[i]);
    printf("\n");
}

// 스택 초기화, 첫 번째 index는 1
void init_Stack(Stack* st)
{
    st->top = 0;
}

// 스택 empty 확인, 비었으면 0, 아니면 1
int isEmpty(Stack* st)
{
    if (st->top == 0)
        return 0;
    else
        return 1;
}

// 스택에 push
void push(Stack* st, int item)
{
    st->top += 1;
    st->stack[st->top] = item;
}

// 스택에서 pop
int pop(Stack* st)
{
    // 스택에서 pop할 것이 없을 때
    if (st->top == 0) {
        fprintf(stderr, "stack is empty\n");
        exit(1);
    }

    else {
        int item;
        item = st->stack[st->top];
        st->top -= 1;
        return item;
    }
}

// i를 이진수열로 표현
void binaryExpansion(int i, Stack* st)
{
    while (i >= 2) {
        push(st, i % 2);
        i /= 2;
    }
    push(st, i);
    return;
}

// 힙의 마지막 노드 찾기
int findLastNode(HeapType* h)
{
    Stack st;
    init_Stack(&st);
    int v = h->heap[1]; // 루트 노드
    int n = h->heap_size; // 힙의 크기
    int idx = 1; // 첫번째 힙부터 탐색
    
    binaryExpansion(n, &st);
    int bit = pop(&st);

    while (isEmpty(&st) != 0) {
        bit = pop(&st);

        // left child
        if (bit == 0)
        {
            v = h->heap[2 * idx];
            idx *= 2;
        }

        // right child
        else
        {
            v = h->heap[2 * idx + 1];
            idx = 2 * idx + 1;
        }
    }

    return v;
}

int main()
{
    HeapType h;
    init(&h);
    srand(time(NULL));
    
    int num; // 힙 원소 개수
    printf("힙 원소 개수를 입력하세요 (최대 100개) : ");
    scanf("%d", &num);
    printf("\n");

    // 힙에 random값 삽입 (0~99)
    for (int i = 1; i <= num; i++) {
        int key = rand() % 100;
        insertItem(&h, key);
    }

    // 힙 출력
    printf("힙 출력\n");
    printHeap(&h);

    // 힙의 마지막 노드 출력
    int lastnode = findLastNode(&h);
    printf("\n마지막 노드 : [%d]\n", lastnode);
}
