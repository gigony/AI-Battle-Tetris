/*******************************************
*CState_Main by Pureku(Gigon Bae)
*Date:2005.12.23
*
********************************************/
#pragma once
#include "state.h"
#include "Player.h"
#include "BlockMgr.h"
class CState_Main :
	public CState
{
public:
	enum PLAYMODE{SOLOMODE,COUPLEMODE,CUSTOMMODE};
	CState_Main(void);
	~CState_Main(void);
	static CState* instance();
	static void finalize();

	void OnMsgProc(CStateController *controller,HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing);
	void OnRender(CStateController *controller,IDirect3DDevice9* pd3dDevice,float fElapsedTime);
	void OnProcess(CStateController *controller,float fElapsedTime);

	void setPlayMode(int value);
	int getPlayMode(){return playMode;}
	void setIsGameEnd(bool value){isGameEnd=value;}
	bool getIsGameEnd(){return isGameEnd;}
	void setTimeToQuit(double value){timeToQuit=value;}
	double getTimeToQuit(){return timeToQuit;}
	void setPSInstance(int index,PSInstance *instance)
	{
		player[index].setPSInstance(instance);
	}
	PSInstance* getPSInstance(int index)
	{
		return player[index].getPSInstance();
	}

	CPlayer* GetPlayer(int index)
	{
		return &player[index];
	}

private:
	static CState *_instance;

	CPlayer player[2];	
	CBlockMgr blockMgr;
	
	int playMode;								//플레이 모드
	bool isPlayingMusic;						//음악 연주 중인가?
	bool isGameEnd;								//게임이 끝났는가?
	double timeToQuit;							//타이틀로 돌아가는 시간
	double startTime;								//게임 시작 시간
	int    eventCount;
	//bool   specialEventExecuted[20];			//스페샬 이벤트를 실행하였는가?
	static double specialEventTime[20];
	static int specialEventLines[20];

};
