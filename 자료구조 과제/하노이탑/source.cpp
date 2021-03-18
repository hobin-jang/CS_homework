#include <stdio.h>
#include<stdlib.h>
#include<memory.h>
int main()
{
	int idx, n; // idx: 원판 번호, n: 탑 높이
	printf("탑의 높이 :");
	scanf_s("%d", &n);

	int* disk = (int *)malloc(sizeof(int) * n);  // 원판 배열
	char* loc = (char*)malloc(sizeof(char) * n); // 원판의 위치(봉) 배열
	char pos; // 옮길 원판의 현재 위치

	memset(disk, 1, sizeof(int) * n); // 원판의 값을 1로 초기화
	memset(loc, 'A', sizeof(char) * n); //원판이 현재 모두 'A'에 있다

	idx = 0; // 0번째 원판 (원판1) 부터 옮긴다

	// 원판 값 1: 옮겨야할 것, 0: 옮겨진 것
	while (1)
	{
		// 원판이 옮겨져있으면 다음 원판을 옮길 대상으로 정한다.
		while (idx < n && disk[idx] == 0)
			idx+=1;

		// 모두 옮겼으면 break
		if (idx == n)
			break;

		// 현재 위치 저장
		pos = loc[idx];

		// 3개의 봉 'A','B','C'에 대해 원판은 바로 아래 원판과
		// 같은 봉으로 옮길 수 없다.
		// idx+n 값이 홀수일 때 A=>C=>B=>A, 짝수일 때 A=>B=>C=>A 순으로 옮기면 된다.
		// 봉이 총 3개이므로, mod 3으로 옮길 위치를 구해준다.

		int tmp = (idx + n) % 2 + loc[idx] - 'A' + 1;
		loc[idx] = tmp % 3 + 'A';
		printf("원판 %d를 %c에서 %c로 옮긴다. \n", idx + 1, pos, loc[idx]);

		// 이동한 원판은 0, 이전 원판으로 이동
		disk[idx] = 0;
		idx -= 1;

		// 옮겨진 원판은 다시 옮겨야하므로 값을 1로 만든다.
		while (idx >= 0 && disk[idx] == 0)
		{
			disk[idx] = 1;
			idx -= 1;
		}

		// idx가 음수면 안된다
		if (idx < 0)
			idx = 0;
	}
}