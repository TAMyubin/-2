#include "TCPClient.h"

TCPClient::TCPClient()  //��ʼ������
{	
	sClient=NULL;
	nRemainLen = 0;

}
TCPClient::~TCPClient()  //��ʼ������
{
	releaseClient();
}


void TCPClient::initSocket(HWND hwnd)  //��ʼ������
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sClient = socket(AF_INET, SOCK_STREAM, 0);
	WSAAsyncSelect(sClient, hwnd, WM_SOCKET, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	this->hwnd = hwnd;
}

BOOL TCPClient::connectServer(const char *ipaddr,short sPort)//���ӷ�����
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
BOOL  TCPClient::releaseClient()         //�رտͻ���
{
	closesocket(sClient);
	WSACleanup();
	return true;
}

int  TCPClient::sendMsg(char* pBuffer, int nLen)//������Ϣ
{
	if (sClient == INVALID_SOCKET)
		return 0;

	int	nBytesSent = 0; //���η��͵��ֽ���
	int nBytes;			//һ�η��͵��ֽ���

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
	case FD_CONNECT:	//lParamΪ���ӷ������Ϣ
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
	//��ʣ�µ�һ���ָ��Ƶ�msgBuffer�У�����¼��һ�������ݳ���nHalfMsgLen
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
		if (nRemainLen < sizeof(struct Msg_Header)) //ʣ�µĳ��Ȳ�����Ϣͷ�ĳ���
		{
			//��pBuffer��ʣ�µĳ��Ȳ�����Ϣͷ�����ݸ��Ƶ�pClient��Ӧ������
			copyRemainMsg(pBuffer, nLen, nRemainLen);
			p = NULL;
			break;
		}
		else
		{
			pHeader = (Msg_Header*)p;
			if (pHeader->nType > 0 && pHeader->nType <= MSGID_MAX)//�ǺϷ�������Ϣ
			{
				if (nRemainLen < pHeader->nLen) //ʣ�µĳ��Ȳ���һ�������ĸýṹ�峤��ʱ
				{
					//��pBuffer��ʣ�µĳ��Ȳ�������Ϣʵ�ʳ��ȵ����ݸ��Ƶ�pClient��Ӧ������
					copyRemainMsg( pBuffer, nLen, nRemainLen);
					p = NULL;
					break;
				}
				else //ʣ�µĳ��ȹ�һ�������ĸýṹ�峤��ʱ
				{
					nRemainLen -= pHeader->nLen;//�õ���ǰʣ���ֽ���
					onRead(s, p, pHeader->nLen); //����������Ϣ	
					if (nRemainLen == 0)//��ʣ���ֽ�������0,˵��û�����ݰ�Ҫ��������ѭ������
					{
						p = NULL;
					
						break;
					}
					else if (nRemainLen > 0) //��ʣ���ֽ�������0��˵���������ݰ���Ҫ������ָ��ָ����һ���ݰ�
					{
						p += pHeader->nLen;
					}
				}
			}
			else  //�ǷǷ����ݣ�ֱ�Ӷ���������ѭ��
			{
				nRemainLen = 0;
				showInfo(TEXT("���յ��Ƿ����ݸ�ʽ���밴��Ϣͷ�����ʽ������Ϣ"));
				break;
			}
		}
	}
}