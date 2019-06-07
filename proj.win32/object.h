#ifndef __ENEMYPLANE_SCENE_H__
#define __ENEMYPLANE_SCENE_H__
#include "cocos2d.h"
#include <vector>
using namespace std;

USING_NS_CC;
class object : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	int type;
	void start(int type, Vec2 osition);
	int blood;
	void enemyDestory();
	Sprite* enemy;
	static object* createObject();
	void objectInit();
	Sprite* chosenEnemy;
	bool attacking;
	virtual bool init();
	//void chosenEnemyForAttack(Sprite* enemy);
	void attactEnemy(Vec2 pos, Vec2 Epos,int num);
	float getDistance(Vec2 pos,Vec2 Epos);
	void finishRunAction(const int num);
	vector<int>use;
	bool used1 = 0;
	CREATE_FUNC(object);
};
#endif