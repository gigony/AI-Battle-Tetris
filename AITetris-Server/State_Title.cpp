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
#include "state_title.h"
#include "State_CustomSetting.h"
#include "Game.h"
#include "definition.h"
#include "State_Main.h"


extern CGame game;

CState* CState_Title::_instance=NULL;

RECT CState_Title::region[4]={{313,274,677,358},{327,375,327+338,375+71},{276,465,276+450,465+65},{391,555,391+206,555+63}};

CState_Title::CState_Title(void)
{
	selectedItem=0;
	isPlayingMusic=false;
}

CState_Title::~CState_Title(void)
{	

}

CState* CState_Title::instance()
{
	if(_instance==NULL)
	{
		_instance=new CState_Title;
	}
	return _instance;

}
void CState_Title::finalize()
{
	if(_instance!=NULL)
	{
		delete  _instance;
		_instance=NULL;
	}
}


void CState_Title::OnMsgProc(CStateController *controller,HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing)
{
	int x,y;
	int i;
	int nowOn=-1;
	x = LOWORD(lParam); 
	y=HIWORD(lParam);
	for(i=0;i<4;i++)
	{
		if(x>region[i].left && x<region[i].right && y>region[i].top && y<region[i].bottom)
		{
			nowOn=i;
			break;
		}
	}

	
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		if(nowOn!=-1 && nowOn!=selectedItem)
		{
			game.ogg.Play(OGG_SELECT);			
		}
		selectedItem=nowOn;
		
		break;
	case WM_LBUTTONDOWN:
		switch(nowOn)
		{
		case 0:game.ogg.Stop(OGG_TITLE);
			   ((CState_Main*)CState_Main::instance())->setPSInstance(0,NULL);		
			   ((CState_Main*)CState_Main::instance())->setPSInstance(1,NULL);		
			   isPlayingMusic=false;
			   ((CState_Main*)CState_Main::instance())->setPlayMode(CState_Main::SOLOMODE);				//1인용
			   game.ogg.Play(OGG_START);
			   changeState(controller,CState_Main::instance());
			   break;
		case 1: game.ogg.Stop(OGG_TITLE);
			   ((CState_Main*)CState_Main::instance())->setPSInstance(0,NULL);		
			   ((CState_Main*)CState_Main::instance())->setPSInstance(1,NULL);		
				isPlayingMusic=false;
				 ((CState_Main*)CState_Main::instance())->setPlayMode(CState_Main::COUPLEMODE);					//2인용
				game.ogg.Play(OGG_START);
				changeState(controller,CState_Main::instance());
				
			break;
		case 2:
			((CState_Main*)CState_Main::instance())->setPSInstance(0,NULL);		
			((CState_Main*)CState_Main::instance())->setPSInstance(1,NULL);
			game.ogg.Stop(OGG_TITLE);
			isPlayingMusic=false;			
			((CState_CustomSetting*)CState_CustomSetting::instance())->SetPlayer1(1);//HUNAN으로 세팅
			((CState_CustomSetting*)CState_CustomSetting::instance())->SetPlayer2(1);//HUNAM으로 세팅
			if( ((CState_CustomSetting*)CState_CustomSetting::instance())->IsInitDialog()) //start버튼 활성화
			    ((CState_CustomSetting*)CState_CustomSetting::instance())->GetUIDialog()->GetButton(8)->SetEnabled(true);

			//기존 네트워크 종료
			PSFinalize();
			//네트워크 프레임웍 초기화
			PSInitialize();

			changeState(controller,CState_CustomSetting::instance());

			
			break;
		case 3:PostQuitMessage(OGG_TITLE);break;

		}

	}
}
void CState_Title::OnRender(CStateController *controller,IDirect3DDevice9* pd3dDevice,float fElapsedTime)
{	
	if(isPlayingMusic==false)
	{
		game.ogg.Play(0,true);
		isPlayingMusic=true;
	}
	else game.ogg.IsPlaying(0);

	game.M_DXSprite->Begin(D3DXSPRITE_ALPHABLEND);
	game.sprCollection.collection[SPR_TITLE]->draw(pd3dDevice,0,0,D3DCOLOR_ARGB(255, 255, 255, 255),NULL);
	for(int i=0;i<4;i++)
	{
		if(selectedItem==i)		
			game.sprCollection.collection[SPR_TITLE_SELECTED]->draw(pd3dDevice,region[i].left,region[i].top,D3DCOLOR_ARGB(255, 255, 255, 255),&region[i]);		
		else
			game.sprCollection.collection[SPR_TITLE_UNSELECTED]->draw(pd3dDevice,region[i].left,region[i].top,D3DCOLOR_ARGB(255, 255, 255, 255),&region[i]);
	}
	RECT titleRect={0,0,500,167};
	
	game.sprCollection.collection[SPR_TITLE_NAME]->draw(pd3dDevice,280,50,D3DCOLOR_ARGB(255, 255, 255, 255),&titleRect);

	game.M_DXSprite->End();	

	/* 블록 그리기
	for(int i=0;i<7;i++)
	{
		for(int t=0;t<4;t++)
			for(int j=0;j<4;j++)			
				for(int k=0;k<4;k++)
				{					
					int value=CBoard::blockData[i+1][t][j*4+k];
					
					if(value==0)
					{	
						{
							RECT rect={8*22,0,8*22+22,22};
							game.sprCollection.collection[SPR_MAIN_BLOCK]->draw(pd3dDevice,300+(22*4+5)*t+ 22*k,(22*4+10)*i+ 22*j,D3DCOLOR_ARGB(255, 255, 255, 255),&rect);		
						}
					}
					else
					{
						{
							RECT rect={(i+1)*22,0,(i+1)*22+22,22};
							game.sprCollection.collection[SPR_MAIN_BLOCK]->draw(pd3dDevice,300+(22*4+5)*t+ 22*k,(22*4+10)*i+ 22*j,D3DCOLOR_ARGB(255, 255, 255, 255),&rect);		
						}
					}
				}
	}
	*/
}
void CState_Title::OnProcess(CStateController *controller,float fElapsedTime)
{
}