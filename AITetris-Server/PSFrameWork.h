#pragma once

//TRACE레벨 설정
//만약 관련 TRACE를 보고싶지 않으면 주석처리를 해주세요
//각 Define에 따른 출력되는 TRACE들은 다음과 같습니다.
//PSFRAMEWORK_USER_DBG     :	MYTRACE 함수를 사용하였을 때(유저 디버그용)
//PSFRAMEWORK_SYSTEM_DBG   :	MYSYSTRACE 함수에 의해 출력되는 TRACE(프레임웍에서 중요한 에러가 발생하였을 때 사용)
//PSFRAMEWORK_SYSDETAIL_DBG: MYSYSDETAILTRACE 함수를 사용하였을 때(프레임웍에서 데이터 전송등의 사소한 내용들 출력할때 사용)
#define PSFRAMEWORK_USER_DBG
#define PSFRAMEWORK_SYSTEM_DBG
#define PSFRAMEWORK_SYSDETAIL_DBG

//TRACE출력될 때에 앞에 붙일 문자열(프로젝트명)
#define PSFRAMWEORK_PROJECTNAME	"[GameServer]"

//--------------------------------------------------------------------------------------
// External functions
//--------------------------------------------------------------------------------------
int PSInitialize();
int PSFinalize();
int PSDeleteInstance(PSInstance *pInstance);
int PSDeleteALLInstance();
PSInstance* PSCreateInstance(const char *ip,unsigned short port);
PSInstance* PSCreateInstance(SOCKET clientSocket);
PSTimer* PSGetGlobalTimer();

//--------------------------------------------------------------------------------------
// Helper macros to build member functions that access member variables 
//--------------------------------------------------------------------------------------
#ifndef SET_ACCESSOR
	#define SET_ACCESSOR( x, y )			inline void Set##y( x t )  { m_##y = t; };
#endif
#ifndef GET_ACCESSOR
	#define GET_ACCESSOR( x, y )			inline x Get##y() { return m_##y; };
#endif
#ifndef GET_SET_ACCESSOR
	#define GET_SET_ACCESSOR( x, y )		SET_ACCESSOR( x, y ) GET_ACCESSOR( x, y )
#endif
#ifndef GET_STATIC_REFERENCE
	#define GET_STATIC_REFERENCE( x, y )	inline static x& Get##y(){ return m_##y; };
#endif
#ifndef GET_REFERENCE
	#define GET_REFERENCE( x, y )			inline x& Get##y(){ return m_##y; };
#endif


///아래 클래스는 다음 페이지를 참고하였습니다. http://www.codeproject.com/debug/location_trace.asp
class PSLogHelper
{

private:
	const char* m_file;
	int m_line;
	int m_type;
	enum { BUFFER_SIZE = 1024 };
	
public:
	PSLogHelper( const char* file, int line ,int type) :
		m_file( file ),
		m_line( line ),
		m_type(type)
	{
	}
	
	void operator()( const char* Format, ... )
	{
		va_list va;
		va_start(va, Format);
			char buf1[BUFFER_SIZE];
			char buf2[BUFFER_SIZE];			
			//파일 경로 때문에 출력물이 길므로 파일이름만 출력하도록 한다.
			int lastWon=0,ptr=0;
			while(m_file[ptr]!=0)
			{
				if(m_file[ptr]=='\\') lastWon=ptr+1;
				ptr++;
			}
			//종류에 따라서 앞에 덧붙이는 문장을 다르게 한다.
			switch(m_type)
			{
			case 3:
			case 1:
                //  add the __FILE__ and __LINE__ to the front:
				if(FAILED(StringCchPrintfA(buf1,BUFFER_SIZE,"%s[TID:%u]%s:%d %s\n",PSFRAMWEORK_PROJECTNAME,GetCurrentThreadId(),&m_file[lastWon], m_line, Format)))
					buf1[BUFFER_SIZE-1] = 0;
				break;
			case 2:
				//  add the __FILE__ and __LINE__ to the front:
				if(FAILED(StringCchPrintfA(buf1,BUFFER_SIZE,"%s[TID:%u][ERROR]%s:%d %s\n",PSFRAMWEORK_PROJECTNAME,GetCurrentThreadId(),&m_file[lastWon], m_line, Format)))
					buf1[BUFFER_SIZE-1] = 0;
				break;
			}


			//  format the message as requested:			
			if(FAILED(StringCchVPrintfA(buf2, BUFFER_SIZE-1, buf1, va)))
				buf2[BUFFER_SIZE-1] = 0;			

			//  write it out!
			OutputDebugStringA( buf2 );
		va_end(va);
	}
};

//#ifdef _DEBUG
#	ifdef PSFRAMEWORK_USER_DBG
#		define MYTRACE (PSLogHelper( __FILE__, __LINE__ ,3))
#	else
#		define MYTRACE			__noop
#	endif
#	ifdef PSFRAMEWORK_SYSTEM_DBG
#		define MYSYSTRACE (PSLogHelper( __FILE__, __LINE__,2 ))
#	else
#		define MYSYSTRACE		__noop
#	endif
#	ifdef PSFRAMEWORK_SYSDETAIL_DBG
#		define MYSYSDETAILTRACE (PSLogHelper( __FILE__, __LINE__,1 ))
#	else
#		define MYSYSDETAILTRACE __noop
#	endif
//#else
//#	define MYTRACE			__noop
//#	define MYSYSTRACE		__noop
//#	define MYSYSDETAILTRACE __noop
//#endif


#ifndef ASSERT
#	ifdef _DEBUG
#		define ASSERT(x)	if((x)==NULL){MYTRACE("ASSERT Error: ("#x")가 True가 아닙니다. 계산값:%d",(x));}
#	else
#		define ASSERT(x)     __noop
#	endif
#endif




//아래 클래스는 다음 사이트를 참조하였습니다.http://www.codeproject.com/debug/cprofile.asp
enum LOGTYPE { LOGNONE, LOGTICKS, LOGSECS, LOGALL, LOGMSGBOX };
class PSProfiler
{
public:	
	PSProfiler(void);
	~PSProfiler(void);

	void		ProfileStart(LOGTYPE logtype = LOGNONE); // starts profiling
	__int64		ProfileEnd  (char* TraceStr = "");       // end profiling										         
	double		SecsFromTicks ( __int64 ticks);
	DWORD		GetLastRetVal(void);	// returns the last error, if any.		
private:	
	LARGE_INTEGER	m_QPFrequency;		// ticks/sec resolution
	LARGE_INTEGER	m_StartCounter;		// start time
	LARGE_INTEGER	m_EndCounter;		// finish time
	__int64			m_ElapsedTime;		// elapsed time
	DWORD			m_Retval;			// return value for API functions
	LOGTYPE			m_LogType;			// logging type
};

////////////////////////////////////////////////////////////////////
//
//
//                        PSFrameWork 클래스
//
//
////////////////////////////////////////////////////////////////////

class PSFrameWork
{
public:	
	~PSFrameWork(void);
	int initialize();
	int finalize();
	int deleteInstance(PSInstance *);
	int deleteALLInstance();	
	PSInstance* createInstance(const char *ip,unsigned short port);
	PSInstance* createInstance(SOCKET clientSocket);
	static PSFrameWork* Instance();
	set<PSInstance*> instanceSet;
protected:
	PSFrameWork(void);
private:
	static PSFrameWork *_Instance;
	
};


