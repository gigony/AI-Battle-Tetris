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
#include "Game.h"
#include ".\player.h"
#include "BlockMgr.h"
#include "State_Main.h"
#include "State_CustomSetting.h"



extern CGame game;
extern CDXUTTextHelper*		g_txtHelper;
extern CDXUTTextHelper*		g_middleTxtHelper;

const double CPlayer::keyBoardInterval=0.17;		//0.20;
const double CPlayer::keyBoardRepeatInterval=0.028;//0.02;
const double CPlayer::dropInterval=100;
const double CPlayer::holdInterval=0.50;
const BYTE  CPlayer::keyPad[2][2][6]={{{VK_UP,VK_LEFT,VK_RIGHT,VK_DOWN,VK_SPACE,VK_LSHIFT},{VK_UP,VK_LEFT,VK_RIGHT,VK_DOWN,VK_SPACE,VK_LSHIFT}},
{{'R','D','G','F',VK_LSHIFT,'Z'},{VK_UP,VK_LEFT,VK_RIGHT,VK_DOWN,VK_RSHIFT,VK_OEM_2}}};
BYTE CPlayer::holeRandom[HOLE_RANDOM_MAXNUM]={0};


CPlayer::CPlayer(void)
{
	psInstance=NULL;
	playMode=0;
	isAlive=true;
	kbdType=0;
	targetPlayer=NULL;	
	memset(message,0,sizeof(message)); //Netowork����϶��� ���� �ʱ�޽��� ����(��������)
	//InitializeCriticalSection(&criticalSection);
	initialize(PERSON);
}

CPlayer::~CPlayer(void)
{
}

void CPlayer::initializeHoleRandom()
{
	
	srand(GetTickCount());
	for(int i=0;i<HOLE_RANDOM_MAXNUM;i++)
	{
		CPlayer::holeRandom[i]=rand()%10;
	}
			
}
void CPlayer::initialize(int pMode)
{	
	memset(&playInfo,0,sizeof(playInfo));	
	playInfo.turnNum=0;
	playInfo.nowCombo=0;
	playInfo.holdNum=0;
	playInfo.isOnHold=FALSE;
	playInfo.nowXPos=3;
	playInfo.nowYPos=0;
	playInfo.holdCount=0;			
	playInfo.lineDeletionCount=0;	
	playInfo.isBackToBack=FALSE;
	playInfo.holeRandomNum=0;
	//���� �ʱ�ȭ
	//board.clearBoard();
	int selectedBoard=((CState_CustomSetting*)CState_CustomSetting::instance())->GetSelectedMapNo();
	memcpy(board.board,CBoard::initialBoard[selectedBoard],22*10);
	for(int i=0;i<6;i++)
	{
		keyboardInputLimitTime[i]=0;			//���� Ű �Է¹ޱ���� �󸶳� ��ٷ��� �ϳ�?
		lastKeyboardTime[i]=0;
		isRepeated[i]=false;
		isKeyDown[i]=false;		
	}
	lastDownTime=playStartTime=DXUTGetTime();
	lastDownTime+=1; //1�� ������ ��������
	isAlive=true;
	isAutoMoving=false;
	isMessageWait=false;
	isMessageReceived=false;
	messageSendedTime=0;
	messageReceivedTime=0;
	eventQueue.clear();

	//�̸� ����
	switch(pMode)
	{
	case PERSON:
		memset(playerName,0,sizeof(playerName));
		memset(message,0,sizeof(message));
		memcpy(playerName,L"���� �հ���",sizeof(L"���� �հ���"));
		break;	
	case NONE:
		memset(playerName,0,sizeof(playerName));
		memset(message,0,sizeof(message));
		break;
	}
}

void CPlayer::setPlayMode(int value, int num,CPlayer *targetP,int kType /*=true*/)
{
	playMode=value;
	playerNum=num;										//�÷��̾� ��ȣ(0 or 1)
	//if(playMode==PERSON && (targetP!=NULL && targetP->getPlayMode()==PERSON) ) kbdType=0; else kbdType=1;		//Ű���� Ÿ�� ����
	kbdType=kType; //Ű���� Ÿ�� ����
	initialize(value);
	targetPlayer=targetP;								//����� �ּ�
																

}
void CPlayer::eventProcess(double nowTime)
{
	int i,j,k;
	stEvent event;
	std::list<stEvent>::iterator iter;
	if(eventQueue.empty()) return;	

	for(iter=eventQueue.begin();iter!=eventQueue.end();)
	{	

		bool erased=false;
		event=(stEvent)(*iter);
		switch(playMode)
		{
		case PERSON:
		case NETWORK:

			switch(event.eventName)
			{
			case PUSHLINE:		//���μ� �ø���
				//����ȿ��
				game.ogg.Stop(OGG_LINE_ATTACKED);
				game.ogg.Play(OGG_LINE_ATTACKED);
				//��ġ�°� üũ ����
				/*
				for(j=0;j<event.numOfLines;j++)
				{
					for(k=0;k<12;k++)
					{
						if(board.board[j][k]>0)
						{
							if(j-event.numOfLines<0)		//���ƴٸ�
							{
								setDead();			//�׾���.								
							}
							k=12;j=(int)event.numOfLines;//������ ����������.
							break;
						}
					}
				}
				*/
				board.pushLine(event.numOfLines,event.posX);
				//���� ��� �߰��� ���ؼ� ���� �������� ��ϰ� ��ġ�� �ȴٸ� ���� ����� �����ϰ� ������������̵�.
				if(board.chkBlock(playInfo.nowXPos,playInfo.nowYPos,CBlockMgr::getBlock(playerNum,playInfo.turnNum),playInfo.nowTurn)==CBoard::ONCOLLISION)
				{
					setNextBlock(nowTime);
				}
				//ť���� �����.
				eventQueue.erase(iter++);	
				erased=true;
				break;			
			}

			break;

		/*case NETWORK:
			//
			//���濡�� �̺�Ʈ ����
			break;*/
		}

		if(!erased) 
			++iter;

		//���� ��Ʈ��ũ ����� �ѹ��� �ѹ� ���ݿ� ���� �͸� �ö�.
		if(playMode==NETWORK)
			break;
	}
}
void CPlayer::keyboardProcess(double nowTime)
{
	bool value;
	int i,j,temp;
	bool isDropped;
	int deletedLine;

	temp=board.chkBlock(playInfo.nowXPos,playInfo.nowYPos,CBlockMgr::getBlock(playerNum,playInfo.turnNum),playInfo.nowTurn);
	if(temp==CBoard::ONCOLLISION)		//���尡 ������ �״°��.,
	{
		MYTRACE("[%d]### Death by collision (posX=%d turn=%d turnNum=%d blockNum=%d)",playerNum,decisionInfo.posX,decisionInfo.turn,playInfo.turnNum, CBlockMgr::getBlock(playerNum,playInfo.turnNum));
		setDead();		
		return;
	}

	isDropped=false;
	for(i=0;i<6;i++)
	{

		value=DXUTIsKeyDown(keyPad[kbdType][playerNum][i]);
		if(value==false) isRepeated[i]=false;

		//���� �ð��� ������ �ʾҴٸ� �Է¹��� �ʴ´�.
		if(isRepeated[i]==true && nowTime-lastKeyboardTime[i]<=keyboardInputLimitTime[i])
		{			
			continue;
		}
		else
		{
			isKeyDown[i]=isKeyDown[i] | value;
		}

		if(!isKeyDown[i]) continue;



		//ù�Է��� �ణ�� �����̸� �ش�.
		if(isRepeated[i]==false)
		{
			isRepeated[i]=true;
			if(i==0 || i==4) //UPŰ�� �����̽� Ű��� �����̸� ���� �ش�.
				keyboardInputLimitTime[i]=100;
			else
				keyboardInputLimitTime[i]=keyBoardInterval;			//�ణ�� ������
		}
		else
		{
			if(i==3)//Down�̶��
				keyboardInputLimitTime[i]=keyBoardRepeatInterval*1.2; //���� �� �ʰ� ����
			else
				keyboardInputLimitTime[i]=keyBoardRepeatInterval;   //���� ������
		}
		lastKeyboardTime[i]=nowTime;
		isKeyDown[i]=false;


		//Ű���� ó��
		switch(i)
		{
		case 0:/*UP*/				
			temp=(playInfo.nowTurn+1)%4;
			//�����̳� ���������� �̵��ؼ� ���� �� �ִٸ� �׷��� �Ѵ�.
			for(j=0;j<4;j++)
			{
				if(board.chkBlock(playInfo.nowXPos+j,playInfo.nowYPos,CBlockMgr::getBlock(playerNum,playInfo.turnNum),temp)==CBoard::ONNOTHING)
				{
					playInfo.nowTurn=temp;
					playInfo.nowXPos+=j;
					game.ogg.Play(OGG_MOVE);
					break;
				}
				if(board.chkBlock(playInfo.nowXPos-j,playInfo.nowYPos,CBlockMgr::getBlock(playerNum,playInfo.turnNum),temp)==CBoard::ONNOTHING)
				{
					playInfo.nowTurn=temp;
					playInfo.nowXPos-=j;
					game.ogg.Play(OGG_MOVE);
					break;
				}

			}
			break;
		case 1:/*LEFT*/
			if(board.chkBlock(playInfo.nowXPos-1,playInfo.nowYPos,CBlockMgr::getBlock(playerNum,playInfo.turnNum),playInfo.nowTurn)==CBoard::ONNOTHING)
			{
				playInfo.nowXPos--;
				game.ogg.Play(OGG_MOVE);
			}			
			break;
		case 2:/*RIGHT*/
			if(board.chkBlock(playInfo.nowXPos+1,playInfo.nowYPos,CBlockMgr::getBlock(playerNum,playInfo.turnNum),playInfo.nowTurn)==CBoard::ONNOTHING)
			{
				playInfo.nowXPos++;
				game.ogg.Play(OGG_MOVE);
			}
			break;
		case 3:/*DOWN*/		
			playInfo.nowYPos++;
			lastDownTime=nowTime;
			game.ogg.Play(OGG_SOFTDROP);
			break;
		case 4:/*DROP*/		
			isDropped=true;
			keyboardInputLimitTime[i]=dropInterval;
			playInfo.isOnHold=FALSE;			
			break;
		case 5:/*HOLD*/
			if(playInfo.isOnHold)
			{
				game.ogg.Play(OGG_HOLDFAIL);
			}
			else
			{
				playInfo.holdCount++;
				playInfo.isOnHold=TRUE;
				game.ogg.Play(OGG_HOLD);		
				keyboardInputLimitTime[i]=holdInterval;				
				if(playInfo.holdNum==0)
				{					
					playInfo.holdNum=CBlockMgr::getBlock(playerNum,playInfo.turnNum);					
					setNextBlock(nowTime);					
				}
				else
				{
					int holdBlock=CBlockMgr::getBlock(playerNum,playInfo.turnNum);
					CBlockMgr::setBlock(playerNum,playInfo.turnNum,playInfo.holdNum);
					playInfo.holdNum=holdBlock;
					playInfo.turnNum--;
					setNextBlock(nowTime);
					return;
				}
			}
			break;


		}		
		
	}

	//�����ð��� �����ٸ� ������.
	if(nowTime-lastDownTime>1.0) // 1.0�� �ð��� ������ ���� �پ��� ������ ��
	{
		lastDownTime=nowTime;
		playInfo.nowYPos++;
	}


	//�浹�Ͽ��ٸ�.
	temp=board.chkBlock(playInfo.nowXPos,playInfo.nowYPos,CBlockMgr::getBlock(playerNum,playInfo.turnNum),playInfo.nowTurn);
	if(isDropped || temp==CBoard::ONCOLLISION)
	{		
		game.ogg.Stop(OGG_HARDDROP);game.ogg.Play(OGG_HARDDROP);
		//���� ���´�.
		board.putBlock(playInfo.nowXPos,playInfo.nowYPos,CBlockMgr::getBlock(playerNum,playInfo.turnNum),playInfo.nowTurn);
		//���� ���� �����.
		if((deletedLine=board.deleteFilledLine())>0)
		{
			playInfo.lineDeletionCount+=deletedLine;
			switch(deletedLine)
			{
			case 1: game.ogg.Stop(OGG_SINGLE);game.ogg.Play(OGG_SINGLE,false);
					playInfo.isBackToBack=FALSE;
					break;
			case 2: game.ogg.Stop(OGG_DOUBLE);game.ogg.Play(OGG_DOUBLE,false);
					playInfo.isBackToBack=FALSE;
					if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(1,false));//old 0
					break;
			case 3: game.ogg.Stop(OGG_TRIPLE);game.ogg.Play(OGG_TRIPLE,false);					
					playInfo.isBackToBack=FALSE;
					if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(2,false)); //old 1
					break;
			case 4: 
					if(playInfo.isBackToBack)
					{
						game.ogg.Stop(OGG_BACKTOBACK_TETRIS);game.ogg.Play(OGG_BACKTOBACK_TETRIS);
						if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(5,false));//old 4
					}											
					else 
					{
						game.ogg.Stop(OGG_TETRIS);game.ogg.Play(OGG_TETRIS);	
						if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(4,false));//old 3
					}
					playInfo.isBackToBack=TRUE;
					break;
			}
			

			//�޺� �� ����.
			playInfo.nowCombo++;
			//�޺��� ���� ���� �߰�
			if(playInfo.nowCombo>=3 && playInfo.nowCombo<=4)
			{				
				if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(1,false));

			}else if(playInfo.nowCombo>=5 && playInfo.nowCombo<=6){				
				if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(2,false));
			}else if(playInfo.nowCombo>=7 && playInfo.nowCombo<=8){				
				if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(3,false));
			}else if(playInfo.nowCombo>=9){				
				if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(4,false));
			}
			/* old
			if(playInfo.nowCombo>=3 && playInfo.nowCombo<=7)
			{				
				if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(1));

			}else if(playInfo.nowCombo>=8 && playInfo.nowCombo<=13){				
				if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(2));
			}else if(playInfo.nowCombo>=14){				
				if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(3));
			}
			*/
			//�޺� ����ȿ��
			switch(playInfo.nowCombo)
			{
			case 0:
			case 1:break;
			case 2:game.ogg.Stop(OGG_COMBO1);game.ogg.Play(OGG_COMBO1);break;
			case 3:game.ogg.Stop(OGG_COMBO2);game.ogg.Play(OGG_COMBO2);break;
			case 4:game.ogg.Stop(OGG_COMBO3);game.ogg.Play(OGG_COMBO3);break;
			case 5:game.ogg.Stop(OGG_COMBO4);game.ogg.Play(OGG_COMBO4);break;
			case 6:game.ogg.Stop(OGG_COMBO5);game.ogg.Play(OGG_COMBO5);break;
			case 7:game.ogg.Stop(OGG_COMBO6);game.ogg.Play(OGG_COMBO6);break;
			case 8:game.ogg.Stop(OGG_COMBO7);game.ogg.Play(OGG_COMBO7);break;			
			default:game.ogg.Stop(OGG_COMBO8);game.ogg.Play(OGG_COMBO8);break;
			}
			//�޺� �ִϸ��̼�
			if(playInfo.nowCombo>1)
			{	
				if(playInfo.nowCombo<18)
					game.effectCollection.insertEffect(ANI_COMBO2+playInfo.nowCombo-2,CBoard::boardPosition[playerNum].x+30,270,nowTime);
				else
					game.effectCollection.insertEffect(ANI_COMBOWOW,CBoard::boardPosition[playerNum].x+30,270,nowTime);
			}


			//���� �Ϳ� ���� effect�� �ش�			
			for(i=0;i<21;i++)
			{
				if(board.deletedLine[i])	//������ ���ο� ���ؼ� effect�� �ش�.
				{			
					game.effectCollection.insertEffect(ANI_BOMB,CBoard::boardPosition[playerNum].x,CBoard::boardPosition[playerNum].y+i*22,nowTime);
					game.effectCollection.insertEffect(ANI_BOMB,CBoard::boardPosition[playerNum].x+64,CBoard::boardPosition[playerNum].y+i*22,nowTime);
					game.effectCollection.insertEffect(ANI_BOMB,CBoard::boardPosition[playerNum].x+128,CBoard::boardPosition[playerNum].y+i*22,nowTime);
				}
			}

		}
		else //������ �������� �ʾҴٸ�
		{			
			playInfo.nowCombo=0;			//�޺� �ʱ�ȭ.
			if(CBlockMgr::getBlock(playerNum,playInfo.turnNum)==5)
				playInfo.isBackToBack=FALSE;
		}		
		setNextBlock(nowTime);				
	}

}

void CPlayer::networkProcess(double nowTime)
{
	bool value;
	int i,j,temp;
	bool isDropped=false;
	int deletedLine;

	
	//EnterCriticalSection(&criticalSection);

	//����� �������� �ϰ� Ư�� �ð��� ���� ���(�ʹ� ���� ���� �� ����)	
	if(isAutoMoving) 
	{	
		int xx=3;
		int tt=0;		
		if(nowTime-messageSendedTime>CState_CustomSetting::GetLimitRespondTime())
		{
//			MYTRACE("[%d]AUTUMOVE:%d %f %f",playerNum,isAutoMoving?1:0,nowTime-messageSendedTime,CState_CustomSetting::GetLimitRespondTime());
			double remainedProcessTime=nowTime-messageSendedTime-CState_CustomSetting::GetLimitRespondTime();
			int iteration=remainedProcessTime/CState_CustomSetting::GetAutoMoveDelay();

	
			if(!decisionInfo.useHold) //Ȧ�� �����ϴ� ���
			{
				//���� �ε����ٰų� ���� �� ���°��
				if(board.chkBlock(decisionInfo.posX,0,CBlockMgr::getBlock(playerNum,playInfo.turnNum),decisionInfo.turn)!=CBoard::ONNOTHING)
				{
					MYTRACE("[%d]### Death by collision (posX=%d turn=%d turnNum=%d blockNum=%d)",playerNum,decisionInfo.posX,decisionInfo.turn,playInfo.turnNum, CBlockMgr::getBlock(playerNum,playInfo.turnNum));
					setDead(); //�״´�.
					return;
				}
			}
			for(i=0;i<iteration;i++)
			{
				
				if(decisionInfo.useHold)//Ȧ�� ����� ���
				{
					if(playInfo.isOnHold) //����Ҽ� ���µ� ����� ���
					{
						game.ogg.Play(OGG_HOLDFAIL);
						setDead(); //�״´�.
						MessageBox(NULL,L"Invalid use of 'Hold'",L"Rule violation",MB_OK);
						MYTRACE("[%d]### Death by invalid use of 'Hold'(Rule violation)(blockNum=%d)",playerNum, CBlockMgr::getBlock(playerNum,playInfo.turnNum));
						return;
					}
					else
					{
						playInfo.holdCount++;
						playInfo.isOnHold=TRUE;
						game.ogg.Play(OGG_HOLD);
						if(playInfo.holdNum==0)
						{					
							playInfo.holdNum=CBlockMgr::getBlock(playerNum,playInfo.turnNum);
							setNextBlock(nowTime);					
						}
						else
						{
							int holdBlock=CBlockMgr::getBlock(playerNum,playInfo.turnNum);
							CBlockMgr::setBlock(playerNum,playInfo.turnNum,playInfo.holdNum);
							playInfo.holdNum=holdBlock;
							playInfo.turnNum--;
							setNextBlock(nowTime);
							//���� �޽����� �޵��� �Ѵ�.
							
						}
					}
					isAutoMoving=false;
					isMessageWait=false;
					isMessageReceived=false;
					break;
				}
				else //Ȧ�� ����� �ƴ� ���
				{
					if(decisionInfo.posX-xx<0)			
						xx--;
					else if(decisionInfo.posX-xx>0)
						xx++;
					else if(decisionInfo.turn-tt<0)
						tt--;
					else if(decisionInfo.turn-tt>0)
						tt++;
					else
					{
						//���ϴ� ��ġ�� �Ǿ�����
						isDropped=true;
						isAutoMoving=false;
						isMessageWait=false;
						isMessageReceived=false;		
						break;
					
					}
				}
			}
		}
		//������ ��� ������ ����
		playInfo.nowXPos=xx;
		playInfo.nowYPos=0;
		playInfo.nowTurn=tt;
	}
	else
	{
		if(isMessageWait) //������ ��ٸ��� ���̶��
		{
			if(isMessageReceived) //������ �޾Ҵٸ�
			{
				//�� ���̱� �̺�Ʈ�� ó���Ѵ�.
				eventProcess(nowTime);

				isMessageWait=false;
				isAutoMoving=true;
				isMessageReceived=false;
				MYTRACE("[%d]RECEIVED(useHold:%d posX:%d turn:%d)",playerNum,decisionInfo.useHold,decisionInfo.posX,decisionInfo.turn);
				//�޽����� �����Ѵ�
				if(((WCHAR*)decisionInfo.message)[0]!=NULL) //null�� �ƴϸ�(�ٲ�� �ϴ� ��Ȳ�̸�)
				{
					memcpy(message,(char*)decisionInfo.message,sizeof(message));
				}
			}

		}
		else //��û�� �ؾ��� �����
		{
			MYTRACE("[%d]REQUEST:start",playerNum);
			sendRequestMessage();
			isMessageWait=true;
			MYTRACE("[%d]REQUEST:end",playerNum);
			messageSendedTime=nowTime;			
		}
	}
	
	if(isDropped)
	{	
		playInfo.isOnHold=FALSE;
		game.ogg.Stop(OGG_HARDDROP);game.ogg.Play(OGG_HARDDROP);
		//���� ���´�.
		board.putBlock(playInfo.nowXPos,playInfo.nowYPos,CBlockMgr::getBlock(playerNum,playInfo.turnNum),playInfo.nowTurn);
		//���� ���� �����.
		if((deletedLine=board.deleteFilledLine())>0)
		{			
			playInfo.lineDeletionCount+=deletedLine;

			switch(deletedLine)
			{
			case 1: game.ogg.Stop(OGG_SINGLE);game.ogg.Play(OGG_SINGLE,false);
					playInfo.isBackToBack=FALSE;
					break;
			case 2: game.ogg.Stop(OGG_DOUBLE);game.ogg.Play(OGG_DOUBLE,false);
					playInfo.isBackToBack=FALSE;
				   if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(1,false));//old 0
					break;
			case 3: game.ogg.Stop(OGG_TRIPLE);game.ogg.Play(OGG_TRIPLE,false);					
					playInfo.isBackToBack=FALSE;
					if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(2,false));//old 1
					break;
			case 4: 
					if(playInfo.isBackToBack)
					{
						game.ogg.Stop(OGG_BACKTOBACK_TETRIS);game.ogg.Play(OGG_BACKTOBACK_TETRIS);
						if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(5,false));//old 4						
					}											
					else 
					{
						game.ogg.Stop(OGG_TETRIS);game.ogg.Play(OGG_TETRIS);	
						if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(4,false));//old 3
					}
					playInfo.isBackToBack=TRUE;
					break;
			}
			
			

			//�޺� �� ����.
			playInfo.nowCombo++;
			//�޺��� ���� ���� �߰�
			if(playInfo.nowCombo>=3 && playInfo.nowCombo<=4)
			{				
				if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(1,false));

			}else if(playInfo.nowCombo>=5 && playInfo.nowCombo<=6){				
				if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(2,false));
			}else if(playInfo.nowCombo>=7 && playInfo.nowCombo<=8){				
				if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(3,false));
			}else if(playInfo.nowCombo>=9){				
				if(targetPlayer!=NULL) targetPlayer->getEventQueue()->push_back(makePushEvent(4,false));
			}


			//�޺� ����ȿ��
			switch(playInfo.nowCombo)
			{
			case 0:
			case 1:break;
			case 2:game.ogg.Stop(OGG_COMBO1);game.ogg.Play(OGG_COMBO1);break;
			case 3:game.ogg.Stop(OGG_COMBO2);game.ogg.Play(OGG_COMBO2);break;
			case 4:game.ogg.Stop(OGG_COMBO3);game.ogg.Play(OGG_COMBO3);break;
			case 5:game.ogg.Stop(OGG_COMBO4);game.ogg.Play(OGG_COMBO4);break;
			case 6:game.ogg.Stop(OGG_COMBO5);game.ogg.Play(OGG_COMBO5);break;
			case 7:game.ogg.Stop(OGG_COMBO6);game.ogg.Play(OGG_COMBO6);break;
			case 8:game.ogg.Stop(OGG_COMBO7);game.ogg.Play(OGG_COMBO7);break;			
			default:game.ogg.Stop(OGG_COMBO8);game.ogg.Play(OGG_COMBO8);break;
			}
			//�޺� �ִϸ��̼�
			if(playInfo.nowCombo>1)
			{	
				if(playInfo.nowCombo<18)
					game.effectCollection.insertEffect(ANI_COMBO2+playInfo.nowCombo-2,CBoard::boardPosition[playerNum].x+30,270,nowTime);
				else
					game.effectCollection.insertEffect(ANI_COMBOWOW,CBoard::boardPosition[playerNum].x+30,270,nowTime);
			}

			//���� �Ϳ� ���� effect�� �ش�			
			for(i=0;i<21;i++)
			{
				if(board.deletedLine[i])	//������ ���ο� ���ؼ� effect�� �ش�.
				{			
					game.effectCollection.insertEffect(ANI_BOMB,CBoard::boardPosition[playerNum].x,CBoard::boardPosition[playerNum].y+i*22,nowTime);
					game.effectCollection.insertEffect(ANI_BOMB,CBoard::boardPosition[playerNum].x+64,CBoard::boardPosition[playerNum].y+i*22,nowTime);
					game.effectCollection.insertEffect(ANI_BOMB,CBoard::boardPosition[playerNum].x+128,CBoard::boardPosition[playerNum].y+i*22,nowTime);
				}
			}

		}
		else //������ �������� �ʾҴٸ�
		{			
			playInfo.nowCombo=0;			//�޺� �ʱ�ȭ.
			if(CBlockMgr::getBlock(playerNum,playInfo.turnNum)==5)
				playInfo.isBackToBack=FALSE;
		}		
		setNextBlock(nowTime);		

		playInfo.nowXPos=3;
		playInfo.nowYPos=0;
		playInfo.nowTurn=0;
	}

	//LeaveCriticalSection(&criticalSection);

}
void CPlayer::sendRequestMessage()
{
	GAME_INFO message;	
	memcpy(&message.board,&board.board,22*10);
	if(targetPlayer!=NULL)
	{
		memcpy(&message.targetBoard,targetPlayer->board.board,22*10);
	}
	else
	{
		memset(&message.targetBoard,0,22*10);
	}

	for(int num=0;num<7;num++)
	{
		int block=CBlockMgr::getBlock(playerNum,playInfo.turnNum+num);
		message.nextBlock[num]=block;
	}

	message.nowCombo=playInfo.nowCombo;
	message.holdBlockNum=playInfo.holdNum;
	message.isOnHold=playInfo.isOnHold;	
	message.numOfGarbageLines=0;
	memset(message.holePositionX,0,sizeof(message.holePositionX));

	stEvent event;
	std::list<stEvent>::iterator iter;
	iter=eventQueue.begin();
	if(iter!=eventQueue.end())
	{			
		event=(stEvent)(*iter);
		switch(event.eventName)
		{
		case PUSHLINE:		
			message.numOfGarbageLines=event.numOfLines;
			memcpy(message.holePositionX,event.posX,sizeof(message.holePositionX));			
			break;			
		}		
	}	
	psInstance->sendData((const char*)(&message),sizeof(message));

}

UINT WINAPI CPlayer::WaitNetworkPlayer(LPVOID pVoid)
{	
	CPlayer *player=(CPlayer*)pVoid;
	PSInstance *psInstance=player->getPSInstance();	
	MYTRACE("[%d]thread start",player->playerNum);
	
//	MYTRACE("isAlive:%d",player->isAlive?1:0);
	while(player->isAlive)
	{
//		EnterCriticalSection(&player->getCriticalSection());
		if(player->isMessageWait && !player->isMessageReceived)
		{
			MYTRACE("[%d]recvData-start",player->playerNum);
			int ret=psInstance->recvData((char*)&(player->decisionInfo),sizeof(DECISION_INFO));
			MYTRACE("[%d]recvData-end(ret:%d)",player->playerNum,ret);
			if(ret==0)
			{
				player->setDead();
				break;
			}
			player->isMessageReceived=true;
			player->messageReceivedTime=DXUTGetTime();
		}		
		//LeaveCriticalSection(&player->getCriticalSection());
		Sleep(10);
	}
	MYTRACE("[%d]thread end",player->playerNum);
	return 1;
}

CPlayer::stEvent CPlayer::makePushEvent(int lines,bool spread/*=true*/)
{
	stEvent result;
	result.eventName=PUSHLINE;
	result.numOfLines=lines;
	memset(result.posX,0,sizeof(result.posX));
	if(spread)
	{
		for(int i=0;i<lines;i++)
		{		
			result.posX[i]=CPlayer::holeRandom[playInfo.holeRandomNum++];	
		}
	}
	else
	{
		int pos=holeRandom[playInfo.holeRandomNum++];
		for(int i=0;i<lines;i++)
		{
			result.posX[i]=pos;
		}
	}
	return result;
}


void CPlayer::drawBlock(IDirect3DDevice9* pd3dDevice,int x,int y,int startY,int index,unsigned char alpha)
{
	RECT rect={index*22,startY,index*22+22,22};
	game.sprCollection.collection[SPR_MAIN_BLOCK]->draw(pd3dDevice,x,y+startY,D3DCOLOR_ARGB(alpha, 255, 255, 255),&rect);		
}
void CPlayer::drawMiniBlock(IDirect3DDevice9* pd3dDevice,int x,int y,int index)
{
	RECT rect={index*16,0,index*16+16,16};
	game.sprCollection.collection[SPR_MAIN_BLOCK_MINI]->draw(pd3dDevice,x,y,D3DCOLOR_ARGB(255, 255, 255, 255),&rect);		
}
void CPlayer::drawBoard(IDirect3DDevice9* pd3dDevice)
{
	game.M_DXSprite->End();

	g_txtHelper->Begin();
		//�̸� ����ϱ�
		if(playerNum==0)		
			g_txtHelper->SetInsertionPos( 218, 650 );
		else
			g_txtHelper->SetInsertionPos( 730, 650 );
		g_txtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		g_txtHelper->DrawTextLine( playerName );

		//�޽��� ����ϱ�
		if(playerNum==0)		
			g_txtHelper->SetInsertionPos( 158, 560 );
		else
			g_txtHelper->SetInsertionPos( 670, 561 );
		g_txtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
		g_txtHelper->DrawTextLine( message );
	
	g_txtHelper->End();	
	

	
	g_middleTxtHelper->Begin();
		//Ȧ�� Ƚ�� ���
		if(playerNum==0)		
			g_middleTxtHelper->SetInsertionPos( 58, 339 );
		else
			g_middleTxtHelper->SetInsertionPos( 570, 340 );		
		g_middleTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		g_middleTxtHelper->DrawFormattedTextLine(L"%3d", playInfo.holdCount);
		//���� ���ż� ���
		if(playerNum==0)		
			g_middleTxtHelper->SetInsertionPos( 83, 165 );
		else
			g_middleTxtHelper->SetInsertionPos( 595, 166 );		
		g_middleTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		g_middleTxtHelper->DrawFormattedTextLine(L"%3d", playInfo.lineDeletionCount);
		


	g_middleTxtHelper->End();

	game.M_DXSprite->Begin(D3DXSPRITE_ALPHABLEND);

	int i,j,num;
	// ���� �׸���
	for(i=1;i<22;i++)
	{
		for(j=0;j<10;j++)
		{
			if(i!=1)
				drawBlock(pd3dDevice,CBoard::boardPosition[playerNum].x+j*22,CBoard::boardPosition[playerNum].y+i*22,0,board.board[i][j]);
			else
				drawBlock(pd3dDevice,CBoard::boardPosition[playerNum].x+j*22,CBoard::boardPosition[playerNum].y+i*22,16,board.board[i][j]);
		}
	}
	//���� ��� �׸���
	int x,y,dropPosY,value;
	if(isAlive)
	{


		int block=CBlockMgr::getBlock(playerNum,playInfo.turnNum);
		int turn=playInfo.nowTurn;
		int dropMapY=board.chkPutY(playInfo.nowXPos,playInfo.nowYPos,block,turn);
		for(i=0;i<4;i++)
		{
			
			int px=CBoard::blockDataX[block][turn][i];
			int py=CBoard::blockDataY[block][turn][i];
			value=CBoard::blockData[block][turn][py*4+px];
			if(value>0)
			{
				x=CBoard::boardPosition[playerNum].x+(playInfo.nowXPos+px)*22;
				y=CBoard::boardPosition[playerNum].y+(playInfo.nowYPos+py)*22;
				dropPosY=CBoard::boardPosition[playerNum].y+(dropMapY+py)*22;
				int mapYPos=playInfo.nowYPos+py;
				if(mapYPos>1)
					drawBlock(pd3dDevice,x,y,0,value);				
				else if(mapYPos==1)
					drawBlock(pd3dDevice,x,y,16,value);
				if(dropMapY>=2) 
					drawBlock(pd3dDevice,x,dropPosY,0,value,70);
				
			}
		}

		
	}

	//���� ��� �׸���
	for(num=0;num<6;num++)
	{
		int block=CBlockMgr::getBlock(playerNum,playInfo.turnNum+num+1);
		int turn=playInfo.nowTurn;
		for(i=0;i<4;i++)
		{			
			int px=CBoard::blockDataX[block][0][i];
			int py=CBoard::blockDataY[block][0][i];

			value=CBoard::blockData[block][0][py*4+px];			
			if(value>0)
			{					
				x=CBoard::nextBlockPosition[playerNum][num].x+px*16;
				y=CBoard::nextBlockPosition[playerNum][num].y+py*16;

				//��翡 ���� ��� ������ ����
				switch(value) {
					case 3: x+=0;y+=0;	
						break;
					case 2:
					case 1:
					case 4:
					case 6:
					case 7:x+=8;y+=0;	
						break;
					case 5:y+=-8;	
						break;											
					default: ;
				}
				drawMiniBlock(pd3dDevice,x,y,value);
			}
		}
	}
	//Ȧ�� ��� �׸���
	int holdBlock=playInfo.holdNum;
	int turn=playInfo.nowTurn;
	if(holdBlock!=-1)
	{
		for(i=0;i<4;i++)
		{		
			int px=CBoard::blockDataX[holdBlock][0][i];
			int py=CBoard::blockDataY[holdBlock][0][i];
			value=CBoard::blockData[holdBlock][0][py*4+px];			
			if(value>0)
			{					
				x=CBoard::holdBlockPosition[playerNum].x+px*16;
				y=CBoard::holdBlockPosition[playerNum].y+py*16;

				//��翡 ���� ��� ������ ����
				switch(value) {
						case 3: x+=0;y+=0;	
							break;
						case 2:
						case 1:
						case 4:
						case 6:
						case 7:x+=8;y+=0;	
							break;
						case 5:y+=-8;	
							break;											
						default: ;
				}
				drawMiniBlock(pd3dDevice,x,y,value);
			}
		}
	}
}
void CPlayer::drawEffect(IDirect3DDevice9* pd3dDevice,double eTime)
{
	
}

void CPlayer::process(double eTime)
{	
	//�÷��̾ ���ٸ� ����
	if(playMode==NONE)
		return;

	//����ִ� ����
	if(isAlive)
	{
		
		if(playMode==PERSON)
		{
			eventProcess(eTime);
			keyboardProcess(eTime);//Ű���峪 �ð��� ���� ó���� �Ѵ�.
		}
		else
			networkProcess(eTime); //��Ʈ��ũ ���� ó���� �Ѵ�.
	}
}
void CPlayer::draw(IDirect3DDevice9* pd3dDevice,double eTime)
{
	//�׷��ֱ�
	drawBoard(pd3dDevice);

	//����Ʈ�ֱ�
	drawEffect(pd3dDevice,eTime);
}

void CPlayer::setDead()
{
	int i,j;
	for(i=1;i<22;i++)
		for(j=0;j<10;j++)
			if(board.board[i][j]>0) board.board[i][j]=8;
	isAlive=false;
	game.ogg.Play(OGG_RETIRE);
}
void CPlayer::setNextBlock(double nowTime)
{
	//�����̽� Ű�� ������ ��� Ű���� interval���� 0��

	for(int i=0;i<4;i++)
	{
		keyboardInputLimitTime[i]=0;
	}
	playInfo.nowXPos=3;
	playInfo.nowYPos=0;
	playInfo.nowTurn=0;
	lastDownTime=nowTime;
	playInfo.turnNum++;
}