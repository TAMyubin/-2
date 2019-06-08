#pragma once
#include "TCPServer.h"
#include "MsgDefine.h"
#include <map>
using namespace std;


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }

const int ID_EDIT = 11;                 //���巢���ı���ID
const int ID_SEND = 12;            //���巢�Ͱ�ťID
const int ID_EDIT_READONLY = 13;            //������հ�ťID

const int PORT = 8000;
struct Client{
	sockaddr_in addr;
	int nRemainLen;  //ʣ�µĳ���
	char msgBuffer[MSG_SIZE * 2];  //�׽��ֽ��ջ����� 
	PlayerInfo player;
};

class Game :
	public TCPServer
{
private:
	HWND hEdit;     //������Ϣ�ı����� 
	HWND hEditReadOnly;  //ֻ���ı���
	HWND hwnd;     //��ǰ���ھ����Ա
	HINSTANCE hInst; //��ǰӦ��ʵ����Ա

	HWND hBtn_Send; //����������Ϣ��ť���
	wchar_t bigBuffer[1024 * 1024];
	map<int, Client* > clientMap;


public:
	Game();
	~Game();
	void initGame(HWND hwnd, HINSTANCE hInst);


	void onCommand(int wmId);

	// �������
	void onAccept(SOCKET sClient, struct sockaddr_in addr);
	// һ�����ӹر�
	void onClose(SOCKET s);
	// һ�������ϵĶ��������
	void onRead(SOCKET s, char *pBuffer, int nLen);

	int sendMsg(SOCKET s, char* buffer, int len); //���͸�ĳ���ͻ�����Ϣ
	int sendMsgToAll(char* buffer, int len);   //������Ϣ�����пͻ�
	int sendMsgExceptOne(SOCKET s, char *buffer, int len);
	void showInfo(wchar_t *p);

	void copyRemainMsg(Client* client, char *pBuffer, int nLen, int nRemainLen);
	void splitMsg(SOCKET s, char *pBuffer, int nLen);
	//����¿ͻ��������Ϣ
	void addClient(int s, struct sockaddr_in addr);
	//ɾ���ҵ��Ŀͻ��������Ϣ
	void deleteClient(int s);
	//�����׽���ID��ȡһ���û���Ϣ
	Client* getClient(int s);
	//�������û��б�
	void clearClientMap();

	void onReadPackage(int s, Client* client, char *pBuffer, int nLen);
};

