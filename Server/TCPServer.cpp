#include "TCPServer.h"

TCPServer::TCPServer()  //初始化函数
{
	sListen = NULL;


}
TCPServer::~TCPServer()  //析构函数
{
	closeServer();
}

void TCPServer::initSocket(HWND hwnd)  //初始化函数
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sListen = socket(AF_INET, SOCK_STREAM, 0);
	this->hwnd = hwnd;
}

BOOL TCPServer::startServer(short sPort)//开始服务器
{
	//填充socketaddr_in结构
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(sPort);
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	//绑定套接字到一个本地地址
	if (bind(sListen, (LPSOCKADDR)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{

		return 2;
	}

	WSAAsyncSelect(sListen, hwnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
	//进入监听模式
	if (listen(sListen, 2) == SOCKET_ERROR)
	{

		return 3;
	}

	return true;
}

BOOL  TCPServer::closeServer()         //关闭服务器
{
	closesocket(sListen);
	WSACleanup();
	return true;
}




LRESULT TCPServer::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:	//lParam为连接服务端消息
	{
						SOCKET	sClient;  //定义数据套接字
						struct sockaddr_in clientAddr;
						int addrlen;
						addrlen = sizeof(clientAddr);
						sClient = accept(sListen, (struct sockaddr *)&clientAddr, &addrlen);
						WSAAsyncSelect(sClient, hwnd, WM_SOCKET, FD_WRITE | FD_READ | FD_CLOSE);
						onAccept(sClient, clientAddr);
	}
		break;
	case FD_READ:
	{
					char recvBuffer[MSG_SIZE];
					memset(recvBuffer, 0, MSG_SIZE);
					int iLen = recv(wParam, recvBuffer, MSG_SIZE, 0);
					if (iLen>0 && iLen<MSG_SIZE)
					{
						recvBuffer[iLen] = '\0';
						onRead(wParam, recvBuffer, iLen);
					}
	}
		break;
	case FD_WRITE:
		break;
	case FD_CLOSE:
		onClose(wParam);
		break;
	}
	return 0;
}

