#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "header.h"

/*
	#########################################################################################################
	###																									  ###
	###	 typedef struct DirectoryNodeType{  // 디렉토리 개개 정보를 담는 노드 							  ###
	###			char name[20];		// 디렉토리및 파일 명												  ###
	###			int chmod;			// 허가권 설정														  ###
	###			char type;			// d : directory , f 파일 , u : 숨김파일							  ###
	###			int month;			// 생성 시기 , 달													  ###
	###			int time;			// 생성 시간														  ###
	###			char username[20];	// 소유자															  ###
	###			char groupname[20];	// 그룹																  ###
	###			struct DirectoryNodeType* Parretn;	// 하위 디렉토리 구조								  ###
	###			struct DirectoryNodeType* LChild;	// 하위 디렉토리 구조								  ###
	###			struct DirectoryNodeType* RSibiling; // 현디렉토리의 다음 파일이나 디렉토리를 연결		  ###
	###		}DirectoryNode;																				  ###
	###																									  ###
	###	typedef struct DirectoryLCRSTreeType{  // 디렉토리의 최상위 접근 createUserList					  ###
	###			DirectoryNode* headerNode;																  ###
	###			DirectoryNode* currentNode;																  ###
	###		}DirTree;																					  ###
	###																									  ###
	#########################################################################################################

*/

DirectoryNode* IsExistItem(DirTree* dTree, char* itemName){
	DirectoryNode* ReturnNode = NULL;
	ReturnNode = dTree->currentNode->LChild;
	while(ReturnNode != NULL)
	{
		if(strcmp(ReturnNode->name,itemName) == 0 )
		{
			break;
		}		
		ReturnNode = ReturnNode->RSibiling;			// 다음 노드..
	}
	return ReturnNode;								// 찾지 못할시 NULL 값을 넘긴다. 
}


void TransChmod( int chmod )		// int로 받아온 chmod 정보를 문자열로 출력 하게 한다. 
{
	char temp[4];
	int i;
	itoa(chmod, temp, 10);
	for(i = 0 ; i < 3 ; i++)
	{
		if( temp[i] == '7' )
			printf("rwx");
		else if( temp[i] == '6' )
			printf("rw-");
		else if( temp[i] == '5' )
			printf("r-x");
		else if( temp[i] == '4' )
			printf("r--");
		else if( temp[i] == '3' )
			printf("-wx");
		else if( temp[i] == '2')
			printf("-w-");
		else if( temp[i] == '1')
			printf("--x");
		else
			printf("---");
	}
}

void pwd(DirTree* dTree)		// 현재위치를 알려주는 pwd 함수 ... 
{
	DirectoryNode* current = dTree->currentNode;
	if(current->Parent == NULL)	// Paretn 노드가 NULL 인경우는 .최상위 디렉토리 이므로 .'/'를 출력한다.
	{
		printf("/\n");
		return ;
	}
	while(current->Parent != NULL)	// 현재위치에서 부터 부모노드로 이동하며 스텍에 push 한다.
	{
		Push(pStack, current->name);
		current = current->Parent;
	}

	while(1)	// push 한 데이터를 스택에 비게 될 때까지... pop 하여 빼낸다.
	{
		if(pStack->pTopElement == NULL)
			break;
		printf("/");
		printf("%s",PopData(pStack) );
		if( isLinkedStackEmpty(pStack) == TRUE )
			break;
	}
	printf("\n");
}

BOOL MoveCurrentNode(DirTree* dTree, char* DirectoryPath)	// 단순히 한단계 이동만 가능..
{
	BOOL ret = FALSE;
	if(strcmp(DirectoryPath,".")==0)		// DirectoryPath가 '.'일 경우는 현재의 경로를 나타낸다. 
	{
		return ret = TRUE;
	}
	else if(strcmp(DirectoryPath,"..")==0)	// ".."일경우..
	{
		if(dTree->currentNode == dTree->headerNode)	// 최상위 '/' 일경우는  현재 노드를 그대로 가르킨다. 
		{
			return ret = TRUE;
		}
		else	// 현재 위치가 최상위 노드가 아닐경우 자신의 상위 디렉토리로 이동한다. 
		{
			dTree->currentNode = dTree->currentNode->Parent;				// currentNode를 상위디렉토리로 이동..
			strncpy(dTree->currentNodeName,dTree->currentNode->name,20);	// 현재 노드의 맞춰 이름을 바꾼다. 
			return TRUE;
		}
	}
	else	// 그외 ....currentNode를 기준으로 하위 폴더로 이동한다. 
	{
		if( IsExistItem(dTree,DirectoryPath) == NULL) // 해당 DirectoryPath가 현재 currentNode를 기준으로 해서 존재하는지 확인한다. 
		{
			// printf("해당 디렉토리가 존재하지 않습니다. \n");
			return ret;
		}
		else	//존재할경우 ..
		{
			if( IsExistItem(dTree,DirectoryPath)->type =='d' || IsExistItem(dTree,DirectoryPath)->type =='s' )  // 해당 이름으로 존재는 하나 디렉토리인지 확인한다. 
			{	// 디렉토리라면 현재 노드를 다음과 같이 변경하고 이름을 바꿔준다. 
				BOOL state = TRUE;
				// ----------------------- 해당 위치에서의 허가권 검사 -----------------------
				if( state = permissionHandler(IsExistItem(dTree,DirectoryPath),'x') == FALSE )
				{
					printf("bash: cd: 허가 거부됨 '%s' \n",IsExistItem(dTree,DirectoryPath)->name);
					return FALSE;
				}
				// ----------------------- @해당 위치에서의 허가권 검사 ----------------------
				dTree->currentNode = IsExistItem(dTree,DirectoryPath);
				strncpy(dTree->currentNodeName,dTree->currentNode->name,20);
				return ret = TRUE;
			}
			else
			{
				// printf("-bash: cd: %s: 디렉토리가 아닙니다\n",DirectoryPath);
				return ret;
			}
		}
	}
}

BOOL MoveCurrentNode2(DirTree* dTree,char* address)	// 절대 및 상대 경로를 받아서 MoveCurrentNode함수를 통해 하나씩 이동한다.
{
	BOOL state = TRUE;
	char* str1 = NULL;
	char* div = "/";
	char temp[50];
	//printf("%s\n",address);
	strncpy(temp,address,50);
	dTree->TempNode = dTree->currentNode; // 잘못된 경로일 경우를 대비하여 TempNode에 저장한다. 
	if( strcmp(address,"/")==0)				// 단순히 경로가 "/"일경우는 최상위 디렉토리로 이동한다. 
	{
		dTree->currentNode = dTree->headerNode;
		strncpy(dTree->currentNodeName,dTree->currentNode->name,20);
	}
	else if( address[0] == '/' ) // 절대경로일경우 
	{
		DeleteSlash(address);		// 양 끝에 '/'가 존재한다면 삭제한다. 
		str1 =strtok(address, div);
		dTree->currentNode = dTree->headerNode;
		while(str1 != NULL)
		{
			if(str1 == NULL)
				break;
			state = MoveCurrentNode(dTree,str1);
			if( state == FALSE)
			{
				if(IsExistItem(dTree,str1) != NULL)
				{
					if(IsExistItem(dTree,str1)->type !='d' || IsExistItem(dTree,str1)->type !='s')
					{//	printf("-bash: cd: %s: 디렉토리가 아닙니다\n",temp);			
					}
				}
				else
				{
					printf("-bash: cd: %s: 그런 파일이나 디렉토리가 없음 \n",temp);
				}
				AdjustCurrentNode();
				return FALSE;
			}
			str1 = strtok(NULL, div );
		}
	}
	else					// 상대경로일 경우...
	{
		DeleteSlash(address);	// 양 끝에 '/'가 존재한다면 삭제한다. 
		str1 = strtok(address, div);
		while(str1 != NULL)
		{
			if(str1 == NULL)
				break;
			state = MoveCurrentNode(dTree,str1);
			if( state == FALSE)
			{
				if(IsExistItem(dTree,str1) != NULL)
				{
					if(IsExistItem(dTree,str1)->type !='d' || IsExistItem(dTree,str1)->type !='s')
					{	//printf("-bash: cd: %s: 디렉토리가 아닙니다\n",temp);
					}
				}
				else
				{
					printf("-bash: cd: %s: 그런 파일이나 디렉토리가 없음 \n",temp);
				}
				AdjustCurrentNode();
				return FALSE;
			}
			str1 = strtok(NULL, div );
		}

	}	
	
	return TRUE;
}
// ------------------------------------------------------------------------------------------------------------------

void AdjustCurrentNode()
{
	dTree->currentNode = dTree->TempNode;
	strncpy(dTree->currentNodeName,dTree->currentNode->name,20);
}


DirectoryNode* ReserchNode(DirTree* dTree, char* DirectoryPath)
{
	DirectoryNode* ret = NULL;
	if(strcmp(DirectoryPath,".")==0)		// DirectoryPath가 '.'일 경우는 현재의 경로를 나타낸다. 
	{
		return ret = dTree->currentNode;
	}
	else if(strcmp(DirectoryPath,"..")==0)	// ".."일경우..
	{
		if(dTree->currentNode == dTree->headerNode)	// 최상위 '/' 일경우는  현재 노드를 그대로 가르킨다. 
		{
			return ret = dTree->headerNode;
		}
		else	// 현재 위치가 최상위 노드가 아닐경우 자신의 상위 디렉토리로 이동한다. 
		{
			dTree->currentNode = dTree->currentNode->Parent;				// currentNode를 상위디렉토리로 이동..
			strncpy(dTree->currentNodeName,dTree->currentNode->name,20);	// 현재 노드의 맞춰 이름을 바꾼다. 
			return ret = dTree->currentNode;
		}
	}
	else	// 그외 ....currentNode를 기준으로 하위 폴더로 이동한다. 
	{
		if( IsExistItem(dTree,DirectoryPath) == NULL) // 해당 DirectoryPath가 현재 currentNode를 기준으로 해서 존재하는지 확인한다. 
		{
			// printf("해당 디렉토리가 존재하지 않습니다. \n");
			return ret = NULL;
		}
		else	//존재할경우 ..
		{
				dTree->currentNode = IsExistItem(dTree,DirectoryPath);
				strncpy(dTree->currentNodeName,dTree->currentNode->name,20);
				return ret = dTree->currentNode;
		}
	}
}


DirectoryNode* IsExistPathPrev(char* Path)		// 해당 노드의 경로 값을 받아서... 존재하면 해당 노드의 주소를 반환한다. 
{
	DirectoryNode* Return = NULL;
	char* str1 = NULL;
	char* div = "/";
	char temp[50];
	char temp2[50];
	//printf("%s\n",address);
	strncpy(temp,Path,50); 
	strncpy(temp2,Path,50);
	dTree->TempNode = dTree->currentNode;
	if(Path[0] =='/')							// 절대경로일 경우..
	{
		dTree->currentNode = dTree->headerNode;		// 경로를 '/' 위치로 이동....
		Path = DeleteSlash(Path);					//양끝의 '/' 를 제거한다. 
		str1 = strtok(Path, div);
		while( str1 != NULL )		//경로의 이름의 값이 없을 때 까지 조샇산다. 
		{
			if(str1 == NULL)
				break;
			Return = ReserchNode(dTree,str1);
			if(Return == NULL )
			{
				printf("mv1 : cannot stat '%s' : No such file or directory\n",temp);		
				return NULL;
			}
			str1 = strtok(NULL, div );
		}
	}
	else										// 상대경로일 경우.....
	{
		int i = 0;
		DeleteSlash(temp);
		str1 = strtok(temp, div);
		while(str1 != NULL)
		{
			if(str1 == NULL)
				break;
			Return = ReserchNode(dTree,str1);
			if(Return == NULL )
			{
				if(dTree->currentNode == dTree->headerNode){
					printf("");
				}
				else{
				// printf("mv2 : cannot stat '%s' : No such file or directory\n",temp2);		
					//return NULL;
				}
			}
			str1 = strtok(NULL, div );
		}
	}
	AdjustCurrentNode();
	return Return;
}

DirectoryNode* IsExistPathPrev2(char* Path)		// 해당 노드의 경로 값을 받아서... 존재하면 해당 노드의 주소를 반환한다. 
{
	DirectoryNode* Return = NULL;
	char* str1 = NULL;
	char* div = "/";
	char temp[50];
	char temp2[50];
	//printf("%s\n",address);
	strncpy(temp,Path,50); 
	strncpy(temp2,Path,50);
	dTree->TempNode = dTree->currentNode;

	if(Path[0] =='/')							// 절대경로일 경우..
	{
		dTree->currentNode = dTree->headerNode;		// 경로를 '/' 위치로 이동....
		Path = DeleteSlash(Path);					//양끝의 '/' 를 제거한다. 
		str1 = strtok(Path, div);
		while( str1 != NULL )		//경로의 이름의 값이 없을 때 까지 조샇산다. 
		{

			if(str1 == NULL)
				break;
			Return = ReserchNode(dTree,str1);
			if(Return == NULL )
			{
			//	printf("mv1 : cannot stat '%s' : No such file or directory\n",temp);		
				return NULL;
			}
			str1 = strtok(NULL, div );
		}
	}
	else										// 상대경로일 경우.....
	{
		int i = 0;
		str1 = strtok(temp, div);
		while(str1 != NULL)
		{
			if(str1 == NULL)
				break;
			Return = ReserchNode(dTree,str1);
			if(Return == NULL )
			{
				if(dTree->currentNode == dTree->headerNode){
					printf("");
				}else{
			//	printf("mv2 : cannot stat '%s' : No such file or directory\n",temp2);		
					return NULL;
				}
			}
			str1 = strtok(NULL, div );
		}
	}
	AdjustCurrentNode();
	return Return;
}





void CopyDirectoryToDirectory(char* PrevPath, char* NextPath)
{
	DirectoryNode* PrevNode = NULL;	// PrevPath 의 메모리 주소값을 저장
	DirectoryNode* NextNode = NULL;	// NextPath 의 메모리 주소값을 저장 
	DirectoryNode* TrailNode = NULL;// Node 복제가 이루어질경우에 쓰인다. 
	DirectoryNode* LeadNode = NULL; // Node 복제가 이루어질경우에 쓰인다. 
	DirectoryNode* currentNode = dTree->currentNode; // Node 복제가 이루어질경우에 쓰인다. 
	char NextPath2[50];
	char tempstr[50]="";
	BOOL state = TRUE;
	strncpy(NextPath2,NextPath,50);
	dTree->TempNode = dTree->currentNode;

	if(IsCharExist(NextPath, '/')== FALSE )	// NextPath 가 현재위치의 파일을 나타낼시 상대경로로 인한 충돌을 방지한다. 
	{
		char strtemp[50] = "./";
		strncat(strtemp,NextPath,50);
		strncpy(NextPath2,strtemp,50);
	}
	
	PrevNode = IsExistPathPrev(PrevPath);		// PrevNode 의 주소를 저장한다. 
	NextNode = IsExistPathPrev2(NextPath2);		// NextNOde 의 주소를 저장한다. 

	strncpy(tempstr,NextPath2,50);				// 파일 이름을 바꿔서 복사하게될 경우를 위해 NextPath2를 복사해둔다. 
	if( PrevNode == NULL)	// 만약 지정된 경로로 주소가 없을 경우는 강제 로 종료한다. 
	{
		printf("mv : cannot stat '%s' : No such file or directory\n",PrevPath);
		return;
	}
//	printf("mv : PrevPath = %s ,name = %s\n",PrevPath,PrevNode->name);

	// ----------------------- 해당 위치에서의 허가권 검사 -----------------------
	if( state = permissionHandler(PrevNode,'w') == FALSE )
	{
		printf("cp: omitting directory '%s' \n",PrevNode->name);
		return ;
	}
	// ----------------------- @해당 위치에서의 허가권 검사 ----------------------

	if( NextNode == NULL)
	{
		int i,j=0,k,cnt=0;
		char* str1 = NULL;
		char* str3=NULL;
		char* str4=NULL;
		
		i = strlen(tempstr);						// str2의 길이를 구한다. 
		for( k =0, j=0; k <i ; k++)
		{
			if( tempstr[k]=='/')
			{
				cnt++;
				j=k;
			}
		}
	//	printf("i = %d,cnt = %d j = %d\n",i,cnt,j);
		str3 = substring(tempstr,0,j+1);
		if( IsCharExist(tempstr,'/') == FALSE)
			str4 = substring(tempstr,j,i);
		else
			str4 = substring(tempstr,j+1,i);
		
		NextNode = IsExistPathPrev(str3);
		if( NextNode == NULL )
		{
			printf(" %s 해당 폴더가 없습니다. \n",str3);
		}
		else
		{
			dTree->currentNode = NextNode;
			strncpy(dTree->currentNodeName, NextNode->name,20);
			// NextNode로 이동... 
			//printf(" 복사대상 : %s %s\n", PrevNode->name,dTree->currentNode);
			LeadNode = TrailNode = dTree->currentNode->LChild; // 검색을 위해서..설정함.				
			while(LeadNode != NULL )
			{
				TrailNode = LeadNode;
				LeadNode = LeadNode -> RSibiling;
			}
			if( TrailNode == NULL )
			{
				dTree->currentNode->LChild = CopyTreeNode( PrevNode,0);
				dTree->currentNode->LChild->Parent = dTree->currentNode;
				strncpy(dTree->currentNode->LChild->name,str4,20);
				if(str4[0] == '.')
				{
					if(dTree->currentNode->LChild->type == 'd')
						dTree->currentNode->LChild->type = 's';
					else if( dTree->currentNode->LChild->type == 'f')
						dTree->currentNode->LChild->type = 'u';
				}
				else
				{
					if(dTree->currentNode->LChild->type == 'd')
						dTree->currentNode->LChild->type = 'd';
					else if( dTree->currentNode->LChild->type == 'f')
						dTree->currentNode->LChild->type = 'f';
				}
			}
			else
			{
				TrailNode->RSibiling = CopyTreeNode( PrevNode,0);
				TrailNode->RSibiling->Parent= dTree->currentNode;
				strncpy(TrailNode->RSibiling->name,str4,20);
				if(str4[0] == '.')
				{
					if(dTree->currentNode->LChild->type == 'd')
						dTree->currentNode->LChild->type = 's';
					else if( dTree->currentNode->LChild->type == 'f')
						dTree->currentNode->LChild->type = 'u';
				}
				else
				{
					if(dTree->currentNode->LChild->type == 'd')
						dTree->currentNode->LChild->type = 'd';
					else if( dTree->currentNode->LChild->type == 'f')
						dTree->currentNode->LChild->type = 'f';
				}
			}
		//	printf("%s 해당 폴더에서... %s 란 이름으로...\n",str3,str4);
		}
	}else
	{
		//printf(" %s 잘 선택 되었음... \n",NextNode->name);
		if( NextNode->type == 'd' || NextNode->type == 's')
		{
			dTree->currentNode = NextNode;
			strncpy(dTree->currentNodeName, NextNode->name,20);
			// NextNode로 이동... 
			//printf(" 복사대상 : %s %s\n", PrevNode->name,dTree->currentNode);
			LeadNode = TrailNode = dTree->currentNode->LChild; // 검색을 위해서..설정함.				
			while(LeadNode != NULL )
			{
				TrailNode = LeadNode;
				LeadNode = LeadNode -> RSibiling;
			}
			if( TrailNode == NULL )
			{
				dTree->currentNode->LChild = CopyTreeNode( PrevNode,0);
				dTree->currentNode->LChild->Parent = dTree->currentNode;
			}
			else
			{
				TrailNode->RSibiling = CopyTreeNode( PrevNode,0);
				TrailNode->RSibiling->Parent= dTree->currentNode;
			}
		}
		else	// 디렉토리가 아니고 파일일경우는..복사가 불가능 하므로...메시지를 띄운다. 
		{
			printf("mv : cannot stat '%s' : is not directory \n",NextPath2);
		}
	}
	dTree->currentNode = currentNode;
	strncpy(dTree->currentNodeName,dTree->currentNode->name,20);	// 다시 현재에 있엇던 위치로 간다. 
}

void MoveDirectoryToDirectory(char* PrevPath, char* NextPath)
{
	DirectoryNode* PrevNode;
	BOOL state;
	PrevNode = IsExistPathPrev(PrevPath);
	//printf("PrevNode :%s,부모 : %s\n", PrevNode->name,PrevNode->Parent->name);
	if( state = permissionHandler(PrevNode,'w') == FALSE )
	{
		printf("mv: cannot move '%s' : 허가 거부됨 \n",PrevNode->name);
		return ;
	}
	CopyDirectoryToDirectory(PrevPath, NextPath);
	dTree->TempNode = dTree->currentNode;
	if( PrevNode == NULL )
	{
		return;
	}
	else{
		if( PrevNode->Parent == NULL)
			dTree->currentNode = dTree->headerNode;
		else
			dTree->currentNode = PrevNode->Parent;
		// printf(" 현재 노드 : %s", PrevNode->Parent->name);
		DeleteDirectory(dTree,PrevNode->name,3);
	}
	AdjustCurrentNode();
}

BOOL ChangeAccessItem(DirTree* dTree, char* itemName, int Acess){			// 해당 대상의 허가권을 변경 
	BOOL ret = FALSE;
	DirectoryNode* SearchNode = NULL;
	char temp[4];
	int i=0,k=0;
	itoa(Acess, temp, 10);
	if( temp[0] > 55 || temp[1] > 55 || temp[2]>55 || temp[0]<48 || temp[1] < 48 || temp[2] <48 ){ // 아스키코드 48 : 0 55 : 7
		printf("chmod: invalid mode '%d' %c %c %c\n",Acess,temp[0],temp[1], temp[2]);
		return FALSE;
	}

	SearchNode = IsExistItem(dTree, itemName);
	// ----------------------- 해당 위치에서의 소유권 검사 -----------------------
	if( strcmp(UList->currentLoginUser,"root")!=0 ) // root 계정은 최고 관리자이므로 일반 사용자일 경우에면 검사한다. 
	{
		if( strcmp(UList->currentLoginUser,SearchNode->username)!=0 )
		{
			printf("chmod: changing permission of '%s' \n",itemName);
			return FALSE;
		}
	}
	// ----------------------- @해당 위치에서의 소유권 검사 ----------------------
	if(SearchNode != NULL )
	{
		SearchNode->chmod = Acess;
		for( i = 0 ; i < 3 ; i++)
		{
			k=3*i;
			if(temp[i] =='0')
			{
				SearchNode->permission[k+0] = 0;
				SearchNode->permission[k+1] = 0;
				SearchNode->permission[k+2] = 0;
			}
			else if(temp[i] =='1')
			{
				SearchNode->permission[k+0] = 0;
				SearchNode->permission[k+1] = 0;
				SearchNode->permission[k+2] = 1;
			}
			else if(temp[i] =='2')
			{
				SearchNode->permission[k+0] = 0;
				SearchNode->permission[k+1] = 1;
				SearchNode->permission[k+2] = 0;
			}
			else if(temp[i] =='3')
			{
				SearchNode->permission[k+0] = 0;
				SearchNode->permission[k+1] = 1;
				SearchNode->permission[k+2] = 1;
			}
			else if(temp[i] =='4')
			{
				SearchNode->permission[k+0] = 1;
				SearchNode->permission[k+1] = 0;
				SearchNode->permission[k+2] = 0;
			}
			else if(temp[i] =='5')
			{
				SearchNode->permission[k+0] = 1;
				SearchNode->permission[k+1] = 0;
				SearchNode->permission[k+2] = 1;
			}
			else if(temp[i] =='6')
			{
				SearchNode->permission[k+0] = 1;
				SearchNode->permission[k+1] = 1;
				SearchNode->permission[k+2] = 0;
			}
			else if(temp[i] =='7')
			{
				SearchNode->permission[k+0] = 1;
				SearchNode->permission[k+1] = 1;
				SearchNode->permission[k+2] = 1;
			}
		}
		ret = TRUE;
	}
	else
	{
		printf(" 해당 파일이 존재하지 않습니다. \n");
	}
	return ret;
}

BOOL ChangeOwnderItem(DirTree* dTree, char* itemName, char* userName, char* groupName){		// 해당 대상의 소유권을 변경 
	BOOL ret = FALSE;	
	BOOL state = TRUE;
	DirectoryNode* SearchNode = NULL;		

	SearchNode = IsExistItem(dTree, itemName);				// 현재위치에서 대상이 존재하는지를 조사. 

	if(SearchNode != NULL )
	{
		// ----------------------- 허가권 검사 -------------------------------------
		if( state = permissionHandler(SearchNode,'w') == FALSE )
		{
			printf("chown: cannot change owner '%s' : 허가 거부됨 \n",SearchNode->name);
			return ;
		}
		// -------------------- 허가권 검사 ---------------------------------------
		strncpy(SearchNode->username,userName,20);
		strncpy(SearchNode->groupname,groupName,20);
		ret = TRUE;
	}
	else
	{
		printf(" 해당 파일이 존재하지 않습니다. \n");
	}
	return ret;
}