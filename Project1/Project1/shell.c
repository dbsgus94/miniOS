#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "header.h"


int main(void)
{
	int i=0;
    FILE *fp;
    char *pstr;
	char* temp;
	char represent ='#';
	char buff[50];
	char command[30];
	dTree = CreateRootDirectory();		// 디렉토리 구조 생성 [ 최상위 디렉토리 ]
	UList = createUserList("1234");		// 유저 리스트 생성, 관리자가 함께 생성

	pStack = createStack();				// 스텍 생성 
	pLogin = createStack();				// 로그인 유저가 쌓이 도록 하는 스텍 생성
	pQueue = createArrayQueue(200);		// 명령어 저장을 위한 큐 생성 
	Push(pLogin, "root");				// 현재 사용자를 스텍에 올림 
	strncpy(UList->currentLoginUser,"root",20);	// 현재 사용자정보를 기록 
	PopData(pLogin);					// 현재 접속한 사용자 "root"를 빼냄 
	WHITE;
	//--------------------- 미리 만들어져야할 데이터들을 구성 ----------------------------
	fp = fopen("initialize.txt", "r");
	while(!feof(fp)){
        pstr = fgets(buff, 100, fp);
		i = strlen(pstr);
		temp = substring(pstr,0,i-2);
		if(pstr == NULL)
                break;
		VerifyCommand(temp);
    }
	fclose(fp);
	//--------------------- 미리 만들어져야할 데이터들을 구성 ----------------------------
	LoginUser();	// 사용자 로그인 화면 
	
	while(1)	// 명령 프롬포트 실행 ~! 무한 실행 됨 
	{
		if( strcmp(UList->currentLoginUser,"root")==0 )
			represent = '#';		// 최고 사용자 root일경우 
		else
			represent = '$';		// 일반 사용자일 경우 
		WHITE;
		printf("[%s@localhost %s] %c ",UList->currentLoginUser,dTree->currentNodeName,represent);
		gets(command);
		VerifyCommand(command);			// command.c 파일에서 입력 받은 명령어를 처리 함.
	}
	return 0;
}
