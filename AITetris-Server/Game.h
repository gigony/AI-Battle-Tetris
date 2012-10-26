/*******************************************
*CGame by Pureku(Gigon Bae)
*Date:2005.12.17
*
********************************************/
#pragma once
#include "definition.h"
#include "SpriteCollection.h"
#include "EffectCollection.h"
#include "StateController.h"
#include "DSClass.h"

struct IDirectMusicLoader8;
struct IDirectMusicPerformance8;
struct IDirectMusicSegment8;

class CGame
{
public:
	enum GAMESTATE{MAINMENU,PLAYGAME,ENDGAME};
	enum KIND{SURFACE=0,SPRITE=1};
	
	CGame(void);
	~CGame(void);
	HRESULT OnCreateDevice(IDirect3DDevice9* pd3dDevice);
	void OnResetDevice(IDirect3DDevice9* pd3dDevice);
	void OnLostDevice();
	void OnDestroyDevice();
	void OnMsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing);
	void render(IDirect3DDevice9* pd3dDevice,float fElapsedTime);
	void process(float fElapsedTime);

	ID3DXSprite* M_DXSprite;
	ID3DXLine *M_DXLine;

	OggPlayer ogg;
	
	void vCleanup( void );
	bool bInitializeSoundSystem( HWND hWnd );	
	

	CSpriteCollection sprCollection;
	CEffectCollection effectCollection;
protected:
	friend class CState_Title;
	friend class CState_Main;    
	
	CStateController stateController;

};
