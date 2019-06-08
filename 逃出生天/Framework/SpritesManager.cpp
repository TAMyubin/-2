#include "SpritesManager.h"


CSpritesManager::~CSpritesManager(void)
{
	list<CSprite *>::iterator ite = spriteList.begin();
	while(ite != spriteList.end())
	{
		delete *ite;
		ite++;
	}
	spriteList.clear();

}
void CSpritesManager::SetRenderTarget(ID2D1HwndRenderTarget *pRenderTarget)
{
	this->pRenderTarget = pRenderTarget;
}
bool CSpritesManager::GetTextureFromResource(const wstring		sResourceName,
											//ResourceType		rt,
											ID2D1Bitmap			**ppTexture,
											int					w,
											int					h,
											int					&nWidth,
											int					&nHeight)
{
	CResourcesPool * rp = CResourcesPool::GetInstance();

	ResDesc *pResDesc = rp->GetResource(sResourceName);
	if (!pResDesc)
	{
		return false;
	}
	nWidth = w;
	nHeight = h;
	*ppTexture =pResDesc->pTexture;
	if (w == 0 || h == 0)
	{
		D2D1_SIZE_F  size = (*ppTexture)->GetSize();

		nWidth = size.width;
		nHeight = size.height;
	}
	
	return true;
}
CSprite * CSpritesManager::CreateSprite(	const wstring		sResourceName,
											//	ResourceType		rt,
											D2D1_POINT_2F		&vPos,
											int					w,
											int					h,
											int					tx,
											int					ty,
											D2D1_POINT_2F		vHotSpot
								)
{
	
	ID2D1Bitmap *pTexture;
	int nWidth;
	int nHeight;
	GetTextureFromResource(sResourceName,&pTexture,w,h,nWidth,nHeight);
	CSprite *pSprite = new CSprite(pRenderTarget,pTexture,vPos,nWidth,nHeight,tx,ty,vHotSpot);
	spriteList.push_back(pSprite);
	return pSprite;
	
}

CAnimationSprite * CSpritesManager::CreateAnimationSprite  (	const wstring		sResourceName,
																//ResourceType		rt,
																int					nFrames,
																float				nFps,
																D2D1_POINT_2F		&vPos,
																int					w,
																int					h,
																int					tx,
																int					ty,
																D2D1_POINT_2F		vHotSpot)
{
	ID2D1Bitmap *pTexture;
	int nWidth;
	int nHeight;
	GetTextureFromResource(sResourceName,&pTexture,w,h,nWidth,nHeight);

	CAnimationSprite *pAnimationSprite = new CAnimationSprite(pRenderTarget,pTexture,vPos,nFrames,nFps,w,h,tx,ty,vHotSpot);

	spriteList.push_back(pAnimationSprite);
	
	return pAnimationSprite;
}

//CEnemySprite * CSpritesManager::CreateEnemySprite(
//	const wstring   sResourceName,
//	//ResourceType	rt,
//	int				nFrames,
//	float			nFps,
//	D2D1_POINT_2F	&vPos,
//	int				w,
//	int				h,
//	int				tx,
//	int				ty,
//	float			speed,
//	int				blood,
//	float			attackRange,
//	float			attack,
//	int				type,
//	D2D1_POINT_2F   vHotSpot)
//{
//	ID2D1Bitmap *pTexture;
//	int nWidth;
//	int nHeight;
//	GetTextureFromResource(sResourceName, &pTexture, w, h, nWidth, nHeight);
//
//	CEnemySprite *pEnemySprite = new CEnemySprite(pRenderTarget, pTexture, vPos, nFrames, nFps, w, h, tx, ty, speed, blood,attackRange,attack, type, vHotSpot);
//
//	spriteList.push_back(pEnemySprite);
//
//	return pEnemySprite;
//}

void CSpritesManager::DeleteSprite(CSprite *pSprite)
{
	//delete pSprite;
	//spriteList.remove(pSprite);
	pSprite->bEnabled=pSprite->bVisible = false;
	toBeDeleted.push_back(pSprite);

	/*list<CSprite*>::iterator ite;
	for (ite = spriteList.begin(); ite != spriteList.end(); ite++)
	{
		if ((*ite) = pSprite)
		{
			spriteList.remove((*ite));
			break;
		}
	}*/
}

void CSpritesManager::Render()
{
	for(list<CSprite*>::iterator ite = spriteList.begin();ite != spriteList.end();ite++)
	{
		if ((*ite)->IsVisible())
		{
			(*ite)->Render();
		}
	}
}

void CSpritesManager::Update(float fDeltaTime)
{
	for(int i=0;i<toBeDeleted.size();++i)
	{
 		delete toBeDeleted[i];
		spriteList.remove(toBeDeleted[i]);
	}
	if (toBeDeleted.size() > 0)
	{
		toBeDeleted.clear();
	}
	for(list<CSprite*>::iterator ite = spriteList.begin();ite != spriteList.end();ite++)
	{
		if ((*ite)->bEnabled )
		{
			(*ite)->Update(fDeltaTime);
		}
	}
}

void CSpritesManager::Release()
{
	for(list<CSprite*>::iterator ite = spriteList.begin();ite != spriteList.end();ite++)
	{
		(*ite)->Release();
	}
}

CSprite *CSpritesManager::Clone(CSprite *pSprite)
{
	CSprite *pSpr = pSprite->Clone();
	spriteList.push_back(pSpr);
	return pSpr;
}
CAnimationSprite *CSpritesManager::Clone(CAnimationSprite *pAniSprite)
{
	CAnimationSprite *pAniSpr = pAniSprite->Clone();
	spriteList.push_back(pAniSpr);
	return pAniSpr;
}
//CEnemySprite *CSpritesManager::Clone(CEnemySprite *pEnemySprite)
//{
//	CEnemySprite *pEnemySpr = pEnemySprite->Clone();
//	spriteList.push_back(pEnemySpr);
//	return pEnemySpr;
//}
CSprite *CSpritesManager::ReZOrder(CSprite *pSprite)
{
	//重新插入该精灵
	if (pSprite == NULL)
	{
		return pSprite;
	}

	//先删除精灵
	for (list<CSprite*>::iterator ite = spriteList.begin(); ite != spriteList.end(); ite++)
	{
		if ((*ite) == pSprite)
		{
			spriteList.remove(pSprite);
			break;
		}
	}

	pSprite->bEnabled = pSprite->bVisible = true;
	int i = 0;
	//遍历，根据Z-Order找到合适的插入位置
	for (list<CSprite*>::iterator ite = spriteList.begin(); ite != spriteList.end(); ite++)
	{
		if ((*ite)->GetZOrder() > pSprite->GetZOrder())
		{
			spriteList.insert(ite--, pSprite);
			i = 1;
			break;
		}
	}
	if (i == 0)
		spriteList.push_back(pSprite);

	return pSprite;
}