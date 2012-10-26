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
#include "State_CustomSetting.h"
#include "Game.h"
#include "definition.h"
#include "State_Title.h"
#include "State_Main.h"
//#include <winsock2.h>
#include <process.h>


extern CGame game;
extern CDXUTTextHelper*		g_txtHelper;

CState* CState_CustomSetting::_instance=NULL;
double CState_CustomSetting::autoMoveDelay=0.05;
double CState_CustomSetting::limitRespondTime=0.3;


CState_CustomSetting::CState_CustomSetting(void)
{
		isPlayingMusic=false;
		isInitDialog=false;
		player1=1;
		player2=1;
		portNumber=50001;		
		selectedMapNo=0;
		currentController=NULL;
		
}

CState_CustomSetting::~CState_CustomSetting(void)
{
}



CState* CState_CustomSetting::instance()
{
	if(_instance==NULL)
	{
		_instance=new CState_CustomSetting;
	}	
	return _instance;

}
void CState_CustomSetting::finalize()
{
	if(_instance!=NULL)
	{
		delete  _instance;
		_instance=NULL;
	}
}


void CState_CustomSetting::OnMsgProc(CStateController *controller,HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing)
{
	currentController=controller;
	//GUI
	if(isInitDialog)
	{
		bool noFurtherProcessing=uiDialog.MsgProc(hWnd,uMsg,wParam,lParam);
		if(noFurtherProcessing)
			return;
	}

	int x,y;
	x = LOWORD(lParam);
	y=HIWORD(lParam);
	
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		
		break;
	case WM_LBUTTONDOWN:		
		break;
	case WM_KEYDOWN:
		switch(wParam)
		{
			case VK_ESCAPE:
				game.ogg.Stop(OGG_SETTING);
				isPlayingMusic=false;
				changeState(controller,CState_Title::instance());
				break;
		}		
	}


	
}
void CState_CustomSetting::OnRender(CStateController *controller,IDirect3DDevice9* pd3dDevice,float fElapsedTime)
{
	currentController=controller;
	if(isInitDialog==false)
	{
		InitDialog();
		isInitDialog=true;
	}

	if(isPlayingMusic==false)
	{
		game.ogg.Play(OGG_SETTING,true);
		isPlayingMusic=true;
	}
	else game.ogg.IsPlaying(OGG_SETTING);

	game.M_DXSprite->Begin(D3DXSPRITE_ALPHABLEND);

	game.sprCollection.collection[SPR_TITLE]->draw(pd3dDevice,0,0,D3DCOLOR_ARGB(255, 255, 255, 255),NULL);
	RECT titleRect={0,0,500,167};	
	game.sprCollection.collection[SPR_TITLE_NAME]->draw(pd3dDevice,280,50,D3DCOLOR_ARGB(255, 255, 255, 255),&titleRect);

	if(CBoard::numOfInitialBoard!=0)
	{		
		for(int i=2;i<22;i++)
		{
			for(int j=0;j<10;j++)
			{				
				drawMiniBlock(pd3dDevice,450+j*16,400+i*16,CBoard::initialBoard[selectedMapNo][i][j]);
			}
		}

	}
	game.M_DXSprite->End();	

	if(CBoard::numOfInitialBoard!=0)
	{		
		g_txtHelper->Begin();
		g_txtHelper->SetInsertionPos( 450+72, 395 );
		g_txtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		g_txtHelper->DrawFormattedTextLine( L"%d",selectedMapNo);
		g_txtHelper->End();
	}

	uiDialog.OnRender( fElapsedTime );
	
	//텍스트 출력
	if(player1>=2 || player2>=2)
	{
		g_txtHelper->Begin();
		g_txtHelper->SetInsertionPos( 50, 350 );
		g_txtHelper->SetForegroundColor( D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
		g_txtHelper->DrawTextLine( L"Waiting network connections..." );
		
		if(player1==3)
		{
			g_txtHelper->DrawTextLine( L"Player 1 is connected" );
		}
		if(player2==3)
		{
			g_txtHelper->DrawTextLine( L"Player 2 is connected" );
		}
		g_txtHelper->End();	
	}	
	
	

}
void CState_CustomSetting::OnProcess(CStateController *controller,float fElapsedTime)
{
}

void CState_CustomSetting::InitDialog()
{
	uiDialog.SetLocation(300,200);
	uiDialog.SetSize(600,500);
	uiDialog.SetCallback(CState_CustomSetting::OnGUIEvent);
	uiDialog.RemoveAllControls();

	int iY = 10; 



	
	uiDialog.AddStatic(0,L"1P",35,10,100,30);
	uiDialog.AddComboBox( 1, 35, iY += 24, 125, 22 );
    uiDialog.GetComboBox( 1 )->AddItem( L"Human", NULL);
    uiDialog.GetComboBox( 1 )->AddItem( L"AI", NULL );	

	uiDialog.AddStatic(0,L"2P",250+35,10,100,30);
	uiDialog.AddComboBox( 2, 250+35, iY, 125, 22 );
    uiDialog.GetComboBox( 2 )->AddItem( L"Human", NULL );
    uiDialog.GetComboBox( 2 )->AddItem( L"AI", NULL );
	uiDialog.GetComboBox( 2 )->AddItem( L"NONE", NULL );
	
	uiDialog.AddEditBox( 3, L"50001", 125+35, iY += 40, 125, 30 );
	uiDialog.AddStatic(0,L"Port",55,iY,100,30);

	uiDialog.AddEditBox( 4, L"0.05", 125+35, iY += 40, 125, 30 );
	uiDialog.AddStatic(0,L"Auto Moving Delay",55,iY,100,30);

	uiDialog.AddEditBox( 5, L"0.3", 125+35, iY += 40, 125, 30 );
	uiDialog.AddStatic(0,L"Respond Time Limit",55,iY,100,30);
	if(CBoard::numOfInitialBoard!=0)
	{
		uiDialog.AddButton( 6, L"◀", 125+35+40-50, iY +=40, 50, 22 );
		uiDialog.AddButton( 7, L"▶", 125+35+40+50, iY , 50, 22 );		
		uiDialog.AddStatic(0,L"Map",55,iY-5,100,30);
		uiDialog.AddButton( 8, L"Start", 125+35+200, iY-20 , 150, 50 );
	}
	else
		uiDialog.AddButton( 8, L"Start", 125+35, iY += 50, 125, 22 );


	

	
	
    

}
void CALLBACK CState_CustomSetting::OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl )
{
	u_short portNum=0;
	CDXUTDialog *uiDialog=NULL;
	CState_CustomSetting* instance=((CState_CustomSetting*)_instance);
	CState_Main *mainInstance=((CState_Main*)CState_Main::instance());	
	int player1Value=1;
	int player2Value=1;
	int selectedMapNoValue=instance->GetSelectedMapNo();
	
	//버튼 입력만 처리
	if(nEvent!=EVENT_BUTTON_CLICKED) return;
	
	switch( nControlID )
    {     
	case 6:

		if(selectedMapNoValue!=0)
			instance->SetSelectedMapNo(selectedMapNoValue-1);
		break;
	case 7:
		if(selectedMapNoValue!=CBoard::numOfInitialBoard-1)
			instance->SetSelectedMapNo(selectedMapNoValue+1);
		break;
	case 8:
		uiDialog=instance->GetUIDialog();
		portNum=(u_short)_wtoi(uiDialog->GetEditBox(3)->GetText());
		if(portNum<1025 || portNum>65535)
			
		{
			MessageBoxExW(NULL,L"Invalid port number. Enter number between 1025 and 65535",L"Error",MB_OK,MAKELANGID(LANG_KOREAN,SUBLANG_ENGLISH_US));
			return;
		}
		instance->SetPortNumber(portNum);
			static double autoMoveDelay;
	
		double value=_wtof(uiDialog->GetEditBox(4)->GetText());
		if(value>0 && value<0.5)		
			CState_CustomSetting::autoMoveDelay=value;		
		else
		{
			MessageBoxExW(NULL,L"Invalid value. Enter number between 0 and 0.5(exclusive)",L"Error",MB_OK,MAKELANGID(LANG_KOREAN,SUBLANG_ENGLISH_US));
			return;
		}

		value=_wtof(uiDialog->GetEditBox(5)->GetText());
		if(value>0 && value<5)		
			CState_CustomSetting::limitRespondTime=value;		
		else
		{
			MessageBoxExW(NULL,L"Invalid value. Enter number between 0 and 5(exclusive)",L"Error",MB_OK,MAKELANGID(LANG_KOREAN,SUBLANG_ENGLISH_US));
			return;
		}
			

		if(wcscmp(uiDialog->GetComboBox(1)->GetSelectedItem()->strText,L"AI")==0)		
		{
			instance->SetPlayer1(2);
			player1Value=2;
			
		}
		

		if(wcscmp(uiDialog->GetComboBox(2)->GetSelectedItem()->strText,L"AI")==0)		
		{
			instance->SetPlayer2(2);
			player2Value=2;		
		}
		else if(wcscmp(uiDialog->GetComboBox(2)->GetSelectedItem()->strText,L"NONE")==0)
		{
			instance->SetPlayer2(0);
			player2Value=0;
		}


		//플레이어 설정
		if(player1Value==1 && player2Value==1)
		{
			mainInstance->GetPlayer(0)->setPlayMode(CPlayer::PERSON,0,mainInstance->GetPlayer(1),1);
			mainInstance->GetPlayer(1)->setPlayMode(CPlayer::PERSON,1,mainInstance->GetPlayer(0),1);
		}
		else if(player1Value==1 && player2Value==2)
		{
			mainInstance->GetPlayer(0)->setPlayMode(CPlayer::PERSON,0,mainInstance->GetPlayer(1),0);
			mainInstance->GetPlayer(1)->setPlayMode(CPlayer::NETWORK,1,mainInstance->GetPlayer(0),0);
		}
		else if(player1Value==2 && player2Value==1)
		{
			mainInstance->GetPlayer(0)->setPlayMode(CPlayer::NETWORK,0,mainInstance->GetPlayer(1),0);
			mainInstance->GetPlayer(1)->setPlayMode(CPlayer::PERSON,1,mainInstance->GetPlayer(0),0);
		}
		else if(player1Value==2 && player2Value==2)
		{
			mainInstance->GetPlayer(0)->setPlayMode(CPlayer::NETWORK,0,mainInstance->GetPlayer(1),0);
			mainInstance->GetPlayer(1)->setPlayMode(CPlayer::NETWORK,1,mainInstance->GetPlayer(0),0);
		}
		else if(player1Value==1 && player2Value==0)
		{
			mainInstance->GetPlayer(0)->setPlayMode(CPlayer::PERSON,0,mainInstance->GetPlayer(1),0);
			mainInstance->GetPlayer(1)->setPlayMode(CPlayer::NONE,1,NULL,0);
		}
		else if(player1Value==2 && player2Value==0)
		{
			mainInstance->GetPlayer(0)->setPlayMode(CPlayer::NETWORK,0,mainInstance->GetPlayer(1),0);
			mainInstance->GetPlayer(1)->setPlayMode(CPlayer::NONE,1,NULL,0);
		}


		//Start버튼 비활성화
		uiDialog->GetButton(8)->SetEnabled(false);
		

		
		//HANDLE hThread;
		//hThread=(HANDLE)_beginthreadex(NULL,NULL,CState_CustomSetting::waitNetworkPlayer,0,0,&dwThreadId);

		//플레이어 기다리기 수행
		UINT dwThreadId;		
		_beginthreadex(NULL,NULL,CState_CustomSetting::WaitNetworkPlayer,0,0,&dwThreadId);
		
	}
}

void CState_CustomSetting::drawMiniBlock(IDirect3DDevice9* pd3dDevice,int x,int y,int index)
{
	RECT rect={index*16,0,index*16+16,16};
	game.sprCollection.collection[SPR_MAIN_BLOCK_MINI]->draw(pd3dDevice,x,y,D3DCOLOR_ARGB(255, 255, 255, 255),&rect);
}


UINT WINAPI CState_CustomSetting::WaitNetworkPlayer(LPVOID pVoid)
{
	CState_CustomSetting* instance=((CState_CustomSetting*)_instance);
	CState_Main *mainInstance=((CState_Main*)CState_Main::instance());	

	int player1=instance->GetPlayer1();
	int player2=instance->GetPlayer2();

	if(player1==1 && (player2==1 || player2==0))
	{
		game.ogg.Stop(OGG_SETTING);			
		instance->SetIsPlayingMusic(false);
		mainInstance->setPlayMode(CState_Main::CUSTOMMODE);
		game.ogg.Play(OGG_START);
		instance->changeState(instance->GetController(),CState_Main::instance());
		return 1;
	}	

	//플레이어 입력 받기
	SOCKET serverSocket;
	serverSocket=socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( serverSocket == INVALID_SOCKET ) //잘못된 소켓을 가지고 있다면
	{
		MessageBox(NULL,L"Socket Initialization Failed!",L"Error",MB_OK);
		game.ogg.Stop(OGG_SETTING);			
		instance->SetIsPlayingMusic(false);				
		instance->changeState(instance->GetController(),CState_Title::instance());
		return 0;
	}
	SOCKADDR_IN serverAddress;
	serverAddress.sin_addr.s_addr	= htonl( INADDR_ANY );
	serverAddress.sin_port			= htons( (u_short)instance->GetPortNumber() );
	serverAddress.sin_family		= AF_INET;

	//delay없이 보내도록 세팅
	int nodelay=1;
	int sockOptSize=sizeof(int);
	setsockopt(serverSocket,IPPROTO_TCP,TCP_NODELAY,(char *)&nodelay,sockOptSize);

	if ( bind( serverSocket, (SOCKADDR *)&serverAddress,
		sizeof( serverAddress ) ) == SOCKET_ERROR )
	{
		MessageBox(NULL,L"Bind error!",L"Network Error",MB_OK);
		return 0;
	}
	int ret;
	SOCKET clientSocket=NULL;
	PSInstance *psInstance=NULL;
	UINT dwThreadId;
	unsigned long ul = 1;  
	char teamName[256];
	if(player1==2)
	{
		ret=listen( serverSocket, 2 );
		if(ret!=0)
		{
			MessageBox(NULL,L"Listen Error!",L"Network Error",MB_OK);
			return 0;
		}
		if((clientSocket=accept(serverSocket,NULL,NULL))==INVALID_SOCKET)
		{
			MessageBox(NULL,L"Accept Error!",L"Network Error",MB_OK);
			return 0;
		}
	
		if( ioctlsocket( clientSocket, FIONBIO, (unsigned long *)&ul ) == SOCKET_ERROR )  
		{  
			MessageBox(NULL,L"Non-blocking Error!",L"Network Error",MB_OK);
		} 
		psInstance=PSCreateInstance(clientSocket);
		mainInstance->setPSInstance(0,psInstance);		
		instance->SetPlayer1(3);

		//기본 데이터 읽기(팀이름)		
		ret=psInstance->recvData(teamName,sizeof(teamName));					
		if(ret==0)
		{
			MessageBox(NULL,L"Can't read team name",L"Network Error",MB_OK);
			return 0;
		}
		mainInstance->GetPlayer(0)->setPlayerName(teamName);


		//데이터 읽기 쓰레드 생성			
		_beginthreadex(NULL,NULL,CPlayer::WaitNetworkPlayer,(LPVOID)mainInstance->GetPlayer(0),0,&dwThreadId);
	}
	
	if(player2==2)
	{
		ret=listen( serverSocket, 2 );
		if(ret!=0)
		{
			MessageBox(NULL,L"Listen Error!",L"Network Error",MB_OK);
			return 0;
		}
		if((clientSocket=accept(serverSocket,NULL,NULL))==INVALID_SOCKET)
		{
			MessageBox(NULL,L"Accept Error!",L"Network Error",MB_OK);
			return 0;
		}
		if( ioctlsocket( clientSocket, FIONBIO, (unsigned long *)&ul ) == SOCKET_ERROR )  
		{  
			MessageBox(NULL,L"Non-blocking Error!",L"Network Error",MB_OK);
		} 
		psInstance=PSCreateInstance(clientSocket);
		mainInstance->setPSInstance(1,psInstance);		
		instance->SetPlayer2(3);
		//기본 데이터 읽기(팀이름)		
		ret=psInstance->recvData(teamName,sizeof(teamName));			
		if(ret==0)
		{
			MessageBox(NULL,L"Can't read team name",L"Network Error",MB_OK);
			return 0;
		}
		mainInstance->GetPlayer(1)->setPlayerName(teamName);

		//데이터 읽기 쓰레드 생성			
		_beginthreadex(NULL,NULL,CPlayer::WaitNetworkPlayer,(LPVOID)(LPVOID)mainInstance->GetPlayer(1),0,&dwThreadId);
	}

	//게임 시작
	
	instance->SetIsPlayingMusic(false);
	game.ogg.Stop(OGG_SETTING);						
	game.ogg.Stop(OGG_TITLE);
	mainInstance->setPlayMode(CState_Main::CUSTOMMODE);
	game.ogg.Play(OGG_START);	

	instance->changeState(instance->GetController(),CState_Main::instance());
	return 1;

}