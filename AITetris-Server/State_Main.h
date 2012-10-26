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
	
	int playMode;								//�÷��� ���
	bool isPlayingMusic;						//���� ���� ���ΰ�?
	bool isGameEnd;								//������ �����°�?
	double timeToQuit;							//Ÿ��Ʋ�� ���ư��� �ð�
	double startTime;								//���� ���� �ð�
	int    eventCount;
	//bool   specialEventExecuted[20];			//���伣 �̺�Ʈ�� �����Ͽ��°�?
	static double specialEventTime[20];
	static int specialEventLines[20];

};
