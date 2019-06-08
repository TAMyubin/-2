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
	SOCKET	sClient;  //���������׽���
	HWND hwnd;
	LRESULT wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// �������
	virtual void onConnect(LPARAM lParam){}
	// һ�����ӹر�
	virtual void onClose(SOCKET s){}
	// һ�������ϵĶ��������
	virtual void onRead(SOCKET s, char *pBuffer, int nLen){}

	virtual void showInfo(WCHAR *p){}
	void     initSocket(HWND hwnd);  //��ʼ������
	BOOL	connectServer(const char *ipaddr,short sPort);//��ʼ���ӷ�����
	BOOL	releaseClient();         //�رտͻ���
	int 	sendMsg(char* pBuffer,int nLen); //������Ϣ

	void copyRemainMsg(char *pBuffer, int nLen, int nRemainLen);
	void splitMsg(SOCKET s, char *pBuffer, int nLen);

};