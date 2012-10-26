#pragma once

class CBoard
{
public:
	enum BLOCKNAME{BLOCK_NULL=0,BLOCK_1,BLOCK_2,BLOCK_3,BLOCK_4,BLOCK_5,BLOCK_6,BLOCK_7,BLOCK_DEAD};
	enum CHKBLOCK{NOTHING=0,WALL,COLLISION};
	CBoard(void);
	CBoard(BYTE brd[22][10]);
	~CBoard(void);
	static void init();
	void operator =(CBoard& Tb);
	void clearBoard();									//Clear board
	int chkBlock(int x,int y,int blockNum,int turn);	//Check if block is crashed against
	int chkPutY(int x,int y,int blockNum,int turn);		//Find proper y-position where block can be dropped
	int dropBlock(int x,int y,int blockNum, int turn); //Drop block on board(similar to chkPutY)
	int deleteFilledLine();								//Delete filled line and return # of deleted lines
	int countFilledLine();								//Return # of filled lines
	void printBoard();									//Print board

	//sample
	int sampleHeuristicValue();

	static BYTE blockData[8][4][16];	
	static BYTE blockDataX[8][4][4]; //blockDataX[blockNum][turn][i]=x-position of the block's ith component
	static BYTE blockDataY[8][4][4]; //blockDataX[blockNum][turn][i]=y-position of the block's ith component
	static UINT maxTurn[8];          //all block can rotate 0~3 times, but somtimes rotate makes same shape. Using 'maxTurn' value can reduce search space.

//private:	
	BYTE board[22][10];			//Board	
};
