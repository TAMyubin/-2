#pragma once

/***************************************************************************
	精灵节点管理类，将精灵用链表串起来
***************************************************************************/
#include "sprite.h"
#include "ResourcesPool.h"
#include "AnimationSprite.h"
//#include "EnemySprite.h"
#include <list>
#include <vector>
#include <string>
using namespace std;
#define MAX_SPRITES	1000
class CSpritesManager
{
private:
	CSpritesManager()
	{
		pRenderTarget = NULL;
	}
	CSpritesManager(const CSpritesManager&){}
	CSpritesManager &operator =(const CSpritesManager&){}
	static CSpritesManager *instance;
public:
	static CSpritesManager *GetInstance()
	{
		if (!instance)
		{
			instance = new CSpritesManager();
		}
		return instance;
	}
	virtual ~CSpritesManager(void);
	
public:
	void SetRenderTarget(ID2D1HwndRenderTarget *pRenderTarget);
	CSprite * CreateSprite	(
		const wstring		sResourceName,
		//ResourceType		rt,
		D2D1_POINT_2F		&vPos,
		int					w = 0,
		int					h=0,
		int					tx = 0,
		int					ty = 0,
		D2D1_POINT_2F		vHotSpot=D2D1::Point2F(0,0)
		);		//创建精灵节点
	CAnimationSprite * CreateAnimationSprite   (
		const wstring   sResourceName,
		//ResourceType	rt,
		int				nFrames,
		float			nFps,
		D2D1_POINT_2F	&vPos,
		int				w,
		int				h,
		int				tx = 0,
		int				ty = 0,
		D2D1_POINT_2F   vHotSpot=D2D1::Point2F(0,0)
		);//创建动画精灵节点
	//CEnemySprite * CreateEnemySprite(
	//	const wstring   sResourceName,
	//	//ResourceType	rt,
	//	int				nFrames,
	//	float			nFps,
	//	D2D1_POINT_2F	&vPos,
	//	int				w,
	//	int				h,
	//	int				tx = 0,
	//	int				ty = 0,
	//	float			speed = 0,
	//	int				blood = 0,
	//	float			attackRange = 0.0f,
	//	float			attack = 0.0f,
	//	int				type = 0,
	//	D2D1_POINT_2F   vHotSpot = D2D1::Point2F(0, 0)
	//	);//创建怪物动画精灵节点
	void DeleteSprite(CSprite *pSpriteNode);	//删除精灵节点				
	void Render();									//渲染	
	void Release();
	void Update(float fDeltaTime);
	
	CSprite *Clone(CSprite *pSprite);
	CAnimationSprite *Clone(CAnimationSprite *pAniSprite); 
	//CEnemySprite *Clone(CEnemySprite *pEnemySprite);
	CSprite *ReZOrder(CSprite *pSprite);					//根据Z-Order重排此精灵
protected:
	bool GetTextureFromResource(const wstring		sResourceName,
								//ResourceType		rt,
								ID2D1Bitmap			**ppTexture,
								int					w,
								int					h,
								int					&nWidth,
								int					&nHeight);

		
	ID2D1HwndRenderTarget       *pRenderTarget; 
	list<CSprite*>				spriteList;
	vector<CSprite*>			toBeDeleted;
	CSprite	*pSprites[MAX_SPRITES];							//数组容器
	/*CAnimationSprite *pAnimationSprite[MAX_SPRITES];*/
	int nZOrder;	//下一个插入精灵Z-Order			
	int nSize;		//下一个插入精灵在数组中的下标
};


