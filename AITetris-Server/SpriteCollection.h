/*******************************************
*CSpriteCollection by Pureku(Gigon Bae)
*Date:2005.12.22
*
********************************************/
#pragma once
#include <vector>
#include "GameSprite.h"



class CSpriteCollection
{
public:
	enum KIND{SURFACE=0,SPRITE=1};
	CSpriteCollection(void);
	~CSpriteCollection(void);
	std::vector<CGameSprite*> collection;

	void insert(WCHAR* fileName,int kind);
	void deleteAllSprite();

	static IDirect3DSurface9* getSurfaceFromBitmap(IDirect3DDevice9* pd3dDevice, WCHAR* filename,D3DXIMAGE_INFO *imageInfo);
	static IDirect3DTexture9* getSpriteFromBitmap(IDirect3DDevice9* pd3dDevice, WCHAR* filename,D3DXIMAGE_INFO *imageInfo);
};
