#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
/* 정윤이가 작업하게될 내용.. 
###########################################################################
###		UserAdd( 유저이름 );  // 유저이름으로 해당 유저를 만든다. 
###			--> 유저를 만드는 동시에 디렉토리를 만들고 , 해당 bash 파일이나 history 파일을 만든다.	
###		User
###
###
###		typedef struct UserDataType{
###			char name[20];
###			char directory[50];
###			char passwd[20];
###			int UID;
###			int GID;
###			char shel;
###			struct UserNodeType* pLink;
###			}UserNode;
###	
###		typedef struct UserDataType{
###			int UID;					// 일반 사용자가 추갈될시 불러올 UID 값 
###			int GID;					// 일반 사용자가 추가될수 불러올 GID 값 
###			char directory[50];			// 유저의 디렉토리가 생성될 기본 디렉토리
###			int currentUserCount;		// 현재 등록된 유저의 수 root 계정 포함..
###			char shel[20];				// 일반 사용자 추가시 저장될 기본 쉘 정보 
###			UserNode* headerNode;		// root 계정을 가르키는 headerNode
###		}UserList;
###
###########################################################################
*/
UserList* createUserList(char* passwd)
{
	UserList *pReturn = NULL;

	pReturn = (UserList *)malloc(sizeof(UserList));
	if (pReturn != NULL) {
		pReturn->headerNode = NULL;				// headerNode는 NULL 값으로 초기화 
		pReturn->UID = pReturn->GID = 500;		// 기본 생성시 부여되는 번호는 500번 부터 부여 되도록 한다. 
		strcpy(pReturn->directory,"/home/");	// 기본 생성시 유저의 디렉토리는 /home/ 디렉토리 밑으로 생성되도록 한다. 
		strcpy(pReturn->shel,"/bin/bash");		// 기본 생성시 유저쉘은 bash 쉘이다. 
		pReturn->currentUserCount = 0;
		AddUser(pReturn,"root");				// root 계정 생성..... 
		strncpy(pReturn->headerNode->directory,"root",50);
		strcpy(pReturn->headerNode->passwd,passwd);	// root 계정 생성후 패스워드 부여.... 
		pReturn->logincount = 0;
	}
	else {
		printf("오류, 메모리할당 UserList()\n");
		return NULL;
	}

	return pReturn;
}

BOOL AddUser(UserList* UList, char* username)
{
	BOOL ret = FALSE;					// 기본적으로 반환값을 FALSE로 지정하고 유저추가시 TRUE 로 반환한다. 
	UserNode* NewUser = NULL;			// 생성될 유저의 정보를 담는다. 
	UserNode* PrevUser = NULL;			// 생성될 유저를 연결하는데 쓰인다. 
	int i;								// 유저 추가시 for문에서 사용한다. 
	char temp[50];							// 디렉토리의 위치를 저장하는데 /home/+유저명 이되도록 하기위해서 임시로 쓰인다. 

	strncpy(temp,UList->directory,50);		// temp 값에다. UList가 지정하는 기본 위치 값을 대입한다. 
	NewUser = (UserNode* )malloc( sizeof( UserNode) );	// NewNode에 동적 할당..	
	if(NewUser != NULL)
	{
		NewUser->pLink = NULL;
		strcat(temp,"/");
		strcat(temp,username);				// /home/ + 유저명 
		strncpy(NewUser->name,username,20);		// 유저 이름 저장. 
		strncpy(NewUser->directory,temp,50);	// 유저 디렉토리 위치 저장 
		strncpy(NewUser->shel,UList->shel,20);	// 기본 쉘 저장 
		if(UList->headerNode == NULL)			// $$$$$ root 계정일 경우 ..
		{
			NewUser->GID = 0;
			NewUser->UID = 0;
			UList->headerNode = NewUser;
		}
		else									// $$$$$ 일반 사용자 계정일 경우 ..
		{
			if( strcmp(UList->currentLoginUser,"root") != 0 )
			{
				printf("not command excute\n");
				return FALSE;
			}
			if( IsExistUser(UList,username) == TRUE )
			{
				printf("useradd user %s exists\n",username);
				return FALSE;
			}
			NewUser->GID = (UList->GID)++;		// root 계정이 아닌경우..
			NewUser->UID = (UList->UID)++;		// root 계정이 아닌경우...
		
			PrevUser = UList->headerNode;
			for( i = 0 ; i < UList->currentUserCount-1; i++)	// 새로운 유저를 추가하기 위해 마지막 노드를 찾는다....
			{
				PrevUser = PrevUser->pLink;
			}
			PrevUser->pLink = NewUser;
			// ----------- 유저 디렉토리 생성 부분 ----------
			dTree->TempNode = dTree->currentNode;
			MoveCurrentNode2(dTree,UList->directory);
			MakeDirectory(dTree,username);
			ChangeOwnderItem(dTree, username, username, username);
			ChangeAccessItem(dTree, username, 700);
			dTree->currentNode = dTree->TempNode;
			strncpy(dTree->currentNodeName,dTree->currentNode,20);
			// -----------------------------------------------
		}
	
		(UList->currentUserCount)++;			// UList에 등록된 사용자 수를 한명더 늘린다. 
		ret = TRUE;
	}
	else
	{
		printf("[Error] NULL \n");
	}

	return ret;
}


void ShowUserList(UserList* UList)
{
	UserNode* temp = UList->headerNode;
	int i;
	if(temp != NULL)

	{
		for(i = 0 ; i <= UList->currentUserCount ; i++ )
		{
			if(temp == NULL)
				break;
			printf("%s:%d:%d:%s:%s\n",temp->name,temp->UID,temp->GID, temp->directory, temp->shel);
			temp = temp->pLink;
		}
	}
	else
	{
		printf("Empty List\n");
	}
}

BOOL DelUser(UserList* UList, char* username,int option)
{
	BOOL ret = FALSE;					// 기본적으로 반환값을 FALSE로 지정하고 유저추가시 TRUE 로 반환한다. 
	UserNode* DelUser = UList->headerNode;			// 삭제될 유저의 정보를 담는다. 
	UserNode* PrevUser = UList->headerNode;			// 삭제될 유저들의 연결에 사용된다.  
	
	if(strcmp(UList->currentLoginUser,"root")!=0)
	{
		printf("-bash : userdel : command not found\n");
		return FALSE;
	}
	if(strcmp(UList->currentLoginUser,username)==0)
	{
		printf("userdel: user %s is currently logged in\n",UList->currentLoginUser);
		return ret;
	}

	if(DelUser != NULL)
	{		
		while(DelUser->pLink != NULL)	// 새로운 유저를 추가하기 위해 마지막 노드를 찾는다....
		{
			if(strcmp(DelUser->name,username)==0)
			{
				break;
			}else
			{
				PrevUser = DelUser;
				DelUser = DelUser->pLink;
			}
		}
		if(strcmp(DelUser->name,username)==0)
		{
			char uname[4];
			char gname[4];
			itoa(DelUser->UID, uname, 10);
			itoa(DelUser->GID, gname, 10);
			(UList->GID)--;	
			(UList->UID)--;	

			// ----------- 유저 디렉토리 삭제 부분 ----------
			dTree->TempNode = dTree->currentNode;
			MoveCurrentNode2(dTree,DelUser->directory);
			strncpy(dTree->currentNode->username,uname,20);
			strncpy(dTree->currentNode->groupname,gname,20);
			//-------------------------------------------------
			//	'-r'	옵션이 있을경우 폴더 삭제하도록.. 추가 필요..
			//-------------------------------------------------
			dTree->currentNode = dTree->TempNode;
			strncpy(dTree->currentNodeName,dTree->currentNode,20);
			// -----------------------------------------------

			PrevUser->pLink = DelUser->pLink;
			free(DelUser);
			(UList->currentUserCount)--;			// UList에 등록된 사용자 수를 감소시킨다. 
			ret = TRUE;		
		}
		else
		{
			printf("userdel : user %s does not exist \n",username);
		}
	}
	else
	{
		printf("[Error] NULL \n");
	}

	return ret;
}
/*
void modUser(UserList* UList, char* username, char option, char* data)
{

}

//////////////////////
*/
BOOL passwdChange(UserList* UList, char* username, char* password)
{
	BOOL ret = FALSE; 
	UserNode* temp = UList->headerNode;

	while(temp->pLink != NULL)
	{
		if(!strcmp(temp->name,username)) break;
		temp=temp->pLink;
	}
	if(!strcmp(temp->name,username))
	{
		ret = TRUE;
		strncpy(temp->passwd, password, 20);
		return ret;
	}
	else
	{
		printf("%s를 찾지 못했습니다.\n", username);
		return ret;
	}

}

void passwd(UserList* UList, char* username)
{
	if(UList != NULL)
	{
		if(strcmp(UList->currentLoginUser,"root")!=0 )
		{
			if(strcmp(UList->currentLoginUser,username)!=0 )
			{
				printf("-bash : passwd : command not found\n");
				return;
			}
			else
			{

			}
		}
		if( IsExistUser(UList, username) == TRUE)
		{
			char password1[20]="";
			char password2[20];
			char c;
			int cnt=0;
			int i = 0;
			do{
				i = 0;
				if(cnt>0)
					printf("Sorry, passwords do not match.\n");
				printf ("New Unix password : ");
				while( ( c = getch() ) != '\r')
				{
					if(c == '\b')
					{
						i--;
						password1[i] = '\0';
					}
					else
					{
						password1[i] = c ;
						i++;
						printf ("");
					}
					if( i == 16777215 )
						i = 0;
				}
				password1[i] = '\0';printf("\n");
				i = 0;
				printf ("Retype New Unix password : ");
				while( ( c = getch() ) != '\r' )
				{
					if(c == '\b')
					{
						i--;
						password2[i] = '\0';
					}
					else
					{
						password2[i] = c ;
						i++;
						printf ("");
					}
					if( i == 16777215 )
						i = 0;
				}
				password2[i] = '\0';
				printf("\n");
				cnt++;
			}while( strcmp(password1,password2)!=0);
			passwdChange(UList,username,password1);
			printf("passwd : all authentication tokens updated successfully.\n");
			}
		else
		{
			printf("passwd : Unknown username'%s' .\n",username);
		}
	}
	else
	{
		return ;
	}

}


// 이부분 AddUser명령어 삽입해서 사용하는데 에러 발생해서 좀 고침..
BOOL IsExistUser(UserList* UList, char* username)
{
	BOOL ret = FALSE;
	UserNode* temp = UList->headerNode;
	while(temp->pLink!=NULL)
	{
		if(strcmp(temp->name,username)==0)
		{
			break;
		}
		temp = temp->pLink;
	}
	if(strcmp(username, temp->name)==0)
		{
			//printf("사용자 %s을(를) 찾았습니다~^^\n", username);
			ret = TRUE;
			return ret;
	}
	else
		return ret;

}

void ChangeUser(int option, char* username)		// option이 0 이면 디렉토리 변경 없음, 1 이면 디렉토리 변경함.
{
	BOOL state;
		if( UList!= NULL ) // UserList가 올바른지 확인한다. 
		{
			state = IsExistUser(UList,username);
			if( state == TRUE )		// user가 존재함 
			{
				char password[20]="";
				int i=0;
				char c;
				UserNode* temp = UList->headerNode;

				if( strcmp(UList->currentLoginUser,"root") == 0 )		// root 사용자일 경우. 비밀번호를 묻지 않는다.
				{
					(UList->logincount)++;
					strncpy(UList->currentLoginUser,username,20);
					Push(pLogin, username);		// 현재 사용자 Push
					while(temp->pLink!=NULL)	// 유저가 존재하므로 , temp를 이용해 유저를 찾는다. 
					{
						if(strcmp(temp->name,username)==0)
						{
							break;
						}
						temp = temp->pLink;
					}
				}
				else										// 일반 사용자일 경우 비밀번호를 물어 확인한다.
				{
					if( IsExistUser(UList, username) == FALSE)	// UserList에 유저가 존재하지 않으므로 함수를 종료한다...
					{
						printf("su: user %s dose not exist\n",username);
						return ;
					}
					while(temp->pLink!=NULL)	// 유저가 존재하므로 , temp를 이용해 유저를 찾는다. 
					{
						if(strcmp(temp->name,username)==0)
						{
							break;
						}
						temp = temp->pLink;
					}
					// ------------- password 를 물어본다. --------------
					printf("Password :");
					while( ( c = getch() ) != '\r' )
					{
						if(c == '\b')
						{
							i--;
							password[i] = '\0';
						}
						else
						{
							password[i] = c ;
							i++;
							printf ("");
						}
						if( i == 16777215 )
							i = 0;
					}
					// -------------------------------------------------

					if( strcmp(temp->passwd,password) == 0 )	// 입력한 패스워드와 유저의 패스워드가 일치한지를 검사한다. 
					{
						(UList->logincount)++;
						strncpy(UList->currentLoginUser,username,20);
						Push(pLogin, username);
						printf("\n");
					}
					else
					{
						printf("su: incorrect password \n",username);
						return ;
					}
				}
				if(option == 1){
					dTree->currentNode = dTree->headerNode;
					MoveCurrentNode2(dTree,temp->directory);
				}
			}
			else					// user가 존재하지 않음 
			{
				printf("su: user %s dose not exist\n",username);
			}
		}
		else
		{
			printf("올바르지 않은 userlist\n");
		}
}

void LoginUser()  // 로그인 부분 .. 
{
	BOOL state = FALSE;
	char username[20]="";
	char password[20]="";
	int i=0;
	char c;
	UserNode* temp = UList->headerNode;
	system("cls");
	printf("CentOS release 5.6(Final)\nKernel 2.6.18-238.el5xen on an 1686\n\n");
	if(UList != NULL)
	{
		do{
			printf("Login as : ");
			gets(username);

			printf("Password :");
			while( ( c = getch() ) != '\r' )
			{
				if(c == '\b')		// 백스페이스 기능이 먹히도록 
				{
					i--;
					password[i] = '\0';
				}
				else
				{
					password[i] = c ;
					i++;
				    printf ("");
				}
				if( i == 16777215 )
					i = 0;
			}
			
			if( IsExistUser(UList, username) == FALSE)	// UserList에 유저가 존재하지 않음.. ..
			{
				state = FALSE;
				printf("\nLogin incorrect\n");
			}
			else										// UserList에 유저가 존재함 
			{
				// ------------- 사용자가 존재하므로 사용자를 찾는다. -----------------
				while(temp->pLink!=NULL)
				{
					if(strcmp(temp->name,username)==0)
					{
						break;
					}
					temp = temp->pLink;
				}
				// ------------- 사용자가 존재하므로 사용자를 찾는다. -----------------
				if( strcmp(temp->passwd,password) == 0 )
				{
					state = TRUE;
				}
				else
				{
					state = FALSE;
					printf("\nLogin incorrect\n");
				}
					dTree->currentNode = dTree->headerNode;
					MoveCurrentNode2(dTree,temp->directory);
			}
			i = 0;	
			printf("\n");
			strncpy(password,"",20);	
		}while(state != TRUE);
		strncpy(UList->currentLoginUser,username,20);
		Push(pLogin, username);
		UList->logincount++;
	}
	else
	{
		printf("올바르지 않은 userlist\n");
	}
}
