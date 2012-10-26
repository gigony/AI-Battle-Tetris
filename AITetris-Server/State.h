/*******************************************
*CState by Pureku(Gigon Bae)
*Date:2005.12.22
*
********************************************/
#pragma once
struct IDirect3DDevice9;
class CStateController;
class CState
{
public:
	CState(void);
	virtual ~CState(void);
	virtual void OnMsgProc(CStateController *controller,HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing){};
	virtual void OnRender(CStateController *controller,IDirect3DDevice9* pd3dDevice,float fElapsedTime){};
	virtual void OnProcess(CStateController *controller,float fElapsedTime){};

protected:
	void changeState(CStateController*, CState *);
};
