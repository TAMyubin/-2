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
	SOCKET  sListen;  //��������׽���
	void     initSocket(HWND hwnd);  //��ʼ������
	BOOL	startServer(short sPort);//��ʼ������
	BOOL	closeServer();         //�رշ�����

	// �������
	virtual void onAccept(SOCKET s, struct sockaddr_in addr){}
	// һ�����ӹر�
	virtual void onClose(SOCKET s){}
	// һ�������ϵĶ��������
	virtual void onRead(SOCKET s, char *pBuffer, int nLen){}
	// һ�������ϵĶ��������
	virtual void onCommand(int wmId){}


	LRESULT  wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};