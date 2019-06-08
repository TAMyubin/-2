#pragma once
#include "TCPClient.h"
#include "resource.h"
#include "MsgDefine.h"
#include <Map>
const int ID_EDIT = 11;                 //���巢���ı���ID
const int ID_SEND = 12;            //���巢�Ͱ�ťID
const int ID_EDIT_READONLY = 13;   //����ֻ���ı���
const int ID_EDIT_P = 14;  //
const int ID_GET = 15;
const int ID_MP = 16;
const int ID_CL = 17;
const char IP[] = "127.0.0.1";
const int PORT = 8000;

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
BOOL WINAPI dialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#include <time.h>
class Game :	public TCPClient
{
private:
	HWND hImage; //ͼ��ؼ�
	int nGuessa[3];

	int nGuess[10];

	int a ;
	int b=3;


	HWND hDlg;  //��¼�Ի����� 
	HWND hDlgName; //��¼�Ի��������������
	HWND hDlgPassword;//��¼�Ի��������������

	HWND hEdit;     //������Ϣ�ı����� 

	HWND hEditp;     //��ȡ�Ƶ��ı���
	
	HWND hwnd;     //��ǰ���ھ����Ա
	HINSTANCE hInst; //��ǰӦ��ʵ����Ա
	
	HWND hBtn_Send;
	HWND hBtn_Get;
	HWND hBtn_Mp;
	HWND  hBtn_Cl;
	HWND hEditReadOnly;  //ֻ���ı���
	WCHAR bigBuffer[1024 * 1024];
	SOCKET sClient_Server;       //��ǰ��sClient���ӵķ������׽���ID
	map<int, PlayerInfo*> clientMap; //�����Ϣ����
public:
	Game();
	~Game();
	void initGame(HWND hwnd,HINSTANCE hInst);
	

	void onCommand(int wmId);
	void onDialogCommand(int wmId);  //����Ի���ťִ�е�����

	// �������
	void onConnect(LPARAM lParam);
	// һ�����ӹر�
	void onClose(SOCKET s);
	// һ�������ϵĶ��������
	void onRead(SOCKET s, char *pBuffer, int nLen);

	void showInfo(WCHAR *p);

	void onReadPackage(SOCKET s, char *pBuffer, int nLen);


	//����¿ͻ��������Ϣ
	void addPlayer(int s, struct PlayerInfo *player);
	//ɾ���ҵ��Ŀͻ��������Ϣ
	void deletePlayer(int s);
	//�����׽���ID��ȡһ���û���Ϣ
	PlayerInfo* getPlayer(int s);
	//�������û��б�
	void clearClientMap();

	wchar_t* MakePlayerInfoOutString();//���ɵ�ǰ��������б������ַ���

};

