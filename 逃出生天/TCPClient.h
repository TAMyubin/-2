#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include "MsgDefine.h"

const int WM_SOCKET = WM_USER + 101;
class TCPClient
{
private:
	int nRemainLen;
	char msgBuffer[MSG_SIZE * 2];
public:
	TCPClient(void);
	~TCPClient(void);
	SOCKET	sClient;  //定义数据套接字
	HWND hwnd;
	LRESULT wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// 连接完成
	virtual void onConnect(LPARAM lParam){}
	// 一个连接关闭
	virtual void onClose(SOCKET s){}
	// 一个连接上的读操作完成
	virtual void onRead(SOCKET s, char *pBuffer, int nLen){}

	virtual void showInfo(WCHAR *p){}
	void     initSocket(HWND hwnd);  //初始化函数
	BOOL	connectServer(const char *ipaddr,short sPort);//开始连接服务器
	BOOL	releaseClient();         //关闭客户端
	int 	sendMsg(char* pBuffer,int nLen); //发送信息

	void copyRemainMsg(char *pBuffer, int nLen, int nRemainLen);
	void splitMsg(SOCKET s, char *pBuffer, int nLen);

};