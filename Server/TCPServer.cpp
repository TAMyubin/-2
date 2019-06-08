#include "TCPServer.h"

TCPServer::TCPServer()  //��ʼ������
{
	sListen = NULL;


}
TCPServer::~TCPServer()  //��������
{
	closeServer();
}

void TCPServer::initSocket(HWND hwnd)  //��ʼ������
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sListen = socket(AF_INET, SOCK_STREAM, 0);
	this->hwnd = hwnd;
}

BOOL TCPServer::startServer(short sPort)//��ʼ������
{
	//���socketaddr_in�ṹ
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(sPort);
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	//���׽��ֵ�һ�����ص�ַ
	if (bind(sListen, (LPSOCKADDR)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{

		return 2;
	}

	WSAAsyncSelect(sListen, hwnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
	//�������ģʽ
	if (listen(sListen, 2) == SOCKET_ERROR)
	{

		return 3;
	}

	return true;
}

BOOL  TCPServer::closeServer()         //�رշ�����
{
	closesocket(sListen);
	WSACleanup();
	return true;
}




LRESULT TCPServer::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:	//lParamΪ���ӷ������Ϣ
	{
						SOCKET	sClient;  //���������׽���
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

