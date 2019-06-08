#include "EnemyGame.h"


CEnemyGame::CEnemyGame()
{
	//怪物初始随机最小最大速度
	MinMaxSpeed[0] = 100;
	MinMaxSpeed[1] = 130;
	MinMaxSpeed[2] = 110;
	MinMaxSpeed[3] = 130;
	//怪物初始攻击范围
	AttackRange[0] = 46.0f;
	AttackRange[1] = 48.0f;

	//for (int i = 0; i < MaxNum; i++)
	//	EnemySprite[i] = NULL;

	NowMushroom = 0;
	InitGame();

	bPlaying = true;
}


CEnemyGame::~CEnemyGame()
{

}

void CEnemyGame::closeEnemy(bool c){
	for (CEnemySprite *e : EnemySpriteList)
	{
		e->Mushroom->MushroomSprite->SetVisible(false);
		e->smallfishAnimation->SetVisible(false);
		e->EnemySpriteAnimation->SetVisible(false);
		e->youguanAnimation->SetVisible(false);
		e->daodanAnimation->SetVisible(false);
		Ship->SetVisible(false);
		Boat->SetVisible(false);
	}
}

//设置游戏状态？
void CEnemyGame::SetbPlaying(bool b)
{
	bPlaying = b;
}

//获得游戏状态？
bool CEnemyGame::GetbPlaying()
{
	return bPlaying;
}


int CEnemyGame::GetNowMushroom()
{
	return NowMushroom;
}

bool CEnemyGame::GetbInited()
{
	return bInited;
}
//更新怪物行走路线
void CEnemyGame::Update(float fDeltaTime)
{
	moveEnemy();
	/*主角操作*/
	NowMushroom = 0;
	PlayerMover(speed);
	/*游戏失败时隐藏障碍物*/
	if (!bPlaying || NowMushroom <= 0)
	{
		for (CEnemySprite *e : EnemySpriteList)
		{
			e->EnemySpriteAnimation->Stop();
			e->EnemySpriteAnimation->SetVisible(false);
			e->youguanAnimation->SetVisible(false);
			e->daodanAnimation->SetVisible(false);
			e->smallfishAnimation->SetVisible(false);
			Ship->SetVisible(false);
			Boat->SetVisible(false);
		}
		bPlaying = false;
	}

	/*游戏进行时*/
	if (bPlaying)
	{
		static int t = 0;
		if (t % 20 == 0)
		{
			if (NowMushroom > 0)
			{
				std::list<CEnemySprite*>::iterator iter;
				for (CEnemySprite *e : EnemySpriteList)
				{
					if (e->EnemySpriteAnimation)
					{
						//判断怪物精灵是否处于攻击状态
						if (e->bIsAttack)
						{
							//是否有目标
							if (e->Mushroom->bIsVisible != false)
							{
								for (CMushroomSprite *m : MushroomSprite)
								{
									if (m == e->Mushroom)
									{
										m->NowBlood = -1;
										m->BloodSprite->SetDestRect(50, 680, m->NowBlood, 6);
										
										if (m->NowBlood <= 0)
										{

											m->MushroomSprite->SetVisible(false);
											m->BloodSpriteBackground->SetVisible(true);
											m->BloodSprite->SetVisible(true);
											m->bIsVisible = m->MushroomSprite->IsVisible();
											e->Mushroom = NULL;
										}
										break;
									}
								}
							}
							else
							{
								e->Mushroom = NULL;
								e->bIsAttack = false;
							}
						}
						else
						{
							D2D1_POINT_2F EnemyPos = e->EnemySpriteAnimation->GetPos();
							if (e->Mushroom == NULL || e->Mushroom->bIsVisible == false)
							{
								float dis = NULL;
								for (CMushroomSprite *m : MushroomSprite)
								{
									if (m->bIsVisible)
									{
										D2D1_POINT_2F pos  = m->MushroomSprite->GetPos();
										float n = sqrt(pow(abs(EnemyPos.x - pos.x), 2) + pow(abs(EnemyPos.y - pos.y), 2));
										if (dis == NULL || dis > n)
										{
											dis = n;
											e->Mushroom = m;
										}
									}
								}
							}
							D2D1_POINT_2F MushroomPos = e->Mushroom->MushroomSprite->GetPos();
							if (e->daodanAnimation->GetPos().x + 100 >= MushroomPos.x&&e->daodanAnimation->GetPos().x - 100 <= MushroomPos.x)
							{
								PosX3 = true;
							}
							else
							{
								PosX3 = false;
							}
							if (e->daodanAnimation->GetPos().y + 50 >= MushroomPos.y&&e->daodanAnimation->GetPos().y - 50 <= MushroomPos.y)
							{
								PosY3 = true;
							}
							else
							{
								PosY3 = false;
							}

							if (PosX3&&PosY3)
							{
								e->bIsAttack = true;
								e->boomAnimation->SetVisible(true);
								e->daodanAnimation->SetVisible(false);
								e->boomAnimation->SetPos(D2D1::Point2F(e->daodanAnimation->GetPos().x, e->daodanAnimation->GetPos().y));
							}
							else
							{
								e->boomAnimation->SetVisible(false);
								e->daodanAnimation->SetVisible(true);
							}

							if (e->smallfishAnimation->GetPos().x + 100 > MushroomPos.x&&e->smallfishAnimation->GetPos().x - 100 < MushroomPos.x)
							{
								PosX2 = true;
							}
							else
							{
								PosX2 = false;
							}
							if (e->smallfishAnimation->GetPos().y + 100 > MushroomPos.y  && e->smallfishAnimation->GetPos().y - 100 < MushroomPos.y)
							{
								PosY2 = true;
							}
							else
							{
								PosY2 = false;
							}

							if (PosX2&&PosY2)
							{
								e->smallfishAnimation->SetVisible(false);
								speed = 4.0f;
							}

							if (e->youguanAnimation->GetPos().x + 50 >= MushroomPos.x&&e->youguanAnimation->GetPos().x - 50 <= MushroomPos.x)
							{
								PosX1 = true;
							}
							else
							{
								PosX1 = false;
							}
							if (e->youguanAnimation->GetPos().y + 200 >= MushroomPos.y&&e->youguanAnimation->GetPos().y <= MushroomPos.y)
							{
								PosY1 = true;
							}
							else
							{
								PosY1 = false;
							}
							if (PosX1&&PosY1)
							{
								e->bIsAttack = true;
								e->boomAnimation->SetVisible(true);
								e->youguanAnimation->SetVisible(false);
								e->boomAnimation->SetPos(D2D1::Point2F(e->youguanAnimation->GetPos().x, e->youguanAnimation->GetPos().y));
							}
							if (Ship->GetPos().x + 200 > MushroomPos.x&&Ship->GetPos().x - 100 < MushroomPos.x)
							{
								PosX = true;
							}
							else
							{
								PosX = false;
							}
							if (Ship->GetPos().y + 200 > MushroomPos.y - 100 && Ship->GetPos().y + 50 < MushroomPos.y)
							{
								PosY = true;
							}
							else
							{
								PosY = false;
							}

							if (PosX&&PosY)
							{
								e->boomAnimation->SetVisible(true);
								e->bIsAttack = true;
							}
						
							float speed = e->Speed;
							float x = 0, y = 0;
							//怪物追踪
							D2D1_POINT_2F daodanPos = e->daodanAnimation->GetPos();
							if (EnemyPos.x > MushroomPos.x)
							{
								e->EnemySpriteAnimation->SetPos(D2D1::Point2F(EnemyPos.x - speed+0.5f, EnemyPos.y));
							}
							if (EnemyPos.x < MushroomPos.x)
							{
								e->EnemySpriteAnimation->SetPos(D2D1::Point2F(EnemyPos.x + speed-0.5f, EnemyPos.y));
							}
							if (EnemyPos.y > MushroomPos.y)
							{
								e->EnemySpriteAnimation->SetPos(D2D1::Point2F(e->EnemySpriteAnimation->GetPos().x, EnemyPos.y - speed+0.5f));
								e->Speed = speed * SpeedVar;
							}
							if (EnemyPos.y < MushroomPos.y)
							{
								e->EnemySpriteAnimation->SetPos(D2D1::Point2F(e->EnemySpriteAnimation->GetPos().x, EnemyPos.y + speed+0.5f));
								e->Speed = speed / SpeedVar;
							}
							if (daodanPos.y > MushroomPos.y)
							{
								e->daodanAnimation->SetPos(D2D1::Point2F(e->daodanAnimation->GetPos().x - speed-1.0f, daodanPos.y - speed+0.25f));
								e->Speed = speed * SpeedVar;
							}
							if (daodanPos.y < MushroomPos.y)
							{

								e->daodanAnimation->SetPos(D2D1::Point2F(e->daodanAnimation->GetPos().x - speed-1.0f, daodanPos.y + speed+0.25f));
								e->Speed = speed / SpeedVar;
							}

							float dis = sqrt(pow(abs(e->EnemySpriteAnimation->GetPos().x - MushroomPos.x), 2) + pow(abs(e->EnemySpriteAnimation->GetPos().y - MushroomPos.y), 2));
							if (dis <= e->AttackRange)
							{
								for (CMushroomSprite *m : MushroomSprite)
								{
									e->bIsAttack = true;
									m->BloodSprite->SetDestRect(50, 680, 0, 6);
									m->NowBlood = 0;
									e->boomAnimation->SetVisible(true);
									e->boomAnimation->SetPos(D2D1::Point2F(e->EnemySpriteAnimation->GetPos().x, e->EnemySpriteAnimation->GetPos().y));
									if (m->NowBlood <= 0)
									{
										m->MushroomSprite->SetVisible(false);
										m->BloodSpriteBackground->SetVisible(false);
										m->BloodSprite->SetVisible(false);
										m->bIsVisible = m->MushroomSprite->IsVisible();
										e->Mushroom = NULL;
									}
								/*	CleanEnemy((CEnemySprite*)e);*/
								}
							break;
						
							}
						}
					}
				}
			}
		}
		else
			t++;
	}
}

//初始化主角位置及血量
void CEnemyGame::InitGame()
{
	static int i = 1;
	if (i <= MushroomNum)
	{
		CMushroomSprite *Mushroom = new CMushroomSprite();
		Mushroom->bIsVisible = true;
		Mushroom->Blood = 100;
		Mushroom->NowBlood = Mushroom->Blood;
		Mushroom->MushroomSprite = CSpritesManager::GetInstance()->CreateAnimationSprite(L"shark", 4, 10, D2D1::Point2F(200, 500), 1000, 300, 0, 0, D2D1::Point2F(800, 300));
		Mushroom->MushroomSprite->SetScaleFactor(0.25f, 0.25f);
		Mushroom->MushroomSprite->Play();
		Mushroom->BloodSpriteBackground = CSpritesManager::GetInstance()->CreateSprite(L"bloodBackground", D2D1::Point2F(50, 680), 100, 6, 0, 0, D2D1::Point2F(0, 6.0f / 2));
		Mushroom->BloodSpriteBackground->SetPos(D2D1::Point2F(50, 680));
		Mushroom->BloodSprite = CSpritesManager::GetInstance()->CreateSprite(L"blood", D2D1::Point2F(50, 680), 100, 6, 0, 0, D2D1::Point2F(0, 6.0f / 2));
		Mushroom->BloodSprite->SetPos(D2D1::Point2F(50, 680));
		MushroomSprite.push_back(Mushroom);
		i++;
	}

	bInited = true;
}
//人物操作
void CEnemyGame::PlayerMover(float speed){
	for (CMushroomSprite *m : MushroomSprite)
	{
		if (m->MushroomSprite->IsVisible())
		{
			m->bIsVisible = m->MushroomSprite->IsVisible();
			/*空气墙*/
			if (walkleft&&m->MushroomSprite->GetPos().x > 100)
			{
				m->MushroomSprite->SetPos(D2D1::Point2F(m->MushroomSprite->GetPos().x - speed, m->MushroomSprite->GetPos().y));
			}
			if (walkright&&m->MushroomSprite->GetPos().x < 1400)
			{
				m->MushroomSprite->SetPos(D2D1::Point2F(m->MushroomSprite->GetPos().x + speed, m->MushroomSprite->GetPos().y));
			}
			if (walkdown&&m->MushroomSprite->GetPos().y<750)
			{
				m->MushroomSprite->SetPos(D2D1::Point2F(m->MushroomSprite->GetPos().x, m->MushroomSprite->GetPos().y + speed));
			}
			if (walkup&&m->MushroomSprite->GetPos().y>150)
			{
				m->MushroomSprite->SetPos(D2D1::Point2F(m->MushroomSprite->GetPos().x, m->MushroomSprite->GetPos().y - speed));
			}

			NowMushroom++;
		}
	}
}
//创建单个怪物
void CEnemyGame::CreateEnemy(int t)
{
	if (Boat == NULL&&Ship == NULL)
	{
		Boat = CSpritesManager::GetInstance()->CreateSprite(L"Boat", D2D1::Point2F(0, 0), 0, 0, 0, 0);
		Ship = CSpritesManager::GetInstance()->CreateSprite(L"Ship", D2D1::Point2F(1000, 700), 1000, 500, 0, 0, D2D1::Point2F(500, 250));
		Ship->SetScaleFactor(0.5f, 0.5f);
		Boat->SetScaleFactor(0.75f, 0.75f);
	}
	D2D1_POINT_2F ShipPos = Ship->GetPos();
	D2D1_POINT_2F BoatPos = Boat->GetPos();

		static float X = 0;
		//随机怪物种类，血量，速度，位置
		int type;
		if (t == 0)
			type = rand() % Type + 1;
		else
			type = t;
		float speed = (float)((rand() % (MinMaxSpeed[(type - 1) * 2 + 1] - MinMaxSpeed[(type - 1) * 2] + 1)) + MinMaxSpeed[(type - 1) * 2]) / (float)100;
		float x = ShipPos.x+500;
		float y = ShipPos.y;

		float Width, Height;
		float Scale;

		CEnemySprite *Enemy = new CEnemySprite();

		Enemy->AttackRange = AttackRange[type - 1];
		Enemy->bIsAttack = false;

		Enemy->Mushroom = NULL;

		Enemy->Speed = speed;


		//产生追踪的
		Enemy->EnemySpriteAnimation = CSpritesManager::GetInstance()->CreateAnimationSprite(L"zhadan", 4, 10, D2D1::Point2F(x, y), 632.0f / 4, 187.0f, 0, 0, D2D1::Point2F(632.0f / 8, 187.0f / 2));
		Enemy->EnemySpriteAnimation->SetScaleFactor(1, 1);
		Enemy->EnemySpriteAnimation->Play();

		Enemy->daodanAnimation = CSpritesManager::GetInstance()->CreateAnimationSprite(L"daodan", 4, 10, D2D1::Point2F(x, y), 1000.0f / 4, 105.0f, 0, 0, D2D1::Point2F(1000.0f / 8, 105.0f / 2));
		Enemy->daodanAnimation->SetScaleFactor(1, 1);
		Enemy->daodanAnimation->Play();
		//产生小鱼
		Enemy->smallfishAnimation = CSpritesManager::GetInstance()->CreateAnimationSprite(L"xiaoyu", 4, 10, D2D1::Point2F(500, 200), 200 / 4, 28, 0, 0, D2D1::Point2F(200 / 8, 28 / 2));
		Enemy->smallfishAnimation->SetScaleFactor(1, 1);
		Enemy->smallfishAnimation->Play();

		Enemy->youguanAnimation = CSpritesManager::GetInstance()->CreateAnimationSprite(L"youguan", 4, 10, D2D1::Point2F(500, 300), 525 / 4, 288, 0, 0, D2D1::Point2F(525 / 8, 288 / 2));
		Enemy->youguanAnimation->SetScaleFactor(1, 1);
		Enemy->youguanAnimation->Play();

		Enemy->boomAnimation = CSpritesManager::GetInstance()->CreateAnimationSprite(L"boom", 4, 4, D2D1::Point2F(500, 400), 704 / 4, 168, 0, 0, D2D1::Point2F(704 / 8, 168 / 2));
		Enemy->boomAnimation->SetScaleFactor(1, 1);
		Enemy->boomAnimation->Play();
		Enemy->boomAnimation->SetVisible(false);
		EnemySpriteList.push_back(Enemy);

	
	
}

//运动轨迹函数
void CEnemyGame::moveEnemy(){
	/*船和潜艇移动轨迹*/
	x--;
	Ship->SetPos(D2D1::Point2F(x*shipspeed, 600));
	Boat->SetPos(D2D1::Point2F(x*boatspeed, 0));
	double Boatx = Boat->GetPos().x;
	if (x*shipspeed < -1000)
	{
		x = 1000;
	}
	if (x*boatspeed < -1000)
	{
		x = 1000;
	}
	for (CEnemySprite *e : EnemySpriteList){
		fishy++;
		youguany++;
		/*小鱼移动*/
		e->smallfishAnimation->SetPos(D2D1::Point2F(fishy*fishScale, 300));
		if (fishy*fishScale>1440)
		{
			fishy = -100;
			e->smallfishAnimation->SetVisible(true);
		}
		/*油罐移动*/
		if (Boatx<=1000)
		{
			e->youguanAnimation->SetPos(D2D1::Point2F(900, youguany*youguanScale));
		
			if (youguany*youguanScale > 1000)
			{
				e->boomAnimation->SetVisible(false);
				e->youguanAnimation->SetVisible(true);
				youguany = 0;
			
			}
		}
	}
}
/*碰撞*/

//创建指定数量个怪物(类型为0时，随机类型，否则指定类型创建多个怪物)
void CEnemyGame::CreateEnemyw(int n)
{
	//创建新的敌人
	for (int j = 0; j < n; j++)
	{
		CreateEnemy();
	}
}

//碰撞后清除敌人
void CEnemyGame::CleanEnemy(CEnemySprite *sprite)
{

	std::list<CEnemySprite*>::iterator iter;
	for (iter = EnemySpriteList.begin(); iter != EnemySpriteList.end();)
	{
		CEnemySprite *e = (*iter);
		if (e == (CEnemySprite*)sprite)
		{
			EnemySpriteList.erase(iter++);
			CSpritesManager *pSpritesManager = CSpritesManager::GetInstance();
			pSpritesManager->DeleteSprite(e->EnemySpriteAnimation);
			pSpritesManager->DeleteSprite(e->smallfishAnimation);
			pSpritesManager->DeleteSprite(e->youguanAnimation);
			pSpritesManager->DeleteSprite(e->boomAnimation);
			pSpritesManager->DeleteSprite(e->daodanAnimation);
	Ship->SetVisible(false);
	Boat->SetVisible(false);
	pSpritesManager->DeleteSprite(Ship);
	pSpritesManager->DeleteSprite(Boat);
			return;
		}
		else
			++iter;
	}

}
void CEnemyGame::Clean()
{
	int i = 0;
	CSpritesManager *pSpritesManager = CSpritesManager::GetInstance();
	for (CMushroomSprite *m : MushroomSprite)
	{
		m->MushroomSprite->SetVisible(false);
		m->BloodSpriteBackground->SetVisible(false);
		m->BloodSprite->SetVisible(false);
		pSpritesManager->DeleteSprite(m->MushroomSprite);
		pSpritesManager->DeleteSprite(m->BloodSpriteBackground);
		pSpritesManager->DeleteSprite(m->BloodSprite);
	
	}
	for (CEnemySprite *e : EnemySpriteList)
	{
		e->EnemySpriteAnimation->SetVisible(false);
		pSpritesManager->DeleteSprite(e->EnemySpriteAnimation);
		pSpritesManager->DeleteSprite(e->smallfishAnimation);
		pSpritesManager->DeleteSprite(e->youguanAnimation);
		pSpritesManager->DeleteSprite(e->boomAnimation);
		pSpritesManager->DeleteSprite(e->daodanAnimation);
	}
	MushroomSprite.clear();
	EnemySpriteList.clear();
	Ship->SetVisible(false);
	Boat->SetVisible(false);
	pSpritesManager->DeleteSprite(Ship);
	pSpritesManager->DeleteSprite(Boat);
}
void CEnemyGame::HandleKeyDown(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 'A':
		walkleft = true;

		break;
	case 'W':
		walkup = true;
		break;
	case 'S':
		walkdown = true;
		break;
	case 'D':
		walkright = true;
		break;
	}
}
void CEnemyGame::HandleKeyUp(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 'A':
		walkleft = false;
		break;
	case 'W':
		walkup = false;
		break;
	case 'S':
		walkdown = false;
		break;
	case 'D':
		walkright = false;
		break;
	}
}