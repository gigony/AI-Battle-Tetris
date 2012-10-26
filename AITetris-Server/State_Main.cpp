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
#include ".\state_main.h"
#include "Game.h"
#include "definition.h"
#include "state_title.h"


extern CGame game;
extern CDXUTTextHelper*		g_middleTxtHelper;

CState* CState_Main::_instance=NULL;
double CState_Main::specialEventTime[20]={60,70,80,90,95,100,105,110,115,120,125,130,135,140, 142,145,147,150,153,155};
int CState_Main::specialEventLines[20]=  { 1, 1, 1, 2, 2,  2,  2,  2,  2,  2,  2,  3,  3,  3,   3,  3,  3,  4,  4,  4};

CState_Main::CState_Main(void)
{	
	isPlayingMusic=false;
	playMode=0;
	isGameEnd=false;
	timeToQuit=0.0;
}

CState_Main::~CState_Main(void)
{
}
CState* CState_Main::instance()
{
	if(_instance==NULL)
	{
		_instance=new CState_Main;
	}
	return _instance;

}
void CState_Main::setPlayMode(int value)
{
	playMode=value;
	blockMgr.makeBlock();						//���ӿ� ����� �� ����.				
	CPlayer::initializeHoleRandom();				//hole ������ġ ����

	switch(value)
	{
	case SOLOMODE:
		player[0].setPlayMode(CPlayer::PERSON,0,NULL,0);
		player[1].setPlayMode(CPlayer::NONE,1,NULL,0);		
		break;
	case COUPLEMODE:
		player[0].setPlayMode(CPlayer::PERSON,0,&player[1],1);
		player[1].setPlayMode(CPlayer::PERSON,1,&player[0],1);		
		break;
	case CUSTOMMODE:
		//State_CustonSetting���� ������		
		break;
	}
	isGameEnd=false;
	timeToQuit=0.0;
	startTime=DXUTGetTime();
	eventCount=0;

	//memset(specialEventExecuted,0,sizeof(specialEventExecuted));
	

}
void CState_Main::finalize()
{
	if(_instance!=NULL)
	{
		delete  _instance;
		_instance=NULL;
	}
}


void CState_Main::OnMsgProc(CStateController *controller,HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing )
{
	int x,y;



	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		x = LOWORD(lParam); 
		y=HIWORD(lParam);
		
		break;
	case WM_LBUTTONDOWN:		
		break;
	case WM_KEYDOWN:
		switch(wParam)
		{
			case VK_ESCAPE:
				game.ogg.Stop(OGG_GAME);
				isPlayingMusic=false;
				isGameEnd=false;
				changeState(controller,CState_Title::instance());
				break;
		}		
	}
	
}
void CState_Main::OnRender(CStateController *controller,IDirect3DDevice9* pd3dDevice,float fElapsedTime)
{   	
	double nowTime;
	nowTime=DXUTGetTime();

	if(isPlayingMusic==false && !isGameEnd)
	{
		game.ogg.Play(OGG_GAME,true);
		isPlayingMusic=true;
	}
	else game.ogg.IsPlaying(OGG_GAME);		//��� ����.	

	game.M_DXSprite->Begin(D3DXSPRITE_ALPHABLEND);
	game.sprCollection.collection[SPR_MAIN_BACKGROUND]->draw(pd3dDevice,0,0,D3DCOLOR_ARGB(255, 250, 250, 250),NULL);

	//�÷��̾� ���� ó���Ѵ�.
	for(int i=0;i<2;i++)
        player[i].draw(pd3dDevice,fElapsedTime);

	//effect���
	game.effectCollection.drawEffect(pd3dDevice,nowTime);
	game.M_DXSprite->End();	
	
	//�ð� ���
	g_middleTxtHelper->Begin();		
		//���� ���ż� ���
		g_middleTxtHelper->SetInsertionPos( 485, 56 );
		g_middleTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		int currentSeconds=((int)(nowTime-startTime));
		int currentMinutes=currentSeconds/60;
		g_middleTxtHelper->DrawFormattedTextLine(L"%02d    %02d", currentMinutes,currentSeconds%60);
	g_middleTxtHelper->End();
}
void CState_Main::OnProcess(CStateController *controller,float fElapsedTime)
{
	double nowTime;
	static double checkTime=0;
	static boolean doCheck=false;
		
	nowTime=DXUTGetTime();
	//�÷��̾� ���� ó���Ѵ�.
	for(int i=0;i<2;i++)
        player[i].process(nowTime);

	//���� ���߿� �ϳ��� �׾�����?
	if(!isGameEnd && (!player[0].getIsAlive() || !player[1].getIsAlive()))
	{
		
		if(checkTime<nowTime && !doCheck)
		{
			checkTime=nowTime;		//�ѳ��� �׾��� �� 0.5�� �ڿ� üũ
			doCheck=true;
		}
		if(doCheck && nowTime-checkTime>0.5)
		{
			MYTRACE("isAlive[0]:%d isAlive[1]:%d",player[0].getIsAlive()?1:0,player[1].getIsAlive()?1:0);
			doCheck=false;
			isGameEnd=true;	

			timeToQuit=nowTime+6;

			if(player[0].getIsAlive())		
				game.effectCollection.insertEffect(ANI_1PWIN,275,218,nowTime);
			if(player[1].getIsAlive())
				game.effectCollection.insertEffect(ANI_2PWIN,275,218,nowTime);
			isPlayingMusic=false;
			game.ogg.Stop(OGG_GAME);
			game.ogg.Play(OGG_WIN,false);
		}		
	}
	
	if(isGameEnd && nowTime>timeToQuit)
	{
		isGameEnd=false;
		player[0].setIsAlive(false);
		player[1].setIsAlive(false);
		game.ogg.Stop(OGG_GAME);
		game.ogg.Stop(OGG_SETTING);

		checkTime=0;
		doCheck=false;

		changeState(controller,CState_Title::instance());
	}


	//�ð��� ���� special event ����(��Ʈ��ũ�� ���� �����ϴ� ���
	if(player[0].getPlayMode()==CPlayer::NETWORK && (player[1].getPlayMode()==CPlayer::NETWORK || player[1].getPlayMode()==CPlayer::NONE))
	{
		if(eventCount<20)
		{
			if(nowTime-startTime>specialEventTime[eventCount])
			{
				CPlayer::stEvent specialEvent1=player[0].makePushEvent(specialEventLines[eventCount]);
				CPlayer::stEvent specialEvent2=player[1].makePushEvent(specialEventLines[eventCount]);
				player[0].getEventQueue()->push_back(specialEvent1);
				player[1].getEventQueue()->push_back(specialEvent2);
				eventCount++;
				game.ogg.Play(OGG_HURRYUP);
			}
		}		
	}

}