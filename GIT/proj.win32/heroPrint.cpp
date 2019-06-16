#include "heroPrint.h"
#include "object.h"
#include "GLOBAL.h"
void heroPrint::initHeroSprite(int direction, cocos2d::Point currentPosition)
{
	this->currentPosition = currentPosition;
	heroSprite = Sprite::create("hero_1_stand11.png");
	heroSprite->setScale(1.0f);
	heroSprite->setPosition(currentPosition);
	progressSprite = Sprite::create("UI/BloodLine.png");
	Loading = ProgressTimer::create(progressSprite);
	addChild(Loading);
	Loading->setType(kCCProgressTimerTypeBar);
	Loading->setScale(0.35f);
	Loading->setMidpoint(Vec2(0, 0));
	Loading->setBarChangeRate(Vec2(1.0, 0));
	Loading->setPercentage(100);


	expsp = Sprite::create("UI/level.png");
	expcir = ProgressTimer::create(expsp);
	addChild(expcir);
	expcir->setType(kCCProgressTimerTypeRadial);
	expcir->setScale(0.35f);
	expcir->setPercentage(0);


	CCString* levCCStr = CCString::createWithFormat("%d", level);
	std::string levStr = levCCStr->getCString();
	lev = Label::create(levStr, "Arial", 30);
	addChild(lev);
	
	if (MyHeroID == 1)
	{
		maxBlood = 500;
		needExprience = 40;
		Aggressivity = 50;
		Armor = 200;
		attack_speed = 1.0;
		magic = 0;
		range = 75;
		exprience = 0;
		blood = 500; level = 1; speed = 75;
	}
	if (MyHeroID == 2)
	{
		maxBlood = 500;
		needExprience = 40;
		Aggressivity = 50;
		Armor = 200;
		attack_speed = 1.0;
		magic = 0;
		range = 200;
		exprience = 0;
		blood = 500; level = 1; speed = 75;
	}
	if (MyHeroID == 3)
	{
		maxBlood = 500;
		needExprience = 40;
		Aggressivity = 50;
		Armor = 200;
		attack_speed = 1.0;
		magic = 0;
		magic_resistance = 200;
		range = 200;
		exprience = 0;
		blood = 500; level = 1; speed = 75;
	}
	addChild(heroSprite);


	Loading->setPosition(Vec2(herosPosition().x, herosPosition().y + 50));
	expcir->setPosition(Vec2(herosPosition().x-50,herosPosition().y + 50));
	lev->setPosition(Vec2(herosPosition().x - 50, herosPosition().y + 50));
	heroSprite->runAction(this->createAnimate(direction, "stand", 7,MyHeroID));
}


Animate* heroPrint::createAnimate(int direction, const char* action, int num,int id)
{
	auto* frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("heroh.plist", "heroh.png");
	Vector <SpriteFrame*> frameArray;
	for (int i = 1; i <= num; i++)
	{
		auto* frame = frameCache->getSpriteFrameByName(String::createWithFormat("hero_%d_%s%d%d.png",id, action, direction, i)->getCString());
		frameArray.pushBack(frame);
	}
	Animation* animation = Animation::createWithSpriteFrames(frameArray);
	animation->setLoops(-1);
	animation->setDelayPerUnit(0.1f);
	return Animate::create(animation);
}
int heroPrint::getDirection(cocos2d::Point start, cocos2d::Point final)
{
	float x = final.x - start.x;
	float y = final.y - start.y;
	float r = sqrt(x * x + y * y);
	if (x > 0 && x * x / (y * y) > 10)
	{
		CCLOG("RIGHT");
		this->direction = 8;
	}
	else if (x < 0 && x * x / (y * y)>10)
	{
		CCLOG("LEFT");
		this->direction = 6;
	}
	else if (y > 0 && y * y / (x * x) > 10)
	{
		CCLOG("UP");
		this->direction = 7;
	}
	else if (y < 0 && y * y / (x * x)  >10)
	{
		CCLOG("DOWN");
		this->direction = 5;
	}
	else if (x > 0 && y > 0)
	{
		CCLOG("RIGHT-UP");
		this->direction = 4;
	}
	else if (x > 0 && y < 0)
	{
		CCLOG("RIGHT-DOWN");
		this->direction = 1;
	}
	else if (x < 0 && y < 0)
	{
		CCLOG("LEFT-DOWN");
		this->direction = 2;
	}
	else if (x < 0 && y > 0)
	{
		CCLOG("LEFT-UP");
		this->direction = 3;
	}
	return this->direction;
}
float heroPrint::getDistance(cocos2d::Point pos1, cocos2d::Point pos2)
{
	float x = pos1.x - pos2.x;
	float y = pos1.y - pos2.y;
	return sqrt(x * x + y * y);
}

void heroPrint::heroMoveTo(cocos2d::Point position)
{
	heroSprite->stopAllActions();
	this->currentPosition = heroSprite->getPosition();
	float distance = getDistance(this->currentPosition, position);
	auto* animate = createAnimate(getDirection(this->currentPosition, position), "run", 7,MyHeroID);
	auto* move = MoveTo::create((float)distance / speed, position);
	auto* callFunc = CallFunc::create(CC_CALLBACK_0(heroPrint::heroResume, this));
	auto* sequence = Sequence::create(move, callFunc, NULL);
	heroSprite->runAction(animate);
	heroSprite->runAction(sequence);
}
Point heroPrint::herosPosition()
{
	Vec2 hero_position = heroSprite->getPosition();
	return hero_position;
}
void heroPrint::heroAttack(Point pos, Point Epos)
{
	heroSprite->stopAllActions();
	if (used == 1) return;
	float distance = getDistance(pos, Epos);
	auto delayTime = DelayTime::create(1.0f);
	auto hit = Sprite::create("AA.jpg");
	hit->setPosition(pos);
	addChild(hit);
	used = 1;
	auto * move = MoveTo::create((float)distance / 500, Epos);
	auto * remove = MoveTo::create(0, Vec2(-2000, -2000));
	auto funcall2 = CallFunc::create(CC_CALLBACK_0(heroPrint::finishRunAction, this));
	auto * sequence = Sequence::create(move, remove, delayTime, funcall2, NULL);
	hit->runAction(sequence);
}
void heroPrint::finishRunAction()
{
	used = false;
}

void heroPrint::heroResume()
{
	heroSprite->stopAllActions();
	heroSprite->runAction(createAnimate(this->direction, "stand", 7,MyHeroID));
}
