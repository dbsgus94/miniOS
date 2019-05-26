#ifndef _Directory_
#define _Directory_

#define BOOL int
#define TRUE 1
#define FALSE 0
// ��Ȥ �ּ��� sina �� ���̴� �κ��� ... ���� �ڱ����� ���� �г����� sina �Դϴ�.
// �׷��� Ȩ������ �ּҵ�.. isina.kr ....^^


//---------------------------- ����ü ����� ---------------------------------
typedef struct DirectoryLCRSTreeNodeType{  // ���丮 ���� ������ ��� UserNode
	char name[20];     // ���丮�� ���� �� 
	int chmod;   // �㰡�� ���� 
	int permission[9];
	char type;         // d : directory , f ���� , u : �������� 
	int month;			// ���� �ñ� , �� 
	int year;			// ���� �ð�
	char username[20];
	char groupname[20];
	struct DirectoryLCRSTreeNodeType* Parent;	// pwd ��ɾ ����ϱ� ���ؼ�... �θ����� ���縦 �˾ƾ� �ϱ⿡.... �߰� [�ڱ���] ������ : 12
	struct DirectoryLCRSTreeNodeType* LChild;	// ���� ���丮 ���� 
	struct DirectoryLCRSTreeNodeType* RSibiling; // �����丮�� ���� �����̳� ���丮�� ����
}DirectoryNode;

typedef struct DirectoryLCRSTreeType{  // ���丮Ÿ���� �ֻ��� ���� createUserList
	char currentNodeName[20];
	char moveNodename[20];
	DirectoryNode* TempNode;
	DirectoryNode* headerNode;
	DirectoryNode* currentNode; // �߰� by [sina] // ���� ��� ������ ��� �ִ´�. �ܵ� �������� 1���� ������ �����Ѵ�. 
}DirTree;
//---------------------------- ����ü ����� ---------------------------------

// ���丮 ���� 
DirTree* CreateRootDirectory();  // ���� �Ϸ� 
BOOL MakeDirectory(DirTree* dTree, char* dirName);  // ���� �Ϸ� 
BOOL MakeFile(DirTree* dTree, char* dirName);		// ���� �Ϸ�  ������ 23
BOOL RemoveFile(DirTree* dTree, char* fileName);
// ���丮 ���� 
void FreeSubDirectory( DirectoryNode* SubTree);		// ���� �Ϸ� [sina], ������ 15 
BOOL DeleteDirectory(DirTree* dTree, char* dirName,int option);// ���� �Ϸ� 
// ���丮 ��ġ ����
void ChangeDirectory(char* dirName,DirTree* dTree);  // ���� �Ϸ� 
BOOL ChangeDirectoryPath( DirectoryNode* dTree, DirTree* Tree, char* dirName); // �����Ϸ�  // ���丮�� ��ġ�� �����Ѵ�.
// �㰡�� ������ ����
BOOL ChangeAccessItem(DirTree* dTree, char* itemName, int Acess);	// ���� �Ϸ� ������ 23
BOOL ChangeOwnderItem(DirTree* dTree, char* itemName, char* userName, char* groupName); //  �����Ϸ� ������ 23
// ������ �׷����� �������� ����

// dTree->currentNode ����
BOOL MoveDirectory(DirTree* dTree,char* PrevdirPath,char* NextdirPath);

// ���丮 �̵� �� ���� 
BOOL CopyDirectory(DirTree* dTree,char* PrevdirPath,char* NextdirPath);
DirectoryNode* CopyTreeNode( DirectoryNode* Original , int i );

DirectoryNode* IsExistItem(DirTree* dTree, char* itemName); // �����Ϸ�  
// �����̳� ���丮�� �����ϴ����� ����. 

void Commnad_LL(DirTree* dTree,int opt); 
void Commnad_LS(DirTree* dTree,int opt);
void ListShowAllDirectory(DirTree* dTree, char* dirName); // �����Ϸ� ���丮�� �������� ���� �� ����Ѵ�. ( ls -al ��ɾ� )
BOOL DefinePathType(char* Path);  // �ش� ���� ��������� ���������� �Ѱ��ش�. 
void AdjustCurrentNode();
DirectoryNode* DirectoryNodeAddressFromDirectPath(DirTree* dTree,char* address);		
// 2.0 ���� �ʿ�, �ش� ��ΰ� �������Ͻ� �ش�Ǵ� ����� �ּҸ� ��ȯ�Ѵ�. 
BOOL MoveCurrentNode2(DirTree* dTree,char* address);
void TransChmod( int chmod );
void pwd(DirTree* dTree);
DirectoryNode* ReserchNode(DirTree* dTree, char* DirectoryPath);
DirectoryNode* IsExistPathPrev(char* Path);
void MoveDirectoryToDirectory(char* PrevPath, char* NextPath);
void CopyDirectoryToDirectory(char* PrevPath, char* NextPath);

BOOL permissionHandler(DirectoryNode* Node, char option);

#endif