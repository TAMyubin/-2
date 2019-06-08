/************************************************************************
��Ϸ�����࣬���ڴ������־���ʹ�������������¼���
����һ�������࣬����ֱ�Ӵ�����������ʹ����Ӧ�ö���һ������̳�
��CGame,������������дInitGame()�������Լ��ĸ�����Դ
************************************************************************/
#pragma once

#include <D2D1.h>
#include <dwrite.h>
#include <wincodec.h>   //����WICλͼ�ļ���Ҫ��
#include "time.h"

#include "ResourcesPool.h"
#include "SpritesManager.h"
#include "TextSpriteManager.h"
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib,"windowscodecs.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")



class CGame
{
public:
	CGame(HWND hWnd,int nFps = 0);											//���캯��
	virtual ~CGame(void);
public:
	//-----������Ӧ�����ͼ����¼�����������Ϊ�麯������������Ը��ݳ�����Ҫ������д---------
	virtual void HandleMouseUp(WPARAM wParam,LPARAM lParam){}				
	virtual void HandleMouseDown(WPARAM wParam,LPARAM lParam){}
	virtual void HandleMouseMove(WPARAM wParam,LPARAM lParam){}
	virtual void HandleDoubleClick(WPARAM wParam,LPARAM lParam){}
	virtual void HandleKeyUp(WPARAM wParam,LPARAM lParam){}
	virtual void HandleKeyDown(WPARAM wParam, LPARAM lParam){}
	virtual void HandleTimer(WPARAM wParam, LPARAM lParam){}
	
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
	virtual void InitGame() = 0;										//���麯����������Դ		
	

	void GameFunc();															//ִ����Ϸ֡����

	
protected:

	HRESULT 	Init(HWND hWnd);
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();


	virtual void Update(float fDeltaTime);								//֡����	
	virtual void Render();		//��Ⱦ	

	ID2D1Factory				*pD2DFactory  ;	 // Direct2D factory
	ID2D1HwndRenderTarget		*pRenderTarget; 
	IWICImagingFactory			*pWICFactory ;
	IDWriteFactory				*pDWriteFactory;
	IDWriteTextFormat			*pTextFormat;
	ID2D1SolidColorBrush		*pTextBrush;

	HINSTANCE			hInstance;
	TextSpriteManager   *pTextSpriteManager;                                //�ı��������ָ��
	CSpritesManager		*pSpritesManager;									//����������ָ��	
	CResourcesPool	    *pResPool	;										//��Դ�������ָ��	
	HWND				hWnd;												//���ھ��
	int					nFps;												//ÿ�����֡
	RECT				rectClient;											//�ͻ�������

	DWORD				t0;													//ǰһ֡��ʱ��
	int					nFixedDeltaTime;									//�̶���ʱ��

	double				t;													//����֡Ƶ����Ҫ���ۼ�ʱ��
	int					nfps;												//֡Ƶ
	unsigned int        nFrames;											//��֡��
	TCHAR               szBuf[255];											//��֡Ƶת��Ϊ�ַ������õ����ַ����� 


	

	
	

	void Cleanup();

	
	

};
