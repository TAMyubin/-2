#pragma once
#include "TCPClient.h"
#include "resource.h"
#include "MsgDefine.h"
#include <Map>
const int ID_EDIT = 11;                 //定义发送文本框ID
const int ID_SEND = 12;            //定义发送按钮ID
const int ID_EDIT_READONLY = 13;   //定义只读文本框
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
	HWND hImage; //图像控件
	int nGuessa[3];

	int nGuess[10];

	int a ;
	int b=3;


	HWND hDlg;  //登录对话框句柄 
	HWND hDlgName; //登录对话框名字输出框句柄
	HWND hDlgPassword;//登录对话框密码输出框句柄

	HWND hEdit;     //发送信息文本框句柄 

	HWND hEditp;     //获取牌的文本框
	
	HWND hwnd;     //当前窗口句柄成员
	HINSTANCE hInst; //当前应用实例成员
	
	HWND hBtn_Send;
	HWND hBtn_Get;
	HWND hBtn_Mp;
	HWND  hBtn_Cl;
	HWND hEditReadOnly;  //只读文本框
	WCHAR bigBuffer[1024 * 1024];
	SOCKET sClient_Server;       //当前与sClient连接的服务器套接字ID
	map<int, PlayerInfo*> clientMap; //玩家信息集合
public:
	Game();
	~Game();
	void initGame(HWND hwnd,HINSTANCE hInst);
	

	void onCommand(int wmId);
	void onDialogCommand(int wmId);  //点击对话框按钮执行的命令

	// 连接完成
	void onConnect(LPARAM lParam);
	// 一个连接关闭
	void onClose(SOCKET s);
	// 一个连接上的读操作完成
	void onRead(SOCKET s, char *pBuffer, int nLen);

	void showInfo(WCHAR *p);

	void onReadPackage(SOCKET s, char *pBuffer, int nLen);


	//添加新客户端玩家信息
	void addPlayer(int s, struct PlayerInfo *player);
	//删除找到的客户端玩家信息
	void deletePlayer(int s);
	//根据套接字ID获取一个用户信息
	PlayerInfo* getPlayer(int s);
	//清空玩家用户列表
	void clearClientMap();

	wchar_t* MakePlayerInfoOutString();//生成当前所有玩家列表的输出字符串

};

