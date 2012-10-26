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
#include ".\blockmgr.h"
#include <time.h>
int CBlockMgr::block[2][NUM_OF_BLOCK_CREATION]={0};
CBlockMgr::CBlockMgr(void)
{
	makeBlock();
}

CBlockMgr::~CBlockMgr(void)
{
}
void CBlockMgr::makeBlock()
{	
	srand(GetTickCount());
	generateBlock((int *)block,NUM_OF_BLOCK_CREATION);	
}
void CBlockMgr::generateBlock(int *blockInfo,int n)
{
	char chk[7];	
	memset(chk,0,7);
	int blockCnt=0;
	for(int i=0;i<n;i++)
	{
		if(blockCnt%7==0)
		{
			memset(chk,0,7);
		}
		int pos=rand()%7;
		while(chk[pos]!=0)
			pos=rand()%7;
		chk[pos]=1;
		blockCnt++;
		blockInfo[i]=pos+1;
		blockInfo[NUM_OF_BLOCK_CREATION+i]=pos+1;
	}
}
