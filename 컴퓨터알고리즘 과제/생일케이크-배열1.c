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

// 계산 실행
int runSimulation(int Arr[], int n, int k)
{
	int r = 0;
	int N = n; // modulo 연산을 위한 전체 개수 저장
	while (n > 1)
	{
		int i = 0; 
		while (i < k) // 남은 양초 중 k번 건너뛰기
		{
			r = (r + 1) % N;
			if (Arr[r] != 0)
				i++;
		}
		Arr[r] = 0; // 양초 끄기
		n--; // n : 남은 양초
		while (Arr[r] == 0) // 다음 시작점 구하기
			r = (r + 1) % N;
	}
	return Arr[r];
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