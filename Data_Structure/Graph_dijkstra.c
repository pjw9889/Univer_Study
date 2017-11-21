/*

파일 indata.txt를 불러들인다.
indata.txt의 첫 번째 줄에서 정수 하나(n)를 읽어들여 n*n의 2차원 배열을 생성한다.
Indata.txt의 내용 중 INF는 -1로 설정하였으며, -1을 읽었을 시 INT_MAX 상수 값으로 반환한다.
그리고 indata.txt의 남은 데이터들을 읽어 2차원 배열에 저장한다.
모든 데이터가 저장된 2차원 배열은 그래프를 나타낸다.
2차원 배열을 Prim, shortest_path 함수의 인자로 준다.
Prim, shortest_path 함수 수행 중 Node 배열을 생성하여, 그래프를 트리화 한다.
Prim, shortest_path 함수가 종료 되기 전 Node 배열을 출력한다.
shortest_path 함수는 distance 배열에 루트 노드에서 각 노드 간 가중치를 저장한다.
shortest_path 함수는 종료 전 루트 노드에서 각 노드 간의 가중치도 출력한다.

get_min_vertex, prim, choose, shortest_path의 소스 내용은 책을 참고하였으며,
함수의 인자 값은 약간 변경하였습니다. 

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_MAX 214748

int get_min_vertex(int * selected, int * dist, int n); // Prim의 간선 가중치가 최소인 노드를 찾는 함수
void prim(int ** arr, int s, int n); // Prim 함수
int choose(int * distance, int n, int * found); // shortest_path 함수에서 최단거리를 선택하게 해주는 함수
void shortest_path(int ** arr, int start, int n); // Dijkstra 최단거리 함수
void main()
{
	int input; // 루트로 지정할 번호 입력
	int inputnumber; // 입력 숫자 변수
	int index; // 정사각 행렬의 size 변수
	int i,j; // 반복문 변수
	int **arr; // 입력받은 그래프를 저장할 포인터 변수
	
	FILE * inputfile; // input 파일 포인터를 저장할 변수
	printf(" 루트로 지정할 번호를 입력해주세요 : ");
	scanf("%d", &input);

	inputfile = fopen("indata.txt", "rt"); // indata.txt를 열어 파일포인터를 input 변수에 대입

	fscanf(inputfile, "%d\n", &inputnumber); // input 파일포인터에서 하나의 정수를 읽어온다.
	index = inputnumber; // 처음 읽어온 정수는 정사각 행렬의 size이다.

	if(index <input) // indata.txt의 파일의 첫 줄(n사이즈의 행렬) 내용보다 입력된 노드 숫자가 크면
		fprintf(stderr, "파일에서 입력된 노드 숫자보다 입력한 노드 숫자가 큽니다.");
	
	else
	{
		// 1차원 배열 생성 후 각각의 배열 요소에 또 다시 배열을 생성하게되면, 2차원배열과 같아진다.
		arr = (int **) malloc (sizeof(int*) * index); // index size만큼의 1차원 배열 생성
		for(i = 0; i<index; i++) // 1차원 배열의 각각의 요소에 1차원 배열 생성
			arr[i] = (int *) malloc (sizeof(int) * index); 

		// n*n 사이즈 행렬에 indata.txt의 데이터를 읽어오는 반복문
		for(i = 0; i<index; i++)
			for(j = 0; j<index; j++)
			{
				fscanf(inputfile, "%d\t", &inputnumber);
				if(inputnumber < 0) // indata.txt에서 읽어 온 값이 -1이면
					inputnumber = INT_MAX; // INF 값 지정
				arr[i][j] = inputnumber;
			}

		prim(arr, input, index); // 2차원 배열, 시작 노드 수, indata.txt에서 읽어온 배열의 크기
		shortest_path(arr, input, index); // 2차원 배열, 시작 노드 수, indata.txt에서 읽어온 배열의 크기
	}

}

int get_min_vertex(int * selected, int * dist, int n)
{
	int v, i;
	for(i = 0; i<n; i++)
		if(!selected[i])
		{
			v = i;
			break;
		}
	for(i=0; i<n; i++)
	{
		if(!selected[i] && (dist[i] < dist[v]))
			v = i;
	}
	
	return (v);
}

void prim(int **arr, int s, int n)
{
	int i, u, v;
	int * selected;
	int * dist;
	int * Prim_node;
	selected = (int *) malloc(sizeof(int) * n); // 노드의 선택여부 판단, select 배열 n사이즈만큼 동적할당
	dist = (int *) malloc(sizeof(int) * n); // 노드간 거리 값 저장 배열, dist 배열 n 사이즈만큼 동적할당
	Prim_node = (int *) malloc(sizeof(int) * n); // Prim 함수 수행 중 Prim 결과로 나타나는 트리 저장할 배열, Prim_node 배열 n 사이즈만큼 동적할당

	// dist, selected, Prim_node 배열 초기화하는 반복문
	for(u = 0; u<n; u++)
	{
		dist[u] = INT_MAX;
		selected[u] = 0;
		Prim_node[u] = 0;
	}

	dist[s] = 0; // 시작 노드의 거리는 0으로 초기화(s node에서 s node의 거리는 0)
	Prim_node[s] = -1; // 시작 노드는 루트이므로 부모노드가 없다. 따라서 -1로 루트임을 표시

	for(i = 0; i<n; i++)
	{
		u = get_min_vertex(selected, dist, n); // 노드간 거리 최소인 노드 u에 대입
		selected[u] = 1; // u 노드 선택됨
		
		if(dist[u] == INT_MAX)
		{
			return ;
		}
		// arr배열의 값이 INF가 아니고, v번째 노드가 선택되지않았고, arr[u][v]의 값이 dist[v]보다 작으면 수행
		else
		{
			for(v = 0; v<n; v++)
				if(arr[u][v] != INT_MAX)
					if(!selected[v] && arr[u][v] < dist[v])
					{
						dist[v] = arr[u][v]; // arr[u][v] 값이 최소 거리이므로 대입
						Prim_node[v] = u; // u 노드를 v번째 Prim_node에 대입함으로써 Prim_node 배열은 Prim이 종료되었을 때, 트리가 저장된다.
					}
		}
	}
	printf("    Prim set node : ");
	for(v = 0; v<n; v++) // 
		printf("%d\t", Prim_node[v]); // Prim_node에 저장된 배열 값들을 출력 -> 트리를 출력
	printf("\n"); 
	
}

int choose(int * distance, int n, int * found)
{
	int i, min, minpos;
	min = INT_MAX;
	minpos = -1;
	for(i = 0; i<n; i++)
		if(distance[i] < min && ! found[i])
		{
			min = distance[i];
			minpos = i;
		}
	return minpos;
}

void shortest_path(int **arr, int start, int n)
{
	int i, u, w;
	int * distance; // 가중치 저장할 포인터 변수, distance 포인터 변수 선언
	int * found; // 선택된 노드 식별할 포인터 변수, found 포인터 변수 선언
	int * Dijkstra_node; // Dijkstra 함수가 수행된 후 생기는 트리를 저장할 포인터 변수, Dijkstra_node 포인터 변수 선언

	distance = (int *) malloc(sizeof(int) * n); // n사이즈만큼 distacne 배열 동적할당
	found = (int *) malloc(sizeof(int) * n); // n사이즈만큼 found 배열 동적할당
	Dijkstra_node = (int *) malloc(sizeof(int) * n); // n사이즈만큼 Dijkstra_node 배열 동적할당
	memset(Dijkstra_node, start, sizeof(int) * n); // Dijkstra_node를 n사이즈만큼 시작 노드 이름으로 초기화

	// distance 배열과 found 배열의 초기화
	for(i = 0; i<n; i++)
	{
		distance[i] = arr[start][i];
		found[i] = 0; // 0은 아직 선택되어지지않은 노드라는 뜻
	}

	found[start] = 1; // 루트 노드에서 시작하므로 선택되어진 노드다. 따라서 1을 대입
	distance[start] = 0; // 루트 노드는 가중치를 가지지 않으므로 0으로 지정
	Dijkstra_node[start] = -1; // 루트 노드는 부모 노드가 없으므로 
	for(i = 0; i<n-1; i++)
	{
		u = choose(distance, n , found); // 루트 노드에서 최소 가중치로 갈 수 있는 노드 번호 대입
		found[u] = 1; // u 노드는 선택되어졌기에 1을 대입
		for(w = 0; w<n; w++)
			if(!found[w])
				if(distance[u] + arr[u][w] < distance[w]) // w노드가 선택되어지지 않았고, u 노드의 가중치 + arr[u][w] 값이 w노드의 가중치보다 크면
				{
					distance[w] = distance[u] + arr[u][w]; // u의 가중치와 arr[u][w]의 가중치가 더 작으므로 w의 가중치에 대입
					Dijkstra_node[w] = u; // u는 w의 상위 노드가 된다. 따라서, Dijkstra_node 배열의 w 인덱스에 u를 대입한다.
					// 여기서 수행 되지 않는 경우는 부모 노드가 루트일 때는 포함되지 않는다.
				}
	}
	
	for(w = 0; w<n; w++)
		if(Dijkstra_node[w] > n || Dijkstra_node[w] < -1) // 부모 노드가 루트인 경우에 수행
			Dijkstra_node[w] = start; // 루트 노드의 번호를 Dijkstra_node 배열의 w 인덱스에 추가한다.

	printf("Dijkstra Pre Node : ");
	for(w = 0; w<n; w++)
		printf("%d\t", Dijkstra_node[w]); // Dijkstra_node 배열 출력
	printf("\n");

	printf("  Dijkstra Weight : ");
	for(w = 0; w<n; w++)
		printf("%d\t", distance[w]); // 가중치 배열 출력
	printf("\n");
}

