#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "header.h"


DirTree* CreateRootDirectory()  // 
{
	DirTree* dTree = (DirTree* )malloc( sizeof(DirTree) );
	
	if(dTree != NULL )
	{
		dTree->headerNode=NULL;
		strncpy(dTree->currentNodeName,"/",20);
		MakeDirectory(dTree,"/");				// �ֻ��� ���丮 "/" ������ ���ؼ�....����.
		dTree->currentNode = dTree->headerNode;
	}
	return dTree;
}

BOOL MakeDirectory(DirTree* dTree, char* dirName){
	BOOL state = TRUE;
	DirectoryNode* NewNode = NULL;
	DirectoryNode* TempNode = NULL;
	time_t rawtime;				//  �ð� ������ ���ؼ�.	...
    struct tm * timeinfo;		//	�ð� ������ ���ؼ�.....
	char month[10];
	char year[10];
	NewNode = (DirectoryNode *)malloc( sizeof(DirectoryNode) );
	NewNode->LChild = NULL;
	NewNode->RSibiling = NULL;	
	//--------------------------------- ���� �ð��� ���� ------------------------------------	
	time ( &rawtime );
    timeinfo = localtime ( &rawtime );
	//--------------------------------- ���� �ð��� ���� ------------------------------------
	if(NewNode != NULL)
	{
		strncpy(NewNode->name,dirName,20);	//  �޾ƿ� ���丮 �̸��� ���� 
		strftime (year,10,"%Y",timeinfo);	//  ���� �ð� ������ ��������, �⵵�� ���� 
		strftime (month,10,"%m",timeinfo);	//  ���� �ð� ������ ��������, ���� ���� 
		NewNode->month = atoi(month);
		NewNode->year = atoi(year);
		if(dirName[0] =='.')				//	���� �������� ������ �ľ��ϰ�, type�� ���Ѵ�. 
			NewNode->type ='s';
		else
			NewNode->type ='d';
		if(dTree->headerNode == NULL )  // �ʱ� �������� ������ ��Ʈ�� �����Ѵ�.	// ���� ���丮 ������....
		{
			dTree->headerNode = NewNode;
			NewNode->chmod = 755;
			strcpy(NewNode->username,"root");
			strcpy(NewNode->groupname,"root");
			// --------------------------- �㰡�� ���� ---------------------------------------
			NewNode->permission[0] = 1;			// ����� �б� r
			NewNode->permission[1] = 1;			// ����� ���� w
			NewNode->permission[2] = 1;			// ����� ���� x
			NewNode->permission[3] = 1;			// �׷�  �б� r
			NewNode->permission[4] = 0;			// �׷� ���� w
			NewNode->permission[5] = 1;			// �׷� ���� x 
			NewNode->permission[6] = 1;			// �ٸ������ �б� r
			NewNode->permission[7] = 0;			// �ٸ������ ���� w
			NewNode->permission[8] = 1;			// �ٸ� ����� ���� x
			// ------------------------- �㰡�� ���� -------------------------------------------
			NewNode->Parent = NULL; // "/" ���� NULL �� �Ͽ��� �ֻ��� ���丮������ Ȯ���ϵ��� �Ѵ�. 
		}
		else // ���� ��� �߰��� root �̰� �� ���Ĵ� 
		{
			// ----------------------- �ش� ��ġ���� ����� �㰡�� �˻� -----------------------
			if( state = permissionHandler(dTree->currentNode,'w') == FALSE )
			{
					printf("mkdir: '%s' ���丮�� ���� �� �����ϴ�.: �㰡 �źε� \n",dirName);
					return FALSE;
			}
			// ----------------------- @�ش� ��ġ���� ����� �㰡�� �˻� ----------------------
			if(IsExistItem(dTree, dirName)!=NULL && IsExistItem(dTree, dirName)->type =='d' )
			{
				printf("mkdir: '%s' ���丮�� ���� �� �����ϴ� : ������ ���� �մϴ�. \n",dirName);
				return FALSE;
			}
			// ---------------------------- �ϴ� ���丮�� ������ �����Ѵ�. ----------------------------------
			NewNode->chmod = 755;
			strcpy(NewNode->username,UList->currentLoginUser);  // �߰��Ϸ��� ������ ...... // �̺κ��� 3.0���� ���� ������ �ʿ���.!! ������ 
			strcpy(NewNode->groupname,UList->currentLoginUser); // �߰��Ϸ��� ������ ...... // �̺κ��� 3.0���� ���� ������ �ʿ���.!! ������ 
			NewNode->permission[0] = 1;
			NewNode->permission[1] = 1;
			NewNode->permission[2] = 1;
			NewNode->permission[3] = 1;
			NewNode->permission[4] = 0;
			NewNode->permission[5] = 1;
			NewNode->permission[6] = 1;
			NewNode->permission[7] = 0;
			NewNode->permission[8] = 1;
			NewNode->Parent = dTree->currentNode; // �� ����� Parrent ���� currentNode�� ���������ν� pwd �� �̿��� ���������� �����ϵ��� ��. 
			// ---------------------------- �ϴ� ���丮�� ������ �����Ѵ�. ----------------------------------
			if(dTree->currentNode->LChild == NULL)  // dTree �� �������ִ� currentNode�� �������� ���� ���� �� ���丮�� �߰��Ѵ�. 
			{
				dTree->currentNode->LChild = NewNode;
			}
			else											// current_node�� �ڽ��� �����ϹǷ� rsibiling���� ��� ���� ��Ų��.
			{
				TempNode = dTree->currentNode->LChild;				// ���� ��ġ�̴�. 
				while (TempNode->RSibiling != NULL)				// �˻��� �ذ��鼭 RSibiling NULL�� �κ��� ã�´�. 
				{
					TempNode = TempNode->RSibiling;
				}
				TempNode->RSibiling = NewNode;
			}
		}
	}
	else
	{
		printf("[Error] NULL , MakeDirectory() \n");
		return FALSE;
	}

	return TRUE;
}


BOOL MakeFile(DirTree* dTree, char* dirName)
{
	BOOL state = TRUE;
	DirectoryNode* NewNode = NULL;
	DirectoryNode* TempNode = NULL;
	time_t rawtime;				//  �ð� ������ ���ؼ�.	...
    struct tm * timeinfo;		//	�ð� ������ ���ؼ�.....
	char month[10];
	char year[10];
	NewNode = (DirectoryNode *)malloc( sizeof(DirectoryNode) );
	NewNode->LChild = NULL;
	NewNode->RSibiling = NULL;	
	//--------------------------------- ���� �ð��� ���� ------------------------------------	
	time ( &rawtime );
    timeinfo = localtime ( &rawtime );
	//--------------------------------- ���� �ð��� ���� ------------------------------------
	if(IsExistItem(dTree, dirName)!=NULL && IsExistItem(dTree, dirName)->type !='d' )
	{
		printf("touch: '%s' ������ ���� �� �����ϴ� : ������ ���� �մϴ�. \n",dirName);
		return FALSE;
	}
	if(NewNode != NULL)
	{
		strncpy(NewNode->name,dirName,20);
		strftime (year,10,"%Y",timeinfo);	//  ���� �ð� ������ ��������, �⵵�� ���� 
		strftime (month,10,"%m",timeinfo);	//  ���� �ð� ������ ��������, ���� ���� 
		NewNode->month = atoi(month);
		NewNode->year = atoi(year);
		if(dirName[0] =='.')
			NewNode->type ='u';
		else
			NewNode->type ='f';
		if(dTree->headerNode == NULL )  // �ʱ� �������� ������ ��Ʈ�� �����Ѵ�.
		{
			dTree->headerNode = NewNode;
			NewNode->chmod = 600;
			strcpy(NewNode->username,"root");
			strcpy(NewNode->groupname,"root");
			NewNode->permission[0] = 1;
			NewNode->permission[1] = 1;
			NewNode->permission[2] = 1;
			NewNode->permission[3] = 0;
			NewNode->permission[4] = 0;
			NewNode->permission[5] = 0;
			NewNode->permission[6] = 0;
			NewNode->permission[7] = 0;
			NewNode->permission[8] = 0;
			NewNode->Parent = NULL; // "/" ���� NULL �� �Ͽ��� �ֻ��� ���丮������ Ȯ���ϵ��� �Ѵ�. 
		}
		else // ���� ��� �߰��� root �̰� �� ���Ĵ� 
		{
			// ----------------------- �ش� ��ġ���� ����� �㰡�� �˻� -----------------------
			if( state = permissionHandler(dTree->currentNode,'w') == FALSE )
			{
					printf("touch: '%s' ������ ���� �� �����ϴ�.: �㰡 �źε� \n",dirName);
					return FALSE;
			}
			// ----------------------- @�ش� ��ġ���� ����� �㰡�� �˻� ----------------------
			// ---------------------------- �ϴ� ���丮�� ������ �����Ѵ�. ----------------------------------
			NewNode->chmod = 644;
			strcpy(NewNode->username,UList->currentLoginUser);  // �߰��Ϸ��� ������ ...... // �̺κ��� 3.0���� ���� ������ �ʿ���.!!
			strcpy(NewNode->groupname,UList->currentLoginUser); // �߰��Ϸ��� ������ ...... // �̺κ��� 3.0���� ���� ������ �ʿ���.!!
			NewNode->permission[0] = 1;
			NewNode->permission[1] = 1;
			NewNode->permission[2] = 0;
			NewNode->permission[3] = 1;
			NewNode->permission[4] = 0;
			NewNode->permission[5] = 0;
			NewNode->permission[6] = 1;
			NewNode->permission[7] = 0;
			NewNode->permission[8] = 0;
			NewNode->Parent = dTree->currentNode; // �� ����� Parrent ���� currentNode�� ���������ν� pwd �� �̿��� ���������� �����ϵ��� ��. 
			// ---------------------------- �ϴ� ���丮�� ������ �����Ѵ�. ----------------------------------
			if(dTree->currentNode->LChild == NULL)  // dTree �� �������ִ� currentNode�� �������� ���� ���� �� ���丮�� �߰��Ѵ�. 
			{
				dTree->currentNode->LChild = NewNode;
			}
			else											// current_node�� �ڽ��� �����ϹǷ� rsibiling���� ��� ���� ��Ų��.
			{
				TempNode = dTree->currentNode->LChild;				// ���� ��ġ�̴�. 
				while (TempNode->RSibiling != NULL)				// �˻��� �ذ��鼭 RSibiling NULL�� �κ��� ã�´�. 
				{
					TempNode = TempNode->RSibiling;
				}
				TempNode->RSibiling = NewNode;
			}
		}
	}
	else
	{
		printf("[Error] NULL , MakeDirectory() \n");
		return FALSE;
	}

	return TRUE;

}

BOOL DeleteDirectory(DirTree* dTree, char* dirName,int option){
	BOOL state = TRUE;
	DirectoryNode* DelNode = NULL;
	DirectoryNode* SearchNode = NULL;
	DirectoryNode* PrevNode = NULL;
	char yesOrno[10];
	PrevNode = SearchNode = dTree->currentNode->LChild;
	

	if(PrevNode == NULL)
	{
		printf("[Error] %s �� �������� �ʽ��ϴ�.\n",dirName);
		return FALSE;
	}
	else if(strcmp(SearchNode->name, dirName)==0)
	{
		dTree->currentNode->LChild = SearchNode->RSibiling;
		DelNode = SearchNode;
	}
	else
	{
		SearchNode = SearchNode->RSibiling;
		while( SearchNode != NULL )
		{
			if( strcmp(SearchNode -> name , dirName) ==0 )
			{
				DelNode = SearchNode;
				break;
			}
			else
			{
				PrevNode = SearchNode;
				SearchNode = SearchNode -> RSibiling;
			}
		}
	}
	if(DelNode != NULL)
	{
		if( DelNode->LChild != NULL && option !=3 )
		{
			printf("rmdir: %s ���丮�� ��� ���� ���� \n",DelNode->name);
			return FALSE;
		}
		// ----------------------- �ش� ��ġ������ �㰡�� �˻� -----------------------
		if( state = permissionHandler(DelNode,'w') == FALSE )
		{
			printf("rmdir: '%s' ��(��) ���� �� �� �����ϴ�.: �㰡�� �źε� \n",dirName);
			return FALSE;
		}
		// ----------------------- @�ش� ��ġ������ �㰡�� �˻� ----------------------

		PrevNode->RSibiling = DelNode->RSibiling;
		if(option == 1)		// �ɼ��� 1�ϰ�� ���丮 ���� ..
		{
			FreeSubDirectory(DelNode->LChild);
		}
		free(DelNode);
		return TRUE;
	}
	else
	{
		printf("[Error] %s �� �������� �ʽ��ϴ�. \n",dirName);
		return FALSE;
	}
}


void FreeSubDirectory( DirectoryNode* SubTree)
{
	if (SubTree != NULL)
	{
		FreeSubDirectory( SubTree -> LChild);
		FreeSubDirectory(SubTree -> RSibiling);		
		free(SubTree);					
	}
}

DirectoryNode* CopyTreeNode( DirectoryNode* Original , int i )
{
	int cnt=i;
	int k = 0;
	DirectoryNode* TempNode = NULL;
	// printf(" %s,%d \n", Original->name,cnt);
	if( Original != NULL )
	{
		TempNode= (DirectoryNode* )malloc(sizeof(DirectoryNode));
		// Parent�� �����ҽ�....�޶����Ƿ� �������� �ʴ´�. .
		strncpy(TempNode->name,Original->name,20);
		strncpy(TempNode->username,Original->username,20);
		strncpy(TempNode->groupname,Original->groupname,20);
		TempNode->chmod = Original->chmod;   // �㰡�� ���� 
		for(k = 0 ; k < 9 ; k++)			// �ڼ��� �㰡�� ����..
			TempNode->permission[k] = Original->permission[k];
		TempNode->type = Original->type;         // d : directory , f ���� , u : �������� 
		TempNode->month = Original->month;			// ���� �ñ� , �� 
		TempNode->year = Original->year;			// ���� �ð��� ���� �ð����� �ٲ� �ʿ䰡 �ִ�.......
		TempNode->LChild = CopyTreeNode(Original -> LChild,cnt);
		if( cnt == 0 )
		{
			TempNode->RSibiling = NULL;
		}else
		{
			TempNode->RSibiling  = CopyTreeNode(Original -> RSibiling,cnt);
		}
		++cnt;
		TempNode->LChild = CopyTreeNode(Original -> LChild,cnt);
		return TempNode;
	}
	return NULL;
}

void Commnad_LL(DirTree* dTree,int opt){
	BOOL state;
	char type;
	int i = 0;
	DirectoryNode* ShowNode = dTree->currentNode->LChild;
	// ----------------------- �ش� ��ġ������ �㰡�� �˻� -----------------------
	if( state = permissionHandler(dTree->currentNode,'r') == FALSE )
	{
		printf("ls: .: �㰡 �źε� \n");
		return ;
	}
	// ----------------------- @�ش� ��ġ������ �㰡�� �˻� ----------------------
	if(ShowNode != NULL)
	{
		while(ShowNode != NULL)
		{
			if( opt == 0)
			{
				if(ShowNode->name[0] == '.')
				{	 
					ShowNode = ShowNode->RSibiling;
					continue;
				}
			}
				WHITE;
				if(ShowNode->type == 'd' || ShowNode->type == 's')
					type = 'd';
				else
					type ='-';
				printf("%c",type);
				TransChmod( ShowNode->chmod );
				printf("%7s %7s \t %d�� %d��",ShowNode->username,ShowNode->groupname,ShowNode->month,ShowNode->year);
				if(ShowNode->type == 'd' || ShowNode->type == 's' )
				{
					BLUE;
				}
				printf("  %s \n",ShowNode->name); WHITE;
			ShowNode = ShowNode->RSibiling;
			if(ShowNode == NULL)
				break;
		}

	}	
	else
	{
		printf("�հ� :0 \n");
	}
}

void Commnad_LS(DirTree* dTree,int opt){
	BOOL state;
	DirectoryNode* ShowNode = dTree->currentNode->LChild;
	int i = 1;
	// ----------------------- �ش� ��ġ������ �㰡�� �˻� -----------------------
	if( state = permissionHandler(dTree->currentNode,'r') == FALSE )
	{
		printf("ls: .: �㰡 �źε� \n");
		return ;
	}
	// ----------------------- @�ش� ��ġ������ �㰡�� �˻� ----------------------
	if(ShowNode != NULL)
	{
		while(ShowNode != NULL)
		{
			if( opt == 0)
			{
				if(ShowNode->type =='u' || ShowNode->type =='s')
				{	 
					ShowNode = ShowNode->RSibiling;
					continue;
				}
			}
			if(++i%6 ==0 )
				printf("\n");
			if(ShowNode->type !='u')
			{	 
				WHITE;
				if(ShowNode->type == 'd' || ShowNode->type == 's' )
				{
					BLUE;
				}
				printf("%-10s ",ShowNode->name); WHITE;
			}
			ShowNode = ShowNode->RSibiling;
			if(ShowNode == NULL)
				break;
		}
	}	
	else
	{
		printf("�հ� :0 \n");
	}
}


BOOL permissionHandler(DirectoryNode* Node, char option)
{
	//printf(" ���� �α��� ���� : %s , ��� ���� �̸�%s \n",UList->currentLoginUser,Node->username);
	if( strcmp(UList->currentLoginUser,"root") == 0)	// �ְ� ������ ��� ������, TRUE
	{
		return TRUE;
	}
	else if( strcmp(UList->currentLoginUser,Node->username) == 0) // ���� �α����� �Ϲ������� , ����� ���� ������ �������..
	{
		if( option == 'r')
		{
			if(Node->permission[0] == 1)
				return TRUE;
			else
				return FALSE;
		}
		else if( option == 'w')
		{
			if(Node->permission[1] == 1)
				return TRUE;
			else
				return FALSE;

		}
		else if( option == 'x')
		{
			if(Node->permission[2] == 1)
				return TRUE;
			else
				return FALSE;
		}
	}
	else													// ���� �α����� ������ , ����� ���� ������ �ٸ����..		
	{
		if( option == 'r')
		{
			if(Node->permission[6] == 1)
				return TRUE;
			else
				return FALSE;
		}
		else if( option == 'w')
		{
			if(Node->permission[7] == 1)
				return TRUE;
			else
				return FALSE;

		}
		else if( option == 'x')
		{
			if(Node->permission[8] == 1)
				return TRUE;
			else
				return FALSE;
		}
	}
	return FALSE;
}