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
		MakeDirectory(dTree,"/");				// 최상위 디렉토리 "/" 생성을 위해서....실행.
		dTree->currentNode = dTree->headerNode;
	}
	return dTree;
}

BOOL MakeDirectory(DirTree* dTree, char* dirName){
	BOOL state = TRUE;
	DirectoryNode* NewNode = NULL;
	DirectoryNode* TempNode = NULL;
	time_t rawtime;				//  시간 설정을 위해서.	...
    struct tm * timeinfo;		//	시간 설정을 위해서.....
	char month[10];
	char year[10];
	NewNode = (DirectoryNode *)malloc( sizeof(DirectoryNode) );
	NewNode->LChild = NULL;
	NewNode->RSibiling = NULL;	
	//--------------------------------- 현재 시간을 구함 ------------------------------------	
	time ( &rawtime );
    timeinfo = localtime ( &rawtime );
	//--------------------------------- 현재 시간을 구함 ------------------------------------
	if(NewNode != NULL)
	{
		strncpy(NewNode->name,dirName,20);	//  받아온 디렉토리 이름을 복사 
		strftime (year,10,"%Y",timeinfo);	//  현재 시간 정보를 바탕으로, 년도를 구함 
		strftime (month,10,"%m",timeinfo);	//  현재 시간 정보를 바탕으로, 월을 구함 
		NewNode->month = atoi(month);
		NewNode->year = atoi(year);
		if(dirName[0] =='.')				//	숨김 파일인지 정보를 파악하고, type을 정한다. 
			NewNode->type ='s';
		else
			NewNode->type ='d';
		if(dTree->headerNode == NULL )  // 초기 헤더노드의 정보를 루트로 연결한다.	// 최초 디렉토리 생성시....
		{
			dTree->headerNode = NewNode;
			NewNode->chmod = 755;
			strcpy(NewNode->username,"root");
			strcpy(NewNode->groupname,"root");
			// --------------------------- 허가권 설정 ---------------------------------------
			NewNode->permission[0] = 1;			// 사용자 읽기 r
			NewNode->permission[1] = 1;			// 사용자 쓰기 w
			NewNode->permission[2] = 1;			// 사용자 실행 x
			NewNode->permission[3] = 1;			// 그룹  읽기 r
			NewNode->permission[4] = 0;			// 그룹 쓰기 w
			NewNode->permission[5] = 1;			// 그룹 실행 x 
			NewNode->permission[6] = 1;			// 다른사용자 읽기 r
			NewNode->permission[7] = 0;			// 다른사용자 쓰기 w
			NewNode->permission[8] = 1;			// 다른 사용자 실해 x
			// ------------------------- 허가권 설정 -------------------------------------------
			NewNode->Parent = NULL; // "/" 위는 NULL 로 하여서 최상위 디렉토리인지를 확인하도록 한다. 
		}
		else // 최초 노드 추가는 root 이고 그 이후는 
		{
			// ----------------------- 해당 위치에서 대상의 허가권 검사 -----------------------
			if( state = permissionHandler(dTree->currentNode,'w') == FALSE )
			{
					printf("mkdir: '%s' 디렉토리를 만들 수 없습니다.: 허가 거부됨 \n",dirName);
					return FALSE;
			}
			// ----------------------- @해당 위치에서 대상의 허가권 검사 ----------------------
			if(IsExistItem(dTree, dirName)!=NULL && IsExistItem(dTree, dirName)->type =='d' )
			{
				printf("mkdir: '%s' 디렉토리를 만들 수 없습니다 : 파일이 존재 합니다. \n",dirName);
				return FALSE;
			}
			// ---------------------------- 일단 디렉토리의 정보를 저장한다. ----------------------------------
			NewNode->chmod = 755;
			strcpy(NewNode->username,UList->currentLoginUser);  // 추가하려는 유저명 ...... // 이부분은 3.0버전 이후 수정이 필요함.!! 수정함 
			strcpy(NewNode->groupname,UList->currentLoginUser); // 추가하려는 유저명 ...... // 이부분은 3.0버전 이후 수정이 필요함.!! 수정함 
			NewNode->permission[0] = 1;
			NewNode->permission[1] = 1;
			NewNode->permission[2] = 1;
			NewNode->permission[3] = 1;
			NewNode->permission[4] = 0;
			NewNode->permission[5] = 1;
			NewNode->permission[6] = 1;
			NewNode->permission[7] = 0;
			NewNode->permission[8] = 1;
			NewNode->Parent = dTree->currentNode; // 각 노드의 Parrent 값을 currentNode로 변경함으로써 pwd 를 이용한 스텍저장을 가능하도록 함. 
			// ---------------------------- 일단 디렉토리의 정보를 저장한다. ----------------------------------
			if(dTree->currentNode->LChild == NULL)  // dTree 가 가지고있는 currentNode를 기준으로 하위 파일 및 디렉토리를 추가한다. 
			{
				dTree->currentNode->LChild = NewNode;
			}
			else											// current_node의 자식이 존재하므로 rsibiling으로 계속 연결 시킨다.
			{
				TempNode = dTree->currentNode->LChild;				// 현재 위치이다. 
				while (TempNode->RSibiling != NULL)				// 검색을 해가면서 RSibiling NULL인 부분을 찾는다. 
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
	time_t rawtime;				//  시간 설정을 위해서.	...
    struct tm * timeinfo;		//	시간 설정을 위해서.....
	char month[10];
	char year[10];
	NewNode = (DirectoryNode *)malloc( sizeof(DirectoryNode) );
	NewNode->LChild = NULL;
	NewNode->RSibiling = NULL;	
	//--------------------------------- 현재 시간을 구함 ------------------------------------	
	time ( &rawtime );
    timeinfo = localtime ( &rawtime );
	//--------------------------------- 현재 시간을 구함 ------------------------------------
	if(IsExistItem(dTree, dirName)!=NULL && IsExistItem(dTree, dirName)->type !='d' )
	{
		printf("touch: '%s' 파일을 만들 수 없습니다 : 파일이 존재 합니다. \n",dirName);
		return FALSE;
	}
	if(NewNode != NULL)
	{
		strncpy(NewNode->name,dirName,20);
		strftime (year,10,"%Y",timeinfo);	//  현재 시간 정보를 바탕으로, 년도를 구함 
		strftime (month,10,"%m",timeinfo);	//  현재 시간 정보를 바탕으로, 월을 구함 
		NewNode->month = atoi(month);
		NewNode->year = atoi(year);
		if(dirName[0] =='.')
			NewNode->type ='u';
		else
			NewNode->type ='f';
		if(dTree->headerNode == NULL )  // 초기 헤더노드의 정보를 루트로 연결한다.
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
			NewNode->Parent = NULL; // "/" 위는 NULL 로 하여서 최상위 디렉토리인지를 확인하도록 한다. 
		}
		else // 최초 노드 추가는 root 이고 그 이후는 
		{
			// ----------------------- 해당 위치에서 대상의 허가권 검사 -----------------------
			if( state = permissionHandler(dTree->currentNode,'w') == FALSE )
			{
					printf("touch: '%s' 파일을 만들 수 없습니다.: 허가 거부됨 \n",dirName);
					return FALSE;
			}
			// ----------------------- @해당 위치에서 대상의 허가권 검사 ----------------------
			// ---------------------------- 일단 디렉토리의 정보를 저장한다. ----------------------------------
			NewNode->chmod = 644;
			strcpy(NewNode->username,UList->currentLoginUser);  // 추가하려는 유저명 ...... // 이부분은 3.0버전 이후 수정이 필요함.!!
			strcpy(NewNode->groupname,UList->currentLoginUser); // 추가하려는 유저명 ...... // 이부분은 3.0버전 이후 수정이 필요함.!!
			NewNode->permission[0] = 1;
			NewNode->permission[1] = 1;
			NewNode->permission[2] = 0;
			NewNode->permission[3] = 1;
			NewNode->permission[4] = 0;
			NewNode->permission[5] = 0;
			NewNode->permission[6] = 1;
			NewNode->permission[7] = 0;
			NewNode->permission[8] = 0;
			NewNode->Parent = dTree->currentNode; // 각 노드의 Parrent 값을 currentNode로 변경함으로써 pwd 를 이용한 스텍저장을 가능하도록 함. 
			// ---------------------------- 일단 디렉토리의 정보를 저장한다. ----------------------------------
			if(dTree->currentNode->LChild == NULL)  // dTree 가 가지고있는 currentNode를 기준으로 하위 파일 및 디렉토리를 추가한다. 
			{
				dTree->currentNode->LChild = NewNode;
			}
			else											// current_node의 자식이 존재하므로 rsibiling으로 계속 연결 시킨다.
			{
				TempNode = dTree->currentNode->LChild;				// 현재 위치이다. 
				while (TempNode->RSibiling != NULL)				// 검색을 해가면서 RSibiling NULL인 부분을 찾는다. 
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
		printf("[Error] %s 는 존재하지 않습니다.\n",dirName);
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
			printf("rmdir: %s 디렉토리가 비어 있지 않음 \n",DelNode->name);
			return FALSE;
		}
		// ----------------------- 해당 위치에서의 허가권 검사 -----------------------
		if( state = permissionHandler(DelNode,'w') == FALSE )
		{
			printf("rmdir: '%s' 을(를) 삭제 할 수 없습니다.: 허가가 거부됨 \n",dirName);
			return FALSE;
		}
		// ----------------------- @해당 위치에서의 허가권 검사 ----------------------

		PrevNode->RSibiling = DelNode->RSibiling;
		if(option == 1)		// 옵션이 1일경우 디렉토리 삭제 ..
		{
			FreeSubDirectory(DelNode->LChild);
		}
		free(DelNode);
		return TRUE;
	}
	else
	{
		printf("[Error] %s 는 존재하지 않습니다. \n",dirName);
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
		// Parent는 복제할시....달라지므로 복제하지 않는다. .
		strncpy(TempNode->name,Original->name,20);
		strncpy(TempNode->username,Original->username,20);
		strncpy(TempNode->groupname,Original->groupname,20);
		TempNode->chmod = Original->chmod;   // 허가권 설정 
		for(k = 0 ; k < 9 ; k++)			// 자세한 허가권 복제..
			TempNode->permission[k] = Original->permission[k];
		TempNode->type = Original->type;         // d : directory , f 파일 , u : 숨김파일 
		TempNode->month = Original->month;			// 생성 시기 , 달 
		TempNode->year = Original->year;			// 생성 시간은 현재 시간으로 바꿀 필요가 있다.......
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
	// ----------------------- 해당 위치에서의 허가권 검사 -----------------------
	if( state = permissionHandler(dTree->currentNode,'r') == FALSE )
	{
		printf("ls: .: 허가 거부됨 \n");
		return ;
	}
	// ----------------------- @해당 위치에서의 허가권 검사 ----------------------
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
				printf("%7s %7s \t %d월 %d년",ShowNode->username,ShowNode->groupname,ShowNode->month,ShowNode->year);
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
		printf("합계 :0 \n");
	}
}

void Commnad_LS(DirTree* dTree,int opt){
	BOOL state;
	DirectoryNode* ShowNode = dTree->currentNode->LChild;
	int i = 1;
	// ----------------------- 해당 위치에서의 허가권 검사 -----------------------
	if( state = permissionHandler(dTree->currentNode,'r') == FALSE )
	{
		printf("ls: .: 허가 거부됨 \n");
		return ;
	}
	// ----------------------- @해당 위치에서의 허가권 검사 ----------------------
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
		printf("합계 :0 \n");
	}
}


BOOL permissionHandler(DirectoryNode* Node, char option)
{
	//printf(" 현재 로그인 유저 : %s , 대상 유저 이름%s \n",UList->currentLoginUser,Node->username);
	if( strcmp(UList->currentLoginUser,"root") == 0)	// 최고 계정일 경우 무조건, TRUE
	{
		return TRUE;
	}
	else if( strcmp(UList->currentLoginUser,Node->username) == 0) // 현재 로그인한 일반유저와 , 대상의 소유 정보가 같을경우..
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
	else													// 현재 로그인한 유저와 , 대상의 소유 정보가 다를경우..		
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