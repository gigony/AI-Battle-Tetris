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
#include ".\spritecollection.h"
#include "GameSprite.h"
#include "GameSpritesurface.h"
CSpriteCollection::CSpriteCollection(void)
{
}

CSpriteCollection::~CSpriteCollection(void)
{
}

void CSpriteCollection::insert(WCHAR *fileName,int kind)
{
	CGameSprite *item=CGameSprite::getInstance(fileName,kind);
	collection.push_back(item);
}
void CSpriteCollection::deleteAllSprite()
{
	CGameSprite *temp;
	
	
	
	for(int i=collection.size()-1;i>=0;i--)
	{
		temp=collection.back();
		temp->release();
		collection.pop_back(); 	// 마지막 엘리먼트를 삭제
		delete temp;
	}
}
//비트맵으로부터 택스쳐를 읽는다. dx예제를 이용.
IDirect3DTexture9* CSpriteCollection::getSpriteFromBitmap(IDirect3DDevice9* pd3dDevice, WCHAR* filename,D3DXIMAGE_INFO *imageInfo)
{

		
	HRESULT hResult;	

	IDirect3DTexture9 *pTex=NULL;
	D3DXIMAGE_INFO d3dxImageInfo;
	// Get the width and height info from this bitmap
	hResult = D3DXGetImageInfoFromFile(filename, imageInfo);
	// Make sure that the call to D3DXGetImageInfoFromFile succeeded
	if FAILED (hResult)
		return NULL;
	hResult=D3DXCreateTextureFromFileEx( pd3dDevice,
		filename,
		D3DX_DEFAULT_NONPOW2,//imageInfo->Width, // I had to set width manually. D3DPOOL_DEFAULT works for textures but causes problems for D3DXSPRITE.
		D3DX_DEFAULT_NONPOW2,//imageInfo->Height, // I had to set height manually. D3DPOOL_DEFAULT works for textures but causes problems for D3DXSPRITE.
		1,   // Don't create mip-maps when you plan on using D3DXSPRITE. It throws off the pixel math for sprite animation.
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0XFF000000,                                 
		&d3dxImageInfo,
		NULL,
		&pTex);

	//hResult=D3DXCreateTextureFromFile(pd3dDevice,filename,pTex);

	if ( FAILED( hResult ) )

		return NULL;	
	return pTex;	
}
//비트맵으로부터 surface를 생성한다. dx예제를 이용.
IDirect3DSurface9* CSpriteCollection::getSurfaceFromBitmap(IDirect3DDevice9* pd3dDevice, WCHAR* filename,D3DXIMAGE_INFO *imageInfo)
{
	HRESULT hResult;
	IDirect3DSurface9* surface = NULL;
	//D3DXIMAGE_INFO imageInfo; // holds details concerning this bitmap
	// Get the width and height info from this bitmap
	hResult = D3DXGetImageInfoFromFile(filename, imageInfo);

	// Make sure that the call to D3DXGetImageInfoFromFile succeeded

	if FAILED (hResult)
		return NULL;
	// Create the offscreen surface that will hold the bitmap
	hResult = pd3dDevice->CreateOffscreenPlainSurface( imageInfo->Width,
		imageInfo->Height,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&surface,
		NULL );
	// Make sure that this function call did not fail; if it did,
	// exit this function
	if ( FAILED( hResult ) )
		return NULL;
	// Load the bitmap into the surface that was created earlier
	hResult = D3DXLoadSurfaceFromFile( surface,
		NULL,
		NULL,
		filename,
		NULL,
		D3DX_DEFAULT,
		0,
		NULL );
	if ( FAILED( hResult ) )
		return NULL;	
	return surface;
}