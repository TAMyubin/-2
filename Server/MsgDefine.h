#pragma once
#define MSG_SIZE 1024
#define MSGID_MAX             100  //消息ID的最大值   

#define MSGID_LOGIN             1   //登录消息
#define MSGID_CHAT              2   //聊天消息
#define MSGID_PLAYER_NEW        3   //添加新玩家消息
#define MSGID_PLAYER_ALL          4   //所有其它玩家消息
#define MSGID_PLAYER_SELF        5   //自已登录成功的用户消息
#define MSGID_PLAYER_DELETE    6   //删除玩家消息

#pragma pack(push) //保存对齐状态
#pragma pack(4)//设定为4字节对齐

struct PlayerInfo
{
	int s;
	wchar_t cName[50];
};

struct Msg_Header
{
	int   nType;  //消息类型
	int   nLen;    //消息长度，需分包时用
};

struct Msg_Login :public Msg_Header
{
	wchar_t cName[50];         //用户名
	wchar_t cPassword[50];          //密码
};


struct Msg_Player :public Msg_Header
{

	struct PlayerInfo player;
};

struct Msg_Player_Delete :public Msg_Header
{
	int s;
};


struct Msg_Chat :public Msg_Header
{
	int toId;
	wchar_t chat[200];
};



#pragma pack() 