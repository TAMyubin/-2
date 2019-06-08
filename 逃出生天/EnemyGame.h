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
	void Update(float fDeltaTime);				//���¹�������·��
	void InitGame();							//��ʼ���������ࡢĢ��λ�ü�Ѫ��
	void CreateEnemy(int t = 0);					//������������
	void CreateEnemyw(int n);			//����ָ������������
	void CleanEnemy(CEnemySprite *sprite);		//�������
	void moveEnemy();//�˶��켣����
	void PlayerMover(float speed);//���ǲٿغ���
	void Clean();
	void SetbPlaying(bool b);	//������Ϸ״̬��
	bool GetbPlaying();	//�����Ϸ״̬��
	bool GetbInited();
	int GetNowMushroom();
	void closeEnemy(bool c);//����ʱ�ѹ��ﶼ�ر�

	//���ﾫ��
	list<CEnemySprite*> EnemySpriteList;
	list<CMushroomSprite*> MushroomSprite;
private:
	CSprite *Boat;
	CSprite *Ship;
	bool walkup = false;
	bool walkdown = false;
	bool walkleft = false;
	bool walkright = false;

	bool bPlaying = false;//��Ϸ�Ƿ�ʼ

	bool PosX = false;//Ǳͧ��ײ��
	bool PosY = false;
	bool PosX1 = false;//�͹���ײ��
	bool PosY1 = false;
	bool PosX2 = false;//С����ײ��
	bool PosY2 = false;
	bool PosX3 = false;//������ײ��
	bool PosY3 = false;

	bool bInited = false;//�����Ƿ����

	float speed = 1.0f;
	float boatspeed = 1;
	float shipspeed = 4;
	int x = 1000;//ƽ��
	int youguany = -100;//����
	int fishy = -100;

	double fishScale = 1.0f;//С�����ϵ��
	double youguanScale = 0.5f;//�͹޼���ϵ��

	int Type = 2;//����������
	int MushroomNum = 2;//����Ѫ��
	int NowMushroom = MushroomNum;	//��ǰʣ��Ѫ����
	//�����ʼ�����С����ٶ�
	int MinMaxSpeed[4];
	//�����ʼ������Χ
	float AttackRange[2];
	//�ٶȱ仯ϵ��
	float SpeedVar = 0.99999f;


};
