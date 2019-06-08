#include "Games.h"

Game *pGame;
Game::Game()
{
	pGame = this;
	srand(time(NULL));
	
}


Game::~Game()
{
}

void Game::initGame(HWND hwnd, HINSTANCE hInst)
{
	this->hwnd = hwnd;
	this->hInst = hInst;
	hDlg = CreateDialogW(hInst, MAKEINTRESOURCE(IDD_LOGIN), hwnd, (DLGPROC)dialogProc);
	hDlgName = GetDlgItem(hDlg, IDC_NAME);
	hDlgPassword = GetDlgItem(hDlg, IDC_PASSWORD);
	hImage = GetDlgItem(hDlg, IDC_IMAGE);
	HBITMAP hBitmap = (HBITMAP)::LoadImage(hInst, TEXT("images/avatar/22.bmp"), IMAGE_BITMAP, 430, 320, LR_LOADFROMFILE);
	SendMessage(hImage, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
	UpdateWindow(hImage);
	ShowWindow(hDlg, SW_SHOW);

}

void Game::onCommand(int wmId)
{
	switch (wmId)
	{
	
	case ID_SEND:
	{
					//WCHAR sendBuffer[MSG_SIZE];
					//GetWindowTextW(hEdit, sendBuffer, MSG_SIZE);
					//sendMsg((char*)sendBuffer,wcslen(sendBuffer)*2);
					Msg_Chat myChat;
					myChat.nType = MSGID_CHAT;
					myChat.nLen = sizeof(myChat);
					myChat.toId = 0;
					GetWindowTextW(hEdit, myChat.chat, MSG_SIZE);
					sendMsg((char*)&myChat, myChat.nLen);
	}
		break;
	
	}
}



void Game::onConnect(LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam) != 0)
		showInfo(TEXT("连接出错或服务器未开放"));
	else
		showInfo(TEXT("连接服务器成功"));

}


void Game::onClose(SOCKET s)
{

}

// 一个连接上的读操作完成
void Game::onRead(SOCKET s, char *pBuffer, int nLen)
{
	WCHAR tmp[200];
	WCHAR xsp[200];
	WCHAR xspa[200];
	Msg_Header* pHeader = (Msg_Header*)pBuffer;
	switch (pHeader->nType) // 判断收到的消息类型
	{

	case MSGID_PLAYER_SELF:  //登录成功时收到自己的用户信息
	{
		struct Msg_Player* pSelf = (struct Msg_Player*)pBuffer;
		nGuess[1] = rand() % 10 + 1;//1
		nGuess[2] = rand() % 10 + 1;//2
		a = nGuess[1] + nGuess[2];
		wsprintf(tmp, TEXT("你登录游戏成功，用户名是%s。"), pSelf->player.cName);
		showInfo(tmp);
		wsprintf(xsp,TEXT("你的第一张牌点数是: %d"), nGuess[1]);
		showInfo(xsp);
		wsprintf(xspa, TEXT("你的第二张牌点数是: %d"), nGuess[2]);
		showInfo(xspa);
		

		sClient_Server = pSelf->player.s;
		if (!clientMap.empty()) //当服务器有其它用户，则输出
		{

			WCHAR* pOutString = MakePlayerInfoOutString();
			showInfo(pOutString);

			delete[] pOutString;
		}
		//将自己的信息存入到玩家信息列表
		addPlayer(pSelf->player.s, &pSelf->player);
	}
		break;
	case MSGID_PLAYER_ALL: //玩家收到当前服务器在线用户信息
	{
		struct Msg_Player* pAll = (struct Msg_Player*)pBuffer;
		addPlayer(pAll->player.s, &pAll->player);


	}
		break;
	case MSGID_PLAYER_NEW://用户在线时收到新玩家登录时消息
	{
		ZeroMemory(tmp, 400);
		struct Msg_Player* pNew = (struct Msg_Player*)pBuffer;
		wsprintf(tmp, TEXT("有新用户登录游戏成功，用户名是%s。"), pNew->player.cName);
		addPlayer(pNew->player.s, &pNew->player);
		showInfo(tmp);

	}
		break;
	case MSGID_PLAYER_DELETE:  //用户在线时收到玩家退出消息
	{
		ZeroMemory(tmp, 400);
		struct Msg_Player_Delete* pDel = (struct Msg_Player_Delete*)pBuffer;

		struct PlayerInfo* player =getPlayer(pDel->s);
		wsprintf(tmp, TEXT("用户%s离开了游戏。"), player->cName);

		showInfo(tmp);
		deletePlayer(pDel->s);

	

	}
		break;
	case MSGID_CHAT:
	{
		Msg_Chat* pChat = (Msg_Chat*)pBuffer;

		showInfo(pChat->chat);
		
	}
		break;
	}
}

void Game::showInfo(WCHAR *p)
{
	wsprintf(bigBuffer, TEXT("%s%s%s"), bigBuffer, p, TEXT("\r\n"));//连接字符串
	SetWindowTextW(hEditReadOnly, bigBuffer);//显示聊天字符串
	int ChatEditLineCount = SendMessage(hEditReadOnly, EM_GETLINECOUNT, 0, 0);	//得到一共多少行
	SendMessage(hEditReadOnly, EM_LINESCROLL, 0, ChatEditLineCount);     //实现滚动到最后一行
}

//生成当前所有玩家列表的输出字符串
wchar_t* Game::MakePlayerInfoOutString()
{
	wchar_t* tmp = new WCHAR[200];
	ZeroMemory(tmp, 400);
	if (clientMap.size() >= 1) {
		wcscat(tmp, TEXT("当前服务器的用户还有"));

		std::map<int, PlayerInfo*>::iterator iter;
		int i = 0;
		for (iter = clientMap.begin(); iter != clientMap.end(); ++iter)
		{
			//send(iter->first, buf, len, 0);
			wcscat(tmp, iter->second->cName);
			if (i < clientMap.size() - 1) {
				wcscat(tmp, TEXT(","));

			}
			i++;
		}

		wcscat(tmp, TEXT("。"));
	}

	return tmp;
}



//添加新客户端玩家信息
void Game::addPlayer(int s, struct PlayerInfo *player)
{
	PlayerInfo *playerAdd = new  PlayerInfo();  //因需要长期存入用户信息列表，所以需要通过创建用户信息堆
	memcpy(playerAdd, player, sizeof(struct PlayerInfo));//将收到缓冲区中的用户信息复制到用户信息堆
	clientMap.insert(std::make_pair(s, playerAdd));
}

//删除找到的客户端玩家信息
void Game::deletePlayer(int s)
{
	std::map<int, PlayerInfo*>::iterator iter = clientMap.begin();
	iter = clientMap.find(s);
	if (iter != clientMap.end()) {
		SAFE_DELETE(iter->second);
		clientMap.erase(iter);
	}
}

//根据套接字ID获取一个用户信息
PlayerInfo* Game::getPlayer(int s)
{
	std::map<int, PlayerInfo*>::iterator iter = clientMap.begin();
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
	std::map<int, PlayerInfo*>::iterator iter = clientMap.begin();
	for (iter = clientMap.begin(); iter != clientMap.end(); ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	clientMap.clear();
	if (clientMap.empty()) {
		printf("clientMap清空了\n");
	}
}


//当登录对话框响应的控件事件，如点击了按钮等
void Game::onDialogCommand(int wmId)
{
	switch (wmId)
	{
	case IDOK:     //当点击登录对话框的确定按钮，则连接服务器并发送登录消息
		this->initSocket(hwnd);
		this->connectServer(IP, PORT);
		

		Msg_Login login;
		login.nType = MSGID_LOGIN;
		login.nLen = sizeof(login);
		GetWindowTextW(hDlgName,login.cName, 50);
		GetWindowTextW(hDlgPassword, login.cPassword, 50);
		sendMsg((char*)&login, login.nLen);

		ShowWindow(hwnd, SW_SHOW);
		ShowWindow(hDlg, SW_HIDE);
		break;
	case IDCANCEL:
		DestroyWindow(hDlg);
		DestroyWindow(hwnd);
		break;
	}
}



BOOL WINAPI dialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int wmId;
	switch (uMsg)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		pGame->onDialogCommand(wmId);
		break;
	case WM_DESTROY:
		PostQuitMessage(WM_QUIT);
		break;
	default:
		break;
	}
	return 0;
}



