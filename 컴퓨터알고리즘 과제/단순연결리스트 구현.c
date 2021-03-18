#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

// 단순열결리스트의 노드
// data와 다음 노드를 가르키는 link로 구성
typedef struct ListNode {
    int data;
    struct ListNode* link;
}ListNode;

// 단순연결리스트의 head부분
// head를 이용하여 탐색 진행
typedef struct
{
    ListNode* head;
}LinkedListType;

// 처음 head는 NULL, 이후 추가와 삭제 진행
void init(LinkedListType* L)
{
    L->head = NULL;
}

// 에러 처리
void error(char* message)
{
    printf("%s\n", message);
    exit(1);
}

// 첫 번째 노드에 추가 
// node : 추가될 노드
void addFirst(LinkedListType* L, int item)
{
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->data = item;
    node->link = L->head;
    L->head = node;
}

// pos번째에 노드 추가
void add(LinkedListType* L, int pos, int item)
{
    // 범위를 벗어난 경우
    if (pos <= 0 || pos > SIZE)
        error("올바른 위치가 아닙니다.\n");

    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    ListNode* before = L->head;
    for (int i = 1; i < pos - 1; i++) {
        before = before->link;

        // 범위를 벗어난 경우
        if (before == NULL)
            error("올바른 위치가 아닙니다.\n");
    }

    node->data = item;
    node->link = before->link;
    before->link = node;
}

// 마지막에 노드 추가
void addLast(LinkedListType* L, int item)
{
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    ListNode* before = L->head;

    // 아직 리스트가 비어있을 경우 첫번째 위치에 추가
    if (before == NULL)
    {
        addFirst(L, item);
        return;
    }

    // before를 마지막 노드로 설정
    while (before->link != NULL)
        before = before->link;

    node->data = item;
    node->link = before->link;
    before->link = node;
}

// 첫 번째 노드 제거
// node : 제거될 노드
int RemoveFirst(LinkedListType* L)
{
    // 리스트가 비어있을 경우
    if (L->head == NULL)
        error("지울 노드가 없습니다.\n");

    ListNode* node = L->head;
    int item = node->data;
    L->head = node->link;
    free(node);
    return item;
}

// pos번째에 노드 제거
int Remove(LinkedListType* L, int pos)
{
    // 범위를 벗어난 경우
    if (pos <= 0 || pos > SIZE)
        error("올바른 위치가 아닙니다.\n");

    ListNode* node;
    ListNode* before = L->head;
    for (int i = 1; i < pos - 1; i++) {
        before = before->link;

        // 범위를 벗어난 경우
        if (before == NULL)
            error("올바른 위치가 아닙니다.\n");
    }
    node = before->link;
    int item = node->data;
    before->link = node->link;
    free(node);
    return item;
}

// 마지막 노드 제거
int RemoveLast(LinkedListType* L)
{
    // 리스트가 비어있을 경우
    if (L->head == NULL)
        error("지울 노드가 없습니다.\n");

    ListNode* node = L->head;
    ListNode* before = L->head;

    // 리스트에 노드 1개인 경우
    if (node->link == NULL)
        return RemoveFirst(L);

    // node를 마지막 노드로 설정
    while (node->link != NULL)
        node = node->link;
    
    // before를 마지막 노드의 직전 노드로 설정 
    while (before->link != node)
        before = before->link;

    int item = node->data;
    before->link = node->link;
    free(node);
    return item;
}

// pos번째 노드의 data 반환
int get(LinkedListType* L, int pos)
{
    ListNode* p = L->head;
    for (int i = 1; i < pos; i++) {
        p = p->link;

        // 범위를 벗어난 경우
        if (p == NULL)
            error("올바른 위치가 아닙니다.\n");
    }
    return p->data;
}

// pos번째 노드의 data를 item으로 갱신
void set(LinkedListType* L, int pos, int item)
{
    ListNode* p = L->head;
    for (int i = 1; i < pos; i++) {
        p = p->link;

        // 범위를 벗어난 경우
        if (p == NULL)
            error("올바른 위치가 아닙니다.\n");
    }
    p->data = item;
}

// 단순연결리스트 전체 출력
void printList(LinkedListType* L)
{
    for (ListNode* p = L->head; p != NULL; p = p->link)
        printf("[%d] -> ", p->data);
    printf("NULL\n");
}

void main()
{
    LinkedListType list;
    init(&list);

    int time; // 횟수
    int data; // 입력 데이터 값
    int pos; // 위치
    
    // 0번 노드가 첫 번째 노드
    printf("첫 번째 노드에 원소 추가\n");printf("실행 횟수 : ");scanf("%d", &time);
    while (time--)
    {
        printf("\n어떤 값을 넣으시겠습니까? ");scanf("%d", &data);
        addFirst(&list, data);
        printList(&list);
    }

    printf("\n=============완료==========\n\n");

    printf("특정 위치에 원소 추가\n");printf("실행 횟수 : ");scanf("%d", &time);
    while (time--)
    {
        printf("\n몇 번째 노드에 넣으시겠습니까? ");scanf("%d", &pos);
        printf("\n어떤 값을 넣으시겠습니까? ");scanf("%d", &data);
        
        if (pos == 1)
            addFirst(&list, data);
        else
            add(&list, pos, data);
        printList(&list);
    }

    printf("\n=============완료==========\n\n");

    printf("\n몇 번째 노드 값 반환하시겠습니까? ");scanf("%d", &pos);
    printf("%d번째 노드의 값 : %d\n", pos, get(&list, pos));

    printf("\n=============완료==========\n\n");

    printf("\n첫 번째 노드 제거\n");printf("[%d] 제거 완료\n", RemoveFirst(&list));
    printList(&list);

    printf("\n몇 번째 노드 제거하시겠습니까? ");scanf("%d", &pos);
    if (pos == 1)
        printf("\n%d번째 노드[%d] 제거 완료\n", pos, RemoveFirst(&list));
    else
        printf("\n%d번째 노드[%d] 제거 완료\n", pos, Remove(&list, pos));
    printList(&list);

    printf("\n=============완료==========\n\n");

    printf("마지막 노드 조절\n");printf("마지막 노드 제거\n");printf("[%d] 제거 완료\n", RemoveLast(&list));
    printList(&list);
    printf("마지막 노드에 추가\n");printf("어떤 값을 넣으시겠습니까? ");scanf("%d", &data);
    addLast(&list, data);
    printList(&list);

    printf("\n=============완료==========\n\n");
}
