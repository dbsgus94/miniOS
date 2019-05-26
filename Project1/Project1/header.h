#include <windows.h>
#include "directory.h"
#include "user.h"
// -----------####### �ܼ� �ؽ�Ʈ ���� ���� .#######----------- 
#define col GetStdHandle(STD_OUTPUT_HANDLE) // �ܼ�â�� �ڵ����� �ޱ� 
#define RED SetConsoleTextAttribute( col,0x000c ); //������ 
#define BLUE SetConsoleTextAttribute( col,0x0001 | 0x0008); //�Ķ��� 
#define HIGH SetConsoleTextAttribute( col,0x00a); // ����
#define WHITE SetConsoleTextAttribute( col,0x000f); // ���
#define SKY SetConsoleTextAttribute( col, 0x000b); //�ϴû�
#define YEL SetConsoleTextAttribute( col, 0x000e); //�����
#define HIG SetConsoleTextAttribute( col, 0x000d); //����
#define GOLD SetConsoleTextAttribute( col, 0x0006); //�ݻ� 
#define JAJU SetConsoleTextAttribute( col, 0x0005); //���ֻ� 
#define PII SetConsoleTextAttribute( col, 0x0004); //�ǻ� 
#define GREEN SetConsoleTextAttribute( col, 0x0002); //��� 
// -----------####### �ܼ� �ؽ�Ʈ ���� ���� .#######----------- 
#ifndef _Linux_Shell_
#define _LInux_Shell_
#define TRUE		1
#define FALSE		0
// ���� ������ ��´�. 

DirTree* dTree;
UserList* UList;


void VerifyCommand(char* command);		// shell.c ���� ���� ��ɾ� ���� �Լ�...

// ---------------- util.c ���� ���̴� ����ü��...-----------------------------

typedef struct StackNodeType
{
	char String[50];
	struct StackNodeType* pLink;
} StackNode;

typedef struct StackType
{
	int currentElementCount;	// ���� ������ ����
	StackNode* pTopElement;		// Top ����� ������
} Stack;

typedef struct ArrayQueueNodeType
{
	char data[50];
} ArrayQueueNode;

typedef struct ArrayQueueType
{
	int maxElementCount;		// �ִ� ���� ����
	int currentElementCount;	// ���� ������ ����
	int front;					// front ��ġ
	int rear;					// rear ��ġ
	ArrayQueueNode *pElement;	// ��� ������ ���� 1���� �迭 ������
} ArrayQueue;


// ---------------- @util.c ���� ���̴� ����ü��...----------------------------
Stack* pStack;
Stack* pLogin;
ArrayQueue *pQueue;

// ---------------- util.c ( ���ð��� �Լ� ) ---------------------
Stack* createStack();
int Push(Stack* pStack, char* string);
char* PopData(Stack* pStack);
BOOL isLinkedStackEmpty(Stack* pStack);
// ---------------- @util.c ( ���ð��� �Լ� ) --------------------

// ---------------- util.c ( ť ���� �Լ� ) ---------------------
ArrayQueue* createArrayQueue(int maxElementCount);
int enqueueAQ(ArrayQueue* pQueue, char* command);
char* *dequeueAQ(ArrayQueue* pQueue);
ArrayQueueNode *peekAQ(ArrayQueue* pQueue);
BOOL isArrayQueueEmpty(ArrayQueue* pQueue);
BOOL isArrayQueueFull(ArrayQueue* pQueue);
void displayArrayQueue(ArrayQueue *pQueue);
// ---------------- util.c ( ť ���� �Լ� ) ---------------------

// ---------------- util.c ( ���ڿ����� �Լ� ) ---------------------
char* DeleteLastChar(char* Path,char t);
char* DeleteFirstSlash(char* Path);
char* DeleteSlash(char* Path);
BOOL IsCharExist(char* Path, char t);
char* substring( char* string , int i, int j);
// ---------------- @util.c ( ���ڿ����� �Լ� ) ---------------------

void help();
#endif