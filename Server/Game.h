#pragma once
#include "TCPServer.h"
#include "MsgDefine.h"
#include <map>
using namespace std;


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }

const int ID_EDIT = 11;                 //定义发送文本框ID
const int ID_SEND = 12;            //定义发送按钮ID
const int ID_EDIT_READONLY = 13;            //定义接收按钮ID

const int PORT = 8000;
struct Client{
	sockaddr_in addr;
	int nRemainLen;  //剩下的长度
	char msgBuffer[MSG_SIZE * 2];  //套接字接收缓冲区 
	PlayerInfo player;
};

class Game :
	public TCPServer
{
private:
	HWND hEdit;     //发送信息文本框句柄 
	HWND hEditReadOnly;  //只读文本框
	HWND hwnd;     //当前窗口句柄成员
	HINSTANCE hInst; //当前应用实例成员

	HWND hBtn_Send; //发送聊天信息按钮句柄
	wchar_t bigBuffer[1024 * 1024];
	map<int, Client* > clientMap;


public:
	Game();
	~Game();
	void initGame(HWND hwnd, HINSTANCE hInst);


	void onCommand(int wmId);

	// 连接完成
	void onAccept(SOCKET sClient, struct sockaddr_in addr);
	// 一个连接关闭
	void onClose(SOCKET s);
	// 一个连接上的读操作完成
	void onRead(SOCKET s, char *pBuffer, int nLen);

	int sendMsg(SOCKET s, char* buffer, int len); //发送给某个客户端信息
	int sendMsgToAll(char* buffer, int len);   //发送消息给所有客户
	int sendMsgExceptOne(SOCKET s, char *buffer, int len);
	void showInfo(wchar_t *p);

	void copyRemainMsg(Client* client, char *pBuffer, int nLen, int nRemainLen);
	void splitMsg(SOCKET s, char *pBuffer, int nLen);
	//添加新客户端玩家信息
	void addClient(int s, struct sockaddr_in addr);
	//删除找到的客户端玩家信息
	void deleteClient(int s);
	//根据套接字ID获取一个用户信息
	Client* getClient(int s);
	//清空玩家用户列表
	void clearClientMap();

	void onReadPackage(int s, Client* client, char *pBuffer, int nLen);
};

