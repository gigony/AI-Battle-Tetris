/*******************************************
*CBoard by Pureku(Gigon Bae)
*Date:2005.12.23
*
********************************************/
#pragma once

class CBoard
{
public:
	enum BLOCKNAME{BLOCK_NULL,BLOCK_1,BLOCK_2,BLOCK_3,BLOCK_4,BLOCK_5,BLOCK_6,BLOCK_7,BLOCK_DEAD};
	enum CHKBLOCK{ONWALL,ONCOLLISION,ONNOTHING};
	CBoard(void);
	~CBoard(void);
	static void init();
	void operator =(CBoard& Tb);
	void clearBoard();
	int chkBlock(int x,int y,int blockNum,int turn);
	int chkPutY(int x,int y,int blockNum,int turn);
	int putBlock(int x,int YY,int blockNum, int turn,bool option=false);
	void pushLine(int lineNum,BYTE *posX);
	int deleteFilledLine();				//꽉찬줄을 지우고 몇줄이나 지워졌는지 리턴.
	int countFilledLine();				//꽉찬줄을 카운트한다.		

	static unsigned char blockData[8][4][16];	
	static unsigned char blockDataX[8][4][4];
	static unsigned char blockDataY[8][4][4];
	static double midPointY[8][4];
	static unsigned int maxTurn[8];	
	static POINT boardPosition[2];
	static POINT nextBlockPosition[2][6];
	static POINT holdBlockPosition[2];
	//초기 보드
	static unsigned char initialBoard[10][22][10];
	static int numOfInitialBoard;
	

//private:	
	unsigned char board[22][10];			//보드값
	int currentTop;
	bool deletedLine[22];				//지워진 라인(deleteFilledLine에서 사용)
};

