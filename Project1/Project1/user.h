#ifndef _LINKEDLIST_
#define _LINKEDLIST_
/*
###########################################################################
###		UserAdd( 유저이름 );  // 유저이름으로 해당 유저를 만든다. 
###			--> 유저를 만드는 동시에 디렉토리를 만들고 , 해당 bash 파일이나 history 파일을 만든다.	
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
typedef struct UserNodeType{  // 유저 개개 정보를 담는 UserNode
		char name[20];
		char directory[50];
		char passwd[20];
		int UID;
		int GID;
		char shel[20];
		struct UserNodeType* pLink;
}UserNode;

typedef struct UserListType{  // 유저의 최상위 접근 createUserList
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
###			함수별 설명...

			creatUserList()
###			UserList* createList(char passwd[]);
###			: 패스워드를 입력 받아서 유저의 정보를 저장하는 리스트와 
###				최고 계정인 root 계정을 저장한다.
###			실제 활용 :
###			UserList* UserData = createUserList("1234");
###			->> createUserList 함수로. 메모리에 동적 할당 하고, 그 주소 값을 UserData에 저장한다.
###			UserList타입으로 기본적인 리스트 틀을 만들고, 
###			root 계정을 생성하도록 createUserList 함수내 에 삽입한다. 즉
###			함수내에 root계정과 비밀번호를 저장하여 headerNode로 연결 시킨다. 
			
			addUser()
###			User를 List에 추가시킨다.
###			실제 적용 : addUser( UserData , "사용자1");
###			이때 성공하면 TREU 을 반환 하고, 실패시 메시지와 함께 FALSE 을 반환 하도록 한다.

			delUser()
###			User를 List에서 지운다. 
###			실제 적용 : delUser( UserData , "사용자1");
###			
###			여기서 UserData는 createUserList()로 반환된 메모리의 주소를 가르킨다. 
###
###

#########################################################################
*/ 


#define BOOL int

UserList* createUserList(char passwd[]);			// 구현완료

BOOL AddUser(UserList* UList, char* username);		// 구현완료 
BOOL DelUser(UserList* UList, char* username,int option);		
BOOL modUser(UserList* UList, char* username, char option, char* data);
BOOL passwdChange(UserList* UList, char* username, char* password);
void passwd(UserList* UList, char* username);
BOOL IsExistUser(UserList* UList, char* username); // 해당 유저가 유저리스트에 존재하는지를 조사, 
void ShowUserList(UserList* UList);
void LoginUser();
void ChangeUser(int option, char* username);
// 디렉토리의 소유권을 변경할때 해당 유저가 존재하는지를 넘겨준다. 존재한다면 TRUE, 안한다면 FALSE를 반환한다. 

#endif

#ifndef _COMMON_LIST_DEF_
#define _COMMON_LIST_DEF_

#define TRUE		1
#define FALSE		0

#endif

/////////////////////