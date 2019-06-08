#include<winsock2.h>
#pragma comment(lib,"WS2_32.lib") 
#include <windows.h>
#include <stdio.h>
#include "Game.h"
LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);


Game myServer; //创建服务器对象

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	HWND hWnd;
	HINSTANCE hInst;
	WNDCLASS wndcls;
	wndcls.cbClsExtra = 0;             //类结构后附加的额外的byte数
	wndcls.cbWndExtra = 0;
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);     //窗口刷新的画笔句柄
	wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndcls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndcls.hInstance = hInstance;
	wndcls.lpfnWndProc = wndProc;      //指定消息回调函数
	wndcls.lpszClassName = TEXT("GameWindow");  //标题栏显示标题
	wndcls.lpszMenuName = NULL;          //菜单
	wndcls.style = CS_HREDRAW | CS_VREDRAW;  //水平重画和竖直重画
	RegisterClass(&wndcls);

	hInst = hInstance; // Store instance handle in our global variable

	//设置窗口风格：
	DWORD dwStyle;
	dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	//取得去掉windows系统菜单栏后的工作区
	RECT rcArea;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
	//取得客户区大小为*600 对应的窗口大小
	RECT rect;
	rect.left = rect.top = 0;
	rect.right = 800;
	rect.bottom = 600;
	::AdjustWindowRectEx(&rect, dwStyle, TRUE, 0);
	// 将视窗的位置设定在屏幕（工作范围）的中央
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;
	int x0 = rcArea.left + (rcArea.right - rcArea.left - w) / 2;
	int y0 = rcArea.top + (rcArea.bottom - rcArea.top - h) / 2;

	// 创建窗口
	hWnd = CreateWindow(TEXT("GameWindow"), TEXT("服务器"), dwStyle,
		x0, y0, w, h, NULL, NULL, hInstance, NULL);
	hInst = hInstance;

	if (!hWnd)  return FALSE;
	ShowWindow(hWnd, SW_SHOWNORMAL);   //显示窗口
	UpdateWindow(hWnd);  //更新窗口
	myServer.initGame(hWnd, hInst);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))//从消息队列中取出消息
	{
		TranslateMessage(&msg); //实现键盘消息翻译,将其翻译成Windows字符集.
		DispatchMessage(&msg);// 将消息发送到窗口处理.
	}
	return 0;
}


LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int wmId;
	switch (uMsg)
	{
	case WM_SOCKET:
		myServer.wndProc(hwnd, uMsg, wParam, lParam);
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		myServer.onCommand(wmId);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		if (IDYES == MessageBox(hwnd, TEXT("是否真的结束？"), TEXT("GameWindow"), MB_YESNO))
		{
			DestroyWindow(hwnd);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

