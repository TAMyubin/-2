#pragma once
#include "AnimationSprite.h"
#include "Game.h"

class CEnemySprite 
{
public:
	CEnemySprite(
		CAnimationSprite	*enemySprite,
		CSprite				*bloodGroundSprite,
		CSprite				*bloodSprite,
		float				speed = 0.0f,
		int					blood = 0,
		float				attackRange = 0.0f,
		float				attack = 0.0f,
		int					type = 0
		);
	~CEnemySprite();
	//血条
	void SetBloodSprite(CSprite *sprite);
	CSprite *GetBloodSprite();
	//怪物速度
	void SetSpeed(float speed);
	float GetSpeed();
	//怪物血量
	void SetBlood(int blood);
	int GetBlood();
	//攻击范围
	void SetAttackRange(float attackRange);
	float GetAttackRange();
	//攻击强度
	void SetAttack(float attack);
	float GetAttack();
	//怪物类型
	void SetType(int type);
	int GetType();
	//是否攻击ing
	void SetbIsAttack(bool b);
	bool GetbIsAttack();
	//目标
	void SetSprite(CSprite *sprite, int n);
	CSprite* GetSprite();
	int GetSpriteNum();
	//缩放
	void SetScale(D2D1_POINT_2F scale);
	D2D1_POINT_2F GetScale();
	CEnemySprite *Clone();

private:
	CAnimationSprite *EnemySprite = NULL;
	CSprite *BloodSprite = NULL;
	CSprite *BloodSpriteBackground = NULL;
	float Speed = 0;
	int Blood = 0;
	float AttackRange = 0.0f;
	float Attack = 0.0f;
	int Type = 0;
	bool bIsAttack = false;
	D2D1_POINT_2F Scale;
	CSprite *Mushroom = NULL;
	int MushroomNum = 0;
};

