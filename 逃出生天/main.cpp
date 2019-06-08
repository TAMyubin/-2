

#include<winsock2.h>
#include <windows.h>
#pragma comment(lib,"WS2_32.lib") 
#include "menuGame.h"

#include <stdio.h>
#include "Games.h"
HWND		hWnd ;


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;	
HINSTANCE hInst;
Game myClient;  //�����ͻ��˶���

CGame		*pGame = NULL;
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("�ӳ�����") ;
	MSG			msg ;
	WNDCLASS	wc ;
	wc.style			= CS_HREDRAW | CS_VREDRAW ;
	wc.lpfnWndProc		= WndProc ;
	wc.cbClsExtra		= 0 ;
	wc.cbWndExtra		= 0 ;
	wc.hInstance		= hInstance ;
	wc.hIcon			= LoadIcon (NULL, IDI_APPLICATION) ;
	wc.hCursor			= LoadCursor (NULL, IDC_ARROW) ;
	wc.hbrBackground	= (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wc.lpszMenuName		= NULL ;
	wc.lpszClassName	= szAppName ;
	hInst = hInstance;
	if (!RegisterClass (&wc))
	{
		MessageBox (	NULL, TEXT ("This program requires Windows NT!"), 
			szAppName, MB_ICONERROR) ;
		return 0 ;
	}
	hWnd = CreateWindow(szAppName,	// window class name
		TEXT("�ӳ�����"),	// window caption
		WS_OVERLAPPEDWINDOW,	// window style
		//WS_POPUP | WS_VISIBLE,
		0,	// initial x position
		0,	// initial y position
		1440,	// initial x size
		800,	// initial y size
		NULL,			// parent window handle
		NULL,	        // window menu handle
		hInstance,	    // program instance handle
		NULL) ; 	    // creation parameters

	//ShowWindow (hWnd, iCmdShow) ;
	UpdateWindow (hWnd) ;
	myClient.initGame(hWnd, hInst);
	
	if (SUCCEEDED(CoInitialize(NULL)))				//�����ʼ��Com����ɹ�
	{
		pGame = new  menuGame(hWnd, 0);
		pGame->InitGame();
		//if (SUCCEEDED(init()))						//��ʼ��
		{
			BOOL  bMessage;
			PeekMessage(&msg, NULL, 0, 0,PM_NOREMOVE);
			while(msg.message != WM_QUIT)			//������Ϣѭ��
			{
				bMessage = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
				if(bMessage)
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				//Render();							//��Ⱦ
				pGame->GameFunc();
			} 
		}
		//Cleanup();									//�ͷ���Դ
		/*if(CResourcesPool::GetInstance())
		{
			delete CResourcesPool::GetInstance();
		}
		if (CSpritesManager::GetInstance())
		{
			delete CSpritesManager::GetInstance();
		}*/
		if (pGame)
		{
			delete pGame;
		}
		CoUninitialize();							//�ͷ�Com���

	}

	return 0 ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT ps;
	RECT		rect;
		int wmId;
		switch (message)
		{
		case WM_SOCKET:

			myClient.wndProc(hwnd, message, wParam, lParam);
			break;

		case WM_COMMAND:
			wmId = LOWORD(wParam);
			myClient.onCommand(wmId);
			break;
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0) ;
		}
		pGame->HandleKeyUp(wParam, lParam);
		return 0;
	case WM_KEYDOWN:
		pGame->HandleKeyDown(wParam, lParam);
	case WM_LBUTTONUP:												//����WM_KEYDWON
		pGame->HandleMouseUp(wParam,lParam);						//����HandleKeyDown�������������Ϣ
			return 0;
	case WM_LBUTTONDOWN:
		pGame->HandleMouseDown(wParam,lParam);
			return 0;
	case WM_DESTROY:
		PostQuitMessage(0) ;
		return 0 ;
	case WM_TIMER:
		pGame->HandleTimer(wParam, lParam);
		return 0;
	case WM_CLOSE:
		if (IDYES == MessageBoxW(hwnd, TEXT("�Ƿ���Ľ�����"), TEXT("GameWindow"), MB_YESNO))
		{
			DestroyWindow(hwnd);
		}
		break;
	}

	return DefWindowProc (hwnd, message, wParam, lParam) ;
}
