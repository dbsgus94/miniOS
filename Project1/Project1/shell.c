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
	dTree = CreateRootDirectory();		// ���丮 ���� ���� [ �ֻ��� ���丮 ]
	UList = createUserList("1234");		// ���� ����Ʈ ����, �����ڰ� �Բ� ����

	pStack = createStack();				// ���� ���� 
	pLogin = createStack();				// �α��� ������ ���� ���� �ϴ� ���� ����
	pQueue = createArrayQueue(200);		// ��ɾ� ������ ���� ť ���� 
	Push(pLogin, "root");				// ���� ����ڸ� ���ؿ� �ø� 
	strncpy(UList->currentLoginUser,"root",20);	// ���� ����������� ��� 
	PopData(pLogin);					// ���� ������ ����� "root"�� ���� 
	WHITE;
	//--------------------- �̸� ����������� �����͵��� ���� ----------------------------
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
	//--------------------- �̸� ����������� �����͵��� ���� ----------------------------
	LoginUser();	// ����� �α��� ȭ�� 
	
	while(1)	// ��� ������Ʈ ���� ~! ���� ���� �� 
	{
		if( strcmp(UList->currentLoginUser,"root")==0 )
			represent = '#';		// �ְ� ����� root�ϰ�� 
		else
			represent = '$';		// �Ϲ� ������� ��� 
		WHITE;
		printf("[%s@localhost %s] %c ",UList->currentLoginUser,dTree->currentNodeName,represent);
		gets(command);
		VerifyCommand(command);			// command.c ���Ͽ��� �Է� ���� ��ɾ ó�� ��.
	}
	return 0;
}
