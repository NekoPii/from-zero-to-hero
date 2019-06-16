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
	int maxBlood;
	int needExprience;
	int Aggressivity;
	float Armor;
	float attack_speed;
	int magic;
	float magic_resistance;
	int speed;
	int range;
	int level;
	int exprience;
	int skillNum = 0;
	bool skill_Q = 0;
	bool skill_W = 0;
	bool skill_E = 0;
	bool skill_R = 0;
	bool zQ;
	int Q = 0;
	int R = 0;
	int E = 0;
	int W = 0;
	int E_object;
	int R_dis;
	int times;
	float greedy = 0;
	int NaiBlood = 0;
	int level_Q = 1;
	int level_W = 1;
	int level_E = 1;
	int level_R = 1;
	void heroAttack(Point pos, Point Epos);
	void initHeroSprite(int direction, cocos2d::Point currentPosition);
	void heroMoveTo(cocos2d::Point final);
	int getDirection(cocos2d::Point start, cocos2d::Point final);
	float getDistance(cocos2d::Point start, cocos2d::Point final);
	Point herosPosition();
	cocos2d::Animate* createAnimate(int direction, const char* action, int num, int id);
	void heroResume();
	bool used = 0;
	void finishRunAction();
	CREATE_FUNC(heroPrint);
	Sprite* progressSprite;
	ProgressTimer* Loading;
	Sprite* expsp;
	ProgressTimer* expcir;
	Label* lev;



};
