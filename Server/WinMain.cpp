#include<winsock2.h>
#pragma comment(lib,"WS2_32.lib") 
#include <windows.h>
#include <stdio.h>
#include "Game.h"
LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);


Game myServer; //��������������

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	HWND hWnd;
	HINSTANCE hInst;
	WNDCLASS wndcls;
	wndcls.cbClsExtra = 0;             //��ṹ�󸽼ӵĶ����byte��
	wndcls.cbWndExtra = 0;
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);     //����ˢ�µĻ��ʾ��
	wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndcls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndcls.hInstance = hInstance;
	wndcls.lpfnWndProc = wndProc;      //ָ����Ϣ�ص�����
	wndcls.lpszClassName = TEXT("GameWindow");  //��������ʾ����
	wndcls.lpszMenuName = NULL;          //�˵�
	wndcls.style = CS_HREDRAW | CS_VREDRAW;  //ˮƽ�ػ�����ֱ�ػ�
	RegisterClass(&wndcls);

	hInst = hInstance; // Store instance handle in our global variable

	//���ô��ڷ��
	DWORD dwStyle;
	dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	//ȡ��ȥ��windowsϵͳ�˵�����Ĺ�����
	RECT rcArea;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
	//ȡ�ÿͻ�����СΪ*600 ��Ӧ�Ĵ��ڴ�С
	RECT rect;
	rect.left = rect.top = 0;
	rect.right = 800;
	rect.bottom = 600;
	::AdjustWindowRectEx(&rect, dwStyle, TRUE, 0);
	// ���Ӵ���λ���趨����Ļ��������Χ��������
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;
	int x0 = rcArea.left + (rcArea.right - rcArea.left - w) / 2;
	int y0 = rcArea.top + (rcArea.bottom - rcArea.top - h) / 2;

	// ��������
	hWnd = CreateWindow(TEXT("GameWindow"), TEXT("������"), dwStyle,
		x0, y0, w, h, NULL, NULL, hInstance, NULL);
	hInst = hInstance;

	if (!hWnd)  return FALSE;
	ShowWindow(hWnd, SW_SHOWNORMAL);   //��ʾ����
	UpdateWindow(hWnd);  //���´���
	myServer.initGame(hWnd, hInst);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))//����Ϣ������ȡ����Ϣ
	{
		TranslateMessage(&msg); //ʵ�ּ�����Ϣ����,���䷭���Windows�ַ���.
		DispatchMessage(&msg);// ����Ϣ���͵����ڴ���.
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
		if (IDYES == MessageBox(hwnd, TEXT("�Ƿ���Ľ�����"), TEXT("GameWindow"), MB_YESNO))
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

