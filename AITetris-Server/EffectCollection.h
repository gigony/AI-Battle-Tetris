/*******************************************
*CEffectCollection by Pureku(Gigon Bae)
*Date:2005.12.27
*
********************************************/
#pragma once
#include <VECTOR>
#include <list>
//using namespace std;
class CEffectCollection
{
public:

	typedef struct _stFrame
	{		
		RECT rect;
		int relateX;
		int relateY;
		int alpha;
	}stFrame;	
	typedef struct _stAnimation
	{
		double duration;
		int size;
		int spriteNum;
		std::vector<stFrame> frame;
	}stAnimation;
	typedef struct _stEffectDat
	{
		int x;
		int y;
		double startTime;
		stAnimation animation;
	}stEffectDat;

	CEffectCollection(void);
	~CEffectCollection(void);

	void drawEffect(IDirect3DDevice9* pd3dDevice,double nowTime);
	void insertEffect(int animationNum,int x,int y, double nowTime);


	std::vector<stAnimation> collection; 

	std::list<stEffectDat> effectList;

	
};
