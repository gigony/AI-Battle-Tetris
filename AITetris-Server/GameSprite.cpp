#include "dxstdafx.h"
#include ".\gamesprite.h"
#include "GameSpriteSpr.h"
#include "GameSpriteSurface.h"

CGameSprite::CGameSprite(void)
{	
}

CGameSprite::~CGameSprite(void)
{
}
CGameSprite* CGameSprite::getInstance(WCHAR *fileName,int kind)
{
	switch(kind)
	{
	case SURFACE: return (CGameSprite*)new CGameSpriteSurface(fileName);break;
	case SPRITE: return (CGameSprite*)new CGameSpriteSpr(fileName);break;
	}
	return NULL;
}

