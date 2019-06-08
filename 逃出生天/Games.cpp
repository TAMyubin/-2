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
		showInfo(TEXT("���ӳ���������δ����"));
	else
		showInfo(TEXT("���ӷ������ɹ�"));

}


void Game::onClose(SOCKET s)
{

}

// һ�������ϵĶ��������
void Game::onRead(SOCKET s, char *pBuffer, int nLen)
{
	WCHAR tmp[200];
	WCHAR xsp[200];
	WCHAR xspa[200];
	Msg_Header* pHeader = (Msg_Header*)pBuffer;
	switch (pHeader->nType) // �ж��յ�����Ϣ����
	{

	case MSGID_PLAYER_SELF:  //��¼�ɹ�ʱ�յ��Լ����û���Ϣ
	{
		struct Msg_Player* pSelf = (struct Msg_Player*)pBuffer;
		nGuess[1] = rand() % 10 + 1;//1
		nGuess[2] = rand() % 10 + 1;//2
		a = nGuess[1] + nGuess[2];
		wsprintf(tmp, TEXT("���¼��Ϸ�ɹ����û�����%s��"), pSelf->player.cName);
		showInfo(tmp);
		wsprintf(xsp,TEXT("��ĵ�һ���Ƶ�����: %d"), nGuess[1]);
		showInfo(xsp);
		wsprintf(xspa, TEXT("��ĵڶ����Ƶ�����: %d"), nGuess[2]);
		showInfo(xspa);
		

		sClient_Server = pSelf->player.s;
		if (!clientMap.empty()) //���������������û��������
		{

			WCHAR* pOutString = MakePlayerInfoOutString();
			showInfo(pOutString);

			delete[] pOutString;
		}
		//���Լ�����Ϣ���뵽�����Ϣ�б�
		addPlayer(pSelf->player.s, &pSelf->player);
	}
		break;
	case MSGID_PLAYER_ALL: //����յ���ǰ�����������û���Ϣ
	{
		struct Msg_Player* pAll = (struct Msg_Player*)pBuffer;
		addPlayer(pAll->player.s, &pAll->player);


	}
		break;
	case MSGID_PLAYER_NEW://�û�����ʱ�յ�����ҵ�¼ʱ��Ϣ
	{
		ZeroMemory(tmp, 400);
		struct Msg_Player* pNew = (struct Msg_Player*)pBuffer;
		wsprintf(tmp, TEXT("�����û���¼��Ϸ�ɹ����û�����%s��"), pNew->player.cName);
		addPlayer(pNew->player.s, &pNew->player);
		showInfo(tmp);

	}
		break;
	case MSGID_PLAYER_DELETE:  //�û�����ʱ�յ�����˳���Ϣ
	{
		ZeroMemory(tmp, 400);
		struct Msg_Player_Delete* pDel = (struct Msg_Player_Delete*)pBuffer;

		struct PlayerInfo* player =getPlayer(pDel->s);
		wsprintf(tmp, TEXT("�û�%s�뿪����Ϸ��"), player->cName);

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
	wsprintf(bigBuffer, TEXT("%s%s%s"), bigBuffer, p, TEXT("\r\n"));//�����ַ���
	SetWindowTextW(hEditReadOnly, bigBuffer);//��ʾ�����ַ���
	int ChatEditLineCount = SendMessage(hEditReadOnly, EM_GETLINECOUNT, 0, 0);	//�õ�һ��������
	SendMessage(hEditReadOnly, EM_LINESCROLL, 0, ChatEditLineCount);     //ʵ�ֹ��������һ��
}

//���ɵ�ǰ��������б������ַ���
wchar_t* Game::MakePlayerInfoOutString()
{
	wchar_t* tmp = new WCHAR[200];
	ZeroMemory(tmp, 400);
	if (clientMap.size() >= 1) {
		wcscat(tmp, TEXT("��ǰ���������û�����"));

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

		wcscat(tmp, TEXT("��"));
	}

	return tmp;
}



//����¿ͻ��������Ϣ
void Game::addPlayer(int s, struct PlayerInfo *player)
{
	PlayerInfo *playerAdd = new  PlayerInfo();  //����Ҫ���ڴ����û���Ϣ�б�������Ҫͨ�������û���Ϣ��
	memcpy(playerAdd, player, sizeof(struct PlayerInfo));//���յ��������е��û���Ϣ���Ƶ��û���Ϣ��
	clientMap.insert(std::make_pair(s, playerAdd));
}

//ɾ���ҵ��Ŀͻ��������Ϣ
void Game::deletePlayer(int s)
{
	std::map<int, PlayerInfo*>::iterator iter = clientMap.begin();
	iter = clientMap.find(s);
	if (iter != clientMap.end()) {
		SAFE_DELETE(iter->second);
		clientMap.erase(iter);
	}
}

//�����׽���ID��ȡһ���û���Ϣ
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


//�������û��б�
void Game::clearClientMap()
{
	std::map<int, PlayerInfo*>::iterator iter = clientMap.begin();
	for (iter = clientMap.begin(); iter != clientMap.end(); ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	clientMap.clear();
	if (clientMap.empty()) {
		printf("clientMap�����\n");
	}
}


//����¼�Ի�����Ӧ�Ŀؼ��¼��������˰�ť��
void Game::onDialogCommand(int wmId)
{
	switch (wmId)
	{
	case IDOK:     //�������¼�Ի����ȷ����ť�������ӷ����������͵�¼��Ϣ
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



