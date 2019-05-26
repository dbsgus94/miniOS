#include <windows.h>
#include "directory.h"
#include "user.h"
// -----------####### 콘솔 텍스트 색깔 지정 .#######----------- 
#define col GetStdHandle(STD_OUTPUT_HANDLE) // 콘솔창의 핸들정보 받기 
#define RED SetConsoleTextAttribute( col,0x000c ); //빨간색 
#define BLUE SetConsoleTextAttribute( col,0x0001 | 0x0008); //파란색 
#define HIGH SetConsoleTextAttribute( col,0x00a); // 연두
#define WHITE SetConsoleTextAttribute( col,0x000f); // 흰색
#define SKY SetConsoleTextAttribute( col, 0x000b); //하늘색
#define YEL SetConsoleTextAttribute( col, 0x000e); //노란색
#define HIG SetConsoleTextAttribute( col, 0x000d); //형광
#define GOLD SetConsoleTextAttribute( col, 0x0006); //금색 
#define JAJU SetConsoleTextAttribute( col, 0x0005); //자주색 
#define PII SetConsoleTextAttribute( col, 0x0004); //피색 
#define GREEN SetConsoleTextAttribute( col, 0x0002); //녹색 
// -----------####### 콘솔 텍스트 색깔 지정 .#######----------- 
#ifndef _Linux_Shell_
#define _LInux_Shell_
#define TRUE		1
#define FALSE		0
// 유저 정보를 담는다. 

DirTree* dTree;
UserList* UList;


void VerifyCommand(char* command);		// shell.c 에서 사용될 명령어 실행 함수...

// ---------------- util.c 에서 쓰이는 구조체들...-----------------------------

typedef struct StackNodeType
{
	char String[50];
	struct StackNodeType* pLink;
} StackNode;

typedef struct StackType
{
	int currentElementCount;	// 현재 원소의 개수
	StackNode* pTopElement;		// Top 노드의 포인터
} Stack;

typedef struct ArrayQueueNodeType
{
	char data[50];
} ArrayQueueNode;

typedef struct ArrayQueueType
{
	int maxElementCount;		// 최대 원소 개수
	int currentElementCount;	// 현재 원소의 개수
	int front;					// front 위치
	int rear;					// rear 위치
	ArrayQueueNode *pElement;	// 노드 저장을 위한 1차원 배열 포인터
} ArrayQueue;


// ---------------- @util.c 에서 쓰이는 구조체들...----------------------------
Stack* pStack;
Stack* pLogin;
ArrayQueue *pQueue;

// ---------------- util.c ( 스택관련 함수 ) ---------------------
Stack* createStack();
int Push(Stack* pStack, char* string);
char* PopData(Stack* pStack);
BOOL isLinkedStackEmpty(Stack* pStack);
// ---------------- @util.c ( 스택관련 함수 ) --------------------

// ---------------- util.c ( 큐 관련 함수 ) ---------------------
ArrayQueue* createArrayQueue(int maxElementCount);
int enqueueAQ(ArrayQueue* pQueue, char* command);
char* *dequeueAQ(ArrayQueue* pQueue);
ArrayQueueNode *peekAQ(ArrayQueue* pQueue);
BOOL isArrayQueueEmpty(ArrayQueue* pQueue);
BOOL isArrayQueueFull(ArrayQueue* pQueue);
void displayArrayQueue(ArrayQueue *pQueue);
// ---------------- util.c ( 큐 관련 함수 ) ---------------------

// ---------------- util.c ( 문자열관련 함수 ) ---------------------
char* DeleteLastChar(char* Path,char t);
char* DeleteFirstSlash(char* Path);
char* DeleteSlash(char* Path);
BOOL IsCharExist(char* Path, char t);
char* substring( char* string , int i, int j);
// ---------------- @util.c ( 문자열관련 함수 ) ---------------------

void help();
#endif