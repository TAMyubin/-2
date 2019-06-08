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
	//Ѫ��
	void SetBloodSprite(CSprite *sprite);
	CSprite *GetBloodSprite();
	//�����ٶ�
	void SetSpeed(float speed);
	float GetSpeed();
	//����Ѫ��
	void SetBlood(int blood);
	int GetBlood();
	//������Χ
	void SetAttackRange(float attackRange);
	float GetAttackRange();
	//����ǿ��
	void SetAttack(float attack);
	float GetAttack();
	//��������
	void SetType(int type);
	int GetType();
	//�Ƿ񹥻�ing
	void SetbIsAttack(bool b);
	bool GetbIsAttack();
	//Ŀ��
	void SetSprite(CSprite *sprite, int n);
	CSprite* GetSprite();
	int GetSpriteNum();
	//����
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

