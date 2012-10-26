/*******************************************
*CGameSprite by Pureku(Gigon Bae)
*Date:2005.12.22
*
********************************************/
#pragma once

struct IDirect3DDevice9;
struct IDirect3DSurface9;
class CGameSprite
{
public:
	enum KIND{SURFACE=0,SPRITE=1};
	CGameSprite(void);
	virtual ~CGameSprite(void);
	virtual void draw(IDirect3DDevice9* pd3dDevice,int x,int y, IDirect3DSurface9* backbuffer){};
	virtual void draw(IDirect3DDevice9* pd3dDevice,int x,int y, D3DCOLOR ,RECT* =0){};
	virtual void release(){};
	
	static CGameSprite* getInstance(WCHAR *fileName,int kind);


};
