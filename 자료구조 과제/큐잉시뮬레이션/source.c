#include<stdio.h>
#include<stdlib.h>

#define MAX_QUEUE_SIZE 20

typedef struct {
	int id;
	int arrival_time;
	int service_time;
}element;

typedef struct {
	element data[MAX_QUEUE_SIZE];
	int front, rear;
}QueueType;

void error(char* message)
{
	printf("%s\n", message);
	exit(1);
}

void init_queue(QueueType* q)
{
	q->front = q->rear = 0;
}

int is_empty(QueueType* q)
{
	return (q->front == q->rear);
}

int is_full(QueueType* q)
{
	return ((q->rear + 1) % MAX_QUEUE_SIZE == q->front);
}

void enqueue(QueueType* q, element item)
{
	if (is_full(q)) 
		error("queue full\n");
	q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
	q->data[q->rear] = item;
}

element dequeue(QueueType* q)
{
	if (is_empty(q))
		error("queue empty\n");
	q->front = (q->front + 1) % MAX_QUEUE_SIZE;
	return q->data[q->front];
}

int main()
{
	int minutes = 60;
	int total_wait = 0;
	int total_customers = 0;

	// 은행원 2명, 대기열 1개이기때문에 은행원 1,2번이
	// 상대해야하는 고객과 대응 시간을 변수 2개씩으로 잡았다.
	// 나머지 조건은 기존과 동일

	//1번 창구로 가는 고객에 대한 변수
	int service_time1 = 0;
	int service_customer1;

	//2번 창구로 가는 고객에 대한 변수
	int service_time2 = 0;
	int service_customer2;

	QueueType queue;
	init_queue(&queue);

	srand(time(NULL));

	for (int clock = 0; clock < minutes; clock++)
	{
		printf("current time:%d\n", clock);
		if ((rand() % 10) < 3)
		{
			element customer;
			customer.id = total_customers++;
			customer.arrival_time = clock;
			customer.service_time = rand() % 3 + 1;
			enqueue(&queue, customer);
			printf("customer: %d, arrival time: %d, service time: %d\n",
				customer.id, customer.arrival_time, customer.service_time);
		}

		if (service_time1 > 0 && service_time2 > 0) //창구 1,2번 둘 다 업무 중
		{
			printf("All counters are full\n");
			printf("servicing customer: %d, %d\n", service_customer1, service_customer2);
			service_time1--;
			service_time2--;
		}
		else if (service_time1 == 0 && service_time2 > 0) //창구 2번만 업무 중
		{
			if (!is_empty(&queue))
			{
				printf("counter 2: servicing customer:%d\n", service_customer2);
				printf("counter 1 is free\n");
				element customer = dequeue(&queue);
				service_customer1 = customer.id;
				service_time1 = customer.service_time;
				printf("customer: %d, start time: %d, wait time: %d\n",
					customer.id, clock, clock - customer.arrival_time);
				total_wait += clock - customer.arrival_time;
				service_time2--;
			}
		}
		else if (service_time1 > 0 && service_time2 == 0) // 창구 1번만 업무 중
		{
			if (!is_empty(&queue))
			{
				printf("counter 1: servicing customer:%d\n", service_customer1);
				printf("counter 2 is free\n");
				element customer = dequeue(&queue);
				service_customer2 = customer.id;
				service_time2 = customer.service_time;
				printf("customer: %d, start time: %d, wait time: %d\n",
					customer.id, clock, clock - customer.arrival_time);
				total_wait += clock - customer.arrival_time;
				service_time1--;
			}
		}
		else // 창구 1,2번 모두 업무 중이 아닐 때
		{
			if (!is_empty(&queue))
			{
				printf("counters are free\n");
				element customer = dequeue(&queue);
				if (rand() % 2 == 0) // 랜덤값이 짝수면 1번 창구로
				{
					service_customer1 = customer.id;
					service_time1 = customer.service_time;
					printf("custoer: %d, start time: %d, wait time: %d\n",
						customer.id, clock, clock - customer.arrival_time);
					total_wait += clock - customer.arrival_time;
				}
				else // 랜던값이 홀수면 2번 창구로
				{
					service_customer2 = customer.id;
					service_time2 = customer.service_time;
					printf("custoer: %d, start time: %d, wait time: %d\n",
						customer.id, clock, clock - customer.arrival_time);
					total_wait += clock - customer.arrival_time;
				}
			}
		}
	}

	printf("total wait time: %d", total_wait);
	return 0;
}