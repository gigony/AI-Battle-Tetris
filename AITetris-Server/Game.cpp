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
#include <dmusici.h>
#include ".\game.h"
#include ".\board.h"

CGame::CGame(void)
{
	
	M_DXSprite=NULL;
	M_DXLine=NULL;
	CBoard::init();  //Board static변수 초기화
/*	m_surTitleBackground=NULL;
	m_surTitle=0;						//타이틀 
	m_surTitleMain1=0;				//타이틀 메인화면1
	m_surTitleMain2=0;				//타이틀 메인화면2*/
}

CGame::~CGame(void)
{
}


HRESULT CGame::OnCreateDevice(IDirect3DDevice9* pd3dDevice)
{
	HRESULT hr;
	hr = D3DXCreateSprite( pd3dDevice, &M_DXSprite );	
	if( FAILED(hr) )
		return DXUT_ERR( L"D3DXCreateSprite", hr );	     
	hr = D3DXCreateLine(pd3dDevice,&M_DXLine);
	if( FAILED(hr) )
		return DXUT_ERR( L"D3DXCreateSprite", hr );	 
	return S_OK;

}
void CGame::OnResetDevice(IDirect3DDevice9* pd3dDevice)
{
	//스프라이트 추가
	sprCollection.deleteAllSprite();
	sprCollection.insert(L".\\data\\title.png",SPRITE);           //0
	sprCollection.insert(L".\\data\\Title-Name.png",SPRITE);		//1
	sprCollection.insert(L".\\data\\Title-Selected.png",SPRITE);	//2
	sprCollection.insert(L".\\data\\Title-unselected.png",SPRITE);	//3
	sprCollection.insert(L".\\data\\Main-BackGround.png",SPRITE);	//4									
	sprCollection.insert(L".\\data\\mino_main.png",SPRITE);			//5
	sprCollection.insert(L".\\data\\mino_next_16.png",SPRITE);		//6
	sprCollection.insert(L".\\data\\Main-Effect.png",SPRITE);		//7
	sprCollection.insert(L".\\data\\WinLose.png",SPRITE);		//8
	

	
	effectCollection.collection.clear();
	CEffectCollection::stFrame frame;
	CEffectCollection::stAnimation animation;
	

	//라인 지워졌을때 animation 추가
	frame.relateX=frame.relateY=0;
	frame.alpha=255;
	int x,y,i,j;
	for(y=0;y<6;y++)
	{
		for(x=0;x<4;x++)
		{
			if(y==5 && x>=2) break;			//12개까지만 삽입
			SetRect(&frame.rect,x*64,y*60,(x+1)*64,(y+1)*60);animation.frame.push_back(frame); 
		}                                                                                            
	}
	animation.spriteNum=SPR_MAIN_EFFECT;
	animation.duration=0.2;
	animation.size=12;
	effectCollection.collection.push_back(animation);  //ANI_BOMB

	//combo animation 추가
		
	animation.spriteNum=SPR_MAIN_EFFECT;
	animation.duration=0.5;
	animation.size=30;
	for(i=0;i<17;i++)//combo2~18
	{
		animation.frame.clear();
		for(j=0;j<30;j++)
		{
			frame.relateX=0;
			frame.relateY=(-2)*j;  //올라가도록
			frame.alpha=255;			
			SetRect(&frame.rect,270,i*30,420,(i+1)*30);animation.frame.push_back(frame);
		}
		effectCollection.collection.push_back(animation);  //ANI_COMBO##
	}

	//WinLose animation 추가
		
	animation.spriteNum=SPR_MAIN_WINLOSE;
	animation.duration=6;
	animation.size=100;
	animation.frame.clear();
	x=0;y=50;
	int alpha=0;
	for(j=0;j<100;j++)
	{
		frame.relateX=0;
		frame.relateY=y;
		frame.alpha=alpha;			
		y-=10;
		alpha+=10;
		if(y<-50) y=-50;
		if(alpha>255) alpha=255;		
		SetRect(&frame.rect,0,0,512,256);animation.frame.push_back(frame);
	}
	effectCollection.collection.push_back(animation);  //ANI_1PWIN

	animation.spriteNum=SPR_MAIN_WINLOSE;
	animation.duration=6;
	animation.size=100;
	animation.frame.clear();
	x=0,y=50,alpha=0;	
	for(j=0;j<100;j++)
	{
		frame.relateX=0;
		frame.relateY=y;
		frame.alpha=alpha;			
		y-=10;
		alpha+=10;
		if(y<-50) y=-50;
		if(alpha>255) alpha=255;		
		SetRect(&frame.rect,0,256,512,512);animation.frame.push_back(frame);
	}
	effectCollection.collection.push_back(animation);  //ANI_2PWIN
	
}
void CGame::OnDestroyDevice()
{
	SAFE_RELEASE(M_DXSprite);
	SAFE_RELEASE(M_DXLine);

	sprCollection.deleteAllSprite();
}
void CGame::OnLostDevice()
{
	if( M_DXSprite )
		M_DXSprite->OnLostDevice();
	if( M_DXLine )
		M_DXLine->OnLostDevice();
	sprCollection.deleteAllSprite();


}
void CGame::OnMsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing)
{
	stateController.OnMsgProc(hWnd, uMsg,wParam,lParam, pbNoFurtherProcessing);

}
void CGame::render(IDirect3DDevice9* pd3dDevice,float fElapsedTime){

	stateController.OnRender(pd3dDevice,fElapsedTime);	
}
void CGame::process(float fElapsedTime){

	stateController.OnProcess(fElapsedTime);	
}
//***********************************************************************************
///                                        사운드
//***********************************************************************************
bool CGame::bInitializeSoundSystem( HWND hWnd )
{	
	if(!ogg.InitDirectSound(hWnd)) return(0);
	ogg.OpenOgg(".\\music\\TE_BG_title_01_Tetris2008_AR.ogg");	 //0
	ogg.OpenOgg(".\\music\\TE_BG_multi_03_togetTetris_AR.ogg");  //1
	ogg.OpenOgg(".\\music\\TE_BG_single_12_LastKiss_MR.ogg");	//2	
	ogg.OpenOgg(".\\music\\TE_SE_combo_01.ogg");                //3
	ogg.OpenOgg(".\\music\\TE_SE_combo_02.ogg");                //4
	ogg.OpenOgg(".\\music\\TE_SE_combo_03.ogg");                //5
	ogg.OpenOgg(".\\music\\TE_SE_combo_04.ogg");                //6
	ogg.OpenOgg(".\\music\\TE_SE_combo_05.ogg");                //7
	ogg.OpenOgg(".\\music\\TE_SE_combo_06.ogg");                //8
	ogg.OpenOgg(".\\music\\TE_SE_combo_07.ogg");                //9
	ogg.OpenOgg(".\\music\\TE_SE_combo_08.ogg");                //10
	ogg.OpenOgg(".\\music\\TE_SE_start.ogg");					//11
	ogg.OpenOgg(".\\music\\TE_SE_move.ogg");					//12
	ogg.OpenOgg(".\\music\\TE_SE_rotate_right.ogg");		    //13
	ogg.OpenOgg(".\\music\\TE_SE_softdrop.ogg");				//14
	ogg.OpenOgg(".\\music\\TE_SE_harddrop.ogg");				//15
	ogg.OpenOgg(".\\music\\TE_SE_hold.ogg");					//16
	ogg.OpenOgg(".\\music\\TE_SE_holdfail.ogg");				//17
	ogg.OpenOgg(".\\music\\TE_SE_single.ogg");					//18
	ogg.OpenOgg(".\\music\\TE_SE_double.ogg");					//19
	ogg.OpenOgg(".\\music\\TE_SE_triple.ogg");					//20
	ogg.OpenOgg(".\\music\\TE_SE_tetris.ogg");					//21
	ogg.OpenOgg(".\\music\\TE_SE_retire.ogg");					//22
	ogg.OpenOgg(".\\music\\TE_BG_win2.ogg");					//23
	ogg.OpenOgg(".\\music\\TE_SE_lineattacked.ogg");			//24
	ogg.OpenOgg(".\\music\\TE_BG_hurryup.ogg");					//25
	ogg.OpenOgg(".\\music\\TE_SE_button_over_01.ogg");			//26
	ogg.OpenOgg(".\\music\\TE_SE_b2btetris.ogg");				//27


	
	// Return success
	return( 1 );
}
void CGame::vCleanup( void )
{
	ogg.Close();
}



