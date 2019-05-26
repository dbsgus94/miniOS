#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "header.h"

/*
	#########################################################################################################
	###																									  ###
	###	 typedef struct DirectoryNodeType{  // ���丮 ���� ������ ��� ��� 							  ###
	###			char name[20];		// ���丮�� ���� ��												  ###
	###			int chmod;			// �㰡�� ����														  ###
	###			char type;			// d : directory , f ���� , u : ��������							  ###
	###			int month;			// ���� �ñ� , ��													  ###
	###			int time;			// ���� �ð�														  ###
	###			char username[20];	// ������															  ###
	###			char groupname[20];	// �׷�																  ###
	###			struct DirectoryNodeType* Parretn;	// ���� ���丮 ����								  ###
	###			struct DirectoryNodeType* LChild;	// ���� ���丮 ����								  ###
	###			struct DirectoryNodeType* RSibiling; // �����丮�� ���� �����̳� ���丮�� ����		  ###
	###		}DirectoryNode;																				  ###
	###																									  ###
	###	typedef struct DirectoryLCRSTreeType{  // ���丮�� �ֻ��� ���� createUserList					  ###
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
		ReturnNode = ReturnNode->RSibiling;			// ���� ���..
	}
	return ReturnNode;								// ã�� ���ҽ� NULL ���� �ѱ��. 
}


void TransChmod( int chmod )		// int�� �޾ƿ� chmod ������ ���ڿ��� ��� �ϰ� �Ѵ�. 
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

void pwd(DirTree* dTree)		// ������ġ�� �˷��ִ� pwd �Լ� ... 
{
	DirectoryNode* current = dTree->currentNode;
	if(current->Parent == NULL)	// Paretn ��尡 NULL �ΰ��� .�ֻ��� ���丮 �̹Ƿ� .'/'�� ����Ѵ�.
	{
		printf("/\n");
		return ;
	}
	while(current->Parent != NULL)	// ������ġ���� ���� �θ���� �̵��ϸ� ���ؿ� push �Ѵ�.
	{
		Push(pStack, current->name);
		current = current->Parent;
	}

	while(1)	// push �� �����͸� ���ÿ� ��� �� ������... pop �Ͽ� ������.
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

BOOL MoveCurrentNode(DirTree* dTree, char* DirectoryPath)	// �ܼ��� �Ѵܰ� �̵��� ����..
{
	BOOL ret = FALSE;
	if(strcmp(DirectoryPath,".")==0)		// DirectoryPath�� '.'�� ���� ������ ��θ� ��Ÿ����. 
	{
		return ret = TRUE;
	}
	else if(strcmp(DirectoryPath,"..")==0)	// ".."�ϰ��..
	{
		if(dTree->currentNode == dTree->headerNode)	// �ֻ��� '/' �ϰ���  ���� ��带 �״�� ����Ų��. 
		{
			return ret = TRUE;
		}
		else	// ���� ��ġ�� �ֻ��� ��尡 �ƴҰ�� �ڽ��� ���� ���丮�� �̵��Ѵ�. 
		{
			dTree->currentNode = dTree->currentNode->Parent;				// currentNode�� �������丮�� �̵�..
			strncpy(dTree->currentNodeName,dTree->currentNode->name,20);	// ���� ����� ���� �̸��� �ٲ۴�. 
			return TRUE;
		}
	}
	else	// �׿� ....currentNode�� �������� ���� ������ �̵��Ѵ�. 
	{
		if( IsExistItem(dTree,DirectoryPath) == NULL) // �ش� DirectoryPath�� ���� currentNode�� �������� �ؼ� �����ϴ��� Ȯ���Ѵ�. 
		{
			// printf("�ش� ���丮�� �������� �ʽ��ϴ�. \n");
			return ret;
		}
		else	//�����Ұ�� ..
		{
			if( IsExistItem(dTree,DirectoryPath)->type =='d' || IsExistItem(dTree,DirectoryPath)->type =='s' )  // �ش� �̸����� ����� �ϳ� ���丮���� Ȯ���Ѵ�. 
			{	// ���丮��� ���� ��带 ������ ���� �����ϰ� �̸��� �ٲ��ش�. 
				BOOL state = TRUE;
				// ----------------------- �ش� ��ġ������ �㰡�� �˻� -----------------------
				if( state = permissionHandler(IsExistItem(dTree,DirectoryPath),'x') == FALSE )
				{
					printf("bash: cd: �㰡 �źε� '%s' \n",IsExistItem(dTree,DirectoryPath)->name);
					return FALSE;
				}
				// ----------------------- @�ش� ��ġ������ �㰡�� �˻� ----------------------
				dTree->currentNode = IsExistItem(dTree,DirectoryPath);
				strncpy(dTree->currentNodeName,dTree->currentNode->name,20);
				return ret = TRUE;
			}
			else
			{
				// printf("-bash: cd: %s: ���丮�� �ƴմϴ�\n",DirectoryPath);
				return ret;
			}
		}
	}
}

BOOL MoveCurrentNode2(DirTree* dTree,char* address)	// ���� �� ��� ��θ� �޾Ƽ� MoveCurrentNode�Լ��� ���� �ϳ��� �̵��Ѵ�.
{
	BOOL state = TRUE;
	char* str1 = NULL;
	char* div = "/";
	char temp[50];
	//printf("%s\n",address);
	strncpy(temp,address,50);
	dTree->TempNode = dTree->currentNode; // �߸��� ����� ��츦 ����Ͽ� TempNode�� �����Ѵ�. 
	if( strcmp(address,"/")==0)				// �ܼ��� ��ΰ� "/"�ϰ��� �ֻ��� ���丮�� �̵��Ѵ�. 
	{
		dTree->currentNode = dTree->headerNode;
		strncpy(dTree->currentNodeName,dTree->currentNode->name,20);
	}
	else if( address[0] == '/' ) // �������ϰ�� 
	{
		DeleteSlash(address);		// �� ���� '/'�� �����Ѵٸ� �����Ѵ�. 
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
					{//	printf("-bash: cd: %s: ���丮�� �ƴմϴ�\n",temp);			
					}
				}
				else
				{
					printf("-bash: cd: %s: �׷� �����̳� ���丮�� ���� \n",temp);
				}
				AdjustCurrentNode();
				return FALSE;
			}
			str1 = strtok(NULL, div );
		}
	}
	else					// ������� ���...
	{
		DeleteSlash(address);	// �� ���� '/'�� �����Ѵٸ� �����Ѵ�. 
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
					{	//printf("-bash: cd: %s: ���丮�� �ƴմϴ�\n",temp);
					}
				}
				else
				{
					printf("-bash: cd: %s: �׷� �����̳� ���丮�� ���� \n",temp);
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
	if(strcmp(DirectoryPath,".")==0)		// DirectoryPath�� '.'�� ���� ������ ��θ� ��Ÿ����. 
	{
		return ret = dTree->currentNode;
	}
	else if(strcmp(DirectoryPath,"..")==0)	// ".."�ϰ��..
	{
		if(dTree->currentNode == dTree->headerNode)	// �ֻ��� '/' �ϰ���  ���� ��带 �״�� ����Ų��. 
		{
			return ret = dTree->headerNode;
		}
		else	// ���� ��ġ�� �ֻ��� ��尡 �ƴҰ�� �ڽ��� ���� ���丮�� �̵��Ѵ�. 
		{
			dTree->currentNode = dTree->currentNode->Parent;				// currentNode�� �������丮�� �̵�..
			strncpy(dTree->currentNodeName,dTree->currentNode->name,20);	// ���� ����� ���� �̸��� �ٲ۴�. 
			return ret = dTree->currentNode;
		}
	}
	else	// �׿� ....currentNode�� �������� ���� ������ �̵��Ѵ�. 
	{
		if( IsExistItem(dTree,DirectoryPath) == NULL) // �ش� DirectoryPath�� ���� currentNode�� �������� �ؼ� �����ϴ��� Ȯ���Ѵ�. 
		{
			// printf("�ش� ���丮�� �������� �ʽ��ϴ�. \n");
			return ret = NULL;
		}
		else	//�����Ұ�� ..
		{
				dTree->currentNode = IsExistItem(dTree,DirectoryPath);
				strncpy(dTree->currentNodeName,dTree->currentNode->name,20);
				return ret = dTree->currentNode;
		}
	}
}


DirectoryNode* IsExistPathPrev(char* Path)		// �ش� ����� ��� ���� �޾Ƽ�... �����ϸ� �ش� ����� �ּҸ� ��ȯ�Ѵ�. 
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
	if(Path[0] =='/')							// �������� ���..
	{
		dTree->currentNode = dTree->headerNode;		// ��θ� '/' ��ġ�� �̵�....
		Path = DeleteSlash(Path);					//�糡�� '/' �� �����Ѵ�. 
		str1 = strtok(Path, div);
		while( str1 != NULL )		//����� �̸��� ���� ���� �� ���� �������. 
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
	else										// ������� ���.....
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

DirectoryNode* IsExistPathPrev2(char* Path)		// �ش� ����� ��� ���� �޾Ƽ�... �����ϸ� �ش� ����� �ּҸ� ��ȯ�Ѵ�. 
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

	if(Path[0] =='/')							// �������� ���..
	{
		dTree->currentNode = dTree->headerNode;		// ��θ� '/' ��ġ�� �̵�....
		Path = DeleteSlash(Path);					//�糡�� '/' �� �����Ѵ�. 
		str1 = strtok(Path, div);
		while( str1 != NULL )		//����� �̸��� ���� ���� �� ���� �������. 
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
	else										// ������� ���.....
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
	DirectoryNode* PrevNode = NULL;	// PrevPath �� �޸� �ּҰ��� ����
	DirectoryNode* NextNode = NULL;	// NextPath �� �޸� �ּҰ��� ���� 
	DirectoryNode* TrailNode = NULL;// Node ������ �̷������쿡 ���δ�. 
	DirectoryNode* LeadNode = NULL; // Node ������ �̷������쿡 ���δ�. 
	DirectoryNode* currentNode = dTree->currentNode; // Node ������ �̷������쿡 ���δ�. 
	char NextPath2[50];
	char tempstr[50]="";
	BOOL state = TRUE;
	strncpy(NextPath2,NextPath,50);
	dTree->TempNode = dTree->currentNode;

	if(IsCharExist(NextPath, '/')== FALSE )	// NextPath �� ������ġ�� ������ ��Ÿ���� ����η� ���� �浹�� �����Ѵ�. 
	{
		char strtemp[50] = "./";
		strncat(strtemp,NextPath,50);
		strncpy(NextPath2,strtemp,50);
	}
	
	PrevNode = IsExistPathPrev(PrevPath);		// PrevNode �� �ּҸ� �����Ѵ�. 
	NextNode = IsExistPathPrev2(NextPath2);		// NextNOde �� �ּҸ� �����Ѵ�. 

	strncpy(tempstr,NextPath2,50);				// ���� �̸��� �ٲ㼭 �����ϰԵ� ��츦 ���� NextPath2�� �����صд�. 
	if( PrevNode == NULL)	// ���� ������ ��η� �ּҰ� ���� ���� ���� �� �����Ѵ�. 
	{
		printf("mv : cannot stat '%s' : No such file or directory\n",PrevPath);
		return;
	}
//	printf("mv : PrevPath = %s ,name = %s\n",PrevPath,PrevNode->name);

	// ----------------------- �ش� ��ġ������ �㰡�� �˻� -----------------------
	if( state = permissionHandler(PrevNode,'w') == FALSE )
	{
		printf("cp: omitting directory '%s' \n",PrevNode->name);
		return ;
	}
	// ----------------------- @�ش� ��ġ������ �㰡�� �˻� ----------------------

	if( NextNode == NULL)
	{
		int i,j=0,k,cnt=0;
		char* str1 = NULL;
		char* str3=NULL;
		char* str4=NULL;
		
		i = strlen(tempstr);						// str2�� ���̸� ���Ѵ�. 
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
			printf(" %s �ش� ������ �����ϴ�. \n",str3);
		}
		else
		{
			dTree->currentNode = NextNode;
			strncpy(dTree->currentNodeName, NextNode->name,20);
			// NextNode�� �̵�... 
			//printf(" ������ : %s %s\n", PrevNode->name,dTree->currentNode);
			LeadNode = TrailNode = dTree->currentNode->LChild; // �˻��� ���ؼ�..������.				
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
		//	printf("%s �ش� ��������... %s �� �̸�����...\n",str3,str4);
		}
	}else
	{
		//printf(" %s �� ���� �Ǿ���... \n",NextNode->name);
		if( NextNode->type == 'd' || NextNode->type == 's')
		{
			dTree->currentNode = NextNode;
			strncpy(dTree->currentNodeName, NextNode->name,20);
			// NextNode�� �̵�... 
			//printf(" ������ : %s %s\n", PrevNode->name,dTree->currentNode);
			LeadNode = TrailNode = dTree->currentNode->LChild; // �˻��� ���ؼ�..������.				
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
		else	// ���丮�� �ƴϰ� �����ϰ���..���簡 �Ұ��� �ϹǷ�...�޽����� ����. 
		{
			printf("mv : cannot stat '%s' : is not directory \n",NextPath2);
		}
	}
	dTree->currentNode = currentNode;
	strncpy(dTree->currentNodeName,dTree->currentNode->name,20);	// �ٽ� ���翡 �־��� ��ġ�� ����. 
}

void MoveDirectoryToDirectory(char* PrevPath, char* NextPath)
{
	DirectoryNode* PrevNode;
	BOOL state;
	PrevNode = IsExistPathPrev(PrevPath);
	//printf("PrevNode :%s,�θ� : %s\n", PrevNode->name,PrevNode->Parent->name);
	if( state = permissionHandler(PrevNode,'w') == FALSE )
	{
		printf("mv: cannot move '%s' : �㰡 �źε� \n",PrevNode->name);
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
		// printf(" ���� ��� : %s", PrevNode->Parent->name);
		DeleteDirectory(dTree,PrevNode->name,3);
	}
	AdjustCurrentNode();
}

BOOL ChangeAccessItem(DirTree* dTree, char* itemName, int Acess){			// �ش� ����� �㰡���� ���� 
	BOOL ret = FALSE;
	DirectoryNode* SearchNode = NULL;
	char temp[4];
	int i=0,k=0;
	itoa(Acess, temp, 10);
	if( temp[0] > 55 || temp[1] > 55 || temp[2]>55 || temp[0]<48 || temp[1] < 48 || temp[2] <48 ){ // �ƽ�Ű�ڵ� 48 : 0 55 : 7
		printf("chmod: invalid mode '%d' %c %c %c\n",Acess,temp[0],temp[1], temp[2]);
		return FALSE;
	}

	SearchNode = IsExistItem(dTree, itemName);
	// ----------------------- �ش� ��ġ������ ������ �˻� -----------------------
	if( strcmp(UList->currentLoginUser,"root")!=0 ) // root ������ �ְ� �������̹Ƿ� �Ϲ� ������� ��쿡�� �˻��Ѵ�. 
	{
		if( strcmp(UList->currentLoginUser,SearchNode->username)!=0 )
		{
			printf("chmod: changing permission of '%s' \n",itemName);
			return FALSE;
		}
	}
	// ----------------------- @�ش� ��ġ������ ������ �˻� ----------------------
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
		printf(" �ش� ������ �������� �ʽ��ϴ�. \n");
	}
	return ret;
}

BOOL ChangeOwnderItem(DirTree* dTree, char* itemName, char* userName, char* groupName){		// �ش� ����� �������� ���� 
	BOOL ret = FALSE;	
	BOOL state = TRUE;
	DirectoryNode* SearchNode = NULL;		

	SearchNode = IsExistItem(dTree, itemName);				// ������ġ���� ����� �����ϴ����� ����. 

	if(SearchNode != NULL )
	{
		// ----------------------- �㰡�� �˻� -------------------------------------
		if( state = permissionHandler(SearchNode,'w') == FALSE )
		{
			printf("chown: cannot change owner '%s' : �㰡 �źε� \n",SearchNode->name);
			return ;
		}
		// -------------------- �㰡�� �˻� ---------------------------------------
		strncpy(SearchNode->username,userName,20);
		strncpy(SearchNode->groupname,groupName,20);
		ret = TRUE;
	}
	else
	{
		printf(" �ش� ������ �������� �ʽ��ϴ�. \n");
	}
	return ret;
}