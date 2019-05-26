#ifndef _Directory_
#define _Directory_

#define BOOL int
#define TRUE 1
#define FALSE 0
// 간혹 주석에 sina 라 보이는 부분은 ... 팀원 박기태의 평상시 닉네임이 sina 입니다.
// 그래서 홈페이지 주소도.. isina.kr ....^^


//---------------------------- 구조체 선언부 ---------------------------------
typedef struct DirectoryLCRSTreeNodeType{  // 디렉토리 개개 정보를 담는 UserNode
	char name[20];     // 디렉토리및 파일 명 
	int chmod;   // 허가권 설정 
	int permission[9];
	char type;         // d : directory , f 파일 , u : 숨김파일 
	int month;			// 생성 시기 , 달 
	int year;			// 생성 시간
	char username[20];
	char groupname[20];
	struct DirectoryLCRSTreeNodeType* Parent;	// pwd 명령어를 사용하기 위해선... 부모노드의 존재를 알아야 하기에.... 추가 [박기태] 리버전 : 12
	struct DirectoryLCRSTreeNodeType* LChild;	// 하위 디렉토리 구조 
	struct DirectoryLCRSTreeNodeType* RSibiling; // 현디렉토리의 다음 파일이나 디렉토리를 연결
}DirectoryNode;

typedef struct DirectoryLCRSTreeType{  // 디렉토리타입의 최상위 접근 createUserList
	char currentNodeName[20];
	char moveNodename[20];
	DirectoryNode* TempNode;
	DirectoryNode* headerNode;
	DirectoryNode* currentNode; // 추가 by [sina] // 현재 노드 정보를 담고 있는다. 단독 변수로의 1차적 접근을 차단한다. 
}DirTree;
//---------------------------- 구조체 선언부 ---------------------------------

// 디렉토리 생성 
DirTree* CreateRootDirectory();  // 구현 완료 
BOOL MakeDirectory(DirTree* dTree, char* dirName);  // 구현 완료 
BOOL MakeFile(DirTree* dTree, char* dirName);		// 구현 완료  리비전 23
BOOL RemoveFile(DirTree* dTree, char* fileName);
// 디렉토리 삭제 
void FreeSubDirectory( DirectoryNode* SubTree);		// 구현 완료 [sina], 리비전 15 
BOOL DeleteDirectory(DirTree* dTree, char* dirName,int option);// 구현 완료 
// 디렉토리 위치 변경
void ChangeDirectory(char* dirName,DirTree* dTree);  // 구현 완료 
BOOL ChangeDirectoryPath( DirectoryNode* dTree, DirTree* Tree, char* dirName); // 구현완료  // 디렉토리의 위치를 변경한다.
// 허가권 소유권 변경
BOOL ChangeAccessItem(DirTree* dTree, char* itemName, int Acess);	// 구현 완료 리비전 23
BOOL ChangeOwnderItem(DirTree* dTree, char* itemName, char* userName, char* groupName); //  구현완료 리비전 23
// 유저와 그룹으로 소유권을 변경

// dTree->currentNode 변경
BOOL MoveDirectory(DirTree* dTree,char* PrevdirPath,char* NextdirPath);

// 디렉토리 이동 및 삭제 
BOOL CopyDirectory(DirTree* dTree,char* PrevdirPath,char* NextdirPath);
DirectoryNode* CopyTreeNode( DirectoryNode* Original , int i );

DirectoryNode* IsExistItem(DirTree* dTree, char* itemName); // 구현완료  
// 파일이나 디렉토리가 존재하는지를 조사. 

void Commnad_LL(DirTree* dTree,int opt); 
void Commnad_LS(DirTree* dTree,int opt);
void ListShowAllDirectory(DirTree* dTree, char* dirName); // 구현완료 디렉토리의 숨김파일 까지 다 출력한다. ( ls -al 명령어 )
BOOL DefinePathType(char* Path);  // 해당 값이 상대경로인지 절대경로인지 넘겨준다. 
void AdjustCurrentNode();
DirectoryNode* DirectoryNodeAddressFromDirectPath(DirTree* dTree,char* address);		
// 2.0 에서 필요, 해당 경로가 절대경로일시 해당되는 노드의 주소를 반환한다. 
BOOL MoveCurrentNode2(DirTree* dTree,char* address);
void TransChmod( int chmod );
void pwd(DirTree* dTree);
DirectoryNode* ReserchNode(DirTree* dTree, char* DirectoryPath);
DirectoryNode* IsExistPathPrev(char* Path);
void MoveDirectoryToDirectory(char* PrevPath, char* NextPath);
void CopyDirectoryToDirectory(char* PrevPath, char* NextPath);

BOOL permissionHandler(DirectoryNode* Node, char option);

#endif