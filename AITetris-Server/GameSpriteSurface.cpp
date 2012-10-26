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
#include ".\gamespritesurface.h"
#include "SpriteCollection.h"
CGameSpriteSurface::CGameSpriteSurface(void)
{
	sprite=NULL;
	
}
CGameSpriteSurface::CGameSpriteSurface(WCHAR *filename)
{
	
	
	IDirect3DDevice9* pd3dDevice=DXUTGetD3DDevice();
	sprite=CSpriteCollection::getSurfaceFromBitmap(pd3dDevice,filename,&info);	
}

CGameSpriteSurface::~CGameSpriteSurface(void)
{
}


void CGameSpriteSurface::draw(IDirect3DDevice9* pd3dDevice,int x,int y, IDirect3DSurface9* backbuffer)
{
	RECT rect;
	SetRect(&rect,0,0,info.Width,info.Height);
	pd3dDevice->StretchRect(sprite,
		&rect,
		backbuffer,
		&rect,
		D3DTEXF_NONE);		
}
void CGameSpriteSurface::release()
{
	SAFE_RELEASE(sprite);
}