#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
// 에러 처리
void error(char* message)
{
	printf("%s\n", message);
	exit(1);
}
// 리스트 생성
void buildList(int Arr[], int n)
{
	for (int i = 0; i < n; i++)
		Arr[i] = i + 1;
}

// r번 원소 제거
void Remove(int Arr[], int n, int r)
{
	if ((r < 0) || (r > n - 1))
		error("올바르지 않은 범위입니다.\n");
	for (int i = r + 1; i < n; i++)
		Arr[i - 1] = Arr[i];
}

// 계산 실행
int runSimulation(int Arr[], int n, int k)
{
	int r = 0;
	while (n > 1) // 1개 남을 때까지 k번 건너뛴 양초 제거
	{
		r = (r + k) % n;
		Remove(Arr, n, r);
		n--;
	}
	return Arr[0];
}
int candle(int Arr[], int n, int k)
{
	buildList(Arr, n);
	return runSimulation(Arr, n, k);
}
void main()
{
	int time;
	printf("실행 횟수를 입력하세요. ");
	scanf("%d", &time);
	while (time--)
	{
		int* Arr;
		int n, k;
		printf("n과 k를 입력하세요.\n");
		printf("n = ");
		scanf("%d", &n);
		printf("k = ");
		scanf("%d", &k);
		if (n <= 0 || k <= 0)
			error("올바르지 않은 값입니다.\n");
		Arr = (int*)malloc(sizeof(int) * n);
		printf("[%d]번째 양초가 정답입니다.\n", candle(Arr, n, k));
		free(Arr);
		printf("=============실행 완료==============\n\n");
	}
}