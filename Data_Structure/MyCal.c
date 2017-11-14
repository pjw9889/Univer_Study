#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct // 스택 구조체 선언
{
	int stack[100];
	int top; 
} StackType;

void init(StackType *s); // 스택 초기화 함수 선언
int is_empty(StackType *s); // 스택 공백 상태 확인 함수 선언
int is_full(StackType *s); // 스택 포화 상태 확인 함수 선언
void push(StackType *s, int item); // 스택 삽입함수 선언
int pop(StackType *s); // 스택 삭제함수 선언
int peek(StackType *s); // top이 가르키는 인덱스의 정보를 보는 함수 선언

enum state {FALSE, TRUE}; // 상태 열거형 선언

struct _exp {		// 후위식 각 요소 저장 구조체
	int number;		// 실제 숫자
	enum state is_op;	// 연산자 확인 변수 연산자일 경우 TRUE, 피연산자일 경우 FALSE
};

typedef struct exp {	//후위식의 길이와 식 저장 구조체
	struct _exp newExp[250];	//후위식 저장 구조체 배열
	int length;		//후위식의 길이 저장
} Exp;

int prec(char op); // 연산자 우선순위 반환 함수 선언
int infix_to_postfix(char*, Exp*); // 중위표기->후위표기 수식 함수 선언
int calculrate(Exp*); // 후위식 계산 함수 선언

void main()
{
	char str[1000]; // 입력받을 배열 선언
	Exp newExp[250];	// 후위식 저장 구조체
	int true_function; // 함수의 Error 값을 저장할 변수
	int result; // 입력받은 수식 연산 값 저장할 변수

	printf("중위 수식을 입력해주세요 : ");
	gets(str);   // str배열에 중위수식 입력
	printf("후위표기식 : ");
	true_function = infix_to_postfix(str, newExp); // 중위 수식 -> 후위 수식 변경

	if (true_function) // 입력받은 중위식에 여닫는 괄호 Matching Error발생 하지않았다면 수행
	{
		result = calculrate(newExp); //	입력받은 수식에 대한 값 계산하는 함수 생성후 함수 실행
		printf("후위 표기식의 연산 값은 : %d\n", result);
	}
	else // 입력받은 중위식에 여닫는 괄호 Matching Error발생시
		fprintf(stderr, "괄호 연산이 잘못되어 연산할 수 없습니다.\n");

}

void init(StackType *s) // 스택 초기화 함수
{
	s->top = -1; // 스택의 top 변수 -1로 초기화
}

int is_empty(StackType *s) // 스택 공백 상태 확인 함수
{
	if (s->top == -1) // 스택의 top 변수 값이 -1이면 공백상태이므로 true 값 return
		return 1;
	else // 스택의 top 변수 값이 -1이 아니면 공백상태가 아니므로 false 값 return
		return 0;
}

int is_full(StackType *s) // 스택 포화 상태 확인 함수
{
	// 스택 구조체의 배열 크기가 100이므로 최대 인덱스는 99이다.
	if (s->top == 99) // 스택의 top 변수 값이 99이면 포화상태이므로 true 값 return
		return 1;
	else // 스택의 top 변수 값이 99가 아니면 포화상태가 아니므로 false 값 return
		return 0;
}

void push(StackType *s, int item) // 스택에 데이터 push하는 함수
{
	if (is_full(s)) // 스택이 가득찼으면
	{
		fprintf(stderr, "스택이 가득 찼습니다. \n"); // 스택이 가득찼다는 Error return
		exit(-1);
	}
	else // 스택이 가득차지 않았으면
	{
		s->top += 1; // 스택의 top 변수 값을 1 증가시킨다.
		s->stack[(s->top)] = item; // 스택 배열의 top 변수 값의 인덱스에 item 값을 push

	}
}

int pop(StackType *s) // 스택에 데이터 pop하는 함수
{
	if (is_empty(s)) { // 스택이 비어있다면
		fprintf(stderr, "스택이 비어있습니다.\n"); // 스택이 비어있다는 Error return
		exit(-1);
	}
	else // 스택이 비어있지 않다면
	{
		int rindex; // pop할 데이터가 저장된 인덱스 값 저장할 변수
		rindex = s->top; // pop할 데이터가 저장된 인덱스 값을 rindex에 대입
		s->top -= 1; // 스택의 top 변수 값을 1 감소시킨다.
		return s->stack[rindex]; // rindex의 값을 return
	}
}

int peek(StackType *s) // 스택의 top 변수 값이 가르키는 배열 인덱스 정보 보는 함수
{
	if (is_empty(s)) // 스택이 비어있다면
	{
		fprintf(stderr, "스택이 비어있습니다.\n"); //스택이 비어있다는 Error return
		exit(-1);
	}
	else return s->stack[s->top]; // 스택이 비어있지않다면 top 변수 값이 가르키는 인덱스의 정보를 return
}


int prec(char op) // 연산자 우선순위 지정 함수
{
	switch (op) {
	case '[': case ']': return 0; // 대괄호는 0
	case '{': case '}': return 1; // 중괄호는 1
	case '(': case')': return 2; // 소괄호는 2
	case '+': case '-': return 3; // 가산, 감산은 3
	case '*': case '/': return 4; // 곱셈, 나눗셈은 4
	}
	return -1;
}

int infix_to_postfix(char exp[], Exp newexp[]) {	// 중위식 -> 후위식 변환 함수
	StackType s; // 스택구조체 선언
	int i; // 반복문 사용 변수
	int newIdx = 0; // 후위식저장 배열의 인덱스
	int temp; // atoi의 결과를 저장하는 변수
	char tok, top_op;	//tok:식의 각 요소를 저장, top_op:스택에서 꺼낸 요소를 저장
	int length = strlen(exp);	//사용자로부터 받은 식의 길이 저장
	newexp->length = 0;	//후위식 배열의 길이 초기화
	
	init(&s);	//스택 초기화

	for (i = 0; i < length; i++) {
		tok = exp[i];

		if (isdigit(tok)) {		//tok가 숫자문자일 경우
			temp = atoi(&exp[i]);	//문자 숫자를 실제 숫자로 변환

			while (isdigit(exp[i]))	//다음 연산자를 만날때까지 i값 증가
				i++;
			i--;

			//후위식 배열에 atoi 결과 저장, 길이 증가
			newexp->newExp[newIdx].number = temp;
			newexp->newExp[newIdx++].is_op = FALSE;
			newexp->length++;
			continue;
		}
		else if (tok == '-' && i==0) {		//맨 앞에 음수가 오는 경우

			temp = atoi(&exp[i]);	//문자 숫자를 실제 숫자로 변환

			while (isdigit(exp[i + 1]))	//다음 연산자를 만날때까지 i값 증가
				i++;

			//후위식 배열에 atoi 결과 저장, 길이 증가
			newexp->newExp[newIdx].number = temp;
			newexp->newExp[newIdx++].is_op = FALSE;
			newexp->length++;
			continue;
		}
		else if (tok == '-' && exp[i-1]!=')' && !isdigit(exp[i - 1])) {	//중간에 음수가 오는 경우

			temp = atoi(&exp[i]);	//문자 숫자를 실제 숫자로 변환

			while (isdigit(exp[i+1]))	//다음 연산자를 만날때까지 i값 증가
				i++;

			//후위식 배열에 atoi 결과 저장, 길이 증가
			newexp->newExp[newIdx].number = temp;
			newexp->newExp[newIdx++].is_op = FALSE;
			newexp->length++;
			continue;
		}

		switch (tok) {
		case '+': case '-': case '*': case '/': // ch 값이 +, -, *, /일 경우
			while (!is_empty(&s) && (prec(tok) <= prec(peek(&s)))) {// 스택이 비어있지않고, ch의 우선순위 < 스택의 top 변수 값이 가르키는 연산자의 우선순위이면
				newexp->newExp[newIdx].number = pop(&s); // 스택 pop 수행
				newexp->newExp[newIdx++].is_op = TRUE;	// 연산자이므로 is_op에 TRUE값 저장
				newexp->length++;	// 연산자이므로 is_op에 TRUE값 저장
			}
			push(&s, tok); // 위 while문의 조건을 만족하지 않는 경우, ch의 값을 스택에 push
			break;
		case '(': case '{': case '[': // ch값이 여는 괄호(소,중,대)일 경우
			push(&s, tok); // 스택에 push 수행
			break;

		case ')': // ch값이 닫는 괄호(소)일 경우
			while (1) {
				if(s.top == -1) // 더 이상 불러낼 여는 소괄호가 없으면 Error 상태 반환
					return 0; 
				else
				{
					top_op = pop(&s); // 스택에서 pop하여 top_op에 저장
					if (top_op == '(') // top_op의 값이 여는 소괄호이면 멈춘다.
						break;
					else if (top_op == '{' || top_op == '[') // top_op의 값이 소괄호가 아닌 다른 괄호이면
						return 0;
					else {	//스택에서 꺼낸 연산자 후위식에 저장
						newexp->newExp[newIdx].number = top_op;
						newexp->newExp[newIdx++].is_op = TRUE;
						newexp->length++;
					}
				}
			}
			break;

		case '}': // ch값이 닫는 괄호(중)일 경우
			while (1) {
				if(s.top == -1) // 더 이상 불러낼 여는 중괄호가 없으면 Error 상태 반환
					return 0; 
				else
				{
					top_op = pop(&s); //스택에서 pop하여 top_op에 저장
					if (top_op == '{') // top_op의 값이 여는 중괄호이면 멈춘다.
						break;
					else if (top_op == '(' || top_op == '[') // top_op의 값이 중괄호가 아닌 다른 괄호이면
						return 0;
					else {	//스택에서 꺼낸 연산자 후위식에 저장
						newexp->newExp[newIdx].number = top_op;
						newexp->newExp[newIdx++].is_op = TRUE;
						newexp->length++;
					}
				}
			}
			break;
		case ']': // ch값이 닫는 괄호(대)일 경우
			while (1) {
				if(s.top == -1) // 더 이상 불러낼 여는 대괄호가 없으면 Error 상태 반환
					return 0; 
				else
				{
					top_op = pop(&s); //스택에서 pop하여 top_op에 저장
					if (top_op == '[') // top_op의 값이 여는 대괄호이면 멈춘다.
						break;
					else if (top_op == '(' || top_op == '{')// top_op의 값이 대괄호가 아닌 다른 괄호이면
						return 0;
					else {	//스택에서 꺼낸 연산자 후위식에 저장
						newexp->newExp[newIdx].number = top_op;
						newexp->newExp[newIdx++].is_op = TRUE;
						newexp->length++;
					}
				}
			}
			break;
		}
	}
	while (!is_empty(&s)) // 스택이 비어있지않다면
	{
		top_op = pop(&s); // 스택에서 pop하여 top_op 변수에 값을 저장한다.
		if (top_op == '(' || top_op == '{' || top_op == '[') // top_op 값이 여는 괄호(소,중,대)이면
			return 0;
		else {		//스택에서 꺼낸 연산자 후위식에 저장
			newexp->newExp[newIdx].number = top_op;
			newexp->newExp[newIdx++].is_op = TRUE;
			newexp->length++;
		}
	}

	//후위식 출력
	for (i = 0; i < newexp->length; i++) {
		if (newexp->newExp[i].is_op)	//연산자일 경우
			printf("%c", newexp->newExp[i].number);		//문자로 출력
		else	//피연산자일경우
			printf("%d", newexp->newExp[i].number);		//숫자로 출력
	}
	puts("");

	return 1;
}


int calculrate(Exp *Postfix)
{
	StackType cal;		//스택 생성
	int tok;		//후위식의 각 요소를 저장할 변수
	int op1, op2, cal_result;	//스택에서 꺼낸 2개의 피연산자를 저장할 변수와 op1과 op2의 계산값을 저장할 변수
	int i = 0, length;	//후위식의 길이와 반복문을 위한 변수
	int sum;	//결과값을 저장할 변수

	init(&cal);	//스택 초기화
	length = Postfix->length;	//후위식의 길이 저장

	for (i = 0; i < length; i++)
	{

		tok = Postfix->newExp[i].number;	//후위식의 요소를 저장

		if ((tok == '+' || tok == '-' || tok == '*' || tok == '/') && Postfix->newExp[i].is_op) {	//후위식 요소가 연산자일 경우
			op2 = pop(&cal);	//스택에 있는 요소를 꺼냄
			op1 = pop(&cal);	//스택에 있는 요소를 꺼냄

			if (tok == '+')		//tok가 사칙연산일 경우 각각을 계산하여 value에 저장
				cal_result = op1 + op2;
			else if (tok == '-')
				cal_result = op1 - op2;
			else if (tok == '*')
				cal_result = op1 * op2;
			else if (tok == '/')
				cal_result = op1 / op2;
			else
				return 0;

			push(&cal, cal_result);	//계산 결과를 스택에 넣는다
		}
		else
			push(&cal, tok); // 후위식 요소가 피연산자일 경우 스택에 넣는다
	}

	sum = pop(&cal);		//결과값을 스택에서 꺼낸다

	if (is_empty(&cal))	//스택이 비었음을 확인 후 결과를 반환
		return sum;
	else
	{
		fprintf(stderr, "Calculrate Error\n");
		exit(1);
	}
}

