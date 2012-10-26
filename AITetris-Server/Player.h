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
		int turnNum;			//���° ���� �����ΰ�?
		int nowXPos;			//���� X��ǥ
		int nowYPos;			//���� Y��ǥ
		int nowTurn;			//���� TURNȽ��
		int nowCombo;			//���� ������		
		int holdNum;			//���� Ȧ���Ϲ�ȣ
		int isOnHold;			//���� Ȧ�� ���ΰ�?
		int holdCount;			//���� ��� Ȧ�带 ��°�?
		int lineDeletionCount;	//��� ������ ���ݴ°�?
		int isBackToBack;		//������ ��Ʈ�����ΰ�?
		int holeRandomNum;		//������ ���° �����ΰ�?
	}playInfo;	
	typedef struct _stEvent
	{
		int eventName;		
		//�̺�Ʈ�� �ʿ��� ������. ����� �̺�Ʈ�� �����߰��ۿ� ����
		//union _data{
			//int line;			
		//}data;
		int numOfLines;
		BYTE posX[5];
		
	}stEvent;
	
	CPlayer(void);
	~CPlayer(void);
	static const double keyBoardInterval;				//Ű�� �Էµǰ� ���� repeat���°� �Ǳ���� �ɸ��� �ð�.
	static const double keyBoardRepeatInterval;			//repeat����϶��� ���� Ű �Է¶����� ���ð�
	static const double dropInterval;					//����� �������� ���� Ű �Է¶����� ���ð�
	static const double holdInterval;					//����� �������� ���� Ű �Է¶����� ���ð�
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
	WCHAR playerName[128];					//�÷��̾� �̸�
	WCHAR message[128];						//�޽���
	int playMode;							//�÷��� ���.
	int playerNum;
	int kbdType;							//Ű���� Ÿ��(0�̸� 1�ο�, 1�̸� 2�ο�)
	CPlayer *targetPlayer;
	std::list<stEvent> eventQueue;				//�̺�Ʈť	
	volatile bool isAlive;							//����ֳ�?	
	float playStartTime;					//�÷��� ���۽ð�.
	float lastDownTime;						//�������� ��ĭ �Ʒ��� �������� �� �ð�
	float lastKeyboardTime[6];				//������ Ű�� �ԷµǾ������� elapsedTime��;
	float keyboardInputLimitTime[6];		//���� Ű �Է¹ޱ���� �󸶳� ��ٷ��� �ϳ�?
	bool  isRepeated[6];					//�ݺ��ǰ� �ֳ�?
	bool  isKeyDown[6];						//Ű�� ��������?

	volatile bool  isMessageWait;					//��Ʈ��ũ �޽����� ��ٸ��� �ִ°�?
	volatile bool isMessageReceived;				//�޽����� �޾Ҵ°�?
	bool  isAutoMoving;						//����� ������ �ִ� ���ΰ�?
	float messageSendedTime;				//�޽����� ���� �ð�.
	float messageReceivedTime;				//�޽����� ���� �ð�.
	volatile DECISION_INFO decisionInfo;
	//CRITICAL_SECTION	criticalSection;		//ũ��Ƽ�� ����

};
