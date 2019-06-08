#include "Framework\Game.h"
//#include "Framework\EnemySprite.h"
#include <cmath>

#pragma once

struct CMushroomSprite
{
	CAnimationSprite *MushroomSprite = NULL;
	CSprite *BloodSprite = NULL;
	CSprite *BloodSpriteBackground = NULL;
	int Blood = 0;
	int NowBlood = 0;
	bool bIsVisible = true;
	D2D1_POINT_2F Scale;
};

struct CEnemySprite
{
	CAnimationSprite *EnemySpriteAnimation = NULL;
	CAnimationSprite *daodanAnimation = NULL;
	CAnimationSprite *smallfishAnimation = NULL;
	CAnimationSprite *boomAnimation = NULL;
	CAnimationSprite *youguanAnimation = NULL;
	float Speed = 0;
	float AttackRange = 0.0f;
	bool bIsAttack = false;
	CMushroomSprite *Mushroom = NULL;
};


class CEnemyGame
{
public:
	CEnemyGame();
	~CEnemyGame();
	void HandleKeyUp(WPARAM wParam, LPARAM lParam);
	void HandleKeyDown(WPARAM wParam, LPARAM lParam);
	void Update(float fDeltaTime);				//更新怪物行走路线
	void InitGame();							//初始化怪物种类、蘑菇位置及血量
	void CreateEnemy(int t = 0);					//创建单个怪物
	void CreateEnemyw(int n);			//创建指定数量个怪物
	void CleanEnemy(CEnemySprite *sprite);		//清除怪物
	void moveEnemy();//运动轨迹函数
	void PlayerMover(float speed);//主角操控函数
	void Clean();
	void SetbPlaying(bool b);	//设置游戏状态？
	bool GetbPlaying();	//获得游戏状态？
	bool GetbInited();
	int GetNowMushroom();
	void closeEnemy(bool c);//结束时把怪物都关闭

	//怪物精灵
	list<CEnemySprite*> EnemySpriteList;
	list<CMushroomSprite*> MushroomSprite;
private:
	CSprite *Boat;
	CSprite *Ship;
	bool walkup = false;
	bool walkdown = false;
	bool walkleft = false;
	bool walkright = false;

	bool bPlaying = false;//游戏是否开始

	bool PosX = false;//潜艇碰撞盒
	bool PosY = false;
	bool PosX1 = false;//油罐碰撞盒
	bool PosY1 = false;
	bool PosX2 = false;//小鱼碰撞盒
	bool PosY2 = false;
	bool PosX3 = false;//导弹碰撞盒
	bool PosY3 = false;

	bool bInited = false;//人物是否产生

	float speed = 1.0f;
	float boatspeed = 1;
	float shipspeed = 4;
	int x = 1000;//平移
	int youguany = -100;//下落
	int fishy = -100;

	double fishScale = 1.0f;//小鱼减速系数
	double youguanScale = 0.5f;//油罐减速系数

	int Type = 2;//怪物总类型
	int MushroomNum = 2;//人物血量
	int NowMushroom = MushroomNum;	//当前剩余血条数
	//怪物初始随机最小最大速度
	int MinMaxSpeed[4];
	//怪物初始攻击范围
	float AttackRange[2];
	//速度变化系数
	float SpeedVar = 0.99999f;


};
