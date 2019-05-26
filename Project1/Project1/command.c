// 명령어들을 처리하는 함수들...
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "header.h"


void VerifyCommand(char* command)
{
	char temp[50];
	char* define1 =" ";
	char* commnad1 = NULL;
	strncpy(temp,command,50);
	if(strcmp(command,"")==0)				// 단순 엔터 입력시, 아래 strtok로 인한 프로그램 종료 방지... 
	{
		return;
	}
	commnad1 = strtok( command, define1);	// 명령어 이름을 입력 받고, 해당 명령어의 이름을 가져옴 
	//hi
	if( strcmp(commnad1, "mkdir") == 0 )
	{
		command = strtok( NULL, define1 );		// 명령어 입력후, 뒤에 디렉토리 명이나 경로가 있을시... " " 공백 기준으로 끊어서 가져옴
		if(command != NULL )
		{
			char* str1 = NULL;
			char* div = "/";					// 경로 구분을 위한 구분자 '/'
			char temp2[20];
			BOOL ret = TRUE;
			if(strcmp(command,"-p") ==0 )			// '-p' 옵션이 있을 경우 .....
			{

				dTree->TempNode = dTree->currentNode;		// 현재위치를 저장
				command = strtok( NULL, define1 ); 
				// -------------- 만약 경로가 없을 경우에는.... 메시지를 띄우고 종료한다. ------------
				if( command == NULL )
				{
					printf("mkdir: missing operand\n");		// 경로 입력이 없을 경우 .... 
					return;
				}
				if( command[0] == '/')						// 처음이 '/'로 시작하므로, 절대경로를 나타낸다. 
				{
					dTree->currentNode = dTree->headerNode;		//절대 경로는 최상위 디렉토리 이므로 ,dTree에 headerNode로 저장된 
				}												// 값을 currentNode 로 저장해서 위치를 최상위 디렉토리로 변경한다.

				DeleteSlash(command);// "/" 를 제거 한다.		// 마지막과 앞의 '/'를 잘라낸다. 
				str1 = strtok(command, div);		// "/" 기준으로 하나 하나 불러낸다. 

				// 경로를 하나 하나 조사 해서 위치가 존재하는지를 조사 ........
				while( str1 != NULL )		//경로의 이름의 값이 없을 때 까지 조샇산다. 
				{
					ret = MoveCurrentNode(dTree, str1);		// 하나하나 MoveCurrentNode를 이용하여 이동한다. 
					if(ret == FALSE )						// 해당 경로가 없다면, "-p"옵션에 의해, 디렉토리를 만들고 해당 이름으로 이동한다. 
					{
						MakeDirectory(dTree,str1);
						MoveCurrentNode(dTree, str1);
					}
					str1 = strtok(NULL, div );
					if(str1 == NULL)						// str1값이 NULL 일 경우에는, 더이상의 탐색을 하지 않는다. 
						break;
				}

				dTree->currentNode = dTree->TempNode;							// 현재 위치를 이전 위치로 되돌아 가기 위해 변경 
				strncpy(dTree->currentNodeName,dTree->currentNode->name,20);	// 쉘에서의 디렉토리 이름이 제대로 출력 되기 위해 변경 
			}
			else
			{
				dTree->TempNode = dTree->currentNode;		// 현재위치를 저장
				DeleteSlash(command);// "/" 를 제거 한다. 

				if( command[0] == '/')						// 절대 경로인경우 
				{
					dTree->currentNode = dTree->headerNode;
				}
				if( IsCharExist(command,'/') == FALSE )		// 만약 입력받은 경로에 /가 존재 하지 않는다면, 현재 위치의 존재하는 디렉토리이므로...
				{											// 단순 이동시킨다. 
					MakeDirectory(dTree,command);
				}
				else
				{
					str1 = strtok(command, div);		// "/" 기준으로 하나 하나 불러낸다. 
					while( str1 != NULL )		//경로의 이름의 값이 없을 때 까지 조샇산다. 
					{
						ret = MoveCurrentNode(dTree, str1);		// 하나하나 이동한다. 
						if(ret == FALSE )
						{
							strncpy(temp2,str1,20);
							str1 = strtok(NULL, div );
							if( str1 == NULL )
							{
								MakeDirectory(dTree,temp2);
							}
							else
							{
								printf("mkdir: '%s' 디렉토리를 만들 수 없습니다: 그런 파일이나 디렉토리가 없음 \n",temp2);
								dTree->currentNode = dTree->TempNode;							// 기존 저장 위치로 ..위치 변경
								strncpy(dTree->currentNodeName,dTree->currentNode->name,20);	// 
								return;
							}
						}
						str1 = strtok(NULL, div );
						if(str1 == NULL)
							break;
					}
				}

				dTree->currentNode = dTree->TempNode;							// 기존 저장 위치로 ..위치 변경
				strncpy(dTree->currentNodeName,dTree->currentNode->name,20);	// 
			}
			enqueueAQ(pQueue,temp);
		}
		else
		{
			printf("mkdir: missing operand\n");		// 단순히 mkdir만 입력 하였을시.... 
		}
	}
	else if( strcmp(commnad1, "ll") == 0 )
	{
		command = strtok( NULL, define1 );
		if(command != NULL)
		{
				BOOL ret = TRUE;
			if(strcmp(command,"-al")==0 )	// "-al" 옵션이 있는경우..
			{
				command = strtok( NULL, define1 ); 
				if(command != NULL)					// 뒤의 절대 및 상대경로가 있을경우.
				{
					dTree->TempNode = dTree->currentNode;			// 현재 위치를 저장 
					ret = MoveCurrentNode2(dTree,command);
					if ( ret == TRUE )
						Commnad_LL(dTree,1);
					printf("");
					dTree->currentNode = dTree->TempNode;
					strncpy(dTree->currentNodeName,dTree->currentNode->name,20);
					enqueueAQ(pQueue,temp);
				}
				else						// 뒤의 절대 및 상대경로가 없는 경우
				{
					Commnad_LL(dTree,1);
					enqueueAQ(pQueue,temp);
				}
			}
			else							// "-al" 옵션이 없는 경우..
			{
				dTree->TempNode = dTree->currentNode;
				ret = MoveCurrentNode2(dTree,command);
				if ( ret == TRUE )
					Commnad_LL(dTree,0);
				printf("");
				dTree->currentNode = dTree->TempNode;
				strncpy(dTree->currentNodeName,dTree->currentNode->name,20);
				enqueueAQ(pQueue,temp);
			}
			return ;
		}
		else
		{
			Commnad_LL(dTree,0);
			enqueueAQ(pQueue,temp);
		}
	}
	else if( strcmp(commnad1, "ls") == 0 )
	{
		command = strtok( NULL, define1 );
		if(command != NULL)
		{
				BOOL ret = TRUE;
			if(strcmp(command,"-al")==0 )	// "-al" 옵션이 있는경우..
			{
				command = strtok( NULL, define1 ); 
				if(command != NULL)					// 뒤의 절대 및 상대경로가 있을경우.
				{
					dTree->TempNode = dTree->currentNode;
					ret = MoveCurrentNode2(dTree,command);
					if ( ret == TRUE )
						Commnad_LL(dTree,1);
					printf("");
					dTree->currentNode = dTree->TempNode;
					strncpy(dTree->currentNodeName,dTree->currentNode->name,20);
					enqueueAQ(pQueue,temp);
				}
				else						// 뒤의 절대 및 상대경로가 있을경우.
				{
					Commnad_LL(dTree,1);
					enqueueAQ(pQueue,temp);
				}
			}
			else							// "-al" 옵션이 없는 경우..
			{
				dTree->TempNode = dTree->currentNode;
				ret = MoveCurrentNode2(dTree,command);
				if ( ret == TRUE )
					Commnad_LS(dTree,0);
				printf("");
				dTree->currentNode = dTree->TempNode;
				strncpy(dTree->currentNodeName,dTree->currentNode->name,20);
				enqueueAQ(pQueue,temp);
			}
			return ;
		}
		else
		{
			Commnad_LS(dTree,0);
			enqueueAQ(pQueue,temp);
		}
	}
	else if( strcmp(commnad1, "chmod") == 0 )
	{
		char command2[20] ="";
		int chmod;
		command = strtok( NULL, define1 );
		chmod = atoi(command);					// 입력받은 문자열을 int 로 변환 ... 
		strncpy(command2,command,20);			// 나중에 오류 발생시, 해당 경로가 존재 하지 않는다고 띄워 주기 위해서... 지금의 command 값 임시 저장
		command = strtok( NULL, define1 );
		if(command != NULL)
		{	
			char* str1 = NULL;
			char* div = "/";
			char temp2[20];
			BOOL ret = TRUE;
			

			if( IsCharExist(command,'/') == FALSE )
			{
				BOOL ret = TRUE;
				dTree->TempNode = dTree->currentNode;
				if ( ret == TRUE ){
				
					ChangeAccessItem(dTree, command, chmod);
					enqueueAQ(pQueue,temp);
				}
				dTree->currentNode = dTree->TempNode;
				strncpy(dTree->currentNodeName,dTree->currentNode->name,20);
				return ;
			}
			else
			{
				dTree->TempNode = dTree->currentNode;
				if( command[0] == '/')
				{
					dTree->currentNode = dTree->headerNode;
				}
				DeleteSlash(command);// "/" 를 제거 한다. 
				str1 = strtok(command, div);		// "/" 기준으로 하나 하나 불러낸다. 

				// 경로를 하나 하나 조사 해서 위치가 존재하는지를 조사 ........
				while( str1 != NULL )		//경로의 이름의 값이 없을 때 까지 조샇산다. 
				{
					ret = MoveCurrentNode(dTree, str1);		// 하나하나 이동한다. 
					if(ret == FALSE )
					{
						return;		// 종료 
					}
					strncpy(temp2,str1,20);
					str1 = strtok(NULL, div );
					if(str1 == NULL)
						break;
				}
				dTree->currentNode= dTree->currentNode->Parent;
				if ( ret == TRUE ){
					ChangeAccessItem(dTree, temp2, chmod);
					enqueueAQ(pQueue,temp);
				}
			}
			dTree->currentNode = dTree->TempNode;
			strncpy(dTree->currentNodeName,dTree->currentNode->name,20);
			return ;
		}
		else
		{
			//Commnad_LS(dTree,0);
			printf("chmod: missing operand\n");	
		}
	}
	else if( strcmp(commnad1, "touch") == 0 )	
	{
		command = strtok( NULL, define1 );
		MakeFile(dTree,command);
		enqueueAQ(pQueue,temp);
	}
	else if( strcmp(commnad1, "cd") == 0 )  // 수정 필요 
	{
		command = strtok( NULL, define1 );
		if(command == NULL)		// 주소 공백일시..
				return;
		MoveCurrentNode2(dTree,command);
		enqueueAQ(pQueue,temp);
	}
	else if( strcmp(commnad1, "cp") == 0 )  // 수정 필요 
	{
		char str1[50];
		command = strtok( NULL, define1 );
		if(command == NULL)		// 주소 공백일시..
		{			
				return;
		}
		else
		{
			strncpy(str1,command,50);
			command = strtok( NULL, define1 );
			if( command == NULL ){
				printf("[error]\n");
				return ;
			}
			if( strcmp(str1,"/") ==0 )	// 최상위 디렉토리를 옮기려는 경우..
			{
				printf("mv : cannot move '/' to '%s' : Device or resuorce busy\n",command);	// 최상위 디렉토리는 복사 하지 못하도록 처리
				return;	// 종료.
			}
			CopyDirectoryToDirectory(str1,command);
			enqueueAQ(pQueue,temp);
		}
	}
	else if( strcmp(commnad1, "mv") == 0 )  // 수정 필요 
	{
		char str1[50];
		command = strtok( NULL, define1 );
		if(command == NULL)		// 주소 공백일시..
		{			
				return;
		}
		else
		{
			strncpy(str1,command,50);
			command = strtok( NULL, define1 );
			if( command == NULL ){
				printf("[error]\n");
				return ;
			}
			if( strcmp(str1,"/") ==0 )	// 최상위 디렉토리를 옮기려는 경우..
			{
				printf("mv : cannot move '/' to '%s' : Device or resuorce busy\n",command);
				return;	// 종료.
			}
			//printf("dirpath1 : %s , dirpath : %s\n",str1,command);
			MoveDirectoryToDirectory(str1,command);
			enqueueAQ(pQueue,temp);
		}
	}
	else if( strcmp(commnad1, "rm") == 0 )
	{
		command = strtok( NULL, define1 );
		if(strcmp(command,"-rf") ==0 ){
			command = strtok( NULL, define1 );
			DeleteDirectory(dTree,command,3);
			enqueueAQ(pQueue,temp);
		}
		else if( IsExistItem(dTree,command)->type =='d')
		{
			printf("디렉토리이므로 삭제가 불가 \n");
		}
		else
		{
			DeleteDirectory(dTree,command,0);
			enqueueAQ(pQueue,temp);
		}
	}
	else if( strcmp(commnad1, "pwd") == 0 )
	{
		pwd(dTree);
		enqueueAQ(pQueue,temp);
	}
	else if( strcmp(commnad1, "chown") == 0 )
	{
		char* name1;
		char* name2;
		char* command3;
		char temp2[40];
		command = strtok( NULL, define1 );
		strncpy(temp2,command,40);
		command3 = strtok( NULL, define1 );
	
		name1 = strtok( temp2,".");		// " . " 을 기준으로 사용자이름을 받고 
		name2 = strtok( NULL, "." );	// 다시 그룹의 이름을 받는다. 

		if(command3 != NULL)
		{	
			char* str1 = NULL;
			char* div = "/";
			char temp2[20];
			BOOL ret = TRUE;
			if( IsCharExist(command3,'/') == FALSE )
			{
				BOOL ret = TRUE;
				dTree->TempNode = dTree->currentNode;
				if ( ret == TRUE ){
				
					ChangeOwnderItem(dTree, command3, name1, name2);
					enqueueAQ(pQueue,temp);
				}
				dTree->currentNode = dTree->TempNode;
				strncpy(dTree->currentNodeName,dTree->currentNode->name,20);
				return ;
			}
			else
			{
				char temp2[20];
				dTree->TempNode = dTree->currentNode;
				if( command3[0] == '/')
				{
					dTree->currentNode = dTree->headerNode;
				}
				DeleteSlash(command3);// "/" 를 제거 한다. 
				str1 = strtok(command3, div);		// "/" 기준으로 하나 하나 불러낸다. 
				// 경로를 하나 하나 조사 해서 위치가 존재하는지를 조사 ........
				while( str1 != NULL )		//경로의 이름의 값이 없을 때 까지 조샇산다. 
				{
					ret = MoveCurrentNode(dTree, str1);		// 하나하나 이동한다. 
					strncpy(temp2,str1,20);
					if(ret == FALSE )
					{
						return;		// 종료 
					}
					strncpy(temp2,str1,20);
					str1 = strtok(NULL, div );
					if(str1 == NULL)
						break;
				}
				dTree->currentNode= dTree->currentNode->Parent;
				if ( ret == TRUE ){
					ChangeOwnderItem(dTree, temp2, name1, name2);
					enqueueAQ(pQueue,temp);
				}
			}
			dTree->currentNode = dTree->TempNode;
			strncpy(dTree->currentNodeName,dTree->currentNode->name,20);
			return ;
		}
		else
		{
			//Commnad_LS(dTree,0);
			printf("chown: missing operand\n");	
		}
	}
	else if( strcmp(commnad1, "rmdir") == 0 )
	{
				char command2[20] ="";
		command = strtok( NULL, define1 );
		if(command != NULL)
		{	
			char* str1 = NULL;
			char* div = "/";
			char temp2[20];
			BOOL ret = TRUE;
			

			if( IsCharExist(command,'/') == FALSE )
			{
				BOOL ret = TRUE;
				dTree->TempNode = dTree->currentNode;
				if ( ret == TRUE ){
				
					DeleteDirectory(dTree,command,1);
					enqueueAQ(pQueue,temp);
				}
				dTree->currentNode = dTree->TempNode;
				strncpy(dTree->currentNodeName,dTree->currentNode->name,20);
				return ;
			}
			else
			{
				dTree->TempNode = dTree->currentNode;
				if( command[0] == '/')
				{
					dTree->currentNode = dTree->headerNode;
				}
				DeleteSlash(command);// "/" 를 제거 한다. 
				str1 = strtok(command, div);		// "/" 기준으로 하나 하나 불러낸다. 

				// 경로를 하나 하나 조사 해서 위치가 존재하는지를 조사 ........
				while( str1 != NULL )		//경로의 이름의 값이 없을 때 까지 조샇산다. 
				{
					ret = MoveCurrentNode(dTree, str1);		// 하나하나 이동한다. 
					if(ret == FALSE )
					{
						return;		// 종료 
					}
					strncpy(temp2,str1,20);
					str1 = strtok(NULL, div );
					if(str1 == NULL)
						break;
				}
				dTree->currentNode= dTree->currentNode->Parent;
				if ( ret == TRUE ){
					DeleteDirectory(dTree,temp2,1);
					enqueueAQ(pQueue,temp);
				}
			}
			dTree->currentNode = dTree->TempNode;
			strncpy(dTree->currentNodeName,dTree->currentNode->name,20);
			return ;
		}
		else
		{
			//Commnad_LS(dTree,0);
			printf("chmod: missing operand\n");	
		}
	}
	else if( strcmp(commnad1, "useradd") == 0 )
	{
		command = strtok( NULL, define1 );
		AddUser(UList,command);
		enqueueAQ(pQueue,temp);
	}
	else if( strcmp(commnad1, "passwd") == 0 )
	{
		command = strtok( NULL, define1 );
		if( command==NULL){	// 단순 passwd 입력시 루트 계정이 비밀번호를 바꾼다. 
			passwd(UList,UList->currentLoginUser);
			enqueueAQ(pQueue,temp);
		}else{
			passwd(UList,command);
			enqueueAQ(pQueue,temp);
		}
	}
	else if( strcmp(commnad1, "userdel") == 0 )
	{
		command = strtok( NULL, define1 );
		DelUser(UList,command,0);
		enqueueAQ(pQueue,temp);
	}
	else if( strcmp(commnad1, "cat") == 0 )			// cat 명령어. 
	{
		command = strtok( NULL, define1 );
		if( strlen(command) == 0)
			return;
		if( strcmp(command, "/etc/passwd")==0 )
		{
			ShowUserList(UList);
			enqueueAQ(pQueue,temp);
		}
		else if( strcmp(command, "passwd")==0 && strcmp(dTree->currentNode,"etc")==0 )
		{
			ShowUserList(UList);
			enqueueAQ(pQueue,temp);
		}
		else
		{
			printf(" 해당 파일이 존재 하지 않습니다. \n");
		}
	}
	else if(strcmp(commnad1, "clear") == 0)
	{	
		system("cls");
	}
	else if(strcmp(commnad1, "help") == 0)
	{	
		help();
		enqueueAQ(pQueue,temp);
	}
	else if(strcmp(commnad1, "ping") == 0)
	{	
		char temp[20] = "ping ";
		command = strtok( NULL, define1 );
		strncat(temp,command,20);
		system(temp);
		enqueueAQ(pQueue,temp);
	}
	else if(strcmp(commnad1, "finduser") == 0)
	{	
		command = strtok( NULL, define1 );
		IsExistUser(UList,command);
	}
	else if( strcmp(commnad1, "su")==0 )
	{
		command = strtok( NULL, define1 );
		if( command == NULL)	// 단순히 "su"만 입력 했을 경우 
		{
			if( strcmp(UList->currentLoginUser,"root") == 0 ) // 현재 사용자가 "root"인경우 ,su 명령어는 실행하지 않는다.
			{
				return ;
			}else	// 일반 사용자일경우 root계정의 비밀번호를 묻는다. 
			{
				ChangeUser(0,"root");
			}
		}
		else		// 뒤에 옵션과 사용자명을 입력 했을 경우. 
		{
			// ------------------- "su -"를 입력 했을 경우 ----------------------------------
			if(command[0] == '-')		// '-' 옵션이 있을 경우 
			{
				command = strtok( NULL, define1 );
				if( command == NULL)	// 단순히 "su -"만 입력 했을 경우 
				{
					if( strcmp(UList->currentLoginUser,"root") == 0 ) // 현재 사용자가 "root"인경우 ,su 명령어는 실행하지 않는다.
					{
						return ;
					}else	// 일반 사용자일경우 root계정의 비밀번호를 묻는다. 
					{
						ChangeUser(1,"root");	// "su -" 이므로 디렉토리 까지 변경한다. 
					}
				}
				else
				{
					ChangeUser(1,command);
				}
			}
			// ------------------- "su -"를 입력 했을 경우 ----------------------------------
			// ------------------- "su"만 입력 했을 경우 디렉토리 변경 없음----------------------------------
			else						// 옵션이 없을 경우..
			{
				if( command == NULL)	// 단순히 "su -"만 입력 했을 경우 
				{
					if( strcmp(UList->currentLoginUser,"root") == 0 ) // 현재 사용자가 "root"인경우 ,su 명령어는 실행하지 않는다.
					{
						return ;
					}else	// 일반 사용자일경우 root계정의 비밀번호를 묻는다. 
					{
						ChangeUser(0,"root");	// "su -" 이므로 디렉토리 까지 변경한다. 
					}
				}
				else
				{
					ChangeUser(0,command);
				}
			}
			// ------------------- "su"만 입력 했을 경우 디렉토리 변경 없음----------------------------------
		}
		enqueueAQ(pQueue,temp);
	}
	else if(strcmp(command,"exit") ==0 || strcmp(command,"logout")==0)
	{
		if( strcmp(UList->currentLoginUser,"root")!= 0 && strcmp(command,"logout")==0 ){
			printf("bash: logout: not login shell: use 'exit'\n");
			return ;
		}
		if( strcmp(UList->currentLoginUser,"root")!= 0 && strcmp(command,"exit")==0 ){
			//printf("exit\n");	
		}
		

		UList->logincount--;
		command = strtok( NULL, define1 );
		if(command == NULL)		// 주소 공백일시..
		{	
			if(UList->logincount==0)
			{
				LoginUser();
			}
			else
			{
				PopData(pLogin);
				strncpy(UList->currentLoginUser,pLogin->pTopElement,20); // 스택에 쌓여 있는 이전 로그인 사용자로 전환함 
				//printf("사용자 변환 기능.....");
				if( strcmp(UList->currentLoginUser,"root")== 0 )
				system("cls");
			}
		}
	}
	else if( strcmp(command,"history") ==0 )
	{
		displayArrayQueue(pQueue);
	}
	else
	{
		printf("bash: %s: command not found \n",commnad1);
	}
}