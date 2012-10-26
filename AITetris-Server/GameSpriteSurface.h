/*******************************************
*CGameSpriteSurface by Pureku(Gigon Bae)
*Date:2005.12.22
*
********************************************/
#pragma once
#include "gamesprite.h"

struct IDirect3DDevice9;
struct IDirect3DSurface9;
class CGameSpriteSurface :
	public CGameSprite
{
public:
	CGameSpriteSurface(void);
	CGameSpriteSurface(WCHAR *filename);	
	~CGameSpriteSurface(void);

	void draw(IDirect3DDevice9* pd3dDevice,int x,int y, IDirect3DSurface9* backbuffer);
	void draw(IDirect3DDevice9* pd3dDevice,int x,int y, D3DCOLOR color,RECT* rect= 0){};
	void release();
	

private:
	D3DXIMAGE_INFO info;
	IDirect3DSurface9* sprite;
};
