// �̰����� �����̳�, ť, ���ڿ� substinrg �� ���� �Լ��� ���� �ȴ�. 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
/*
		#######################################################################
		###		�����̰� �۾��ؾߵ� ����.. 									###
		###		��ũ�� ����Ʈ�� �� ����, ��ũ�� ����Ʈ�� �� ���� ť 		###
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

// ---------------------- ���ÿ� ���õ� �Լ� ---------------------------
Stack* createStack()
{
	Stack *pReturn = NULL;
	int i = 0;
	pReturn = (Stack *)malloc(sizeof(Stack));
	if (pReturn != NULL) {
		memset(pReturn, 0, sizeof(Stack));
	}
	else {
		printf("����, �޸��Ҵ�,createStack()\n");
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
			printf("����, �޸��Ҵ�,pushLS()\n");
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
// --------------------- @ ���ÿ� ���õ� �Լ� --------------------------


// --------------- ��ɾ� ����� ,�����ο� ���̴� ���ڿ� �Լ���.... ------------------------- 
char* substring( char* string , int i, int j)							// substr~!
{				
        int cnt = strlen(string); // �Է¹��� ���ڿ��� ����
		int size = j - i + 1 ;   // �ڸ����� ������ ũ�� 
		char * temp = (char *) malloc(j - i ); // 
		if(temp!=NULL){
			if( j <= cnt) {
				strncpy(temp , string+i, size );
				*(temp + j - i ) = '\0'; //  NULL �� ����! 
			}else{
			printf(" ���� , ���� �ʰ� ! [%d /%d]",j , cnt);
			return 0;
			}
		 }else{
			printf("�޸� �Ҵ� ���� ! ");
			return 0;
		 }
 return temp;
}

char* DeleteFirstSlash(char* Path)			// ������ ���� '/' ����..
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

		
char* DeleteSlash(char* Path)		// ���� ����Ͻ� �� ���� '/'�� �����Ͽ� strtok �Լ��� ��밡���ϵ��� �Ѵ�.
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
// --------------- ��ɾ� ����� ,�����ο� ���̴� ���ڿ� �Լ���.... ------------------------- 

// ----------------------- ť�� ���õ� �Լ� ----------------------------
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
			printf("����,�޸��Ҵ�,createArrayQueue()\n");
			return NULL;
		}
	}
	else {
		printf("����,ť ũ��� 0���� Ŀ�� �մϴ�\n");
		return NULL;
	}
	pReturn->pElement = (ArrayQueueNode *)malloc(sizeof(ArrayQueueNode) * maxElementCount);
	if (pReturn->pElement != NULL) {
		memset(pReturn->pElement, 0, sizeof(ArrayQueueNode) * maxElementCount);
	}
	else {
		printf("����, �޸� �Ҵ�2,createArrayQueue()\n");
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
			printf("����, ť�� ���� á���ϴ�, enqueueAQ()\n");
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
				printf("����,�޸��Ҵ�,dequeueAQ()\n");
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
		printf("ť�� ũ��: %d, ���� ��� ����: %d\n",
			pQueue->maxElementCount,
			pQueue->currentElementCount);

		maxElementIndex = pQueue->front + pQueue->currentElementCount;
		for(i = pQueue->front + 1; i <= maxElementIndex; i++) {
			pos = i % pQueue->maxElementCount;
			printf("[%d]-[%s]\n", pos, pQueue->pElement[ pos ].data);
		}
	}
}

// ---------------------- @ ť�� ���õ� �Լ� ---------------------------

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
	printf("  mkdir \t - ���丮 ���� \n");
	printf("        \t ��'-p' ���� ���丮���� �Բ� ����\n");
	printf("  touch \t - �� ���� ���� \n");
	printf("  cp \t - ���� �� ���丮 ����\n");
	printf("  mv \t - ���� �� ���丮 �̵� �� �̸�����\n");
	printf("  ls \t - ���丮 ���� ���� \n");
	printf("  ls \t ��'-al' �������� ���� �Բ� ǥ��\n");
	printf("  ll \t - ���丮 �ڼ��� ����\n");
	printf("  cd \t - ���丮 �̵� �ϱ�\n");
	printf("  rm \t - -rf - ���� ���丮���� �ѹ��� ����\n");
	printf("     \t ��    - ���� ����\n");
	printf("  rmdir \t - DeleteDirectory(dTree,command,1)\n");
	printf("  useradd  - ���� �߰�\n");
	printf("  passwd \t -������ ��й�ȣ�� ����\n");
	printf("  userdel  - ���� ����\n");
	printf("  chmod - ���丮 �� ������ �㰡�� ���� \n");
	printf("  chown - ���丮 �� ������ ����� �� �׷� ������ ����\n");
	printf("  cat \t - ���� ���� ����\n");
	printf("  clear \t - ȭ�� Ŭ����\n");
	printf("  ping \t - ping �׽�Ʈ)\n");
}