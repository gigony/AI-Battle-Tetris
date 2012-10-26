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
#include ".\effectcollection.h"
#include "Game.h"
extern CGame game;
CEffectCollection::CEffectCollection(void)
{
}

CEffectCollection::~CEffectCollection(void)
{
}

void CEffectCollection::insertEffect(int animationNum,int x,int y, double nowTime)
{
	stEffectDat effect={x,y,nowTime,collection[animationNum]};
	effectList.push_back(effect);

}
void CEffectCollection::drawEffect(IDirect3DDevice9* pd3dDevice,double nowTime)
{

	int i;
	CEffectCollection::stAnimation animation;
	std::list<stEffectDat>::iterator iter;
	if(effectList.empty()) return;	

	int x,y,size;	
	int alpha;	
	int targetFrame;
	int spriteNum;
	double startTime;
	double duration;

	for(iter=effectList.begin();iter!=effectList.end();)
	{	
		animation=((stEffectDat)(*iter)).animation;
		x=((stEffectDat)(*iter)).x;
		y=((stEffectDat)(*iter)).y;		
		startTime=((stEffectDat)(*iter)).startTime;

		spriteNum=animation.spriteNum;
		duration=animation.duration;
		size=animation.size;
		if(nowTime-startTime>=duration)				//½Ã°£ÀÌ Áö³µ´Ù¸é ¾ø¾Ø´Ù.
		{
			effectList.erase(iter++);			
			continue;
		}

		targetFrame=(int)((nowTime-startTime)/duration*size);		
		x+=animation.frame[targetFrame].relateX;
		y+=animation.frame[targetFrame].relateY;
		alpha=animation.frame[targetFrame].alpha;
		
		game.sprCollection.collection[spriteNum]->draw(pd3dDevice,x,y,D3DCOLOR_ARGB(alpha, 255, 255, 255),&animation.frame[targetFrame].rect);		

		++iter;
		
	}

}