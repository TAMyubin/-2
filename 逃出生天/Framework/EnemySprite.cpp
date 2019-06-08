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

//Ѫ��
void CEnemySprite::SetBloodSprite(CSprite *sprite)
{
	BloodSprite = sprite;
}

CSprite *CEnemySprite::GetBloodSprite()
{
	return BloodSprite;
}

//�����ٶ�
void CEnemySprite::SetSpeed(float speed)
{
	Speed = speed;
}

float CEnemySprite::GetSpeed()
{
	return Speed;
}

//����Ѫ��
void CEnemySprite::SetBlood(int blood)
{
	Blood = blood;
}

int CEnemySprite::GetBlood()
{
	return Blood;
}

//��������
void CEnemySprite::SetType(int type)
{
	Type = type;
}

int CEnemySprite::GetType()
{
	return Type;
}

//�����Ƿ���빥��״̬
void CEnemySprite::SetbIsAttack(bool b)
{
	bIsAttack = b;
}

bool CEnemySprite::GetbIsAttack()
{
	return bIsAttack;
}

//������ǰ���ľ���
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

//���﹥����Χ
void CEnemySprite::SetAttackRange(float attackRange)
{
	AttackRange = attackRange;
}

float CEnemySprite::GetAttackRange()
{
	return AttackRange;
}

//���﹥��ǿ��
void CEnemySprite::SetAttack(float attack)
{
	Attack = attack;
}

float CEnemySprite::GetAttack()
{
	return Attack;
}

//����
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
