/*******************************************
*CPlayer by Pureku(Gigon Bae)
*Date:2005.12.23
*
********************************************/
#pragma once
#include "Board.h"
#include <list>
#include "MessageStruct.h"
#define HOLE_RANDOM_MAXNUM 10000
//using namespace std;
class CPlayer
{
public:
	enum PLAYMODE{NONE,PERSON,NETWORK};	
	enum EVENT{PUSHLINE};

	struct _stInfo
	{		
		int turnNum;			//몇번째 놓을 차례인가?
		int nowXPos;			//현재 X좌표
		int nowYPos;			//현재 Y좌표
		int nowTurn;			//현재 TURN횟수
		int nowCombo;			//현재 콤포수		
		int holdNum;			//현재 홀드블록번호
		int isOnHold;			//현재 홀드 중인가?
		int holdCount;			//현재 몇번 홀드를 썼는가?
		int lineDeletionCount;	//몇개의 라인을 없앴는가?
		int isBackToBack;		//백투백 테트리스인가?
		int holeRandomNum;		//구멍이 몇번째 구멍인가?
	}playInfo;	
	typedef struct _stEvent
	{
		int eventName;		
		//이벤트에 필요한 데이터. 현재는 이벤트가 라인추가밖에 없음
		//union _data{
			//int line;			
		//}data;
		int numOfLines;
		BYTE posX[5];
		
	}stEvent;
	
	CPlayer(void);
	~CPlayer(void);
	static const double keyBoardInterval;				//키가 입력되고 다음 repeat상태가 되기까지 걸리는 시간.
	static const double keyBoardRepeatInterval;			//repeat모드일때에 다음 키 입력때까지 대기시간
	static const double dropInterval;					//블록이 놓여진후 다음 키 입력때까지 대기시간
	static const double holdInterval;					//블록이 놓여진후 다음 키 입력때까지 대기시간
	static const BYTE keyPad[2][2][6];
	static BYTE holeRandom[HOLE_RANDOM_MAXNUM];


	void setPSInstance(PSInstance *instance){psInstance=instance;}
	PSInstance* getPSInstance(){return psInstance;}
	void setPlayerName(char *name)
	{
		memset(playerName,0,256);
		memcpy(playerName,name,254);
	}
	WCHAR* getPlayerName(){return playerName;}
	void setPlayMode(int value,int playerNum,CPlayer *targetPlayer, int kType=0);	
	int getPlayMode(){return playMode;}	
	bool getIsAlive(){return isAlive;}
//	CRITICAL_SECTION getCriticalSection(){return criticalSection;}
	static void initializeHoleRandom();
	void initialize(int pMode);
	void process(double nowTime);
	void draw(IDirect3DDevice9* pd3dDevice,double eTime);	
	void eventProcess(double eTime);
	void keyboardProcess(double eTime);
	void networkProcess(double eTime);
	void sendRequestMessage();
	static UINT WINAPI WaitNetworkPlayer(LPVOID pVoid);
	stEvent makePushEvent(int lines,bool spread=true);

	void drawBlock(IDirect3DDevice9* pd3dDevice,int x,int y,int startY,int index,unsigned char alpha=255);
	void drawMiniBlock(IDirect3DDevice9* pd3dDevice,int x,int y,int index);
	void drawBoard(IDirect3DDevice9* pd3dDevice);
	void drawEffect(IDirect3DDevice9* pd3dDevice,double nowTime);


	void setIsAlive(bool value){isAlive=value;}
	void setDead();
	void setNextBlock(double nowTime);

	std::list<stEvent>* getEventQueue(){return &eventQueue;}
	
private:
	CBoard board;
	PSInstance *psInstance;
	WCHAR playerName[128];					//플레이어 이름
	WCHAR message[128];						//메시지
	int playMode;							//플레이 모드.
	int playerNum;
	int kbdType;							//키보드 타입(0이면 1인용, 1이면 2인용)
	CPlayer *targetPlayer;
	std::list<stEvent> eventQueue;				//이벤트큐	
	volatile bool isAlive;							//살아있나?	
	float playStartTime;					//플레이 시작시간.
	float lastDownTime;						//마지막에 한칸 아래로 떨어졌을 때 시간
	float lastKeyboardTime[6];				//이전에 키가 입력되었을때의 elapsedTime값;
	float keyboardInputLimitTime[6];		//다음 키 입력받기까지 얼마나 기다려야 하나?
	bool  isRepeated[6];					//반복되고 있나?
	bool  isKeyDown[6];						//키가 눌러졌나?

	volatile bool  isMessageWait;					//네트워크 메시지를 기다리고 있는가?
	volatile bool isMessageReceived;				//메시지를 받았는가?
	bool  isAutoMoving;						//블록을 내리고 있는 중인가?
	float messageSendedTime;				//메시지를 보낸 시간.
	float messageReceivedTime;				//메시지를 받은 시간.
	volatile DECISION_INFO decisionInfo;
	//CRITICAL_SECTION	criticalSection;		//크리티컬 섹션

};
