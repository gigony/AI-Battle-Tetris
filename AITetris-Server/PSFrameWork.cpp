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
//--------------------------------------------------------------------------------------
// External functions implementations
//--------------------------------------------------------------------------------------
int PSInitialize()
{
	return PSFrameWork::Instance()->initialize();

}
int PSFinalize()
{
	return PSFrameWork::Instance()->finalize();
}
int PSDeleteInstance(PSInstance *pInstance)
{
	return PSFrameWork::Instance()->deleteInstance(pInstance);
}

int PSDeleteALLInstance()
{
	return PSFrameWork::Instance()->deleteALLInstance();
}
PSInstance* PSCreateInstance(const char *ip,unsigned short port)
{
	return PSFrameWork::Instance()->createInstance(ip,port);
}
PSInstance* PSCreateInstance(SOCKET clientSocket)
{
	return PSFrameWork::Instance()->createInstance(clientSocket);
}
PSTimer* PSGetGlobalTimer()
{
	static PSTimer timer;
    return &timer;
}


//////////////////////////////////////////////////////////////////////////////////
//
//  PSProfiler 클래스
//
//	-Preformance Test를 할 때에 사용한다.
//
//	 사용법은 다음 사이트를 참조:.http://www.codeproject.com/debug/cprofile.asp
//
//////////////////////////////////////////////////////////////////////////////////


PSProfiler::PSProfiler(void)
{
	// constructor zeroes all LARGE_INTEGER data
	// members and gets the correct counter frequency
	// for the system.
	ZeroMemory(&m_QPFrequency, sizeof(m_QPFrequency));
	ZeroMemory(&m_ElapsedTime, sizeof(m_ElapsedTime));
	ZeroMemory(&m_StartCounter,sizeof(m_StartCounter));
	m_Retval = 0;

	// Query Frecuency
	m_Retval = QueryPerformanceFrequency(&m_QPFrequency);	

}
PSProfiler::~PSProfiler(void)
{	
}

/////////////////////////////////////////////////////////////
// GetLastRetVal()
// returns error code for the latest API function called
/////////////////////////////////////////////////////////////
inline DWORD PSProfiler::GetLastRetVal()
{
	return static_cast<DWORD>(m_Retval);
}

/////////////////////////////////////////////////////////////
// ProfileStart
// Starts time count, specifying the debug log type
// which is written when ProfileEnd is called.
// May be LOGNONE (default), LOGTICKS, LOGSECS, LOGALL
////////////////////////////////////////////////////////////
void PSProfiler::ProfileStart(LOGTYPE logtype)
{
	// get and store start time
	m_Retval = QueryPerformanceCounter (&m_StartCounter);
	// store logging type
	m_LogType = logtype;
}

/////////////////////////////////////////////////////////////
// ProfileEnd 
// End profiling and optionally write a string to the
// debug window or to a message box, depending on the
// LOGTYPE specified on ProfileStart(...)
////////////////////////////////////////////////////////////
__int64 PSProfiler::ProfileEnd(char* TraceStr)
{
	// get and store finishing time and calc elapsed time(ticks)
	m_Retval = QueryPerformanceCounter (&m_EndCounter);
	m_ElapsedTime = (m_EndCounter.QuadPart  - m_StartCounter.QuadPart );

    // output debugging log?
    if (m_LogType != LOGNONE)
	{
		// variables for output
		char* MsgOut;
		char tmpbuf[300];        
		
		if (m_LogType == LOGTICKS)	// output in ticks
            StringCchPrintfA(tmpbuf,300, "** ProfileEnd: %I64d clock ticks elapsed.\n", m_ElapsedTime);

		if (m_LogType == LOGSECS)  // output in secs		
			StringCchPrintfA(tmpbuf,300, "** ProfileEnd: %.3fsecs. elapsed.\n", SecsFromTicks(m_ElapsedTime)); 		

		if (m_LogType == LOGMSGBOX)
			StringCchPrintfA(tmpbuf,300, "\n\n%I64d clock ticks (%.3fsecs.) elapsed.\n", 
					m_ElapsedTime, SecsFromTicks(m_ElapsedTime));

		if (m_LogType == LOGALL)
			StringCchPrintfA(tmpbuf,300, "** Profile End **\n[CPU Hires counter freq is: %I64d clock ticks" 
			                "per second.\n%I64d clock ticks (%.3fsecs.) elapsed.\n", 
							m_QPFrequency.QuadPart ,m_ElapsedTime, SecsFromTicks(m_ElapsedTime)); 

		int msgLength=strlen(tmpbuf)+strlen(TraceStr)+1;
		MsgOut = new char[msgLength];
		StringCchCopyA(MsgOut,msgLength,TraceStr);
		StringCchCatA(MsgOut,msgLength,tmpbuf);

		// select output to msgbox or debug-results window
		if (m_LogType == LOGMSGBOX)
		{
            MessageBoxA(NULL, MsgOut, "PSProfiler::ProfileEnd Timing", 
						MB_ICONINFORMATION | MB_OK);
		}
		else
		{
			OutputDebugStringA(MsgOut);
		}

		delete [] MsgOut;
	}
	return m_ElapsedTime;
}

// The following function convert ticks to seconds
double PSProfiler::SecsFromTicks (__int64 ticks)
{
	return static_cast<double>(ticks) / static_cast<double>(m_QPFrequency.QuadPart);
}



/////////////////////////////////////////////////////////////////////
//
//
//        PSFramework 클래스
//
//
/////////////////////////////////////////////////////////////////////


PSFrameWork* PSFrameWork::_Instance=0;

PSFrameWork::PSFrameWork(void)
{
}

PSFrameWork::~PSFrameWork(void)
{
	
}

//싱글턴 객체를 리턴한다.
PSFrameWork* PSFrameWork::Instance()
{
	if(_Instance==0)
	{
		_Instance=new PSFrameWork();
		MYSYSDETAILTRACE("PSFrameWork 클래스의 싱글턴 객체를 생성하였습니다");
	}
	return _Instance;
}


//네트워크 초기화를 하고, 내부적으로 관리하는 통신 인스턴스들의 연결을 모두 끊고 초기화한다.
int PSFrameWork::initialize()
{
	//네트워크 초기화
	WSADATA wsaData;
	if (WSAStartup (MAKEWORD(2, 2), &wsaData))
	{
		MYSYSTRACE("네트워크 초기화 과정에서 에러가 발생했습니다");
		return 0;
	}
	deleteALLInstance();
	return 1;
}

//내부적으로 관리하는 통신 인스턴스들의 연결을 모두 끊고 네트워크 종료
int PSFrameWork::finalize()
{
	//리스트에 있는 instance들을 모두 닫는다.
	deleteALLInstance();	
	WSACleanup ();
	if(_Instance!=0)
	{
        delete _Instance;
		_Instance=NULL;
	}
	return 1;
}

//내부에서 관리하는 통신인스턴스들 중에서 특정 인스턴스의 연결을 끊고 관리대상에서 삭제한다.
int PSFrameWork::deleteInstance(PSInstance * pInstance)
{	
	if(pInstance==NULL) return 0;
	set<PSInstance*>::iterator iter=instanceSet.find(pInstance);
	if(iter != instanceSet.end())
	{
		MYSYSDETAILTRACE("통신 인스턴스를 삭제 시도합니다. InstancePtr=0X%x",(unsigned int)pInstance);
		if(((PSInstance*)(*iter))->closeConnection())
		{
			MYSYSDETAILTRACE("통신 인스턴스를 삭제 성공");
		}
		else MYSYSDETAILTRACE("통신 인스턴스 삭제에 실패하였습니다");
		delete ((PSInstance*)(*iter));		
		instanceSet.erase(iter);
		MYSYSDETAILTRACE("남은 통신 인스턴스 갯수:%d",instanceSet.size());
		return 1;
	}

	return 0;
}
//내부적으로 관리하는 통신 인스턴스들의 연결을 모두 끊고 초기화.
int PSFrameWork::deleteALLInstance()
{
	set<PSInstance*>::iterator iter;
	MYSYSDETAILTRACE("통신 인스턴스를 모두 삭제합니다");
	for(iter=instanceSet.begin();iter!=instanceSet.end();++iter)
	{
		((PSInstance*)(*iter))->closeConnection();
		delete ((PSInstance*)(*iter));		
	}
	instanceSet.clear();
	return 1;
}
//특정 Ip와 포트번호로 접속을 한다(서버로 접속). 그리고 소켓 정보를 가지고 있는 PSInstance객체를 리턴
PSInstance* PSFrameWork::createInstance(const char *ip,unsigned short port)
{	
	PSInstance *pInstance=new PSInstance();
	ASSERT(pInstance!=NULL);
	//서버에 연결
	if(pInstance->makeConnection(ip,port)==NULL)
	{
		delete pInstance;
		pInstance=NULL;
		MYSYSTRACE("서버 접속에 실패하였습니다");
		return 0;
	}
	else
	{
		//instance리스트에 넣는다.
		instanceSet.insert(pInstance);
		MYSYSDETAILTRACE("프레임웍의 통신 인스턴스Set에 집어넣습니다. 프레임웍 통신 인스턴스 갯수:%d",instanceSet.size());
	}
	return pInstance;
}

//인수로 받은 clientSocket을 멤버변수에 저장하고 클라이언트 소켓 정보를 가지고 있는 PSInstance객체를 리턴
PSInstance* PSFrameWork::createInstance(SOCKET clientSocket)
{	
	PSInstance *pInstance=new PSInstance();
	ASSERT(pInstance!=NULL);
	//서버에 연결
	if(pInstance->acceptConnection(clientSocket)==NULL)
	{
		delete pInstance;
		MYSYSTRACE("클라이언트의 접속 받아들이는데 실패하였습니다");
		pInstance=NULL;
		return 0;
	}
	else
	{
		//instance리스트에 넣는다.
		instanceSet.insert(pInstance);
		MYSYSDETAILTRACE("프레임웍의 통신 인스턴스Set에 집어넣습니다. 프레임웍 통신 인스턴스 갯수:%d",instanceSet.size());
	}
	return pInstance;
}