#pragma once
#include "cocos2d.h"
USING_NS_CC;
class heroPrint :public cocos2d::CCNode 
{
public:

	int heroNumber;
	int direction;
	cocos2d::Point currentPosition;
	cocos2d::Sprite* heroSprite;
	void initHeroSprite(int direction, cocos2d::Point currentPosition);
	void heroMoveTo(cocos2d::Point final);
	int getDirection(cocos2d::Point start, cocos2d::Point final);
	float getDistance(cocos2d::Point start, cocos2d::Point final);
	Point heroPrint::herosPosition();
	cocos2d::Animate* createAnimate(int direction, const char *action, int num);
	void heroResume();
	CREATE_FUNC(heroPrint);

	
	
};

