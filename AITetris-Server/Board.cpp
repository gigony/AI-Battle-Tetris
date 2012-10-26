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
#include ".\board.h"
#include <fstream>
#include <iostream>
using namespace std;

POINT CBoard::boardPosition[2]={{151,104-44},{663,105-44}};
POINT CBoard::nextBlockPosition[2][6]={{{403,158},{403,260},{403,321},{403,382},{403,443},{403,504}},{{915,159},{915,261},{915,322},{915,383},{915,444},{915,505}}};
POINT CBoard::holdBlockPosition[2]={{49,284},{561,285}};

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
double CBoard::midPointY[8][4]={0.0};
unsigned char CBoard::initialBoard[10][22][10]={0};
int CBoard::numOfInitialBoard=0;




CBoard::CBoard(void)
{
	currentTop=0;
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
void CBoard::init()
{
	int i,j,k,l;
	int value;
	int N;

	ifstream fin("board.dat");
	if(!fin.fail())
	{
		fin>>N;
		numOfInitialBoard=N;

		for(int i=0;i<N;i++)
		{
			for(j=0;j<22;j++)
			{
				for(k=0;k<10;k++)
				{					
					fin>>value;
					initialBoard[i][j][k]=value;
				}
			}
		}
	}
	
	for(i=0;i<8;i++)
	{		
		for(j=0;j<4;j++)
		{
			int minY=4;
			int maxY=-1;
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

						if(minY>k)
							minY=k;
						if(maxY<k)
							maxY=k;
					}
				}
				midPointY[i][j]=(0.5)*(minY+maxY);
		}
	}
}
void CBoard::pushLine(int lineNum,BYTE *pos)
{	
	if(lineNum>21) return;								//push범위를 넘어가면 리턴
	memcpy(board[0],board[lineNum],(22-lineNum)*10);		//맵 이동
	memset(board[22-lineNum],8,lineNum*10);				//push 
	
	for(int i=22-lineNum;i<22;i++)
	{
		int x=pos[i-22+lineNum];
		board[i][x]=0;
	}

}
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
			deletedLine[chkStart]=false;
			for(j=0;j<10;j++)		
			{
				if (board[chkStart][j]==0) {sw=false;break;}
			}
			if(sw)
			{
				deletedLine[chkStart]=true;
				count++;
				chkStart--;
			}
		}while(sw);
		//if(chkStart<=0) break;
		if(chkStart!=fillStart)
		{
			memcpy(board[fillStart],board[chkStart],10);

		}
		fillStart--;
		chkStart--;
	}
	memset(board[0],0,(fillStart+1)*10);  //블럭이 없는 부분을 모두 0으로 채운다.	
	memset(deletedLine,0,(fillStart+1)*sizeof(bool));	//false로 표시
	return count;
}
int CBoard::chkBlock(int x,int y, int blockNum, int turn)
{	

	int j;

	//옆벽에 부딪히는 경우인가를 체크;

	int value;
	for(j=0;j<4;j++)
	{
		value=x+blockDataX[blockNum][turn][j];
		if(value>9 || value<0) return ONWALL;
	}

	/*//움직이는데 걸리는게 있는가?
	int step=(x-3>0)?1:-1;
	for(int posX=3;posX!=x;posX+=step)
	{
		for(int nextTurn=0;nextTurn<turn;nextTurn++)
		{
			for(j=0;j<4;j++)
			{

				int pieceX=posX+blockDataX[blockNum][nextTurn][j];
				int pieceY=2+blockDataY[blockNum][nextTurn][j];
				if(pieceX>=0 && pieceX<=9 && board[pieceY][pieceX]>0)
					return ONCOLLISION;
			}
		}
	}
	*/

	for(j=0;j<4;j++)
	{		
		int pieceX=x+blockDataX[blockNum][turn][j];
		int pieceY=y+blockDataY[blockNum][turn][j];
		if(pieceY>=0 && ((pieceY)>21 || board[pieceY][pieceX]>0))
			return ONCOLLISION;	
	}			
	return ONNOTHING;
}
int CBoard::chkPutY(int x,int y, int blockNum, int turn)
{	
	int j;

	//옆벽에 부딪히는 경우인가를 체크;
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

			if((pieceY)>21 || board[pieceY][pieceX]>0)
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

int CBoard::putBlock(int x,int y, int blockNum, int turn, bool option/*=false*/)
{
	int j;

	//옆벽에 부딪히는 경우인가를 체크;
	int value;
	for(j=0;j<4;j++)
	{
		value=x+blockDataX[blockNum][turn][j];
		if(value>9 || value<0) return 0;
	}

	/*//움직이는데 걸리는게 있는가?
	int step=(x-3>0)?1:-1;
	for(int posX=3;posX!=x;posX+=step)
	{
		for(int nextTurn=0;nextTurn<turn;nextTurn++)
		{
			for(j=0;j<4;j++)
			{
			
				int pieceX=posX+blockDataX[blockNum][nextTurn][j];
				int pieceY=2+blockDataY[blockNum][nextTurn][j];
				if(pieceX>=0 && pieceX<=9 && board[pieceY][pieceX]>0)
					return 0;
			}
		}

	}*/



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

			if((pieceY)>21 || board[pieceY][pieceX]>0)
			{
				sw=true;
				break;
			}
		}			
		
		if(sw)	
		{
//			int value;
			y--;
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