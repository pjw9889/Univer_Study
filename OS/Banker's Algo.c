#include <stdio.h>
#pragma warning(disable:4996)

int alloc[5][5], claim[5][5], avl[5]; // 할당행렬, 요구행렬, 가용 벡터 생성
int all_alloc[5] = { 0, 0, 0, 0, 0 }; // 할당된 총 자원의 수 측정
int resource[5], running[5], safe = 0; // 자원 벡터, 실행여부 판단 배열, 안전상태판단여부확인 변수 지정
int count = 0, i, j, exec; // 프로세스 개수 변수, 반복문 변수 i,j, 실행판단여부확인 변수, 
int process_number, resource_number; // 프로세스 개수, 자원 개수 입력 변수

									 // 프로세스 개수, 자원의 개수, 자원 벡터, 할당행렬, 요구행렬을 입력한다. 프로세스 개수, 자원 개수는 최대 5개
									 // 그리고 입력한 자원 벡터, 할당행렬, 요구행렬을 다시 출력한다.
									 // count가 0이 아니면, 즉 입력받은 프로세스의 개수가 0이 아니면 안전한 상태인지 판단한다.
int main()
{
	printf("프로세스의 개수를 입력하세요(최대 5개) : ");
	scanf("%d", &process_number);
	if (process_number > 5)
	{
		printf("5개를 초과하였습니다\n");
		return 0;
	}
	printf("\n");

	for (i = 0; i < process_number; i++) {
		running[i] = 1; // 입력받은 프로세스 개수만큼 running의 인덱스 값을 1로 바꾸어준다.
						// 이때 1이 입력된 인덱스 값만큼 사용되었다는 뜻이다.
		count++; // 프로세스의 개수만큼 count가 증가한다. 따라서 count는 process의 개수이다.
	}
	printf("자원의 개수를 입력하세요(최대 5개): ");
	scanf("%d", &resource_number);
	if (resource_number > 5)
	{
		printf("5개를 초과하였습니다\n");
		return 0;
	}
	printf("\n");

	printf("자원 벡터를 입력하세요:");
	for (i = 0; i < resource_number; i++) {
		scanf("%d", &resource[i]);
	}
	printf("\n");

	printf("할당 행렬을 입력하세요.\n");
	for (i = 0; i < process_number; i++) {
		for (j = 0; j < resource_number; j++) {
			scanf("%d", &alloc[i][j]);
		}
	}
	printf("\n");

	printf("요구행렬을 입력하세요.\n");
	for (i = 0; i < process_number; i++) {
		for (j = 0; j < resource_number; j++) {
			scanf("%d", &claim[i][j]);
		}
	}
	printf("\n");

	printf("입력한 자원벡터 : ");
	for (i = 0; i < resource_number; i++) {
		printf("\t%d", resource[i]);
	}
	printf("\n");

	printf("입력한 할당행렬 :\n");
	for (i = 0; i < process_number; i++) {
		for (j = 0; j < resource_number; j++) {
			printf("\t%d", alloc[i][j]);
		}

		printf("\n");
	}
	printf("\n");

	printf("입력한 요구행렬: \n");
	for (i = 0; i < process_number; i++) {
		for (j = 0; j < resource_number; j++) {
			printf("\t%d", claim[i][j]);
		}

		printf("\n");
	}
	printf("\n");

	for (i = 0; i < process_number; i++) {
		for (j = 0; j < resource_number; j++) {
			all_alloc[j] += alloc[i][j];
		}
	}

	printf("할당된 자원 :");
	for (i = 0; i < resource_number; i++) {
		printf("\t%d", all_alloc[i]);
	}
	printf("\n");

	for (i = 0; i < resource_number; i++) {
		avl[i] = resource[i] - all_alloc[i];
	}

	printf("가용 자원 :");
	for (i = 0; i < resource_number; i++) {
		printf("\t%d", avl[i]);
	}
	printf("\n");


	while (count != 0) {
		safe = 0;
		for (i = 0; i < process_number; i++) {
			if (running[i]) { // 입력받은 프로세스의 개수가 0이 아니면 참
				exec = 1; // 실행판단 변수의 값을 참으로 설정한다.
				for (j = 0; j < resource_number; j++) {
					if (claim[i][j] - alloc[i][j] > avl[j]) { // 요구행렬 - 할당행렬이 가용 자원보다 크면 멈춘다.
						exec = 0;
						break;
					}
				}
				if (exec) { // 실행판단 변수의 값이 참이면 즉, 위의 멈춤 조건에서 만족하지 못하고, 프로세스의 개수가 0 이 아니면
					printf("\n");
					running[i] = 0; // 현재 실행중인 프로세스의 값을 0으로 바꾼다. 즉 프로세스를 종료한다.
					count--; // 그리고 프로세스의 숫자를 하나 줄인다.
					safe = 1; // 위의 조건에서 멈추지 않았기 때문에 안전한 조건이므로 안전한 상태를 만든다.

					for (j = 0; j < resource_number; j++) {
						avl[j] += alloc[i][j]; // 그리고 종료된 프로세스의 자원은 반환한다.
					}

					break;
				}
			}
		}
		if (!safe) { // 안전한 상태가 아니면 반복문을 빠져나간다.
			printf("\n");
			printf("불안전한 상태입니다.\n");
			break;
		}
		else { // 안전한 상태이면 수행
			printf("안전한 상태입니다.\n");
			printf("Process%d에 필요한 자원을 제공하여 실행하였습니다.\n", i + 1);
			printf("Process%d가 종료 된 후 Process%d의 자원을 반납하였습니다.\n", i + 1, i + 1);
			printf("현재 가용 자원 :");

			for (i = 0; i < resource_number; i++) {
				printf("\t%d", avl[i]);
			}

			printf("\n");
			printf("\n");
		}
	}

	return 0;
}
