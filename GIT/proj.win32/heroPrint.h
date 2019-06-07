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
	void heroAttack(Point pos, Point Epos);
	void initHeroSprite(int direction, cocos2d::Point currentPosition);
	void heroMoveTo(cocos2d::Point final);
	int getDirection(cocos2d::Point start, cocos2d::Point final);
	float getDistance(cocos2d::Point start, cocos2d::Point final);
	Point herosPosition();
	cocos2d::Animate* createAnimate(int direction, const char* action, int num);
	void heroResume();
	bool used = 0;
	void finishRunAction();
	void shoot(int used, Point pos, Point Epos);
	CREATE_FUNC(heroPrint);
};

