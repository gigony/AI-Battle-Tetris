/*******************************************
*CState_Title by Pureku(Gigon Bae)
*Date:2005.12.22
*
********************************************/
#pragma once
#include "State.h"


class CState_Title:
	public CState
{
public:
	CState_Title(void);
	~CState_Title(void);
	static CState* instance();
	static void finalize();

	void OnMsgProc(CStateController *controller,HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing);
	void OnRender(CStateController *controller,IDirect3DDevice9* pd3dDevice,float fElapsedTime);
	void OnProcess(CStateController *controller,float fElapsedTime);

private:
	static CState *_instance;

	static RECT region[4];
	int selectedItem;
	bool isPlayingMusic;
};
