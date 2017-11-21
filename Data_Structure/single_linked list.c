#include <stdio.h>
#include <stdlib.h>

struct listNode{
	int data;
	struct listNode *next;
};
typedef struct listNode ListNode; 
typedef struct listNode * ListNodePtr; 

void insert(ListNodePtr *, int); // insert 함수 원형
int delete_(ListNodePtr *, int); // delete 함수 원형
int isEmpty(ListNodePtr); // isEmpty 함수 원형
void printList(ListNodePtr); // printList 함수 원형
void instructions(void); // instructions 함수 원형

int main()
{
	ListNode * head = 0; // head 노드 선언 및 초기화
	
	int choice = 0; // case 선택 값 초기화
	int input, delete_key; // 삽입할 데이터 입력 값, 삭제할 데이터 입력 값 변수
	int a, b; // isEmpty 함수 결과값 반환, delete_ 함수 결과값 반환 변수
	
	instructions(); // instructions 함수 호출
	
	// choice 값을 초기화하였기 때문에, while 1번은 수행.
	while(choice !=3){ // choice가 3이면 while문 종료
		if(choice > 3) // choice 값이 3이상 입력될 경우 출력될 문장.
		{
			printf("1:입력 \n");
			printf("2:삭제 \n");
			printf("3:종료 \n");
		}
		else // choice가 3이하일 경우 수행
			printf("원하는 기능을 선택하세요 <1:입력, 2:삭제 3:종료>");
		scanf("%d", &choice); // choice 값 입력
		switch(choice)
		{
		case 1 : // 메뉴에서 1번 선택한 경우 수행
			printf("데이터를 입력하세요 : ");
			scanf("%d", &input); // 입력할 데이터 값 입력
			insert(&head, input); // insert함수 수행
			printf("리스트 : \n");
			printList(head); // printList함수 수행
			break;

		case 2 : // 메뉴에서 2번 입력한 경우 수행
			a = isEmpty(head); // 리스트가 비어있다면 1을 리턴, 비어있지않다면 0을 리턴
			if(a == 0 ) // 리스트가 비어있지 않다면
			{
				printf("삭제를 원하는 데이터를 입력하세요 : ");
				scanf("%d", &delete_key); // 삭제할 데이터 값 입력
				b = delete_(&head, delete_key); // delete_ 함수 수행 후 반환 값 b에 저장
				if(b == 0) // 삭제할 데이터 값을 찾았을 경우 b의 값은 0
					printList(head); // 데이터 삭제 후 리스트 출력
				else // 삭제할 데이터 값을 찾지 못했을 경우
					printf("\n");
			}
			else
				printf("\n");
			break;

		case 3 : // 메뉴에서 3번 입력한 경우 수행
			break;

		default : // 메뉴에서 3이상 값 입력시 수행
			printf("잘못된 선택임 다시 선택하세요\n");
			
			break;
		}
	}
	printf("종료되었음.\n"); // 종료 알림 구문
	
}
/*insert 함수는 데이터 값을 입력받아, 노드에 추가하는 함수
  입력시 오름차순 정렬을 하게 되어있으며, 동일 값 입력시 동일 값을 가진 노드 뒤에 추가
  ex) 1 -> 1 -> 2 -> 3 -> NULL */
void insert(ListNodePtr *head, int input) // *head는 시작노드의 주소 값, input은 입력할 데이터 값
{
	ListNode * newNode; // 데이터를 입력받을 새로운 노드 생성
	ListNode * temp = 0; // 데이터를 임시 저장할 노드 생성(수행 중 현재 노드 값 저장 용도)
	ListNode * prev = 0; // 데이터를 임시 저장할 노드 생성(수행 중 이전 노드 값 저장 용도)
	newNode = (ListNode *)malloc(sizeof(ListNode)); // 새로운 노드 메모리 동적할당
	newNode->data = input; // 입력할 데이터 값 새로운 노드에 저장
	newNode->next = 0; // 새로운 노드가 가르키는 주소를 0으로 초기화
	
	if(*head == 0) // *head의 주소번지가 0이면, 맨 처음 노드라는 뜻
	{
		*head = newNode; // 새로운 노드의 값들을 시작노드의 주소번지에 저장
	}
	else // 맨처음 노드가 아니면 실행
	{
		temp = *head; // 시작노드의 주소를 임시 저장 노드에 저장
		
		/* 기존 노드의 데이터 값이 새로운 노드의 입력 데이터 값보다 크면,
		   새로운 노드는 기존 노드 앞에 위치해야한다.*/
		if(temp->data > newNode->data) 
		{
			*head = newNode; // 새로운 노드의 주소를 시작 노드의 주소에 저장
			newNode->next = temp; // 임시 저장 노드의 주소를 새로운 노드의 next에 저장.
		}	
		else // 기존노드의 데이터 값이 새로운 노드의 입력 데이터보다 작거나 같으면 수행
		{
			/* while문의 조건 2가지 모두 충족하지 못하면 while문 종료*/
			while(temp != 0 && temp->data <= newNode->data) //중복된 값이 입력되는 경우 포함
			{
				prev = temp; // 임시저장 노드의 값들을 prev 임시저장 노드에 저장
				temp = temp->next; // 현재 임시 저장 노드가 가르키는 다음 노드의 주소를 임시 저장 노드에 저장
			}
			if(temp == 0) /* while문의 첫 번째 조건 만족 못할 경우
							 즉, 새롭게 생긴 노드가 가장 마지막에 삽입될 경우*/
			{
				prev->next = newNode; // 새로운 노드의 주소를 임시 이전 노드의 next에 저장
			}
			else /* while문의 두 번째 조건 만족 못할 경우
					즉, 새롭게 생긴 노드가 중간에 삽입될 경우*/
			{
				newNode->next = prev->next; // 이전 노드가 가르키는 다음 노드의 주소를 새로운 노드의 next에 저장
				prev->next = newNode; // 새로운 노드의 주소를 이전 노드의 next에 저장
			}
		}
	}
		
}
// delete_ 함수는 입력받은 데이터 값을 찾아 삭제하는 함수
int delete_(ListNodePtr *del, int delete_key) // *del은 시작 노드 주소, delete_key는 삭제할 데이터 값
{
	ListNode * temp = *del; // 데이터를 임시 저장할 노드 생성(수행 중인 현재 노드 값 저장 용도 및 시작 노드 주소 대입)
	ListNode * prev = 0; // 데이터를 임시 저장할 노드 생성(수행 중인 이전 노드 값 저장 용도)
	
	/* while문의 조건 2가지 모두 충족하지 못하면 while문 종료*/
	while(temp != 0 && temp->data != delete_key)
		{
			prev = temp; // 현재 수행 중 임서 저장 노드의 값을 이전 임시 저장 노드에 저장 
			temp = temp->next; // 현재 임시 저장노드가 가르키는 다음 노드의 주소를 임시 저장 노드에 저장
		}
	// 마지막 노드까지 검색하였지만, 리스트에 삭제할 데이터가 존재하지 않으면 수행
		if(temp == 0) // while문의 첫 번째 조건 만족 못할 시 수행 
		{
			printf("%d는 리스트에 없음\n",delete_key);
			return 1; // 삭제할 데이터가 존재하지 않음을 구분하기 위해 1을 리턴
		}
		// 노드들의 데이터 값을 검색하던 도중, 삭제할 데이터 값이 일치하는 경우 수행
		else // while문의 두 번째 조건 만족 못할 시 수행
		{
			// while문의 두 번째 조건을 한번에 만족 못하여 while문을 한 번도 수행하지 못한 경우 수행
			if(prev == 0) // 삭제할 데이터가 처음 노드에 들어있는 경우
			{
				*del = temp->next; // 현재 임시 저장 노드가 가르키는 다음 노드의 주소를 시작 노드의 주소에 저장
			} 
			// while문의 두 번째 조건을 만족하지 못한 경우
			else // 삭제할 데이터가 처음 노드에 들어있지 않은 경우
			{
				prev->next = temp->next; // 현재 임시 저장 노드가 가르키는 다음 노드의 주소를 이전 임시 저장 노드의 next에 저장
				free(temp); // 현재 임시 저장 노드 삭제
			}
		return 0; // 해당 데이터를 삭제했음을 구분하기 위해 0을 리턴
		}
	
}
int isEmpty(ListNodePtr Empty) // 리스트가 비어있는지 확인하는 함수
{
	if(Empty == 0) // 전달받은 시작 노드가 NULL이면 리스트는 비어있다.
	{
		printf("리스트가 비어있음.\n");
		return 1; // 비어있음을 확인하기 위해 1을 리턴
	}
	else
		return 0; // 비어있지않음을 확인하기 위해 0을 리턴
}
void printList(ListNodePtr Print) // 입력된 데이터 값들을 출력하는 함수
{
	ListNode * temp; // 임시저장 노드 생성
	temp = Print; // 전달받은 시작 노드 저장
	while (temp != NULL) // 임시 저장 노드의 주소가 0이면 while 종료
	{
		printf("%d -> ", temp->data) ; // 임시저장 노드의 데이터 값 출력
		temp = temp->next; // 다음 노드의 주소를 임시저장 노드에 저장
	}

	printf("NULL \n");
	printf("\n");
}
void instructions() // 프로그램 설명 함수
{
	printf("단순 연결리스트 과제\n");
	printf("1번 입력시 데이터입력\n");
	printf("2번 입력시 삭제할 데이터 입력 후 해당 데이터 존재 시 데이터 삭제\n");
	printf("3번 입력시 프로그램 종료\n");

	printf("\n\n");
}