#pragma once
#define MSG_SIZE 1024
#define MSGID_MAX             100  //��ϢID�����ֵ   

#define MSGID_LOGIN             1   //��¼��Ϣ
#define MSGID_CHAT              2   //������Ϣ
#define MSGID_PLAYER_NEW        3   //����������Ϣ
#define MSGID_PLAYER_ALL          4   //�������������Ϣ
#define MSGID_PLAYER_SELF        5   //���ѵ�¼�ɹ����û���Ϣ
#define MSGID_PLAYER_DELETE    6   //ɾ�������Ϣ

#pragma pack(push) //�������״̬
#pragma pack(4)//�趨Ϊ4�ֽڶ���

struct PlayerInfo
{
	int s;
	wchar_t cName[50];
};

struct Msg_Header
{
	int   nType;  //��Ϣ����
	int   nLen;    //��Ϣ���ȣ���ְ�ʱ��
};

struct Msg_Login :public Msg_Header
{
	wchar_t cName[50];         //�û���
	wchar_t cPassword[50];          //����
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