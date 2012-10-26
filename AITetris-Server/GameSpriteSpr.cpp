/*
 Copyright (c) 2005-2012 Gigon Bae

 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE, DAMMIT.
*/
#include "dxstdafx.h"
#include ".\gamespritespr.h"
#include "SpriteCollection.h"
#include "Game.h"

extern CGame game;
CGameSpriteSpr::CGameSpriteSpr(void)
{
	sprite=NULL;	
}

CGameSpriteSpr::~CGameSpriteSpr(void)
{
	
}
CGameSpriteSpr::CGameSpriteSpr(WCHAR *fileName)
{
	IDirect3DDevice9* pd3dDevice=DXUTGetD3DDevice();
	sprite=CSpriteCollection::getSpriteFromBitmap(pd3dDevice,fileName,&info);	
	
}
void CGameSpriteSpr::release()
{
	SAFE_RELEASE(sprite);	
}
void CGameSpriteSpr::draw(IDirect3DDevice9* pd3dDevice,int x,int y, D3DCOLOR color ,RECT *rect /*=0*/)
{
	RECT tempRect;
	D3DXVECTOR3 vPos( x,y, 0.0f );
	if(rect==NULL)
	{
		SetRect(&tempRect,0,0,info.Width,info.Height);
		game.M_DXSprite->Draw(sprite,&tempRect,NULL,&vPos,color);
	}
	else
	{
		game.M_DXSprite->Draw(sprite,rect,NULL,&vPos,color);
	}

}