#pragma once
#include "Framework\Game.h"
#include "EnemyGame.h"
#include<list>
#include "Framework\XAudio2SoundPlayer.h"

#define	TIMER_ID		1 

class CGameScene 
{

public:
	CGameScene(HWND hwnd);
	~CGameScene();
	void Update(float fDeltaTime);
	void InitGame();
	void HandleMouseUp(WPARAM wParam, LPARAM lParam);
	void HandleMouseDown(WPARAM wParam, LPARAM lParam);
	void HandleKeyUp(WPARAM wParam, LPARAM lParam);
	void HandleKeyDown(WPARAM wParam, LPARAM lParam);
	void HandleTimer(WPARAM wParam, LPARAM lParam);
	void closeEnemy(bool c);
	void Clear();	
	void SetbPlaying(bool b);	//设置游戏状态？
	bool GetbPlaying();	//获得游戏状态？
	int GetNowMushroom();
	bool GetbInited();
protected:
	CEnemyGame *enemy;
private:
	HWND hwnd;
	int gametime = 1000*15;//1秒钟=1000
	bool bInited = false;
};