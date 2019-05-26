// 이곳에는 스텍이나, 큐, 문자열 substinrg 과 같은 함수가 정의 된다. 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
/*
		#######################################################################
		###		혜원이가 작업해야될 내용.. 									###
		###		링크드 리스트로 된 스택, 링크드 리스트로 된 원형 큐 		###
		###		typedef struct StackNodeType								###
		###		{															###
		###			char String[50];										###
		###			struct StackNodeType* NextNode;							###
		###		}StackNode;													###
		###																	###
		###		typedef struct StackType									###
		###		{															###
		###			StackNode* TOP;											###
		###		}Stack;														###
		#######################################################################
*/

// ---------------------- 스택에 관련된 함수 ---------------------------
Stack* createStack()
{
	Stack *pReturn = NULL;
	int i = 0;
	pReturn = (Stack *)malloc(sizeof(Stack));
	if (pReturn != NULL) {
		memset(pReturn, 0, sizeof(Stack));
	}
	else {
		printf("오류, 메모리할당,createStack()\n");
		return NULL;
	}

	return pReturn;
}

BOOL Push(Stack* pStack, char* string)
{
	int ret = FALSE;
	StackNode *pNode = NULL;

	if (pStack != NULL) {
		pNode = (StackNode *)malloc(sizeof(StackNode));
		if (pNode != NULL) {
			memset(pNode, 0, sizeof(StackNode));
			strncpy(pNode->String,string,50);
			pNode->pLink = pStack->pTopElement;
			pStack->pTopElement = pNode;
			  
			pStack->currentElementCount++;
			ret = TRUE;
		}
		else {
			printf("오류, 메모리할당,pushLS()\n");
		}
	}

	return ret;
}

BOOL isLinkedStackEmpty(Stack* pStack)
{
	BOOL ret = FALSE;

	if (pStack != NULL) {
		if (pStack->currentElementCount == 0) {
			ret = TRUE;
		}
	}

	return ret;
}

char* PopData(Stack* pStack)
{
	StackNode* pReturn = NULL;
	if (pStack != NULL)	{
		if (isLinkedStackEmpty(pStack) == FALSE) {

			pReturn = pStack->pTopElement;
		
			pStack->pTopElement = pReturn->pLink;
			pReturn->pLink = NULL;

			pStack->currentElementCount--;
			return &(pReturn->String);
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}
// --------------------- @ 스택에 관련된 함수 --------------------------


// --------------- 명령어 실행시 ,절대경로에 쓰이는 문자열 함수들.... ------------------------- 
char* substring( char* string , int i, int j)							// substr~!
{				
        int cnt = strlen(string); // 입력받은 문자열의 길이
		int size = j - i + 1 ;   // 자르려는 구간의 크기 
		char * temp = (char *) malloc(j - i ); // 
		if(temp!=NULL){
			if( j <= cnt) {
				strncpy(temp , string+i, size );
				*(temp + j - i ) = '\0'; //  NULL 값 삽입! 
			}else{
			printf(" 오류 , 범위 초과 ! [%d /%d]",j , cnt);
			return 0;
			}
		 }else{
			printf("메모리 할당 오류 ! ");
			return 0;
		 }
 return temp;
}

char* DeleteFirstSlash(char* Path)			// 절대경로 앞의 '/' 제거..
{
	int i = strlen(Path);
	char *temp = NULL;
	if( Path[0] == '/')
	{
		temp = substring(Path,1,i);
	}
	return temp;
}

char* DeleteLastChar(char* Path,char t)
{
	int i = strlen(Path);
	char *temp = NULL;
	if( Path[i-1] == t)
	{
		temp = substring(Path,0,i-1);
	}
	return temp;
}

		
char* DeleteSlash(char* Path)		// 절대 경로일시 양 끝의 '/'를 제거하여 strtok 함수가 사용가능하도록 한다.
{
	int i = strlen(Path);
	char *temp = NULL;
	if( Path[0] == '/')
	{
		temp = DeleteFirstSlash(Path);
	}
	if( Path[i-1] == '/')
	{
		temp = DeleteLastChar(Path,'/');
	}
	return temp;
}
// --------------- 명령어 실행시 ,절대경로에 쓰이는 문자열 함수들.... ------------------------- 

// ----------------------- 큐와 관련된 함수 ----------------------------
ArrayQueue* createArrayQueue(int maxElementCount)
{
	ArrayQueue *pReturn = NULL;
	int i = 0;

	if (maxElementCount > 0) {
		pReturn = (ArrayQueue *)malloc(sizeof(ArrayQueue));
		if (pReturn != NULL) {
			memset(pReturn, 0, sizeof(ArrayQueue));
			pReturn->maxElementCount = maxElementCount;
			pReturn->front = -1;
			pReturn->rear = -1;
		}
		else {
			printf("오류,메모리할당,createArrayQueue()\n");
			return NULL;
		}
	}
	else {
		printf("오류,큐 크기는 0보다 커야 합니다\n");
		return NULL;
	}
	pReturn->pElement = (ArrayQueueNode *)malloc(sizeof(ArrayQueueNode) * maxElementCount);
	if (pReturn->pElement != NULL) {
		memset(pReturn->pElement, 0, sizeof(ArrayQueueNode) * maxElementCount);
	}
	else {
		printf("오류, 메모리 할당2,createArrayQueue()\n");
		free(pReturn);
		return NULL;
	}
	return pReturn;
}

BOOL enqueueAQ(ArrayQueue* pQueue, char* command)
{
	BOOL ret = FALSE;
	int i=0;

	if (pQueue != NULL) {
		if (isArrayQueueFull(pQueue) == FALSE) {
			pQueue->rear = (pQueue->rear + 1) % pQueue->maxElementCount;
			strncpy(pQueue->pElement[pQueue->rear].data,command,50);
			pQueue->currentElementCount++;
			ret = TRUE;
		}
		else {
			printf("오류, 큐가 가득 찼습니다, enqueueAQ()\n");
		}
	}

	return ret;
}
BOOL isArrayQueueEmpty(ArrayQueue* pQueue)
{
	BOOL ret = FALSE;

	if (pQueue != NULL) {
		if (pQueue->currentElementCount == 0) {
			ret = TRUE;
		}
	}

	return ret;
}
char* *dequeueAQ(ArrayQueue* pQueue)
{
	ArrayQueueNode *pReturn = NULL;
	if (pQueue != NULL) {
		if (isArrayQueueEmpty(pQueue) == FALSE) {
			pReturn = (ArrayQueueNode *)malloc(sizeof(ArrayQueueNode));
			if (pReturn != NULL) {
				pQueue->front = (pQueue->front + 1) % pQueue->maxElementCount;
				strncpy(pReturn->data,pQueue->pElement[ pQueue->front ].data,50);
				pQueue->currentElementCount--;
			}
			else {
				printf("오류,메모리할당,dequeueAQ()\n");
			}
		}
	}

	return &(pReturn->data);
}

BOOL isArrayQueueFull(ArrayQueue* pQueue)
{
	BOOL ret = FALSE;

	if (pQueue != NULL) {
		if (pQueue->currentElementCount == pQueue->maxElementCount) {
			ret = TRUE;
		}
	}

	return ret;
}

void displayArrayQueue(ArrayQueue *pQueue)
{
	int i = 0, maxElementIndex = 0, pos = 0;
	if (pQueue != NULL) {
		printf("큐의 크기: %d, 현재 노드 개수: %d\n",
			pQueue->maxElementCount,
			pQueue->currentElementCount);

		maxElementIndex = pQueue->front + pQueue->currentElementCount;
		for(i = pQueue->front + 1; i <= maxElementIndex; i++) {
			pos = i % pQueue->maxElementCount;
			printf("[%d]-[%s]\n", pos, pQueue->pElement[ pos ].data);
		}
	}
}

// ---------------------- @ 큐와 관련된 함수 ---------------------------

BOOL IsCharExist(char* Path, char t)
{
	int i = strlen(Path);
	int k,cnt=0;
	for( k = 0 ; k < i ; k++)
	{
		if(Path[k] ==t){
			cnt++;
		}
	}
	if( cnt==0)
		return FALSE;
	else
		return TRUE;

}


void help()
{
	printf("  mkdir \t - 디렉토리 생성 \n");
	printf("        \t ㄴ'-p' 하위 디렉토리까지 함께 생성\n");
	printf("  touch \t - 빈 파일 생성 \n");
	printf("  cp \t - 파일 및 디렉토리 복사\n");
	printf("  mv \t - 파일 및 디렉토리 이동 및 이름변경\n");
	printf("  ls \t - 디렉토리 간략 보기 \n");
	printf("  ls \t ㄴ'-al' 숨김파일 까지 함께 표시\n");
	printf("  ll \t - 디렉토리 자세히 보기\n");
	printf("  cd \t - 디렉토리 이동 하기\n");
	printf("  rm \t - -rf - 하위 디렉토리까지 한번에 삭제\n");
	printf("     \t ㄴ    - 파일 삭제\n");
	printf("  rmdir \t - DeleteDirectory(dTree,command,1)\n");
	printf("  useradd  - 유저 추가\n");
	printf("  passwd \t -유저의 비밀번호를 설정\n");
	printf("  userdel  - 유저 삭제\n");
	printf("  chmod - 디렉토리 및 파일의 허가권 변경 \n");
	printf("  chown - 디렉토리 및 파일의 사용자 및 그룹 소유권 변경\n");
	printf("  cat \t - 파일 내용 보기\n");
	printf("  clear \t - 화면 클리어\n");
	printf("  ping \t - ping 테스트)\n");
}