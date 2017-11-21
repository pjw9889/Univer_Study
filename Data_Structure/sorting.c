#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define Input_size 10000 // 최대 입력 크기 정의

void Selection_sort(int arr[], int n); // 선택정렬 함수 선언
void Insertion_sort(int arr[], int n); // 삽입 정렬 함수 선언
void Bubble_sort(int arr[], int n); // 버블 정렬 함수 선언   
void merge_sort(int arr[], int left, int right); // 합병정렬 중 Devide 함수 선언
void merge(int arr[], int left, int mid, int right);    // 합병정렬 중 Conquer 함수 선언
void quick_sort(int arr[], int left, int right); // Quick정렬 함수 선언
int partition(int arr[], int left, int right); // Qucik정렬의 Partition 위치지정 함수 선언
void insert_sort(FILE *input, FILE *output, int n); // 모든 정렬 수행 함수 선언
void file_errorchecking(int close); // 입력 값 범위 초과 확인 함수 선언
int merge_sort_cmpcount = 0; // 합병정렬 비교연산횟수 저장할 전역변수 선언 및 초기화
int quick_sort_cmpcount = -2; // 퀵 정렬 비교연산횟수 저장할 전역변수 선언 및 초기화

// 퀵 정렬 비교연산횟수 저장 변수의 초기 값으로 -2를 준 것은 하나의 do~while 문안에서 기본적으로 2번의 do while문이 수행이 되기때문입니다.
void main()
{
   int inputnumber; // 입력받을 숫자에 대한 변수 선언
   FILE *input = 0; // FILE 포인터 선언 및 초기화
   FILE *output = 0; // FILE 포인터 선언 및 초기화

   printf("숫자를 입력하세요(숫자 범위는 1~10000) : ");
   scanf("%d", &inputnumber);
   if (inputnumber > Input_size) // 입력된 값이 최대 입력 크기보다 크면
      fprintf(stderr, "입력된 값이 수용할 수 있는 크기를 넘었습니다");
   else
      insert_sort(input, output, inputnumber);
}

void insert_sort(FILE *input, FILE *output, int n) //모든 정렬 수행 함수
{
   //i는 반복문 변수, Random은 난수 값 저장하는 변수, close는 fclose 반환 값 저장 변수
	int i, Random, close, load_number; // load_number는 파일에서 값을 읽어올 때 저장하는 변수
   int * arr = 0; // 파일에서 읽어온 값들을 저장할 포인터 선언
   int * select_arr = 0; // 선택정렬 된 값들을 저장할 포인터 선언
   int * insert_arr = 0; // 삽입정렬 된 값들을 저장할 포인터 선언
   int * bubble_arr = 0; // 버블정렬 된 값들을 저장할 포인터 선언
   int * merge_arr = 0; // 합병정렬된 값들을 저장할 포인터 선언
   int * quick_arr = 0; // 퀵 정렬된 값들을 저장할 포인터 선언

   arr = (int *)malloc(sizeof(int)*n); // 입력받은 크기만큼 arr 동적할당
   select_arr = (int *)malloc(sizeof(int)*n); // 입력받은 크기만큼 select_arr 동적할당
   insert_arr = (int *)malloc(sizeof(int)*n); // 입력받은 크기만큼 insert_arr 동적할당
   bubble_arr = (int *)malloc(sizeof(int)*n); // 입력받은 크기만큼 buuble_arr 동적할당
   merge_arr = (int *)malloc(sizeof(int)*n); // 입력받은 크기만큼 merge_arr 동적할당
   quick_arr = (int *)malloc(sizeof(int)*n); // 입력받은 크기만큼 quick_arr 동적할당

   srand((unsigned)time(NULL)); // time seed 값 설정
   input = fopen("input1.txt", "wt"); // 파일 쓰기형태로 열기
   for (i = 1; i <= n; i++)  // 입력받은 숫자만큼 반복
   {
      Random = rand() % 10000 + 1; // 1~10000의 난수 생성
      fprintf(input, "%d\t", Random); // 열어둔 파일에 난수 값 저장
      if (i % 10 == 0) // 10개 입력되었을 때마다 줄 바꿈
         fprintf(input, "\n");
   }
   close = fclose(input); // 파일 닫기
   file_errorchecking(close); // 파일 닫힘여부 확인

   input = fopen("input1.txt", "rt"); // 파일 읽기형태로 열기
   for (i = 0; i<n; i++) // 입력받은 숫자만큼 반복
   {
      fscanf(input, "%d\t", &load_number); // 파일 내용 하나 읽어오기
      arr[i] = load_number; // 읽어온 내용 arr에 저장
      // 각각의 배열들은 현재 쓰레기 값이 들어가 있으므로, 내용 하나씩 읽을때마다 모두 넣어준다.
	  select_arr[i] = arr[i]; 
      insert_arr[i] = arr[i];
      bubble_arr[i] = arr[i];
      merge_arr[i] = arr[i];
      quick_arr[i] = arr[i];
   }
   close = fclose(input); // 파일닫기
   file_errorchecking(close); // 파일 닫힘여부 확인

   Selection_sort(select_arr, n); // 선택정렬 함수 호출, select_arr의 값들을 정렬한다.
   output = fopen("output1.txt", "w"); // 쓰기형태로 파일 열기
   fprintf(output, "선택정렬\n "); // 정렬 index 파일에 쓰기
   for (i = 1; i <= n; i++) // 입력받은 숫자만큼 반복
   {
      fprintf(output, "%d\t", select_arr[i - 1]); // select_arr의 값을 파일에 저장
      if (i % 10 == 0) // 10개 입력되면 줄 바꿈
         fprintf(output, "\n");
   }
   fprintf(output, "\n");
   close = fclose(output); // 파일 닫기
   file_errorchecking(close); // 파일 닫힘여부 확인

   Insertion_sort(insert_arr, n); // 삽입정렬 함수 호출, insert_arr의 값들을 정렬한다.
   output = fopen("output1.txt", "a"); // 이어쓰기 형태로 파일 열기
   fprintf(output, "삽입정렬\n "); // 정렬 index 파일에 쓰기
   for (i = 1; i <= n; i++) // 입력받은 숫자만큼 반복
   {
      fprintf(output, "%d\t", insert_arr[i - 1]); // insert_arr 값을 파일에 저장
      if (i % 10 == 0) // 10개 입력되면 줄 바꿈
         fprintf(output, "\n");
   }
   fprintf(output, "\n");
   close = fclose(output); // 파일닫기
   file_errorchecking(close); //파일 닫힘여부 확인

   Bubble_sort(bubble_arr, n); // 버블정렬 함수 호출, buuble_arr의 값들을 정렬한다.
   output = fopen("output1.txt", "a"); // 이어쓰기 형태로 파일 열기
   fprintf(output, "버블정렬\n"); // 정렬 index 파일에 쓰기
   for (i = 1; i <= n; i++) // 입력받은 숫자만큼 반복
   {
      fprintf(output, "%d\t", bubble_arr[i - 1]); // buuble_arr 값을 파일에 저장
      if (i % 10 == 0) // 10개 입력되면 줄 바꿈
         fprintf(output, "\n");
   }
   fprintf(output, "\n");
   close = fclose(output); // 파일닫기
   file_errorchecking(close); // 파일 닫힘여부 확인

   merge_sort(merge_arr, 0, n - 1); // 합병정렬 함수 호출, merge_arr의 값들을 정렬한다.
   printf("합병정렬 비교연산횟수 : %d\n", merge_sort_cmpcount); // 합병정렬 비교연산횟수 출력
   output = fopen("output1.txt", "a"); // 이어쓰기 형태로 파일 열기
   fprintf(output, "합병정렬\n"); // 정렬 index 파일에 쓰기
   for (i = 1; i <= n; i++) // 입력받은 숫자만큼 반복
   {
      fprintf(output, "%d\t", merge_arr[i - 1]); // merge_arr 값을 파일에 저장
      if (i % 10 == 0) // 10개 입력되면 줄 바꿈
         fprintf(output, "\n");
   }
   fprintf(output, "\n"); 
   close = fclose(output); // 파일 닫기
   file_errorchecking(close); // 파일 닫힘여부 확인

   quick_sort(quick_arr, 0, n - 1); // 퀵 정렬 함수 호출 quick_arr의 값들을 정렬한다.
    printf("퀵정렬 비교연산횟수 : %d\n", quick_sort_cmpcount); // 퀵 정렬 비교연산횟수 출력
   output = fopen("output1.txt", "a"); // 이어쓰기 형태로 파일 열기
   fprintf(output, "퀵정렬\n"); // 정렬 index 파일에 쓰기
   for (i = 1; i <= n; i++) // 입력받은 숫자만큼 반복
   {
      fprintf(output, "%d\t", quick_arr[i - 1]); // quick_arr의 값들을 정렬한다.
      if (i % 10 == 0) // 10개 입력되면 줄 바꿈
         fprintf(output, "\n");
   }
   fprintf(output, "\n");
   close = fclose(output); // 파일닫기
   file_errorchecking(close); // 파일 닫힘여부 확인
}
void file_errorchecking(int close) // File close 반환 값으로 파일 닫힌 여부 상태확인 함수
{
   if (close != 0)
   {
      printf("파일이 닫히지 않았습니다.\n");
      exit(1);
   }
}
void Selection_sort(int arr[], int n) // 선택정렬 함수
{
   int i, j, least, temp;
   int cmp = 0; // 비교연산 횟수 변수

   for (i = 0; i<n - 1; i++)
   {
      least = i;
      for (j = i + 1; j<n; j++)
      {
         if (arr[least] > arr[j])
            least = j;
         cmp++; // IF문의 비교연산으로 인하여 비교연산 횟수 증가
      }

      temp = arr[i];
      arr[i] = arr[least];
      arr[least] = temp;
   }
   printf("선택정렬 비교연산횟수 : %d\n", cmp);
}
void Insertion_sort(int arr[], int n) // 삽입정렬 함수
{
   int i, j, key;
   int cmp = 0; // 비교연산 횟수 변수
   for (i = 1; i<n; i++)
   {
      key = arr[i];
      for (j = i - 1; j>-1 && arr[j]>key; j--)
      {
         arr[j + 1] = arr[j];
         cmp++; // for문의 arr[j]>key 비교연산하므로 비교연산 횟수 증가
      }
      arr[j + 1] = key;

   }
   printf("삽입정렬 비교연산횟수 : %d\n", cmp);
}
void Bubble_sort(int arr[], int n) // 버블정렬 함수 
{
   int i, j, temp;
   int cmp = 0; // 비교연산 횟수 변수
   for (i = n - 1; i>0; i--)
   {
      for (j = 0; j<i; j++)
      {
         if (arr[j] > arr[j + 1])
         {
            temp = arr[j];
            arr[j] = arr[j + 1];
            arr[j + 1] = temp;
           
         }
		  cmp++; // IF문 수행으로 인한 비교연산 횟수 증가
      }
   }
   printf("버블정렬 비교연산횟수 : %d\n", cmp);
}
void merge_sort(int arr[], int left, int right) // 합병정렬 중 Devide 함수 선언
{
   int mid;
   
   if (left<right)
   {
      merge_sort_cmpcount++; // IF문 수행으로 인한 비교연산 증가
      mid = (right + left) / 2;

      merge_sort(arr, left, mid);
      merge_sort(arr, mid + 1, right);
      merge(arr, left, mid, right);
   }  
}

void merge(int arr[], int left, int mid, int right) // 합병정렬 중 Conquer 함수 선언
{ 
   int i, j, k, l;
   
   int * temp; // 합병할 새로운 포인터 생성
   i = left;
   j = mid + 1;
   k = left;

   temp = (int *)malloc(sizeof(int)*(right + 1)); // right+1 크기의 동적할당

   while (i <= mid && j <= right)
   {
      
      if (arr[i] <= arr[j])
      {
         temp[k++] = arr[i++];
	  }
      else
      {
         temp[k++] = arr[j++];
      }
      	 merge_sort_cmpcount++; // 비교연산 수행 횟수 증가
   }

   if (i > mid)
   {
      for (l = j; l <= right; l++)
         temp[k++] = arr[l];
   }
   else
   {
      for (l = i; l <= mid; l++)
         temp[k++] = arr[l];
   }
   
   for (l = left; l <= right; l++)
      arr[l] = temp[l];
     
}

void quick_sort(int arr[], int left, int right) // 퀵정렬 함수 
{
   if (left <= right)
   {
      int mid = partition(arr, left, right);
      quick_sort(arr, left, mid - 1);
      quick_sort(arr, mid + 1, right);
   }
  
}
// 비교연산 횟수 저장하는 변수의 값은 -2이다.
// do ~ while문은 while의 조건을 만족하지 못해도 1번은 실행하여야 하기때문에, 변수 값을 -2로 주었으며,
// 초기 값은 0과 같다.
int partition(int arr[], int left, int right)// Qucik정렬의 Partition 위치지정 함수
{
   int pivot = arr[left];
   int low = left, high = right + 1;
   int temp;


   do {
      do
	  {
         low++;
		  quick_sort_cmpcount++; // 비교연산 횟수 증가
	  }
      while (low <= right && arr[low] < pivot);

      do
	  {
         high--;
		 quick_sort_cmpcount++; // 비교연산 횟수 증가
	  }
      while (high >= left && arr[high] > pivot);


      if (low < high)
      {
		 quick_sort_cmpcount++; // IF문 수행하므로 비교연산 횟수 증가
         temp = arr[low];
         arr[low] = arr[high];
         arr[high] = temp;
      }
   } while (low < high);

   temp = arr[left];
   arr[left] = arr[high];
   arr[high] = temp;

   return high;
}

