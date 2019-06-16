#ifndef __ENEMYPLANE_SCENE_H__
#define __ENEMYPLANE_SCENE_H__
#include "cocos2d.h"
#include <vector>
#include "GLOBAL.h"
using namespace std;

USING_NS_CC;
class object : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	int type;
	void start(int type, Vec2 osition);
	int blood;
	int maxBlood;
	int Aggressivity;
	float Armor;
	float attack_speed;
	int magic;
	float magic_resistance;
	bool kongzhi = 0;;
	int speed;
	int level;
	int range;
	int used;
	int exprience;
	int nowexp=0;
	int maxexp=40;
	int gold;
	int T;
	float greedy=0;
	int NaiBlood=0;
	void enemyDestory();
	Sprite* enemy;
	static object* createObject();
	void objectInit();
	Sprite* chosenEnemy;
	bool attacking;
	virtual bool init();
	//void chosenEnemyForAttack(Sprite* enemy);
	void attactEnemy(Vec2 pos, Vec2 Epos, int num);
	float getDistance(Vec2 pos, Vec2 Epos);
	void finishRunAction(const int num);
	vector<int>use;
	bool used1 = 0;
	Sprite* progressSprite;
	ProgressTimer* Loading;
	Sprite* expsp;
	ProgressTimer* expcir;
	Label* lev;
	CREATE_FUNC(object);
};
#endif