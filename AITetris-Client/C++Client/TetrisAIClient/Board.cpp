#include "StdAfx.h"

unsigned char CBoard::blockData[8][4][16]=
{{{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
{{1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
{0,0,1,0,0,1,1,0 ,0,1,0,0,0,0,0,0},
{0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},
{0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0}},
{{0,0,2,0,2,2,2,0,0,0,0,0,0,0,0,0},
{0,2,0,0,0,2,0,0,0,2,2,0,0,0,0,0},
{0,0,0,0,2,2,2,0,2,0,0,0,0,0,0,0},
{2,2,0,0,0,2,0,0,0,2,0,0,0,0,0,0}},
{{0,3,3,0 ,0,3,3,0 ,0,0,0,0,0,0,0,0},
{0,3,3,0 ,0,3,3,0 ,0,0,0,0,0,0,0,0},
{0,3,3,0 ,0,3,3,0 ,0,0,0,0,0,0,0,0},
{0,3,3,0 ,0,3,3,0 ,0,0,0,0,0,0,0,0}},
{{0,4,4,0,4,4,0,0,0,0,0,0,0,0,0,0},
{0,4,0,0 ,0,4,4,0 ,0,0,4,0,0,0,0,0},
{0,0,0,0,0,4,4,0,4,4,0,0,0,0,0,0},
{4,0,0,0,4,4,0,0,0,4,0,0,0,0,0,0}},
{{0,0,0,0,5,5,5,5,0,0,0,0,0,0,0,0},
{0,0,5,0,0,0,5,0,0,0,5,0,0,0,5,0},
{0,0,0,0,0,0,0,0,5,5,5,5,0,0,0,0},
{0,5,0,0,0,5,0,0,0,5,0,0,0,5,0,0}},
{{6,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0},
{0,6,6,0,0,6,0,0,0,6,0,0,0,0,0,0},
{0,0,0,0,6,6,6,0,0,0,6,0,0,0,0,0},
{0,6,0,0,0,6,0,0,6,6,0,0,0,0,0,0}},
{{0,7,0,0,7,7,7,0,0,0,0,0,0,0,0,0},
{0,7,0,0,0,7,7,0,0,7,0,0,0,0,0,0},
{0,0,0,0,7,7,7,0,0,7,0,0,0,0,0,0},
{0,7,0,0,7,7,0,0,0,7,0,0,0,0,0,0}}};
unsigned char CBoard::blockDataX[8][4][4];
unsigned char CBoard::blockDataY[8][4][4];
unsigned int CBoard::maxTurn[8]={0,2,4,1,2,2,4,4};

CBoard::CBoard(void)
{	
	memset(board,0,22*10);
}
CBoard::CBoard(BYTE brd[22][10])
{	
	memcpy(board,brd,22*10);
}

CBoard::~CBoard(void)
{
}
void CBoard::operator =(CBoard& Tb)
{
	memcpy(board,Tb.board,22*10);	
}
void CBoard::clearBoard()
{
	memset(board,0,22*10);
}
//Construct block information(should be called at first)
void CBoard::init()
{
	int i,j,k,l;
	int value;
	for(i=0;i<8;i++)
	{		
		for(j=0;j<4;j++)
		{			
			int count=0;
			for(k=0;k<4;k++)
				for(l=0;l<4;l++)
				{ 					
					value=blockData[i][j][k*4+l];
					if(value>0)
					{
						blockDataX[i][j][count]=l;
						blockDataY[i][j][count]=k;
						count++;
					}
				}
		}
	}
}

//Return # of filled lines
int CBoard::countFilledLine()
{
	int i,j,count=0;
	bool sw;	
	for(i=2;i<22;i++)
	{
		sw=true;
		for(j=0;j<10;j++)		
			if (board[i][j]==0) {sw=false;break;}
			if (sw)	count++;		
	}
	return count;
}

//Delete filled lines and return # of deleted lines
int CBoard::deleteFilledLine()
{
	int j,count=0;
	bool sw;	
	int fillStart,chkStart;
	fillStart=21;chkStart=21;
	while(chkStart>=0)	
	{
		sw=true;
		do
		{			
			
			for(j=0;j<10;j++)		
			{
				if (board[chkStart][j]==0) {sw=false;break;}
			}
			if(sw)
			{			
				count++;
				chkStart--;
			}
		}while(sw);		
		if(chkStart!=fillStart)
		{
			memcpy(board[fillStart],board[chkStart],10);

		}
		fillStart--;
		chkStart--;
	}
	memset(board[0],0,(fillStart+1)*10);  //블럭이 없는 부분을 모두 0으로 채운다.	
	return count;
}
//Check if block is crashed against
int CBoard::chkBlock(int x,int y, int blockNum, int turn)
{	int j;

	
	int value;
	for(j=0;j<4;j++)
	{
		value=x+blockDataX[blockNum][turn][j];
		if(value>9 || value<0) return WALL;
	}
	
	for(j=0;j<4;j++)
	{		
		int pieceX=x+blockDataX[blockNum][turn][j];
		int pieceY=y+blockDataY[blockNum][turn][j];
		if(pieceY>=0 && ((pieceY)>21 || board[pieceY][pieceX]>0))
			return COLLISION;	
	}			
	return NOTHING;
}
//Find proper y-position where block can be dropped
//     ##If you can store the height of dropped blocks in advance, you can save processing time using parameter 'y'
//[return value]
//      0     : can't find proper y-position
//   otherwise: proper y-position where block can be dropped
int CBoard::chkPutY(int x,int y, int blockNum, int turn)
{	
	int j;

	//check if block is crashed against wall
	int value;
	for(j=0;j<4;j++)
	{
		value=x+blockDataX[blockNum][turn][j];
		if(value>9 || value<0) return 0;
	}


	for(j=0;j<4;j++)
	{		
		int pieceX=x+blockDataX[blockNum][turn][j];
		int pieceY=blockDataY[blockNum][turn][j];
		if(pieceY>=0 && board[pieceY][pieceX]>0)
			return 0;	
	}			

	for(;y<=21;y++)
	{
		bool sw;
		sw=false;
		for(j=0;j<4;j++)
		{
		
			int pieceX=x+blockDataX[blockNum][turn][j];
			int pieceY=y+blockDataY[blockNum][turn][j];

			if((pieceY)>21 || board[pieceY][pieceX]>0) // if crashed
			{
				sw=true;
				break;
			}
		}		
		if(sw)	
		{
			y--;			
			return y;
		}	
	}
	return 0;
}

//Drop block on board(similar to chkPutY)
//     ##If you can store the height of dropped blocks in advance, you can save processing time using parameter 'y'
//[return value]
//      0     : can't drop block on board
//   otherwise: proper y-position where block can be dropped
int CBoard::dropBlock(int x,int y, int blockNum, int turn)
{
	int j;	
	int value;
	//side wall crash check
	for(j=0;j<4;j++)
	{
		value=x+blockDataX[blockNum][turn][j];

		if(value>9 || value<0) return 0;
	}

	//crash check
	for(j=0;j<4;j++)
	{		
		int pieceX=x+blockDataX[blockNum][turn][j];
		int pieceY=blockDataY[blockNum][turn][j];
		if(pieceY>=0 && board[pieceY][pieceX]>0)
			return 0;	
	}			

	for(;y<=21;y++)
	{
		bool sw;
		sw=false;
		for(j=0;j<4;j++)
		{
		
			int pieceX=x+blockDataX[blockNum][turn][j];
			int pieceY=y+blockDataY[blockNum][turn][j];

			
			if((pieceY)>21 || board[pieceY][pieceX]>0)// if touched
			{
				sw=true;
				break;
			}
		}			
		
		if(sw)	
		{
			y--;
			//drop block
			for(j=0;j<4;j++)
			{
				int pieceX=blockDataX[blockNum][turn][j];
				int pieceY=blockDataY[blockNum][turn][j];
				
				board[y+pieceY][x+pieceX]=blockNum;
			}
			return y;
		}	
	}
	return 0;
}

void CBoard::printBoard()
{
	for(int i=0;i<22;i++)
	{
		for(int j=0;j<10;j++)
		{
			printf("%d ",board[i][j]);
		}
		printf("\n");
	}
}

int CBoard::sampleHeuristicValue()
{

	int score=0;
	int topPosY=22;
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<22;j++)
		{
			if(board[j][i]!=BLOCK_NULL)
			{
				if(j<topPosY)				
					topPosY=j;

				break;				
			}			
			else 
				score++;
		}		
	}
	return score+topPosY*10;
}