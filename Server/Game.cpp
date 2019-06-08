#include "Game.h"


Game::Game()
{
}


Game::~Game()
{

}

void Game::initGame(HWND hwnd, HINSTANCE hInst)
{
	this->hwnd = hwnd;
	this->hInst = hInst;

	//�����ı����Ӵ���
	hEditReadOnly = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
		| ES_MULTILINE | ES_LEFT | ES_READONLY | ES_AUTOVSCROLL | WS_VSCROLL,
		10, 10, 780, 550, hwnd, (HMENU)ID_EDIT_READONLY, hInst, NULL);
	//�����ı����Ӵ���




	this->initSocket(hwnd);
	this->startServer(PORT);

}

void Game::onCommand(int wmId)
{
	switch (wmId)
	{
	case ID_SEND:
		//WCHAR sendBuffer[MSG_SIZE];
		//GetWindowTextW(hEdit, sendBuffer, MSG_SIZE);
		//sendMsgToAll((char*)&sendBuffer,wcslen(sendBuffer)*2);
		Msg_Chat myChat;
		myChat.nType = MSGID_CHAT;
		myChat.nLen = sizeof(myChat);
		myChat.toId = 0;
		GetWindowTextW(hEdit, myChat.chat, MSG_SIZE);
		sendMsgToAll((char*)&myChat, myChat.nLen);
		break;

	}
}

//�յ��ͻ�����������
void Game::onAccept(SOCKET s, struct sockaddr_in addr)
{
	WCHAR msg[MSG_SIZE];
	wmemset(msg, 0, MSG_SIZE);
	wsprintf(msg, TEXT("�������յ������ӣ�IP:%S,�˿�:%d,�׽���ID:%d"), inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), s);
	showInfo(msg);
	addClient(s, addr);


}

//�յ��ͻ��˶Ͽ���Ϣ
void Game::onClose(SOCKET s)
{



	Msg_Player_Delete playerDelete;
	playerDelete.nType = MSGID_PLAYER_DELETE;
	playerDelete.nLen = sizeof(playerDelete);
	playerDelete.s = s;
	sendMsgToAll((char*)&playerDelete, sizeof(playerDelete));

	deleteClient(s);

	closesocket(s);




}



// һ�������ϵĶ��������
void Game::onRead(SOCKET s, char *pBuffer, int nLen)
{
	//���յ����ݰ������÷ְ��㷨
	splitMsg(s, pBuffer, nLen);
}

//������Ϣ���ض��׽���
int Game::sendMsg(SOCKET s, char* buffer, int len) //���͸�ĳ���ͻ�����Ϣ
{
	int nLen = send(s, buffer, len, 0);
	return nLen;
}

//������Ϣ���������ӵĿͻ���
int Game::sendMsgToAll(char* buffer, int len) //���͸�ȫ���ͻ�����Ϣ
{
	map<int, Client*>::iterator iter;
	for (iter = clientMap.begin(); iter != clientMap.end(); iter++)
	{
		send((*iter).first, buffer, len, 0);//��������
	}

	return 0;
}


//������Ϣ������ĳ���ͻ�������������пͻ���
int Game::sendMsgExceptOne(SOCKET s, char *buffer, int len)
{
	std::map<int, Client*>::iterator iter;
	for (iter = clientMap.begin(); iter != clientMap.end(); ++iter)
	{
		if (iter->first != s) {
			send(iter->first, buffer, len, 0);
		}
	}
	return 0;
}

void Game::showInfo(WCHAR *p)
{
	wsprintf(bigBuffer, TEXT("%s%s%s"), bigBuffer, p, TEXT("\r\n"));//�����ַ���
	SetWindowTextW(hEditReadOnly, bigBuffer);//��ʾ�����ַ���
	int ChatEditLineCount = SendMessage(hEditReadOnly, EM_GETLINECOUNT, 0, 0);	//�õ�һ��������
	SendMessage(hEditReadOnly, EM_LINESCROLL, 0, ChatEditLineCount);     //ʵ�ֹ��������һ��
}

void Game::copyRemainMsg(Client* client, char *pBuffer, int nLen, int nRemainLen)
{
	ZeroMemory(client->msgBuffer, MSG_SIZE * 2);
	//��pBuffer��ʣ�µ�һ���ָ��Ƶ�msgBuffer�У�����¼��һ�������ݳ���nRemainLen
	memcpy(client->msgBuffer, pBuffer + nLen - nRemainLen, nRemainLen);
	client->nRemainLen = nRemainLen;
}

void Game::splitMsg(SOCKET s, char *pBuffer, int nLen)
{
	Client* pClient = getClient(s);
	memcpy(pClient->msgBuffer + pClient->nRemainLen, pBuffer, nLen);
	char* p = pClient->msgBuffer;
	int nRemainLen = nLen + pClient->nRemainLen;

	Msg_Header* pHeader;
	while (p != NULL)
	{
		if (nRemainLen < sizeof(struct Msg_Header)) //ʣ�µĳ��Ȳ�����Ϣͷ�ĳ���
		{
			//��pBuffer��ʣ�µĳ��Ȳ�����Ϣͷ�����ݸ��Ƶ�pClient��Ӧ������
			copyRemainMsg(pClient, pBuffer, nLen, nRemainLen);
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
					copyRemainMsg(pClient, pBuffer, nLen, nRemainLen);
					p = NULL;
					break;
				}
				else //ʣ�µĳ��ȹ�һ�������ĸýṹ�峤��ʱ
				{
					nRemainLen -= pHeader->nLen;//�õ���ǰʣ���ֽ���
					onReadPackage(s, pClient, p, pHeader->nLen); //����������Ϣ	
					if (nRemainLen == 0)//��ʣ���ֽ�������0,˵��û�����ݰ�Ҫ��������ѭ������
					{
						p = NULL;
						pClient->nRemainLen = 0;
						break;
					}
					else if (nRemainLen > 0) //��ʣ���ֽ�������0��˵���������ݰ���Ҫ������ָ��ָ����һ���ݰ�
					{
						p += pHeader->nLen;
						pClient->nRemainLen = nRemainLen;
					}
				}
			}
			else  //�ǷǷ����ݣ�ֱ�Ӷ���������ѭ��
			{
				pClient->nRemainLen = 0;
				showInfo(TEXT("���յ��Ƿ����ݸ�ʽ���밴��Ϣͷ�����ʽ������Ϣ"));
				break;
			}
		}
	}
}



//����¿ͻ��������Ϣ
void Game::addClient(int s, struct sockaddr_in addr)
{
	Client* client = new Client();

	client->addr = addr;
	client->nRemainLen = 0;
	memset(client->msgBuffer, 0, MSG_SIZE * 2);
	clientMap.insert(std::make_pair(s, client));

}

//ɾ���ҵ��Ŀͻ��������Ϣ
void Game::deleteClient(int s)
{
	std::map<int, Client*>::iterator iter = clientMap.begin();
	iter = clientMap.find(s);
	if (iter != clientMap.end()) {
		wchar_t msg[MSG_SIZE];
		wmemset(msg, 0, MSG_SIZE);
		wsprintf(msg, TEXT("%s�뿪����Ϸ���׽���ID:%d"), (*iter).second->player.cName, s);
		showInfo(msg);
		SAFE_DELETE(iter->second);
		clientMap.erase(iter);
	}

}

//�����׽���ID��ȡһ���û���Ϣ
Client* Game::getClient(int s)
{
	std::map<int, Client*>::iterator iter = clientMap.begin();
	iter = clientMap.find(s);
	if (iter != clientMap.end())
	{
		return iter->second;
	}
	else
	{
		return NULL;
	}
}


//�������û��б�
void Game::clearClientMap()
{
	std::map<int, Client*>::iterator iter = clientMap.begin();
	for (iter = clientMap.begin(); iter != clientMap.end(); ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	clientMap.clear();
	if (clientMap.empty()) {
		showInfo(TEXT("clientMap�����"));

	}
}


// һ�������ϵĶ��������
void Game::onReadPackage(int s, Client* client, char *pBuffer, int nLen)
{
	Msg_Header *pHeader = (Msg_Header*)pBuffer;
	switch (pHeader->nType)//�ж���Ϣ������
	{
	case MSGID_CHAT:
	{
					   //WCHAR sendBuffer[MSG_SIZE];
					   //GetWindowTextW(hEdit, sendBuffer, MSG_SIZE);
					   //sendMsgToAll((char*)sendBuffer, wcslen(sendBuffer) * 2);
					   Msg_Chat* pChat = (Msg_Chat*)pBuffer;

					   PlayerInfo *player = &getClient(s)->player;
					   wchar_t tchar[200];
					   wsprintf(tchar, TEXT("%s˵:%s"), player->cName, pChat->chat);
					   showInfo(tchar);
					   memset(pChat->chat, 0, 200);
					   wcscpy(pChat->chat, tchar);
					   sendMsgToAll(pBuffer, nLen);
	}
		break;
	case MSGID_LOGIN:
	{
						Msg_Login *pLogin = (Msg_Login*)pBuffer;

						WCHAR tchar[MSG_SIZE];
						ZeroMemory(tchar, MSG_SIZE);
						wsprintf(tchar, TEXT("�û�%s��¼����Ϸ��"), pLogin->cName);
						showInfo(tchar);
						//����¼���û������׽�����Ϣ���뵽Client�ṹ�����PlayerInfo��
						wcscpy(client->player.cName, pLogin->cName);
						client->player.s = s;

						//��ǰ�û��������������û���Ϣ
						Msg_Player allPlayer;
						allPlayer.nType = MSGID_PLAYER_ALL;
						allPlayer.nLen = sizeof(allPlayer);
						map<int, Client*>::iterator iter;
						for (iter = clientMap.begin(); iter != clientMap.end(); ++iter)
						{
							if ((*iter).first != s) //������ǵ�ǰ��¼�û�����Ϣ����ѭ�����͸���ǰ��¼���û�
							{
								memcpy(&allPlayer.player, &(*iter).second->player, sizeof(struct PlayerInfo));
								sendMsg(s, (char *)&allPlayer, allPlayer.nLen);
							}

						}


						//�������������ҷ��������û���¼���û���Ϣ
						Msg_Player newPlayer;
						newPlayer.nType = MSGID_PLAYER_NEW;
						newPlayer.nLen = sizeof(newPlayer);
						memcpy(&newPlayer.player, &client->player, sizeof(struct PlayerInfo));
						sendMsgExceptOne(s, (char*)&newPlayer, newPlayer.nLen);

						//��ǰ��ҷ��͵�ǰ����Լ����û���Ϣ
						newPlayer.nType = MSGID_PLAYER_SELF;
						sendMsg(s, (char*)&newPlayer, newPlayer.nLen);

	}
		break;
	}
}