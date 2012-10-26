/*******************************************
*CStateController by Pureku(Gigon Bae)
*Date:2005.12.22
*
********************************************/
#pragma once
class CState;
struct IDirect3DDevice9;

class CStateController
{
public:
	void OnMsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing);
	void OnRender(IDirect3DDevice9* pd3dDevice,float fElapsedTime);
	void OnProcess(float fElapsedTime);
	CStateController(void);
	~CStateController(void);
private:
	friend class CState;
	void changedState(CState*);
private:
	CState * _state;
};
