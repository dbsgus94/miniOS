// ��ɾ���� ó���ϴ� �Լ���...
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
	if(strcmp(command,"")==0)				// �ܼ� ���� �Է½�, �Ʒ� strtok�� ���� ���α׷� ���� ����... 
	{
		return;
	}
	commnad1 = strtok( command, define1);	// ��ɾ� �̸��� �Է� �ް�, �ش� ��ɾ��� �̸��� ������ 
	//hi
	if( strcmp(commnad1, "mkdir") == 0 )
	{
		command = strtok( NULL, define1 );		// ��ɾ� �Է���, �ڿ� ���丮 ���̳� ��ΰ� ������... " " ���� �������� ��� ������
		if(command != NULL )
		{
			char* str1 = NULL;
			char* div = "/";					// ��� ������ ���� ������ '/'
			char temp2[20];
			BOOL ret = TRUE;
			if(strcmp(command,"-p") ==0 )			// '-p' �ɼ��� ���� ��� .....
			{

				dTree->TempNode = dTree->currentNode;		// ������ġ�� ����
				command = strtok( NULL, define1 ); 
				// -------------- ���� ��ΰ� ���� ��쿡��.... �޽����� ���� �����Ѵ�. ------------
				if( command == NULL )
				{
					printf("mkdir: missing operand\n");		// ��� �Է��� ���� ��� .... 
					return;
				}
				if( command[0] == '/')						// ó���� '/'�� �����ϹǷ�, �����θ� ��Ÿ����. 
				{
					dTree->currentNode = dTree->headerNode;		//���� ��δ� �ֻ��� ���丮 �̹Ƿ� ,dTree�� headerNode�� ����� 
				}												// ���� currentNode �� �����ؼ� ��ġ�� �ֻ��� ���丮�� �����Ѵ�.

				DeleteSlash(command);// "/" �� ���� �Ѵ�.		// �������� ���� '/'�� �߶󳽴�. 
				str1 = strtok(command, div);		// "/" �������� �ϳ� �ϳ� �ҷ�����. 

				// ��θ� �ϳ� �ϳ� ���� �ؼ� ��ġ�� �����ϴ����� ���� ........
				while( str1 != NULL )		//����� �̸��� ���� ���� �� ���� �������. 
				{
					ret = MoveCurrentNode(dTree, str1);		// �ϳ��ϳ� MoveCurrentNode�� �̿��Ͽ� �̵��Ѵ�. 
					if(ret == FALSE )						// �ش� ��ΰ� ���ٸ�, "-p"�ɼǿ� ����, ���丮�� ����� �ش� �̸����� �̵��Ѵ�. 
					{
						MakeDirectory(dTree,str1);
						MoveCurrentNode(dTree, str1);
					}
					str1 = strtok(NULL, div );
					if(str1 == NULL)						// str1���� NULL �� ��쿡��, ���̻��� Ž���� ���� �ʴ´�. 
						break;
				}

				dTree->currentNode = dTree->TempNode;							// ���� ��ġ�� ���� ��ġ�� �ǵ��� ���� ���� ���� 
				strncpy(dTree->currentNodeName,dTree->currentNode->name,20);	// �������� ���丮 �̸��� ����� ��� �Ǳ� ���� ���� 
			}
			else
			{
				dTree->TempNode = dTree->currentNode;		// ������ġ�� ����
				DeleteSlash(command);// "/" �� ���� �Ѵ�. 

				if( command[0] == '/')						// ���� ����ΰ�� 
				{
					dTree->currentNode = dTree->headerNode;
				}
				if( IsCharExist(command,'/') == FALSE )		// ���� �Է¹��� ��ο� /�� ���� ���� �ʴ´ٸ�, ���� ��ġ�� �����ϴ� ���丮�̹Ƿ�...
				{											// �ܼ� �̵���Ų��. 
					MakeDirectory(dTree,command);
				}
				else
				{
					str1 = strtok(command, div);		// "/" �������� �ϳ� �ϳ� �ҷ�����. 
					while( str1 != NULL )		//����� �̸��� ���� ���� �� ���� �������. 
					{
						ret = MoveCurrentNode(dTree, str1);		// �ϳ��ϳ� �̵��Ѵ�. 
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
								printf("mkdir: '%s' ���丮�� ���� �� �����ϴ�: �׷� �����̳� ���丮�� ���� \n",temp2);
								dTree->currentNode = dTree->TempNode;							// ���� ���� ��ġ�� ..��ġ ����
								strncpy(dTree->currentNodeName,dTree->currentNode->name,20);	// 
								return;
							}
						}
						str1 = strtok(NULL, div );
						if(str1 == NULL)
							break;
					}
				}

				dTree->currentNode = dTree->TempNode;							// ���� ���� ��ġ�� ..��ġ ����
				strncpy(dTree->currentNodeName,dTree->currentNode->name,20);	// 
			}
			enqueueAQ(pQueue,temp);
		}
		else
		{
			printf("mkdir: missing operand\n");		// �ܼ��� mkdir�� �Է� �Ͽ�����.... 
		}
	}
	else if( strcmp(commnad1, "ll") == 0 )
	{
		command = strtok( NULL, define1 );
		if(command != NULL)
		{
				BOOL ret = TRUE;
			if(strcmp(command,"-al")==0 )	// "-al" �ɼ��� �ִ°��..
			{
				command = strtok( NULL, define1 ); 
				if(command != NULL)					// ���� ���� �� ����ΰ� �������.
				{
					dTree->TempNode = dTree->currentNode;			// ���� ��ġ�� ���� 
					ret = MoveCurrentNode2(dTree,command);
					if ( ret == TRUE )
						Commnad_LL(dTree,1);
					printf("");
					dTree->currentNode = dTree->TempNode;
					strncpy(dTree->currentNodeName,dTree->currentNode->name,20);
					enqueueAQ(pQueue,temp);
				}
				else						// ���� ���� �� ����ΰ� ���� ���
				{
					Commnad_LL(dTree,1);
					enqueueAQ(pQueue,temp);
				}
			}
			else							// "-al" �ɼ��� ���� ���..
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
			if(strcmp(command,"-al")==0 )	// "-al" �ɼ��� �ִ°��..
			{
				command = strtok( NULL, define1 ); 
				if(command != NULL)					// ���� ���� �� ����ΰ� �������.
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
				else						// ���� ���� �� ����ΰ� �������.
				{
					Commnad_LL(dTree,1);
					enqueueAQ(pQueue,temp);
				}
			}
			else							// "-al" �ɼ��� ���� ���..
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
		chmod = atoi(command);					// �Է¹��� ���ڿ��� int �� ��ȯ ... 
		strncpy(command2,command,20);			// ���߿� ���� �߻���, �ش� ��ΰ� ���� ���� �ʴ´ٰ� ��� �ֱ� ���ؼ�... ������ command �� �ӽ� ����
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
				DeleteSlash(command);// "/" �� ���� �Ѵ�. 
				str1 = strtok(command, div);		// "/" �������� �ϳ� �ϳ� �ҷ�����. 

				// ��θ� �ϳ� �ϳ� ���� �ؼ� ��ġ�� �����ϴ����� ���� ........
				while( str1 != NULL )		//����� �̸��� ���� ���� �� ���� �������. 
				{
					ret = MoveCurrentNode(dTree, str1);		// �ϳ��ϳ� �̵��Ѵ�. 
					if(ret == FALSE )
					{
						return;		// ���� 
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
	else if( strcmp(commnad1, "cd") == 0 )  // ���� �ʿ� 
	{
		command = strtok( NULL, define1 );
		if(command == NULL)		// �ּ� �����Ͻ�..
				return;
		MoveCurrentNode2(dTree,command);
		enqueueAQ(pQueue,temp);
	}
	else if( strcmp(commnad1, "cp") == 0 )  // ���� �ʿ� 
	{
		char str1[50];
		command = strtok( NULL, define1 );
		if(command == NULL)		// �ּ� �����Ͻ�..
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
			if( strcmp(str1,"/") ==0 )	// �ֻ��� ���丮�� �ű���� ���..
			{
				printf("mv : cannot move '/' to '%s' : Device or resuorce busy\n",command);	// �ֻ��� ���丮�� ���� ���� ���ϵ��� ó��
				return;	// ����.
			}
			CopyDirectoryToDirectory(str1,command);
			enqueueAQ(pQueue,temp);
		}
	}
	else if( strcmp(commnad1, "mv") == 0 )  // ���� �ʿ� 
	{
		char str1[50];
		command = strtok( NULL, define1 );
		if(command == NULL)		// �ּ� �����Ͻ�..
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
			if( strcmp(str1,"/") ==0 )	// �ֻ��� ���丮�� �ű���� ���..
			{
				printf("mv : cannot move '/' to '%s' : Device or resuorce busy\n",command);
				return;	// ����.
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
			printf("���丮�̹Ƿ� ������ �Ұ� \n");
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
	
		name1 = strtok( temp2,".");		// " . " �� �������� ������̸��� �ް� 
		name2 = strtok( NULL, "." );	// �ٽ� �׷��� �̸��� �޴´�. 

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
				DeleteSlash(command3);// "/" �� ���� �Ѵ�. 
				str1 = strtok(command3, div);		// "/" �������� �ϳ� �ϳ� �ҷ�����. 
				// ��θ� �ϳ� �ϳ� ���� �ؼ� ��ġ�� �����ϴ����� ���� ........
				while( str1 != NULL )		//����� �̸��� ���� ���� �� ���� �������. 
				{
					ret = MoveCurrentNode(dTree, str1);		// �ϳ��ϳ� �̵��Ѵ�. 
					strncpy(temp2,str1,20);
					if(ret == FALSE )
					{
						return;		// ���� 
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
				DeleteSlash(command);// "/" �� ���� �Ѵ�. 
				str1 = strtok(command, div);		// "/" �������� �ϳ� �ϳ� �ҷ�����. 

				// ��θ� �ϳ� �ϳ� ���� �ؼ� ��ġ�� �����ϴ����� ���� ........
				while( str1 != NULL )		//����� �̸��� ���� ���� �� ���� �������. 
				{
					ret = MoveCurrentNode(dTree, str1);		// �ϳ��ϳ� �̵��Ѵ�. 
					if(ret == FALSE )
					{
						return;		// ���� 
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
		if( command==NULL){	// �ܼ� passwd �Է½� ��Ʈ ������ ��й�ȣ�� �ٲ۴�. 
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
	else if( strcmp(commnad1, "cat") == 0 )			// cat ��ɾ�. 
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
			printf(" �ش� ������ ���� ���� �ʽ��ϴ�. \n");
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
		if( command == NULL)	// �ܼ��� "su"�� �Է� ���� ��� 
		{
			if( strcmp(UList->currentLoginUser,"root") == 0 ) // ���� ����ڰ� "root"�ΰ�� ,su ��ɾ�� �������� �ʴ´�.
			{
				return ;
			}else	// �Ϲ� ������ϰ�� root������ ��й�ȣ�� ���´�. 
			{
				ChangeUser(0,"root");
			}
		}
		else		// �ڿ� �ɼǰ� ����ڸ��� �Է� ���� ���. 
		{
			// ------------------- "su -"�� �Է� ���� ��� ----------------------------------
			if(command[0] == '-')		// '-' �ɼ��� ���� ��� 
			{
				command = strtok( NULL, define1 );
				if( command == NULL)	// �ܼ��� "su -"�� �Է� ���� ��� 
				{
					if( strcmp(UList->currentLoginUser,"root") == 0 ) // ���� ����ڰ� "root"�ΰ�� ,su ��ɾ�� �������� �ʴ´�.
					{
						return ;
					}else	// �Ϲ� ������ϰ�� root������ ��й�ȣ�� ���´�. 
					{
						ChangeUser(1,"root");	// "su -" �̹Ƿ� ���丮 ���� �����Ѵ�. 
					}
				}
				else
				{
					ChangeUser(1,command);
				}
			}
			// ------------------- "su -"�� �Է� ���� ��� ----------------------------------
			// ------------------- "su"�� �Է� ���� ��� ���丮 ���� ����----------------------------------
			else						// �ɼ��� ���� ���..
			{
				if( command == NULL)	// �ܼ��� "su -"�� �Է� ���� ��� 
				{
					if( strcmp(UList->currentLoginUser,"root") == 0 ) // ���� ����ڰ� "root"�ΰ�� ,su ��ɾ�� �������� �ʴ´�.
					{
						return ;
					}else	// �Ϲ� ������ϰ�� root������ ��й�ȣ�� ���´�. 
					{
						ChangeUser(0,"root");	// "su -" �̹Ƿ� ���丮 ���� �����Ѵ�. 
					}
				}
				else
				{
					ChangeUser(0,command);
				}
			}
			// ------------------- "su"�� �Է� ���� ��� ���丮 ���� ����----------------------------------
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
		if(command == NULL)		// �ּ� �����Ͻ�..
		{	
			if(UList->logincount==0)
			{
				LoginUser();
			}
			else
			{
				PopData(pLogin);
				strncpy(UList->currentLoginUser,pLogin->pTopElement,20); // ���ÿ� �׿� �ִ� ���� �α��� ����ڷ� ��ȯ�� 
				//printf("����� ��ȯ ���.....");
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