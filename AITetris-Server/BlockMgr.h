/*******************************************
*CBlockMgr by Pureku(Gigon Bae)
*Date:2005.12.23
*
********************************************/
#pragma once
#define NUM_OF_BLOCK_CREATION       10000
class CBlockMgr
{
public:
	CBlockMgr(void);
	~CBlockMgr(void);
	static void generateBlock(int *blockInfo,int n);
	static void makeBlock();
	static int getBlock(int playerNum,int index) {return block[playerNum][index%NUM_OF_BLOCK_CREATION];}
	static void setBlock(int playerNum,int index,int value){block[playerNum][index%NUM_OF_BLOCK_CREATION]=value;}


	static int block[2][NUM_OF_BLOCK_CREATION];		//게임에 사용할 블럭..
};
