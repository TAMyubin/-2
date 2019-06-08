#include "EnemySprite.h"


CEnemySprite::CEnemySprite(
	CAnimationSprite	*enemySprite,
	CSprite				*bloodGroundSprite,
	CSprite				*bloodSprite,
	float				speed,
	int					blood,
	float				attackRange,
	float				attack,
	int					type 
	)
{
	EnemySprite = enemySprite;
	BloodSprite = bloodSprite;
	BloodSpriteBackground = bloodGroundSprite;
	Speed = speed;
	Blood = blood;
	AttackRange = attackRange;
	Attack = attack;
	Type = type;
}


CEnemySprite::~CEnemySprite()
{
	CSpritesManager *pSpritesManager = CSpritesManager::GetInstance();
	pSpritesManager->DeleteSprite(EnemySprite);
	pSpritesManager->DeleteSprite(BloodSprite);
	pSpritesManager->DeleteSprite(BloodSpriteBackground);
}

//血条
void CEnemySprite::SetBloodSprite(CSprite *sprite)
{
	BloodSprite = sprite;
}

CSprite *CEnemySprite::GetBloodSprite()
{
	return BloodSprite;
}

//怪物速度
void CEnemySprite::SetSpeed(float speed)
{
	Speed = speed;
}

float CEnemySprite::GetSpeed()
{
	return Speed;
}

//怪物血量
void CEnemySprite::SetBlood(int blood)
{
	Blood = blood;
}

int CEnemySprite::GetBlood()
{
	return Blood;
}

//怪物种类
void CEnemySprite::SetType(int type)
{
	Type = type;
}

int CEnemySprite::GetType()
{
	return Type;
}

//怪物是否进入攻击状态
void CEnemySprite::SetbIsAttack(bool b)
{
	bIsAttack = b;
}

bool CEnemySprite::GetbIsAttack()
{
	return bIsAttack;
}

//攻击或前进的精灵
void CEnemySprite::SetSprite(CSprite *sprite,int n)
{
	Mushroom = sprite;
	MushroomNum = n;
}

CSprite* CEnemySprite::GetSprite()
{
	return Mushroom;
}

int CEnemySprite::GetSpriteNum()
{
	return MushroomNum;
}

//怪物攻击范围
void CEnemySprite::SetAttackRange(float attackRange)
{
	AttackRange = attackRange;
}

float CEnemySprite::GetAttackRange()
{
	return AttackRange;
}

//怪物攻击强度
void CEnemySprite::SetAttack(float attack)
{
	Attack = attack;
}

float CEnemySprite::GetAttack()
{
	return Attack;
}

//缩放
void CEnemySprite::SetScale(D2D1_POINT_2F scale)
{
	Scale = scale;
	CSprite::SetScaleFactor(Scale.x, Scale.y);
}

D2D1_POINT_2F CEnemySprite::GetScale()
{
	return Scale;
}

CEnemySprite *CEnemySprite::Clone()
{
	CEnemySprite *pEnemySpr = new CEnemySprite(pRenderTarget, pTexture, vPos, nFrames, nFps, width, height, tx, ty, Speed, Blood,AttackRange,Attack,Type, vHotSpot, wsName);
	if (bPlaying)
	{
		pEnemySpr->Play();
	}
	return pEnemySpr;
}
