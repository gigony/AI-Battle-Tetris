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
#include ".\statecontroller.h"
#include "State_Title.h"
#include "State_Main.h"

CStateController::CStateController(void)
{
	_state=CState_Title::instance();
}

CStateController::~CStateController(void)
{
	CState_Title::finalize();
	CState_Main::finalize();
}
void CStateController::changedState(CState* s)
{
	_state=s;
}
void CStateController::OnMsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing)
{
	_state->OnMsgProc(this,hWnd,uMsg,wParam,lParam,pbNoFurtherProcessing);
}
void CStateController::OnRender(IDirect3DDevice9* pd3dDevice,float fElapsedTime)
{
	_state->OnRender(this,pd3dDevice,fElapsedTime);
}
void CStateController::OnProcess(float fElapsedTime)
{
	_state->OnProcess(this,fElapsedTime);
}