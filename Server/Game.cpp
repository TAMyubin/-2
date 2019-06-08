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

	//创建文本框子窗口
	hEditReadOnly = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
		| ES_MULTILINE | ES_LEFT | ES_READONLY | ES_AUTOVSCROLL | WS_VSCROLL,
		10, 10, 780, 550, hwnd, (HMENU)ID_EDIT_READONLY, hInst, NULL);
	//创建文本框子窗口




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

//收到客户端连接请求
void Game::onAccept(SOCKET s, struct sockaddr_in addr)
{
	WCHAR msg[MSG_SIZE];
	wmemset(msg, 0, MSG_SIZE);
	wsprintf(msg, TEXT("服务器收到新连接，IP:%S,端口:%d,套接字ID:%d"), inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), s);
	showInfo(msg);
	addClient(s, addr);


}

//收到客户端断开消息
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



// 一个连接上的读操作完成
void Game::onRead(SOCKET s, char *pBuffer, int nLen)
{
	//当收到数据包，调用分包算法
	splitMsg(s, pBuffer, nLen);
}

//发送信息给特定套接字
int Game::sendMsg(SOCKET s, char* buffer, int len) //发送给某个客户端信息
{
	int nLen = send(s, buffer, len, 0);
	return nLen;
}

//发送信息给所有连接的客户端
int Game::sendMsgToAll(char* buffer, int len) //发送给全部客户端信息
{
	map<int, Client*>::iterator iter;
	for (iter = clientMap.begin(); iter != clientMap.end(); iter++)
	{
		send((*iter).first, buffer, len, 0);//发送数据
	}

	return 0;
}


//发送信息给除了某个客户端外的其它所有客户端
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
	wsprintf(bigBuffer, TEXT("%s%s%s"), bigBuffer, p, TEXT("\r\n"));//连接字符串
	SetWindowTextW(hEditReadOnly, bigBuffer);//显示聊天字符串
	int ChatEditLineCount = SendMessage(hEditReadOnly, EM_GETLINECOUNT, 0, 0);	//得到一共多少行
	SendMessage(hEditReadOnly, EM_LINESCROLL, 0, ChatEditLineCount);     //实现滚动到最后一行
}

void Game::copyRemainMsg(Client* client, char *pBuffer, int nLen, int nRemainLen)
{
	ZeroMemory(client->msgBuffer, MSG_SIZE * 2);
	//将pBuffer中剩下的一部分复制到msgBuffer中，并记录这一部分数据长度nRemainLen
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
		if (nRemainLen < sizeof(struct Msg_Header)) //剩下的长度不够消息头的长度
		{
			//将pBuffer中剩下的长度不够消息头的数据复制到pClient对应缓冲区
			copyRemainMsg(pClient, pBuffer, nLen, nRemainLen);
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
					copyRemainMsg(pClient, pBuffer, nLen, nRemainLen);
					p = NULL;
					break;
				}
				else //剩下的长度够一个完整的该结构体长度时
				{
					nRemainLen -= pHeader->nLen;//得到当前剩余字节数
					onReadPackage(s, pClient, p, pHeader->nLen); //解析处理消息	
					if (nRemainLen == 0)//当剩余字节数等于0,说明没有数据包要处理，跳出循环，当
					{
						p = NULL;
						pClient->nRemainLen = 0;
						break;
					}
					else if (nRemainLen > 0) //当剩余字节数大于0，说明还有数据包需要处理，则指针指向下一数据包
					{
						p += pHeader->nLen;
						pClient->nRemainLen = nRemainLen;
					}
				}
			}
			else  //是非法数据，直接丢弃，跳出循环
			{
				pClient->nRemainLen = 0;
				showInfo(TEXT("接收到非法数据格式，请按消息头定义格式传送信息"));
				break;
			}
		}
	}
}



//添加新客户端玩家信息
void Game::addClient(int s, struct sockaddr_in addr)
{
	Client* client = new Client();

	client->addr = addr;
	client->nRemainLen = 0;
	memset(client->msgBuffer, 0, MSG_SIZE * 2);
	clientMap.insert(std::make_pair(s, client));

}

//删除找到的客户端玩家信息
void Game::deleteClient(int s)
{
	std::map<int, Client*>::iterator iter = clientMap.begin();
	iter = clientMap.find(s);
	if (iter != clientMap.end()) {
		wchar_t msg[MSG_SIZE];
		wmemset(msg, 0, MSG_SIZE);
		wsprintf(msg, TEXT("%s离开的游戏，套接字ID:%d"), (*iter).second->player.cName, s);
		showInfo(msg);
		SAFE_DELETE(iter->second);
		clientMap.erase(iter);
	}

}

//根据套接字ID获取一个用户信息
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


//清空玩家用户列表
void Game::clearClientMap()
{
	std::map<int, Client*>::iterator iter = clientMap.begin();
	for (iter = clientMap.begin(); iter != clientMap.end(); ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	clientMap.clear();
	if (clientMap.empty()) {
		showInfo(TEXT("clientMap清空了"));

	}
}


// 一个连接上的读操作完成
void Game::onReadPackage(int s, Client* client, char *pBuffer, int nLen)
{
	Msg_Header *pHeader = (Msg_Header*)pBuffer;
	switch (pHeader->nType)//判断消息包类型
	{
	case MSGID_CHAT:
	{
					   //WCHAR sendBuffer[MSG_SIZE];
					   //GetWindowTextW(hEdit, sendBuffer, MSG_SIZE);
					   //sendMsgToAll((char*)sendBuffer, wcslen(sendBuffer) * 2);
					   Msg_Chat* pChat = (Msg_Chat*)pBuffer;

					   PlayerInfo *player = &getClient(s)->player;
					   wchar_t tchar[200];
					   wsprintf(tchar, TEXT("%s说:%s"), player->cName, pChat->chat);
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
						wsprintf(tchar, TEXT("用户%s登录了游戏。"), pLogin->cName);
						showInfo(tchar);
						//将登录的用户名和套接字信息存入到Client结构体里的PlayerInfo里
						wcscpy(client->player.cName, pLogin->cName);
						client->player.s = s;

						//向当前用户发送所有在线用户信息
						Msg_Player allPlayer;
						allPlayer.nType = MSGID_PLAYER_ALL;
						allPlayer.nLen = sizeof(allPlayer);
						map<int, Client*>::iterator iter;
						for (iter = clientMap.begin(); iter != clientMap.end(); ++iter)
						{
							if ((*iter).first != s) //如果不是当前登录用户的信息，则循环发送给当前登录的用户
							{
								memcpy(&allPlayer.player, &(*iter).second->player, sizeof(struct PlayerInfo));
								sendMsg(s, (char *)&allPlayer, allPlayer.nLen);
							}

						}


						//向服务器其它玩家发送有新用户登录的用户信息
						Msg_Player newPlayer;
						newPlayer.nType = MSGID_PLAYER_NEW;
						newPlayer.nLen = sizeof(newPlayer);
						memcpy(&newPlayer.player, &client->player, sizeof(struct PlayerInfo));
						sendMsgExceptOne(s, (char*)&newPlayer, newPlayer.nLen);

						//向当前玩家发送当前玩家自己的用户信息
						newPlayer.nType = MSGID_PLAYER_SELF;
						sendMsg(s, (char*)&newPlayer, newPlayer.nLen);

	}
		break;
	}
}