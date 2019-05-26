#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
/* �����̰� �۾��ϰԵ� ����.. 
###########################################################################
###		UserAdd( �����̸� );  // �����̸����� �ش� ������ �����. 
###			--> ������ ����� ���ÿ� ���丮�� ����� , �ش� bash �����̳� history ������ �����.	
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
###			int UID;					// �Ϲ� ����ڰ� �߰��ɽ� �ҷ��� UID �� 
###			int GID;					// �Ϲ� ����ڰ� �߰��ɼ� �ҷ��� GID �� 
###			char directory[50];			// ������ ���丮�� ������ �⺻ ���丮
###			int currentUserCount;		// ���� ��ϵ� ������ �� root ���� ����..
###			char shel[20];				// �Ϲ� ����� �߰��� ����� �⺻ �� ���� 
###			UserNode* headerNode;		// root ������ ����Ű�� headerNode
###		}UserList;
###
###########################################################################
*/
UserList* createUserList(char* passwd)
{
	UserList *pReturn = NULL;

	pReturn = (UserList *)malloc(sizeof(UserList));
	if (pReturn != NULL) {
		pReturn->headerNode = NULL;				// headerNode�� NULL ������ �ʱ�ȭ 
		pReturn->UID = pReturn->GID = 500;		// �⺻ ������ �ο��Ǵ� ��ȣ�� 500�� ���� �ο� �ǵ��� �Ѵ�. 
		strcpy(pReturn->directory,"/home/");	// �⺻ ������ ������ ���丮�� /home/ ���丮 ������ �����ǵ��� �Ѵ�. 
		strcpy(pReturn->shel,"/bin/bash");		// �⺻ ������ �������� bash ���̴�. 
		pReturn->currentUserCount = 0;
		AddUser(pReturn,"root");				// root ���� ����..... 
		strncpy(pReturn->headerNode->directory,"root",50);
		strcpy(pReturn->headerNode->passwd,passwd);	// root ���� ������ �н����� �ο�.... 
		pReturn->logincount = 0;
	}
	else {
		printf("����, �޸��Ҵ� UserList()\n");
		return NULL;
	}

	return pReturn;
}

BOOL AddUser(UserList* UList, char* username)
{
	BOOL ret = FALSE;					// �⺻������ ��ȯ���� FALSE�� �����ϰ� �����߰��� TRUE �� ��ȯ�Ѵ�. 
	UserNode* NewUser = NULL;			// ������ ������ ������ ��´�. 
	UserNode* PrevUser = NULL;			// ������ ������ �����ϴµ� ���δ�. 
	int i;								// ���� �߰��� for������ ����Ѵ�. 
	char temp[50];							// ���丮�� ��ġ�� �����ϴµ� /home/+������ �̵ǵ��� �ϱ����ؼ� �ӽ÷� ���δ�. 

	strncpy(temp,UList->directory,50);		// temp ������. UList�� �����ϴ� �⺻ ��ġ ���� �����Ѵ�. 
	NewUser = (UserNode* )malloc( sizeof( UserNode) );	// NewNode�� ���� �Ҵ�..	
	if(NewUser != NULL)
	{
		NewUser->pLink = NULL;
		strcat(temp,"/");
		strcat(temp,username);				// /home/ + ������ 
		strncpy(NewUser->name,username,20);		// ���� �̸� ����. 
		strncpy(NewUser->directory,temp,50);	// ���� ���丮 ��ġ ���� 
		strncpy(NewUser->shel,UList->shel,20);	// �⺻ �� ���� 
		if(UList->headerNode == NULL)			// $$$$$ root ������ ��� ..
		{
			NewUser->GID = 0;
			NewUser->UID = 0;
			UList->headerNode = NewUser;
		}
		else									// $$$$$ �Ϲ� ����� ������ ��� ..
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
			NewUser->GID = (UList->GID)++;		// root ������ �ƴѰ��..
			NewUser->UID = (UList->UID)++;		// root ������ �ƴѰ��...
		
			PrevUser = UList->headerNode;
			for( i = 0 ; i < UList->currentUserCount-1; i++)	// ���ο� ������ �߰��ϱ� ���� ������ ��带 ã�´�....
			{
				PrevUser = PrevUser->pLink;
			}
			PrevUser->pLink = NewUser;
			// ----------- ���� ���丮 ���� �κ� ----------
			dTree->TempNode = dTree->currentNode;
			MoveCurrentNode2(dTree,UList->directory);
			MakeDirectory(dTree,username);
			ChangeOwnderItem(dTree, username, username, username);
			ChangeAccessItem(dTree, username, 700);
			dTree->currentNode = dTree->TempNode;
			strncpy(dTree->currentNodeName,dTree->currentNode,20);
			// -----------------------------------------------
		}
	
		(UList->currentUserCount)++;			// UList�� ��ϵ� ����� ���� �Ѹ�� �ø���. 
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
	BOOL ret = FALSE;					// �⺻������ ��ȯ���� FALSE�� �����ϰ� �����߰��� TRUE �� ��ȯ�Ѵ�. 
	UserNode* DelUser = UList->headerNode;			// ������ ������ ������ ��´�. 
	UserNode* PrevUser = UList->headerNode;			// ������ �������� ���ῡ ���ȴ�.  
	
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
		while(DelUser->pLink != NULL)	// ���ο� ������ �߰��ϱ� ���� ������ ��带 ã�´�....
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

			// ----------- ���� ���丮 ���� �κ� ----------
			dTree->TempNode = dTree->currentNode;
			MoveCurrentNode2(dTree,DelUser->directory);
			strncpy(dTree->currentNode->username,uname,20);
			strncpy(dTree->currentNode->groupname,gname,20);
			//-------------------------------------------------
			//	'-r'	�ɼ��� ������� ���� �����ϵ���.. �߰� �ʿ�..
			//-------------------------------------------------
			dTree->currentNode = dTree->TempNode;
			strncpy(dTree->currentNodeName,dTree->currentNode,20);
			// -----------------------------------------------

			PrevUser->pLink = DelUser->pLink;
			free(DelUser);
			(UList->currentUserCount)--;			// UList�� ��ϵ� ����� ���� ���ҽ�Ų��. 
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
		printf("%s�� ã�� ���߽��ϴ�.\n", username);
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


// �̺κ� AddUser��ɾ� �����ؼ� ����ϴµ� ���� �߻��ؼ� �� ��ħ..
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
			//printf("����� %s��(��) ã�ҽ��ϴ�~^^\n", username);
			ret = TRUE;
			return ret;
	}
	else
		return ret;

}

void ChangeUser(int option, char* username)		// option�� 0 �̸� ���丮 ���� ����, 1 �̸� ���丮 ������.
{
	BOOL state;
		if( UList!= NULL ) // UserList�� �ùٸ��� Ȯ���Ѵ�. 
		{
			state = IsExistUser(UList,username);
			if( state == TRUE )		// user�� ������ 
			{
				char password[20]="";
				int i=0;
				char c;
				UserNode* temp = UList->headerNode;

				if( strcmp(UList->currentLoginUser,"root") == 0 )		// root ������� ���. ��й�ȣ�� ���� �ʴ´�.
				{
					(UList->logincount)++;
					strncpy(UList->currentLoginUser,username,20);
					Push(pLogin, username);		// ���� ����� Push
					while(temp->pLink!=NULL)	// ������ �����ϹǷ� , temp�� �̿��� ������ ã�´�. 
					{
						if(strcmp(temp->name,username)==0)
						{
							break;
						}
						temp = temp->pLink;
					}
				}
				else										// �Ϲ� ������� ��� ��й�ȣ�� ���� Ȯ���Ѵ�.
				{
					if( IsExistUser(UList, username) == FALSE)	// UserList�� ������ �������� �����Ƿ� �Լ��� �����Ѵ�...
					{
						printf("su: user %s dose not exist\n",username);
						return ;
					}
					while(temp->pLink!=NULL)	// ������ �����ϹǷ� , temp�� �̿��� ������ ã�´�. 
					{
						if(strcmp(temp->name,username)==0)
						{
							break;
						}
						temp = temp->pLink;
					}
					// ------------- password �� �����. --------------
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

					if( strcmp(temp->passwd,password) == 0 )	// �Է��� �н������ ������ �н����尡 ��ġ������ �˻��Ѵ�. 
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
			else					// user�� �������� ���� 
			{
				printf("su: user %s dose not exist\n",username);
			}
		}
		else
		{
			printf("�ùٸ��� ���� userlist\n");
		}
}

void LoginUser()  // �α��� �κ� .. 
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
				if(c == '\b')		// �齺���̽� ����� �������� 
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
			
			if( IsExistUser(UList, username) == FALSE)	// UserList�� ������ �������� ����.. ..
			{
				state = FALSE;
				printf("\nLogin incorrect\n");
			}
			else										// UserList�� ������ ������ 
			{
				// ------------- ����ڰ� �����ϹǷ� ����ڸ� ã�´�. -----------------
				while(temp->pLink!=NULL)
				{
					if(strcmp(temp->name,username)==0)
					{
						break;
					}
					temp = temp->pLink;
				}
				// ------------- ����ڰ� �����ϹǷ� ����ڸ� ã�´�. -----------------
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
		printf("�ùٸ��� ���� userlist\n");
	}
}
