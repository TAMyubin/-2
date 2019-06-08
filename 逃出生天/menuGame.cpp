#include "menuGame.h"

menuGame::menuGame(HWND hWnd, int nFps) :CGame(hWnd, nFps)
{

}


menuGame::~menuGame()
{
}

void menuGame::Update(float fDeltaTime)
{
//	times--;


	CGame::Update(fDeltaTime);

	animation();
	if (gameScene != NULL) 
	{
		
		if (gameScene->GetbInited())
		{
			if (gameScene->GetbPlaying())
			{
				coutdown->SetVisible(true);
				wsprintf(text, TEXT("距离逃出口：%dM"), times);
				couttimes += fDeltaTime;
	/*			if (couttimes >= 1){
					times --;
					couttimes = 0;
				}*/
				times--;
				if (times < 0){
					times = 3600;
					couttimes = 0;
				}
				gameScene->Update(fDeltaTime);
			
				if (gameScene->GetNowMushroom() <= 0)
					gameScene->SetbPlaying(false);
			}
			else
			{
				
				if (gameScene->GetNowMushroom() <= 0)
				{
					times = 3600;
					finish->SetVisible(true);
					coutdown->SetVisible(false);
					failSprite->SetVisible(true);
					menuSpriteSecondBackground2->SetVisible(true);
					KillTimer(hWnd, TIMER_Game);
					x = 0;
				}
			}
		}
	}
}
void menuGame::AddTexture(){//加载图片
	CResourcesPool::GetInstance()->AddTexture(L"logo", L"RES/logo.png");
	CResourcesPool::GetInstance()->AddTexture(L"play", L"RES/play1.png");
	CResourcesPool::GetInstance()->AddTexture(L"Story", L"RES/Stroy.png");
	CResourcesPool::GetInstance()->AddTexture(L"edit", L"RES/win.png");
	CResourcesPool::GetInstance()->AddTexture(L"quit", L"RES/Quit.png");
	CResourcesPool::GetInstance()->AddTexture(L"help", L"RES/help.png");
	CResourcesPool::GetInstance()->AddTexture(L"jieju1", L"RES/jieju1.png");
	CResourcesPool::GetInstance()->AddTexture(L"jieju2", L"RES/jieju2.png");
	CResourcesPool::GetInstance()->AddTexture(L"jieju3", L"RES/jieju3.png");
	CResourcesPool::GetInstance()->AddTexture(L"jieju4", L"RES/jieju4.png");
	CResourcesPool::GetInstance()->AddTexture(L"secondbackground", L"RES/backgroundlong.png");
	CResourcesPool::GetInstance()->AddTexture(L"finish", L"RES/beijing.png");
	CResourcesPool::GetInstance()->AddTexture(L"Return", L"RES/Return.png");
	CResourcesPool::GetInstance()->AddTexture(L"zhadan", L"RES/zhadan.png");
	CResourcesPool::GetInstance()->AddTexture(L"daodan", L"RES/daodan.png");
	CResourcesPool::GetInstance()->AddTexture(L"shark", L"RES/shark.png");
	CResourcesPool::GetInstance()->AddTexture(L"xiaoyu", L"RES/xiaoyu.png");
	CResourcesPool::GetInstance()->AddTexture(L"youguan", L"RES/youguan.png");
	CResourcesPool::GetInstance()->AddTexture(L"boom", L"RES/boom.png");
	CResourcesPool::GetInstance()->AddTexture(L"Ship", L"RES/ship1.png");
	CResourcesPool::GetInstance()->AddTexture(L"Boat", L"RES/boat.png");
	CResourcesPool::GetInstance()->AddTexture(L"music1", L"RES/music1.png");
	CResourcesPool::GetInstance()->AddTexture(L"music2", L"RES/music2.png");
	CResourcesPool::GetInstance()->AddTexture(L"stone", L"RES/stone.png");
	CResourcesPool::GetInstance()->AddTexture(L"blood", L"RES/blood.png");
	CResourcesPool::GetInstance()->AddTexture(L"bloodBackground", L"RES/bloodBackground.png");
	CResourcesPool::GetInstance()->AddTexture(L"victory", L"RES/victory.png");
	CResourcesPool::GetInstance()->AddTexture(L"fail", L"RES/fail.png");
}
void menuGame::InitGame()
{
	times = 3600;
	
	wsprintf(text, TEXT("3000"));
	coutdown = TextSpriteManager::GetInstance()->CreateTextSprite(text, D2D1::RectF(100, 100, 650, 200),D2D1::ColorF::Black,32);


	AddTexture();
	GetClientRect(hWnd, &rectClient);
	srand(time(NULL));
	/*音乐播放*/
	musicPlayer = new XAudio2SoundPlayer();
	musicPlayer->LoadSound("bg", L"RES\\Luigi Rubino.wav");
	CSpritesManager *pSpritesManager = CSpritesManager::GetInstance();
	menuSpriteSecondBackground = pSpritesManager->CreateSprite(L"secondbackground", D2D1::Point2F(0, 0), 0, 0, 0, 0);
	menuSpriteStart = pSpritesManager->CreateSprite(L"play", D2D1::Point2F(400, 200), 0, 0, 0, 0);
	menuSpriteStory = pSpritesManager->CreateSprite(L"Story", D2D1::Point2F(700, 500), 0, 0, 0, 0);
	menuSpriteLogo = pSpritesManager->CreateSprite(L"logo", D2D1::Point2F(600, 50), 0, 0, 0, 0);
	menuSpriteEdit = pSpritesManager->CreateSprite(L"edit", D2D1::Point2F(850, 0), 0, 0, 0, 0);
	menuSpriteQuit = pSpritesManager->CreateSprite(L"quit", D2D1::Point2F(500, 350), 0, 0, 0, 0);
	menuSpriteHelp = pSpritesManager->CreateSprite(L"help", D2D1::Point2F(1300, 650), 0, 0, 0, 0);
	menuMusic2 = pSpritesManager->CreateSprite(L"music2", D2D1::Point2F(1300, 25), 0, 0, 0, 0);
	menuMusic1 = pSpritesManager->CreateSprite(L"music1", D2D1::Point2F(1300, 25), 0, 0, 0, 0);
	menuMusic1->SetZOrder(10000);
	menuMusic2->SetZOrder(10000);
	pSpritesManager->ReZOrder(menuMusic1);
	pSpritesManager->ReZOrder(menuMusic2);
	menuSpriteReturn = pSpritesManager->CreateSprite(L"Return", D2D1::Point2F(50, 50), 0, 0, 0, 0);
	menuSpriteReturn->SetScaleFactor(0.5f, 0.5f);
	menuSpriteSecondBackground2 = pSpritesManager->CreateSprite(L"finish", D2D1::Point2F(0, 0), 0, 0, 0, 0);
	victorySprite = pSpritesManager->CreateSprite(L"victory", D2D1::Point2F(220, 200), 0, 0, 0, 0);
	failSprite = pSpritesManager->CreateSprite(L"fail", D2D1::Point2F(300, 250), 0, 0, 0, 0);

	int type;
	if (t == 0)
		type = (int)(rand() *10%3) ;
	wstring   sResourceName;
	switch (type)
	{
	case 0:sResourceName = L"jieju1"; break;
	case 1:sResourceName = L"jieju2";  break;
	case 2:sResourceName = L"jieju3";  break;
	case 3:sResourceName = L"jieju4";  break;
	default:
		break;
	}
	finish = pSpritesManager->CreateSprite(sResourceName, D2D1::Point2F(150, 150), 0, 0, 0, 0);
	finish->SetScaleFactor(1.5f, 1.5f);
	shark = CSpritesManager::GetInstance()->CreateAnimationSprite(L"shark", 4, 10, D2D1::Point2F(200, 500), 1000, 300, 0, 0, D2D1::Point2F(800, 300));
	shark->SetScaleFactor(0.25f, 0.25f);
	shark->Play();
	Unsetvisiable();
}
void menuGame::Unsetvisiable(){//开始暂时隐藏的图片

	shark->SetVisible(false);
	menuSpriteReturn->SetVisible(false);
	victorySprite->SetVisible(false);
	finish->SetVisible(false);
	failSprite->SetVisible(false);
	menuSpriteEdit->SetVisible(false);
	menuSpriteQuit->SetVisible(false);
	menuSpriteLogo->SetVisible(true);
	menuMusic2->SetVisible(false);
	menuSpriteSecondBackground2->SetVisible(false);
	coutdown->SetVisible(false);
}

void menuGame::animation(){//背景的动画，结束时鲨鱼的动画
	x++;
	y--;
	if (ani = true){

		shark->SetPos(D2D1::Point2F(x*speed, 500));
		if (x*speed >= 1400)
		{
			shark->SetVisible(false);
			x = 0;
		}
	}
	if (menuSpriteSecondBackground)
		menuSpriteSecondBackground->SetPos(D2D1::Point2F(y*speed, 0));
	if (y*speed <= -8500)
	{
		y = 0;
	}

}
void menuGame::HandleMouseDown(WPARAM wParam, LPARAM lParam)
{
	
	if (gameScene != NULL) { gameScene->HandleMouseDown(wParam, lParam); }
	POINT p;
	p.x = LOWORD(lParam);
	p.y = HIWORD(lParam);
	//音效
	if (menuMusic2->IsSelected(p.x, p.y) && menuMusic2->IsVisible())
	{
		menuMusic1->SetVisible(true);
		menuMusic2->SetVisible(false);

		musicPlayer->StopSound("bg");
	}
	else if (menuMusic1->IsSelected(p.x, p.y) && menuMusic1->IsVisible())
	{
		menuMusic1->SetVisible(false);
		menuMusic2->SetVisible(true);

		musicPlayer->PlaySound("bg");
	}

	if (menuSpriteStart->IsSelected(p.x, p.y) && menuSpriteStart->IsVisible())
	{
		
		menuSpriteStart->SetVisible(false);
		menuSpriteLogo->SetVisible(false);
		menuSpriteHelp->SetVisible(false);
		menuSpriteReturn->SetVisible(true);
		menuSpriteStory->SetVisible(false);
		if (gameScene == NULL)
		{
			x = 0;
			gameScene = new CGameScene(hWnd);
			gameScene->SetbPlaying(true);
			SetTimer(hWnd, TIMER_Game, gametime, NULL);
		}
	}
	//游戏帮助
	if (menuSpriteHelp->IsSelected(p.x, p.y) && menuSpriteHelp->IsVisible())
	{
		menuSpriteStart->SetVisible(false);
		menuSpriteQuit->SetVisible(true);
		menuSpriteHelp->SetVisible(false);
		menuSpriteStory->SetVisible(false);

	}
	//点击提示返回开始
	if (menuSpriteQuit->IsSelected(p.x, p.y) && menuSpriteQuit->IsVisible())
	{
		menuSpriteStart->SetVisible(true);
		menuSpriteQuit->SetVisible(false);
		menuSpriteHelp->SetVisible(true);
		menuSpriteStory->SetVisible(true);

	}
	if (menuSpriteStory->IsSelected(p.x, p.y) && menuSpriteStory->IsVisible())//退出游戏
	{
		PostQuitMessage(0);
	}

	if (menuSpriteReturn->IsSelected(p.x, p.y) && menuSpriteReturn->IsVisible())
	{
		times = 3600;
		coutdown->SetVisible(false);
		menuSpriteHelp->SetVisible(true);
		menuSpriteLogo->SetVisible(true);//
		menuSpriteStart->SetVisible(true);
		menuSpriteReturn->SetVisible(false);
		menuSpriteSecondBackground->SetVisible(true);
		menuSpriteStory->SetVisible(true);
		gameScene->SetbPlaying(false);
		menuSpriteEdit->SetVisible(false);
		x = 0;
		gameScene->Clear();
		delete gameScene;
		gameScene = NULL;
	}


	if (victorySprite->IsSelected(p.x, p.y) && victorySprite->IsVisible())
	{
		
		menuSpriteSecondBackground2->SetVisible(false);
		victorySprite->SetVisible(false);
		menuSpriteLogo->SetVisible(true);
		menuSpriteEdit->SetVisible(false);
		menuSpriteStart->SetVisible(true);
		menuSpriteStory->SetVisible(true);/*退出游戏*/
		menuSpriteSecondBackground2->SetVisible(false);
		menuSpriteSecondBackground->SetVisible(true);
		menuSpriteReturn->SetVisible(false);
		gameScene->Clear();
		delete gameScene;
		gameScene = NULL;
	}

	if (failSprite->IsSelected(p.x, p.y) && failSprite->IsVisible())
	{
		
		finish->SetVisible(false);
		menuSpriteLogo->SetVisible(true);
		menuSpriteStart->SetVisible(true);
		menuSpriteReturn->SetVisible(false);
		menuSpriteStory->SetVisible(true);/*退出游戏*/
		failSprite->SetVisible(false);
		menuSpriteSecondBackground->SetVisible(true);
		menuSpriteSecondBackground2->SetVisible(false);
		menuSpriteHelp->SetVisible(true);
		gameScene->Clear();
		delete gameScene;
		gameScene = NULL;
	}
}
void menuGame::HandleMouseUp(WPARAM wParam, LPARAM lParam)
{
	if (gameScene != NULL)
	{
		if (gameScene->GetbPlaying())
			gameScene->HandleMouseUp(wParam, lParam);
	}
}

void menuGame::HandleKeyUp(WPARAM wParam, LPARAM lParam)
{
	if (gameScene != NULL)
	{
		if (gameScene->GetbPlaying())
			gameScene->HandleKeyUp(wParam, lParam);
	}


}
void menuGame::HandleKeyDown(WPARAM wParam, LPARAM lParam)
{
	if (gameScene != NULL)
	{
		if (gameScene->GetbPlaying())
			gameScene->HandleKeyDown(wParam, lParam);
	}

}


void menuGame::HandleTimer(WPARAM wParam, LPARAM lParam)
{
	if (gameScene != NULL)
	{
		if (gameScene->GetbPlaying())
		{
			gameScene->HandleTimer(wParam, lParam);
			switch (wParam)
			{
			case TIMER_Game:
				if (gameScene->GetbPlaying() && gameScene->GetNowMushroom() > 0)
				{
					coutdown->SetVisible(false);
					gameScene->closeEnemy(true);
					gameScene->SetbPlaying(false);
					KillTimer(hWnd, TIMER_Game);
					shark->SetVisible(true);
						ani = true;
						victorySprite->SetVisible(true);
					menuSpriteEdit->SetVisible(true);
		
				}
				break;
			}

		}
	}
}
