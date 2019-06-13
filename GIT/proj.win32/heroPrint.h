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
	int blood;
	int maxBoold;
	int needExprience;
	int Aggressivity;
	int Armor;
	float attack_speed;
	int magic;
	int magic_resistance;
	int speed;
	int range;
	int level;
	int exprience;
	int skillNum = 0;
	bool skill_Q = 0;
	bool skill_W = 0;
	bool skill_E = 0;
	bool skill_R = 0;
	int Q = 0;
	int R = 0;
	int level_Q = 1;
	int level_W = 0;
	int level_E = 1;
	int level_R = 0;
	void heroAttack(Point pos, Point Epos);
	void initHeroSprite(int direction, cocos2d::Point currentPosition);
	void heroMoveTo(cocos2d::Point final);
	int getDirection(cocos2d::Point start, cocos2d::Point final);
	float getDistance(cocos2d::Point start, cocos2d::Point final);
	Point herosPosition();
	cocos2d::Animate* createAnimate(int direction, const char* action, int num,int id);
	void heroResume();
	bool used = 0;
	void finishRunAction();
	CREATE_FUNC(heroPrint);



};
