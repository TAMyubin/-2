#include "TCPClient.h"

TCPClient::TCPClient()  //初始化函数
{	
	sClient=NULL;
	nRemainLen = 0;

}
TCPClient::~TCPClient()  //初始化函数
{
	releaseClient();
}


void TCPClient::initSocket(HWND hwnd)  //初始化函数
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sClient = socket(AF_INET, SOCK_STREAM, 0);
	WSAAsyncSelect(sClient, hwnd, WM_SOCKET, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	this->hwnd = hwnd;
}

BOOL TCPClient::connectServer(const char *ipaddr,short sPort)//连接服务器
{
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(ipaddr);
	serverAddr.sin_port = htons(sPort);
	
	int nRet=connect(sClient,(struct sockaddr *)&serverAddr,sizeof(serverAddr)); 
	if (nRet==0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
BOOL  TCPClient::releaseClient()         //关闭客户端
{
	closesocket(sClient);
	WSACleanup();
	return true;
}

int  TCPClient::sendMsg(char* pBuffer, int nLen)//发送信息
{
	if (sClient == INVALID_SOCKET)
		return 0;

	int	nBytesSent = 0; //当次发送的字节数
	int nBytes;			//一次发送的字节数

	while (nBytesSent < nLen)
	{
		if ((nBytes = send(sClient, (char*)pBuffer + nBytesSent, nLen - nBytesSent, 0)) == SOCKET_ERROR)
		{
			if (GetLastError() == WSAEWOULDBLOCK)
			{
				Sleep(50);
			}
			else
			{
				return 0;
			}
		}
		else
		{
			nBytesSent += nBytes;
		}
	}
	return 1;
}



LRESULT TCPClient::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:	//lParam为连接服务端消息
		onConnect(lParam);
		break;
	case FD_READ:
	{
		char recvBuffer[MSG_SIZE];
		memset(recvBuffer, 0, MSG_SIZE);
		int iLen = recv(wParam, recvBuffer, MSG_SIZE, 0);
		if (iLen>0 && iLen<MSG_SIZE)
		{
			recvBuffer[iLen] = '\0';
			//onRead(wParam,recvBuffer, iLen);
			splitMsg(wParam, recvBuffer, iLen);
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

void TCPClient::copyRemainMsg(char *pBuffer, int nLen, int nRemainLen)
{
	ZeroMemory(msgBuffer, MSG_SIZE * 2);
	//将剩下的一部分复制到msgBuffer中，并记录这一部分数据长度nHalfMsgLen
	memcpy(msgBuffer, pBuffer + nLen - nRemainLen, nRemainLen);
	this->nRemainLen = nRemainLen;
}

void TCPClient::splitMsg(SOCKET s, char *pBuffer, int nLen)
{
	memcpy(msgBuffer + nRemainLen, pBuffer, nLen);
	char* p = msgBuffer;
	nRemainLen = nLen + nRemainLen;
	
	Msg_Header* pHeader;
	while (p != NULL)
	{
		if (nRemainLen < sizeof(struct Msg_Header)) //剩下的长度不够消息头的长度
		{
			//将pBuffer中剩下的长度不够消息头的数据复制到pClient对应缓冲区
			copyRemainMsg(pBuffer, nLen, nRemainLen);
			p = NULL;
			break;
		}
		else
		{
			pHeader = (Msg_Header*)p;
			if (pHeader->nType > 0 && pHeader->nType <= MSGID_MAX)//是合法类型消息
			{
				if (nRemainLen < pHeader->nLen) //剩下的长度不够一个完整的该结构体长度时
				{
					//将pBuffer中剩下的长度不够该消息实际长度的数据复制到pClient对应缓冲区
					copyRemainMsg( pBuffer, nLen, nRemainLen);
					p = NULL;
					break;
				}
				else //剩下的长度够一个完整的该结构体长度时
				{
					nRemainLen -= pHeader->nLen;//得到当前剩余字节数
					onRead(s, p, pHeader->nLen); //解析处理消息	
					if (nRemainLen == 0)//当剩余字节数等于0,说明没有数据包要处理，跳出循环，当
					{
						p = NULL;
					
						break;
					}
					else if (nRemainLen > 0) //当剩余字节数大于0，说明还有数据包需要处理，则指针指向下一数据包
					{
						p += pHeader->nLen;
					}
				}
			}
			else  //是非法数据，直接丢弃，跳出循环
			{
				nRemainLen = 0;
				showInfo(TEXT("接收到非法数据格式，请按消息头定义格式传送信息"));
				break;
			}
		}
	}
}