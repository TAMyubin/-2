#pragma once
#include "Framework\Game.h"
#include "GameScene.h"
#include"EnemyGame.h"
#include "Framework\XAudio2SoundPlayer.h"
#define	TIMER_Game		2
class menuGame :
	public CGame
{
public:
	menuGame(HWND hWnd, int nFps);
	~menuGame();
	void Update(float fDeltaTime);

	void InitGame();

	void HandleMouseUp(WPARAM wParam, LPARAM lParam);
	void HandleMouseDown(WPARAM wParam, LPARAM lParam);
	void HandleKeyUp(WPARAM wParam, LPARAM lParam);
	void HandleKeyDown(WPARAM wParam, LPARAM lParam);
	void HandleTimer(WPARAM wParam, LPARAM lParam);

	void AddTexture();//加载图片
	void Unsetvisiable();//开始暂时隐藏的图片
	void animation();//背景的动画，结束时鲨鱼的动画

	WCHAR *text = new WCHAR[2];
	TextSprite *coutdown;
	int times=60;
	float couttimes=2;
protected:
	//CAnimationSprite *menuSprite;
	CSprite  *menuSpriteStart;
	CSprite  *menuSpriteHelp;
	CSprite  *menuSpriteStory;
	CSprite  *menuSpriteBackGround;
	CSprite  *menuSpriteSecondBackground;
	CSprite  *menuSpriteSecondBackground2;
	CSprite  *menuSpriteReturn;
	CSprite *menuSpriteLogo;
	CSprite *menuSpriteEdit;
	CSprite *menuSpriteQuit;

	CSprite  *victorySprite;
	CSprite *failSprite;
	CSprite *menuMusic1;
	CSprite *menuMusic2;
	CGameScene *gameScene;
	CEnemyGame *cenemygame;

	CSprite *finish;
	CAnimationSprite *shark;
	XAudio2SoundPlayer *musicPlayer;

	int speed = 5;
	int x = 0;
	int y = 0;
	int gametime = 1000 * 60;
	bool ani = false;


	


};

