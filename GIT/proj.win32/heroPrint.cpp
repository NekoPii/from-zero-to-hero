#include "heroPrint.h"
#include "object.h"

void heroPrint::initHeroSprite(int direction, cocos2d::Point currentPosition)
{
	this->currentPosition = currentPosition;
	heroSprite = Sprite::create("hero_1_stand11.png");
	heroSprite->setScale(1.0f);
	heroSprite->setPosition(currentPosition);
	addChild(heroSprite);
	heroSprite->runAction(this->createAnimate(direction, "stand", 7));
}



Animate* heroPrint::createAnimate(int direction, const char* action, int num)
{
	auto* frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("hero_1.plist", "hero_1.png");
	Vector <SpriteFrame*> frameArray;
	for (int i = 1; i <= num; i++)
	{
		auto* frame = frameCache->getSpriteFrameByName(String::createWithFormat("hero_1_%s%d%d.png", action, direction, i)->getCString());
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
	auto* animate = createAnimate(getDirection(this->currentPosition, position), "run", 7);
	auto* move = MoveTo::create((float)distance / 200, position);
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
	heroSprite->runAction(createAnimate(this->direction, "stand", 7));
}
