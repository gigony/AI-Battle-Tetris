#include "StdAfx.h"

AI::AI(void)
{
}

AI::~AI(void)
{
}

DECISION_INFO AI::decision(GAME_INFO gameInfo)
{
	DECISION_INFO result={0};

	CBoard myBoard(gameInfo.board);    //myboard
	CBoard targetBoard(gameInfo.targetBoard);//enemy's board

	//myBoard.printBoard();	
	
	int currentBlock=gameInfo.nextBlock[0];
	int maxTurn=CBoard::maxTurn[currentBlock];

	CBoard tmp;

	int maxScore=-999999999;

	for(int x=-2;x<10;x++) //for possible X
	{		
		for(int turn=0;turn<maxTurn;turn++) //for possible rotation
		{
			tmp=myBoard;			
			int posY=tmp.dropBlock(x,0,currentBlock,turn);
			if(posY==0) continue;

			int filledLine=tmp.countFilledLine();

			int score=calculate(tmp);
			score+=filledLine;
			//printf("[[%d::( %d, %d ) ]]    ",score,x,turn);

			if(score>maxScore)
			{
				maxScore=score;
				result.posX=x;
				result.turn=turn;
				result.useHold=0;
				
			}
		}
	}	
	printf("\n");
	printf("put block on...  posX:%d turn: %d\n",result.posX,result.turn);

	int randomMessage=rand()%4;	
	switch(randomMessage)
	{
	case 0:
		makeMessage(result,L"hello~ \r\nhi~");		
		break;
	case 1:
		makeMessage(result,L"100ÄÞº¸!\r\n1000ÄÞº¸!");
		break;
	case 2:
		//clear message
		makeMessage(result,L" ");		
		break;
	case 3:
		makeMessage(result,NULL);
		//Reuse previous message(if all data is NULL, server uses previous message)
		break;
	}	
	
/*
	myBoard.printBoard();
    System.out.println();
    myBoard.dropBlock(result.posX,0,gameInfo.nextBlock[0],result.turn);
    myBoard.deleteFilledLine();
    myBoard.printBoard();
*/
/*
	result.posX=rand()%10;
	result.turn=rand()%4;
	result.useHold=0;
	memset(result.message,0,sizeof(result.message)); //set all data to NULL
*/	

	return result;
}

int AI::calculate(CBoard &board)
{
	return board.sampleHeuristicValue();
}

void AI::makeMessage(DECISION_INFO& decisionInfo,WCHAR *text)
{
	memset(decisionInfo.message,0,sizeof(decisionInfo.message));
	if(text==NULL)
		return;
	memcpy_s(decisionInfo.message,sizeof(decisionInfo.message),(char*)text,wcslen(text)*2+2);
}