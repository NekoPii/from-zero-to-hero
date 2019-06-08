#include "object.h"
#include <vector>
using namespace std;

object* object::createObject()
{
	object* obj = new object();
	if (obj && obj->init())
	{
		obj->autorelease();
		obj->objectInit();
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return NULL;
}
bool object::init()
{
	if (!Layer::init())
	{
		return false;
	}
	for (int i = 0; i < 50; i++)
	{
		int n = 0;
		use.push_back(n);
	}
	
	return true;
}
void object::objectInit()
{
	this->type = 0;
	enemy = Sprite::create("gold.png");
	enemy->setPosition(Vec2(-100, -100));
	addChild(enemy);
	progressSprite = Sprite::create("Progress.png");
	Loading = ProgressTimer::create(progressSprite);
	addChild(Loading);
	Loading->setType(kCCProgressTimerTypeBar);
	Loading->setScale(0.1f);
	Loading->setMidpoint(Vec2(0, 0));
	Loading->setBarChangeRate(Vec2(1.0, 0));
	Loading->setPercentage(100);
	
	
	
}
void object::start(int type, Vec2 position)
{
	this->type = type;
	if (type == 1)
	{
		
		enemy = Sprite::create("protect.png");
		addChild(enemy);
		enemy->setScale(0.3);
		//enemy->setScale(0.5f);
		blood = 50;
	}
	if (type == 2)
	{
		enemy = Sprite::create("protect.png");
		addChild(enemy);
		enemy->setScale(0.3);
		blood = 50;
	}
	if (type == 3)
	{
	//	auto frame = SpriteFrame::create("solders2.png", Rect(0, 0, 39, 46));
	//	enemy->setDisplayFrame(frame);
		blood = 10;
	}
	if (type == 4)
	{
	//	auto frame = SpriteFrame::create("solders3.png", Rect(0, 0, 39, 46));
	//	enemy->setDisplayFrame(frame);
		blood = 10;
	}

	enemy->setPosition(position);
	Loading->setPosition(Vec2(position.x,position.y+50));
	
}
void object::attactEnemy(Vec2 pos ,Vec2 Epos,int num)
{
	if (use[num] == 1) return;
	CCLOG("%d**   %d", use.size(),use[40]);
	float distance = getDistance(pos, Epos);
	auto delayTime = DelayTime::create(1.0f);
	auto hit = Sprite::create("AA.jpg");
	hit->setScale(1.0f);
	hit->setPosition(pos);
	addChild(hit);
	use[num] = 1;
	const int m = num;
	auto * move = MoveTo::create((float)distance / 2000, Epos);
	auto * remove = MoveTo::create(0, Vec2(-2000, -2000));
	auto funcall1 = CallFunc::create(CC_CALLBACK_0(object::finishRunAction, this, m));
	auto funcall2 = CallFunc::create(CC_CALLBACK_0(object::finishRunAction, this,m));
	auto * sequence = Sequence::create(move, remove, delayTime, funcall2, NULL);
	hit->runAction(sequence);
}
void object::finishRunAction(const int num)
{
	use[num] = 0;
}
float object::getDistance(Vec2 pos, Vec2 Epos)
{
	float x = pos.x - Epos.x;
	float y = pos.y - Epos.y;
	return sqrt(x * x + y * y);
}
void object::enemyDestory()
{
	if (blood > 0)
	{
		blood--;
	}
	else
	{
		// 敌机被取消
		CCLOG("dead");
		// 将其瞬移到屏幕外面，看上去就好像是消失了一样
		enemy->stopAllActions();
		enemy->setPosition(0, -200);
		type = 0;
	}
}