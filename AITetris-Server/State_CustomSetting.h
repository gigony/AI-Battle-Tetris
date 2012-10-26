/*******************************************
*CState_Main by Pureku(Gigon Bae)
*Date:2005.12.23
*
********************************************/
#pragma once
#include "state.h"
#include "Player.h"
#include "BlockMgr.h"
class CState_CustomSetting :
	public CState
{
public:	
	CState_CustomSetting(void);
	~CState_CustomSetting(void);
	static CState* instance();
	static void finalize();
	void OnMsgProc(CStateController *controller,HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing);
	void OnRender(CStateController *controller,IDirect3DDevice9* pd3dDevice,float fElapsedTime);
	void OnProcess(CStateController *controller,float fElapsedTime);

	void InitDialog();
	static void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl );

	void drawMiniBlock(IDirect3DDevice9* pd3dDevice,int x,int y,int index);

	CDXUTDialog* GetUIDialog(){return &uiDialog;}
	void SetPlayer1(int value){player1=value;}
	void SetPlayer2(int value){player2=value;}
	int GetPlayer1(){return player1;}
	int GetPlayer2(){return player2;}
	void SetPortNumber(u_short value){portNumber=value;}
	u_short GetPortNumber(){return portNumber;}
	static void SetAutoMoveDelay(double value){autoMoveDelay=value;}
	static double GetAutoMoveDelay(){return autoMoveDelay;}
	static void SetLimitRespondTime(double value){limitRespondTime=value;}
	static double GetLimitRespondTime(){return limitRespondTime;}
	void SetIsPlayingMusic(bool value){isPlayingMusic=value;}
	bool IsInitDialog(){return isInitDialog;}
	static UINT WINAPI WaitNetworkPlayer(LPVOID pVoid);
	void SetSelectedMapNo(int value){selectedMapNo=value;}
	int GetSelectedMapNo(){return selectedMapNo;}
	CStateController* GetController(){return currentController;}

	static double autoMoveDelay;
	static double limitRespondTime;	

private:
	static CState *_instance;
	volatile bool isPlayingMusic;						//음악 연주 중인가?
	bool isInitDialog;							//다이얼로그가 setting되었나?
	CDXUTDialog             uiDialog;			//UI
	volatile int player1;								//1: HUMAN 2: AI;
	volatile int player2;								//1: HUMAN 2: AI;
	volatile u_short portNumber;							//port number;	
	volatile int selectedMapNo;						//맵의 번호
	CStateController *currentController;	//약간의 트릭. Thread에서 controller을 알아내기 위해 사용
};

