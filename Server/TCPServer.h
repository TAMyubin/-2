#include <iostream>
using namespace std;
#include "MsgDefine.h"
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>



const int WM_SOCKET = WM_USER + 101;

class TCPServer
{
private:

	HWND hwnd;

public:

	TCPServer(void);
	~TCPServer(void);
	SOCKET  sListen;  //定义监听套接字
	void     initSocket(HWND hwnd);  //初始化函数
	BOOL	startServer(short sPort);//开始服务器
	BOOL	closeServer();         //关闭服务器

	// 连接完成
	virtual void onAccept(SOCKET s, struct sockaddr_in addr){}
	// 一个连接关闭
	virtual void onClose(SOCKET s){}
	// 一个连接上的读操作完成
	virtual void onRead(SOCKET s, char *pBuffer, int nLen){}
	// 一个连接上的读操作完成
	virtual void onCommand(int wmId){}


	LRESULT  wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};