/************************************************************************
游戏管理类，用于创建各种精灵和处理各种鼠标键盘事件。
这是一个抽象类，不能直接创建对象，所以使用者应该定义一个子类继承
自CGame,并在子类里重写InitGame()，创建自己的各种资源
************************************************************************/
#pragma once

#include <D2D1.h>
#include <dwrite.h>
#include <wincodec.h>   //创建WIC位图文件需要的
#include "time.h"

#include "ResourcesPool.h"
#include "SpritesManager.h"

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib,"windowscodecs.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")



class CGame
{
public:
	CGame(HWND hWnd,int nFps = 0);											//构造函数
	virtual ~CGame(void);
public:
	//-----处理相应的鼠标和键盘事件，这里设置为虚函数，其子类可以根据程序需要加以重写---------
	virtual void HandleMouseUp(WPARAM wParam,LPARAM lParam){}				
	virtual void HandleMouseDown(WPARAM wParam,LPARAM lParam){}
	virtual void HandleMouseMove(WPARAM wParam,LPARAM lParam){}
	virtual void HandleDoubleClick(WPARAM wParam,LPARAM lParam){}
	virtual void HandleKeyUp(WPARAM wParam,LPARAM lParam){}
	virtual void HandleKeyDown(WPARAM wParam,LPARAM lParam){}
	
	/*virtual bool IsKeyUp(int key)
	{
		pInput->ReadKeyboard();
		return pInput->IsKeyUp(key);
	}
	virtual bool IsKeyPressed(int key)
	{
		pInput->ReadKeyboard();
		return pInput->IsKeyPressed(key);
	}
	virtual bool IsLButtonPressed()
	{
		pInput->ReadMouse();
		return pInput->IsLButtonPressed();
	}
	virtual bool IsRButtonPressed()
	{
		pInput->ReadMouse();
		return pInput->IsRButtonPressed();
	}

	virtual bool IsLButtonUp()
	{
		pInput->ReadMouse();
		return pInput->IsLButtonUp();
	}
	virtual bool IsRButtonUp()
	{
		pInput->ReadMouse();
		return pInput->IsRButtonUp();
	}*/

	//-----------------------------------------------------------------------------------------
	virtual void InitGame() = 0;										//纯虚函数，创建资源		
	

	void GameFunc();															//执行游戏帧函数

	
protected:

	HRESULT 	Init(HWND hWnd);
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();


	virtual void Update(float fDeltaTime);								//帧函数	
	virtual void Render();		//渲染	

	ID2D1Factory				*pD2DFactory  ;	 // Direct2D factory
	ID2D1HwndRenderTarget		*pRenderTarget; 
	IWICImagingFactory			*pWICFactory ;
	IDWriteFactory				*pDWriteFactory;
	IDWriteTextFormat			*pTextFormat;
	ID2D1SolidColorBrush		*pTextBrush;

	HINSTANCE			hInstance;

	CSpritesManager		*pSpritesManager;									//精灵管理对象指针	
	CResourcesPool	    *pResPool	;										//资源管理对象指针	
	HWND				hWnd;												//窗口句柄
	int					nFps;												//每秒多少帧
	RECT				rectClient;											//客户区矩形

	DWORD				t0;													//前一帧的时刻
	int					nFixedDeltaTime;									//固定的时间

	double				t;													//计算帧频所需要的累计时间
	int					nfps;												//帧频
	unsigned int        nFrames;											//总帧数
	TCHAR               szBuf[255];											//将帧频转换为字符串所用到的字符缓冲 


	

	
	

	void Cleanup();

	
	

};
