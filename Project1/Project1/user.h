#ifndef _LINKEDLIST_
#define _LINKEDLIST_
/*
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
###			int UID;
###			int GID;
###			char directory[50];
###			int currentUserCount; 
###			char shel[20];
###			UserNode* headerNode;
###		}UserList;
###
###########################################################################
*/
typedef struct UserNodeType{  // ���� ���� ������ ��� UserNode
		char name[20];
		char directory[50];
		char passwd[20];
		int UID;
		int GID;
		char shel[20];
		struct UserNodeType* pLink;
}UserNode;

typedef struct UserListType{  // ������ �ֻ��� ���� createUserList
	int UID;
	int GID;
	int logincount;
	int currentUserCount;
	char directory[50];
	char shel[20];
	UserNode* headerNode;
	char currentLoginUser[20];
}UserList;

/*
#########################################################################
###
###			�Լ��� ����...

			creatUserList()
###			UserList* createList(char passwd[]);
###			: �н����带 �Է� �޾Ƽ� ������ ������ �����ϴ� ����Ʈ�� 
###				�ְ� ������ root ������ �����Ѵ�.
###			���� Ȱ�� :
###			UserList* UserData = createUserList("1234");
###			->> createUserList �Լ���. �޸𸮿� ���� �Ҵ� �ϰ�, �� �ּ� ���� UserData�� �����Ѵ�.
###			UserListŸ������ �⺻���� ����Ʈ Ʋ�� �����, 
###			root ������ �����ϵ��� createUserList �Լ��� �� �����Ѵ�. ��
###			�Լ����� root������ ��й�ȣ�� �����Ͽ� headerNode�� ���� ��Ų��. 
			
			addUser()
###			User�� List�� �߰���Ų��.
###			���� ���� : addUser( UserData , "�����1");
###			�̶� �����ϸ� TREU �� ��ȯ �ϰ�, ���н� �޽����� �Բ� FALSE �� ��ȯ �ϵ��� �Ѵ�.

			delUser()
###			User�� List���� �����. 
###			���� ���� : delUser( UserData , "�����1");
###			
###			���⼭ UserData�� createUserList()�� ��ȯ�� �޸��� �ּҸ� ����Ų��. 
###
###

#########################################################################
*/ 


#define BOOL int

UserList* createUserList(char passwd[]);			// �����Ϸ�

BOOL AddUser(UserList* UList, char* username);		// �����Ϸ� 
BOOL DelUser(UserList* UList, char* username,int option);		
BOOL modUser(UserList* UList, char* username, char option, char* data);
BOOL passwdChange(UserList* UList, char* username, char* password);
void passwd(UserList* UList, char* username);
BOOL IsExistUser(UserList* UList, char* username); // �ش� ������ ��������Ʈ�� �����ϴ����� ����, 
void ShowUserList(UserList* UList);
void LoginUser();
void ChangeUser(int option, char* username);
// ���丮�� �������� �����Ҷ� �ش� ������ �����ϴ����� �Ѱ��ش�. �����Ѵٸ� TRUE, ���Ѵٸ� FALSE�� ��ȯ�Ѵ�. 

#endif

#ifndef _COMMON_LIST_DEF_
#define _COMMON_LIST_DEF_

#define TRUE		1
#define FALSE		0

#endif

/////////////////////