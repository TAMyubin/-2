#include "GameScene.h"
CSpritesManager *pSpritesManager = CSpritesManager::GetInstance();

CGameScene::CGameScene(HWND hwnd)
{
	this->hwnd = hwnd;
	InitGame();
}


CGameScene::~CGameScene()
{
	//pSpritesManager->DeleteSprite(Player);
}


void CGameScene::closeEnemy(bool c){
	enemy->closeEnemy(true);
}
//设置游戏状态？
void CGameScene::SetbPlaying(bool b)
{
	if (enemy != NULL)
		enemy->SetbPlaying(b);

}

//获得游戏状态？
bool CGameScene::GetbPlaying()
{
	if (enemy != NULL)
		return enemy->GetbPlaying();
}

int CGameScene::GetNowMushroom()
{
	if (enemy != NULL)
		return enemy->GetNowMushroom();
}

bool CGameScene::GetbInited()
{
	return bInited;
}

void CGameScene::Update(float fDeltaTime)
{
	if (enemy != NULL)
	{
		if (enemy->GetbInited())
		{
			if (enemy->GetbPlaying())
			{
				enemy->Update(fDeltaTime);
				if (enemy->GetNowMushroom() <= 0)
					enemy->SetbPlaying(false);
			}
		}
	}
		if (enemy != NULL) { enemy->Update(fDeltaTime);	
	}
}

void CGameScene::InitGame()
{
	enemy = new CEnemyGame();
	enemy->CreateEnemy(1);//调用产生怪物数量
	SetTimer(hwnd, TIMER_ID, gametime, NULL);
	bInited = true;	
}

void CGameScene::HandleMouseDown(WPARAM wParam, LPARAM lParam)
{
}
void CGameScene::HandleMouseUp(WPARAM wParam, LPARAM lParam)
{	
}

void CGameScene::HandleKeyDown(WPARAM wParam, LPARAM lParam)
{

	if (enemy != NULL)
	{
		if (enemy->GetbPlaying())
			enemy->HandleKeyDown(wParam, lParam);
	}
}
void CGameScene::HandleKeyUp(WPARAM wParam, LPARAM lParam)
{
	if (enemy != NULL)
	{
		if (enemy->GetbPlaying())
			enemy->HandleKeyUp(wParam, lParam);
	}
}

void CGameScene::HandleTimer(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case TIMER_ID:
		if (enemy->GetbPlaying())
		{
			enemy->CreateEnemy(1);//
			KillTimer(hwnd, TIMER_ID);
			SetTimer(hwnd, TIMER_ID, gametime, NULL);
		}
		break;
	}
}

void CGameScene::Clear()
{
	
	enemy->Clean();
	delete enemy;
	
	KillTimer(hwnd, TIMER_ID);

}

