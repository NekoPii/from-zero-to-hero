#include "proj.win32/MapScene.h"
#include "proj.win32/HeroMakeSure1.h"
#include "proj.win32/HeroMakeSure2.h"
#include "proj.win32/HeroMakeSure3.h"
#include "HelloWorldScene.h"
#include "GLOBAL.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "deprecated/CCString.h"
#include "object.h"
#include <vector>
using namespace std;

USING_NS_CC;
USING_NS_CC_EXT;

int current = 0;


Scene *MapScene::createScene()
{
	Scene *scene = Scene::create();
	MapScene *layer = MapScene::create();
	scene->addChild(layer);
	return scene;
}

bool MapScene::init()
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
	use[40] = 1;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//地图
	auto MAP = Sprite::create("map.jpg");
	MAP->setPosition(Vec2(0, 0));
	MAP->setScale(1.0f);
	map = TMXTiledMap::create("mapCan/map5.tmx");
	MAP->setAnchorPoint(Point(0, 0));
	//map->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	//map->setScale(3.0f);
	//map->setPosition(Vec2(0, 0));
	Vec2 pos = map->getPosition();
	CCLOG("%f   %f", pos.x, pos.y);
	pos = MAP->getPosition();
	CCLOG("%f   %f", pos.x, pos.y);
	auto* layer1 = Layer::create();
	layer1->setAnchorPoint(Vec2(0, 0));
	layer1->setPosition((Vec2(0, 0)));
	layer1->addChild(MAP);
	layer1->addChild(map);
	this->addChild(layer1, 0, 1);
	_colliable = map->getLayer("colidable");
	canMove = 0;
	//_colliable->setVisible(false);
	auto protect1 = object::createObject();
	addChild(protect1, 0, 2045);
	protect1->start(1, Vec2(1730, 1020));
	Enemy.pushBack(protect1->enemy);
	chosenEnemy = NULL;
	auto protect2 = object::createObject();
	addChild(protect2, 0, 2044);
	protect2->start(2, Vec2(500, 420));
	Friend.pushBack(protect2->enemy);
	chosenFriend = NULL;
	//游戏返回按钮

	auto pBackButtonItem = MenuItemImage::create("Backbutton.png", "Backbutton.png", CC_CALLBACK_1(MapScene::EnterHelloWorldScene, this));
	auto BackButton = Menu::create(pBackButtonItem, NULL);
	BackButton->setPosition(Vec2(80, visibleSize.height - 30));
	addChild(BackButton, 1);

	//商城图标
	Button *ShopButton = Button::create();
	ShopButton->loadTextures("shop.png", "shop.png", "");
	ShopButton->setPosition(Vec2(50, 500));
	ShopButton->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopcall, this));
	addChild(ShopButton, 9,50); 

	//金币数字
	CCString *goldCCstring=CCString::createWithFormat("%d",Gold);
	std::string goldstring = goldCCstring->getCString();
	Label *GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
	GoldNum->setPosition(Vec2(50,450));
	addChild(GoldNum, 9,55);

	frameCacheH = SpriteFrameCache::getInstance();
	frameCacheH->addSpriteFramesWithFile("heroh.plist", "heroh.png");
	frameCacheS = SpriteFrameCache::getInstance();
	frameCacheS->addSpriteFramesWithFile("heros.plist", "heros.png");
	LBS = Sprite::create("LBS.png");
	addChild(LBS,3);
	hero = new heroPrint();
	hero->initHeroSprite(8, Vec2(860, 540));
	addChild(hero,0,2046);
	Friend.pushBack(hero->heroSprite);

	
	
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto myKeyListener = EventListenerKeyboard::create();	
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	myKeyListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	dispatcher->addEventListenerWithSceneGraphPriority(myKeyListener, this);
	this->schedule(schedule_selector(MapScene::heroIn), 0.1f);
	this->schedule(schedule_selector(MapScene::Tower1), 0.1f);
	this->schedule(schedule_selector(MapScene::Tower2), 0.1f);
	this->schedule(schedule_selector(MapScene::soldersMake), 5.0f);
	this->schedule(schedule_selector(MapScene::soldersMake1), 35.0f);
	this->schedule(schedule_selector(MapScene::soldersMake2), 65.0f);
	this->schedule(schedule_selector(MapScene::soldersContrl), 0.5f);
	this->schedule(schedule_selector(MapScene::mark), 0.1f);
	return true;
}
void MapScene::mark(float dt)
{
	
	if (current == 0)
	{
		auto* movea = MoveTo::create(0.0, Vec2(-200,-200));
		LBS->runAction(movea);
		return;
	}
	else
	{
		object* cur = (object*)getChildByTag(current);
		Vec2 tar_pos = cur->enemy->getPosition();
		auto* moveb = MoveTo::create(0.0,Vec2(tar_pos.x,tar_pos.y+30));
		LBS->runAction(moveb);

	}
}
Animate* MapScene::createAnimateH(int direction, const char* action, int num, int id)
{
	
	Vector <SpriteFrame*> frameArray;
	for (int i = 1; i <= num; i++)
	{
		auto* frame = frameCacheH->getSpriteFrameByName(String::createWithFormat("hero_%d_%s%d%d.png", id, action, direction, i)->getCString());
		frameArray.pushBack(frame);
	}
	Animation* animation = Animation::createWithSpriteFrames(frameArray);
	animation->setLoops(-1);
	animation->setDelayPerUnit(0.1f);
	return Animate::create(animation);
}

Animate* MapScene::createAnimateS(int direction, const char* action, int num, int id,int loop)
{

	Vector <SpriteFrame*> frameArray;
	for (int i = 1; i <= num; i++)
	{
		auto* frame = frameCacheS->getSpriteFrameByName(String::createWithFormat("hero_%d_%s%d%d.png", id, action, direction, i)->getCString());
		frameArray.pushBack(frame);
	}
	Animation* animation = Animation::createWithSpriteFrames(frameArray);
	animation->setLoops(loop);
	animation->setDelayPerUnit(0.1f);
	return Animate::create(animation);
}


bool MapScene::onTouchBegan(Touch* touch, Event* unused_event)
{
	object* protect1 = (object*)getChildByTag(2045);
	int c = protect1->getTag();
	current = 0;
	CCLOG("%d", c);
	//Vec2 solders1=
	hero->getDirection(hero->currentPosition, touch->getLocation());
	Vec2 tiledPositon = tiledpos(touch->getLocation());
	/*Size s = protect1->enemy->getContentSize();
	CCLOG("%f * %f", s.width, s.height);
	Rect rect = Rect(0, 0, s.width, s.height);*/
	int tileGid = _colliable->getTileGIDAt(tiledPositon);
	Vec2 hero_position = hero->herosPosition();
	for (int i = 0; i < Enemy.size(); i++)
	{
		auto a = Enemy.at(i);
		Size s = a->getContentSize();
		CCLOG("%f * %f", s.width, s.height);
		Rect rect = Rect(0, 0, s.width, s.height);
		Vec2 soldersPostion = a->getPosition();
		//Vec2 soldersPostion = protect1->enemy->getPosition();
		if (touch->getLocation().x <= soldersPostion.x + s.width / 2 && touch->getLocation().x >= soldersPostion.x - s.width / 2)//鍒ゆ柇瑙︽懜鐐规槸鍚﹀湪鐩爣鐨勮寖鍥村唴
		{
			if (touch->getLocation().y <= soldersPostion.y + s.height / 2 && touch->getLocation().y >= soldersPostion.y - s.height / 2)
			{
				CCLOG("1");
				float distance = hero->getDistance(hero_position, soldersPostion);
				if (distance <= 200)
				{
					int x = i+1023;
					if (i == 0)
					{
						current = 2045;
					}
					else
					{
						current = x;
					}
				//	CCLOG("%d*****+***", current);
					//hero->heroAttack(hero_position, soldersPostion);
					hero->heroSprite->stopAllActions();
					hero->heroSprite->runAction(createAnimateH(4,"att", 7, HeroID));
					if(i!=0)
					attactEnemy(hero_position, soldersPostion, 49,2046,x);
					if (i == 0)
						attactEnemy(hero_position, soldersPostion, 49, 2046, 2045);
					return false;
				}
			}
		}
	}
	if (canMove == 1 && touch->getLocation() != touchLocation)
	{
		canMove = 2;
	}
	touchLocation = touch->getLocation();
	/*if (tileGid > 0)
		return false;
	else*/
	hero->heroMoveTo(touch->getLocation());
	return false;
}
void MapScene::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event) {
	Vec2 hero_position = hero->herosPosition();
	Layer* layer1 = (Layer*)getChildByTag(1);
	Vec2 layer_position = layer1->getPosition();
	CCLOG("%f %f", layer_position.x, layer_position.y);
	if (keycode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
		CCLOG("按下上键");
		if (layer_position.y <= -1600)
		{return; }
		hero->heroSprite->setPosition(Vec2(hero_position.x, hero_position.y - 100));
		layer1->setPosition(Vec2(layer_position.x, layer_position.y - 100));
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
		CCLOG("按下左键");
		if (layer_position.x >= 2300)
		{return;}
		hero->heroSprite->setPosition(Vec2(hero_position.x + 100, hero_position.y));
		layer1->setPosition(Vec2(layer_position.x + 100, layer_position.y));
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
		CCLOG("按下右键");
		if (layer_position.x <= -2300)
		{return;}
		hero->heroSprite->setPosition(Vec2(hero_position.x - 100, hero_position.y));
		layer1->setPosition(Vec2(layer_position.x - 100, layer_position.y));
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
		if (layer_position.y >= 1600)
		{return;}
		hero->heroSprite->setPosition(Vec2(hero_position.x, hero_position.y + 100));
		layer1->setPosition(Vec2(layer_position.x, layer_position.y + 100));
		CCLOG("按下下键");
	}
}

void MapScene::heroIn(float dt)
{
	Vec2 hero_position = hero->herosPosition();
	Vec2 tiledPositon = tiledpos(hero_position);
	int tileGid = _colliable->getTileGIDAt(tiledPositon);
	CCLOG("%d     ,  ,   %d", tiledPositon.x, tiledPositon.y);
	if (tileGid > 0 && canMove != 2)
	{
		CCLOG("%d     ,  ,   %d", tiledPositon.x, tiledPositon.y);
		hero->heroMoveTo(hero_position);
		canMove = 1;
		return;
	}
	if (canMove == 2 && tileGid <= 0)
	{
		canMove = 0;
	}
	return;
}
void MapScene::Tower1(float dt)
{
	object* protect1 = (object*)getChildByTag(2045);
	Vec2 Tower1_position = protect1->enemy->getPosition();
	int range = 400;
	if (chosenEnemy)
	{
		Vec2 objectPositon = chosenEnemy->getPosition();
		if (getDistance(Tower1_position, objectPositon) <= range)
		{
			int n = 0;
			for (int i = 0; i < Friend.size(); i++)
			{
				auto a = Friend.at(i);
				if (a == chosenEnemy)
				{
					n = i; break;
				}
			}
			if(n==1)
			attactEnemy(Tower1_position, objectPositon, 0,2045,2046);
			if(n>1)
			attactEnemy(Tower1_position, objectPositon, 0, 2045, 2046+n);
			return;
		}
		else
			chosenEnemy = NULL;
	}
	else
	{
		for (int i = Friend.size() - 1; i >= 0; i--)
		{
			auto s = Friend.at(i);
			Vec2 objectPositon = s->getPosition();
			//CCLOG("%f  *** %f", objectPositon.x, objectPositon.y);
			if (getDistance(Tower1_position, objectPositon) <= range)
			{
				chosenEnemy = s;
				if (i == 1)
					attactEnemy(Tower1_position, objectPositon, 0, 2045, 2046);
				if (i > 1)
					attactEnemy(Tower1_position, objectPositon, 0, 2045, 2046 + i);
				return;
			}
		}
	}
}
void MapScene::Tower2(float dt)
{
	object* protect2 = (object*)getChildByTag(2044);
	Vec2 Tower1_position = protect2->enemy->getPosition();
	int range = 400;
	if (chosenFriend)
	{
		Vec2 objectPositon = chosenFriend->getPosition();
		if (getDistance(Tower1_position, objectPositon) <= range)
		{
			int n = 0;
			for (int i = 0; i < Enemy.size(); i++)
			{
				auto a = Enemy.at(i);
				if (a == chosenFriend)
				{
					n = i; break;
				}
			}
			if (n >= 1)
			{
				attactEnemy(Tower1_position, objectPositon, 1, 2044, 1023 + n);
				return;
			}
		}
		else
			chosenFriend = NULL;
	}
	else
	{
		for (int i = Enemy.size() - 1; i >= 0; i--)
		{
			auto s = Enemy.at(i);
			Vec2 objectPositon = s->getPosition();
			//CCLOG("%f  *** %f", objectPositon.x, objectPositon.y,0);
			if (getDistance(Tower1_position, objectPositon) <= range)
			{
				chosenFriend = s;
				attactEnemy(Tower1_position, objectPositon, 1,2044,1023+i);
				return;
			}
		}
	}
}
void MapScene::soldersMake(float dt)
{
	if (time == 0)
	{//500 420
		auto solder1 = object::createObject();
		addChild(solder1, 0, 2048);
		solder1->start(3, Vec2(600, 500));
		auto solder2 = object::createObject();
		addChild(solder2, 0, 2049);
		solder2->start(3, Vec2(650, 450));
		auto solder3 = object::createObject();
		addChild(solder3, 0, 2050);
		solder3->start(3, Vec2(550, 550));
		Friend.pushBack(solder1->enemy);
		Friend.pushBack(solder2->enemy);
		Friend.pushBack(solder3->enemy);
		auto solder11 = object::createObject();
		addChild(solder11, 0, 1024);//1730 1020
		solder11->start(4, Vec2(1600, 900));
		auto solder22 = object::createObject();
		addChild(solder22, 0, 1025);
		solder22->start(4, Vec2(1650, 850));
		auto solder33 = object::createObject();
		addChild(solder33, 0, 1026);
		solder33->start(4, Vec2(1550, 950));
		Enemy.pushBack(solder11->enemy);
		Enemy.pushBack(solder22->enemy);
		Enemy.pushBack(solder33->enemy);
		time++;
	}
}
void MapScene::soldersMake1(float dt)
{
	if (time == 1)
	{
		auto solder4 = object::createObject();
		addChild(solder4, 0, 2051);
		solder4->start(3, Vec2(600, 500));
		auto solder5 = object::createObject();
		addChild(solder5, 0, 2052);
		solder5->start(3, Vec2(650, 450));
		auto solder6 = object::createObject();
		addChild(solder6, 0, 2053);
		solder6->start(3, Vec2(550, 550));
		Friend.pushBack(solder4->enemy);
		Friend.pushBack(solder5->enemy);
		Friend.pushBack(solder6->enemy);
		auto solder44 = object::createObject();
		addChild(solder44, 0, 1027);//1730 1020
		solder44->start(4, Vec2(1600, 900));
		auto solder55 = object::createObject();
		addChild(solder55, 0, 1028);
		solder55->start(4, Vec2(1650, 850));
		auto solder66 = object::createObject();
		addChild(solder66, 0, 1029);
		solder66->start(4, Vec2(1550, 950));
		Enemy.pushBack(solder44->enemy);
		Enemy.pushBack(solder55->enemy);
		Enemy.pushBack(solder66->enemy);
		time++;
	}
}
void MapScene::soldersMake2(float dt)
{
	if (time == 2)
	{
		auto solder7 = object::createObject();
		addChild(solder7, 0, 2054);
		solder7->start(3, Vec2(600, 500));
		auto solder8 = object::createObject();
		addChild(solder8, 0, 2055);
		solder8->start(3, Vec2(650, 450));
		auto solder9 = object::createObject();
		addChild(solder9, 0, 2056);
		solder9->start(3, Vec2(550, 550));
		Friend.pushBack(solder7->enemy);
		Friend.pushBack(solder8->enemy);
		Friend.pushBack(solder9->enemy);
		auto solder77 = object::createObject();
		addChild(solder77, 0, 1030);//1730 1020
		solder77->start(4, Vec2(1600, 900));
		auto solder88 = object::createObject();
		addChild(solder88, 0, 1031);
		solder88->start(4, Vec2(1650, 850));
		auto solder99 = object::createObject();
		addChild(solder99, 0, 1032);
		solder99->start(4, Vec2(1550, 950));
		Enemy.pushBack(solder77->enemy);
		Enemy.pushBack(solder88->enemy);
		Enemy.pushBack(solder99->enemy);
		time++;
	}
}
void MapScene::soldersContrl(float dt)
{


	if (Friend.size() >= 2)
	{
		for (int i = 2; i < Friend.size(); i++)
		{
			auto a = Friend.at(i);
			Size s = a->getContentSize();
			//Rect rect = Rect(0, 0, s.width, s.height);
			object* solder = (object*)getChildByTag(2046 + i);
			auto bloodLine = solder->Loading;
			if (solder->blood <= 0)
				continue;
			Vec2 soldersPostion = a->getPosition();
			if ((i + 1) % 3 == 0)
			{
				float distance = getDistance(soldersPostion, Vec2(1600, 900));
				auto* move = MoveTo::create((float)distance / 50, Vec2(1600, 900));
				auto* moveBlood = MoveTo::create((float)distance / 50, Vec2(1600, 950));
				auto* sequence = Sequence::create(move, NULL);
				int min; float minDis = 200000;
				for (int n = 0; n < Enemy.size(); n++)
				{
					auto b = Enemy.at(n);
					Size t = b->getContentSize();
					float minDistance = getDistance(soldersPostion, b->getPosition());
					if (minDis > minDistance)
					{
						minDis = minDistance;
						min = n;
					}
				}
				if (minDis <= 100)
				{
					a->stopAllActions();
					a->runAction(createAnimateS(4, "att", 14, 4,1));
					bloodLine->stopAllActions();
					if(min!=0)
					attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i,2046+i,1023+min);
					else
						attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i, 2046 + i, 2045);
				}
				if (minDis <= 200 && minDis > 100)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(4,"run", 8, 4,-1));
					float Distance = getDistance(soldersPostion, Enemy.at(min)->getPosition());
					auto* Move = MoveTo::create((float)Distance / 50, Enemy.at(min)->getPosition());
					auto* MoveBlood= MoveTo::create((float)Distance / 50, Vec2(Enemy.at(min)->getPosition().x, Enemy.at(min)->getPosition().y+50));
					a->runAction(Move);
					bloodLine->runAction(MoveBlood);

				}
				if (minDis > 200)
				{
					a->stopAllActions();
					bloodLine-> stopAllActions();
					a->runAction(createAnimateS(4, "run", 8, 4,-1));
					a->runAction(sequence);
					bloodLine->runAction(moveBlood);
				}
			}
			if ((i + 1) % 3 == 1)
			{
				float distance = getDistance(soldersPostion, Vec2(1650, 850));
				auto* move = MoveTo::create((float)distance / 50, Vec2(1650, 850));
				auto* moveBlood = MoveTo::create((float)distance / 50, Vec2(1650, 850));
				auto* sequence = Sequence::create(move, NULL);
				int min; float minDis = 200000;
				for (int n = 0; n < Enemy.size(); n++)
				{
					auto b = Enemy.at(n);
					Size t = b->getContentSize();
					float minDistance = getDistance(soldersPostion, b->getPosition());

					if (minDis > minDistance)
					{
						minDis = minDistance;
						min = n;
					}
				}
				if (minDis <= 100)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(4, "att", 14, 5,1));
					if (min != 0)
						attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i, 2046 + i, 1023 + min);
					else
						attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i, 2046 + i, 2045);
				}
				if (minDis <= 200 && minDis > 100)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(4, "run", 8, 5,-1));
					float Distance = getDistance(soldersPostion, Enemy.at(min)->getPosition());
					auto* Move = MoveTo::create((float)Distance / 50, Enemy.at(min)->getPosition());
					auto* MoveBlood= MoveTo::create((float)Distance / 50, Vec2(Enemy.at(min)->getPosition().x, Enemy.at(min)->getPosition().y+50));
					a->runAction(Move);
					bloodLine->runAction(MoveBlood);

				}
				if (minDis > 200)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(4, "run", 8, 5,-1));
					a->runAction(sequence);
					bloodLine->runAction(moveBlood);
				}
			}
			if ((i + 1) % 3 == 2)
			{
				float distance = getDistance(soldersPostion, Vec2(1550, 950));
				auto* move = MoveTo::create((float)distance / 50, Vec2(1550, 950));
				auto* moveBlood = MoveTo::create((float)distance / 50, Vec2(1550, 950));
				auto* sequence = Sequence::create(move, NULL);
				int min; float minDis = 200000;
				for (int n = 0; n < Enemy.size(); n++)
				{
					auto b = Enemy.at(n);
					Size t = b->getContentSize();
					float minDistance = getDistance(soldersPostion, b->getPosition());
					if (minDis > minDistance)
					{
						minDis = minDistance;
						min = n;
					}
				}
				if (minDis <= 100)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(4, "att", 14, 6,1));
					if (min != 0)
						attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i, 2046 + i, 1023 + min);
					else
						attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i, 2046 + i, 2045);
				}
				if (minDis <= 200 && minDis > 100)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(4, "run", 8, 6,-1));
					float Distance = getDistance(soldersPostion, Enemy.at(min)->getPosition());
					auto* Move = MoveTo::create((float)Distance / 50, Enemy.at(min)->getPosition());
					auto* MoveBlood = MoveTo::create((float)Distance / 50, Vec2(Enemy.at(min)->getPosition().x, Enemy.at(min)->getPosition().y + 50));
					a->runAction(Move);
					bloodLine->runAction(MoveBlood);

				}
				if (minDis > 200)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(4, "run", 8, 6,-1));
					a->runAction(sequence);
					bloodLine->runAction(moveBlood);
				}
			}
		}
	}
	if (Enemy.size() >= 2)
	{
		for (int i = 1; i < Enemy.size(); i++)
		{
			auto a = Enemy.at(i);
			Size s = a->getContentSize();
			//Rect rect = Rect(0, 0, s.width, s.height);
			object* solder = (object*)getChildByTag(1023 + i);
			auto bloodLine = solder->Loading;
			//CCLOG("**%d**", solder->blood);
			if (solder->blood <= 0)
				continue;
			Vec2 soldersPostion = a->getPosition();
			if ((i + 2) % 3 == 0)
			{
				float distance = getDistance(Vec2(600, 500), soldersPostion);
				auto* move = MoveTo::create((float)distance / 50, Vec2(600, 500));
				auto* moveBlood = MoveTo::create((float)distance / 50, Vec2(600, 550));
				auto* sequence = Sequence::create(move, NULL);
				int min; float minDis = 200000;
				for (int n = 0; n < Friend.size(); n++)
				{
					auto b = Friend.at(n);
					Size t = b->getContentSize();
					float minDistance = getDistance(soldersPostion, b->getPosition());
					if (minDis > minDistance)
					{
						minDis = minDistance;
						min = n;
					}
				}
				if (minDis <= 200 && minDis > 100)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(2, "run", 8, 4, -1));
					float Distance = getDistance(soldersPostion, Friend.at(min)->getPosition());
					auto* Move = MoveTo::create((float)Distance / 50, Friend.at(min)->getPosition());
					auto* MoveBlood = MoveTo::create((float)Distance / 50, Vec2(Friend.at(min)->getPosition().x, Friend.at(min)->getPosition().y + 50));
					a->runAction(Move);
					bloodLine->runAction(MoveBlood);

				}
				if (minDis <= 100)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(2, "att", 14, 4, 1));
					if (min > 1)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 1023 + i, 2046 + min);
					if(min==0)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 1023 + i, 2044);
					if(min==1)
					attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 1023 + i, 2046);
				}
				if (minDis > 200)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(2, "run", 8, 4,-1));
					a->runAction(sequence);
					bloodLine->runAction(moveBlood);
				}
			}
			if ((i + 2) % 3 == 1)
			{
				float Distance = getDistance(Vec2(650, 450), soldersPostion);
				auto* move = MoveTo::create((float)Distance / 50, Vec2(650, 450));
				auto* moveBlood = MoveTo::create((float)Distance / 50, Vec2(650, 500));
				auto* sequence = Sequence::create(move, NULL);
				int min; float minDis = 200000;
				for (int n = 0; n < Friend.size(); n++)
				{
					auto b = Friend.at(n);
					Size t = b->getContentSize();
					float minDistance = getDistance(soldersPostion, b->getPosition());
					if (minDis > minDistance)
					{
						minDis = minDistance;
						min = n;
					}
				}
				if (minDis <= 200 && minDis > 100)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(2, "run", 8, 5, -1));
					float distance = getDistance(soldersPostion, Friend.at(min)->getPosition());
					auto* Move = MoveTo::create((float)distance / 50, Friend.at(min)->getPosition());
					auto* MoveBlood = MoveTo::create((float)Distance / 50, Vec2(Friend.at(min)->getPosition().x, Friend.at(min)->getPosition().y + 50));
					a->runAction(Move);
					bloodLine->runAction(MoveBlood);
				}
				if (minDis <= 100)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(2, "att", 14, 5, 1));
					if (min > 1)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 1023 + i, 2046 + min);
					if (min == 0)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 1023 + i, 2044);
					if (min == 1)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 1023 + i, 2046);
				}
				if (minDis > 200)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(2, "run", 8, 5,-1));
					a->runAction(sequence);
					bloodLine->runAction(moveBlood);
				}
			}
			if ((i + 2) % 3 == 2)
			{
				float distance = getDistance(Vec2(550, 550), soldersPostion);
				auto* move = MoveTo::create((float)distance / 50, Vec2(550, 550));
				auto* moveBlood = MoveTo::create((float)distance / 50, Vec2(550, 600));
				auto* sequence = Sequence::create(move, NULL);
				int min; float minDis = 200000;
				for (int n = 0; n < Friend.size(); n++)
				{
					auto b = Friend.at(n);
					Size t = b->getContentSize();
					float minDistance = getDistance(soldersPostion, b->getPosition());
					if (minDis > minDistance)
					{
						minDis = minDistance;
						min = n;
					}
				}
				if (minDis <= 200 && minDis > 100)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(2, "run", 8, 6, -1));
					float Distance = getDistance(soldersPostion, Friend.at(min)->getPosition());
					auto* Move = MoveTo::create((float)Distance / 50, Friend.at(min)->getPosition());
					auto* MoveBlood = MoveTo::create((float)Distance / 50, Vec2(Friend.at(min)->getPosition().x, Friend.at(min)->getPosition().y + 50));
					a->runAction(Move);
					bloodLine->runAction(MoveBlood);
				}
				if (minDis <= 100)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(2, "att", 14, 6, 1));
					bloodLine->stopAllActions();
					if (min > 1)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 1023 + i, 2046 + min);
					if (min == 0)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 1023 + i, 2044);
					if (min == 1)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 1023 + i, 2046);
				}
				if (minDis > 200)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(2, "run", 8, 6,-1));
					a->runAction(sequence);
					bloodLine->runAction(moveBlood);
				}
			}
		}
	}
}
float MapScene::getDistance(Vec2 me, Vec2 you)
{
	float x = me.x - you.x;
	float y = me.y - you.y;
	return sqrt(x * x + y * y);
}

Vec2 MapScene::tiledpos(Vec2 pos)
{
	int x = pos.x / map->getTileSize().width;
	int y = ((map->getTileSize().height * map->getMapSize().height) - pos.y) / map->getTileSize().height;
	return Vec2(x, y);
}
void MapScene::attactEnemy(Vec2 pos, Vec2 Epos, int num,int tag1,int tag2)
{
	if (use[num] == 1) return;
	float distance = getDistance(pos, Epos);
	auto delayTime = DelayTime::create(1.0f);
	auto hit = Sprite::create("AA.jpg");
	hit->setScale(1.0f);
	hit->setPosition(pos);
	addChild(hit);
	use[num] = 1;
	const int m = num;
	const int x = tag1;
	const int y = tag2;
	auto * move = MoveTo::create((float)distance / 1000, Epos);
	auto * remove = MoveTo::create(0, Vec2(-2000, -2000));
	auto funcall1 = CallFunc::create(CC_CALLBACK_0(MapScene::finishAttack, this,x,y ));
	auto funcall2 = CallFunc::create(CC_CALLBACK_0(MapScene::finishRunAction, this, m));
	auto * sequence = Sequence::create(move, remove, funcall1,delayTime, funcall2, NULL);
	hit->runAction(sequence);
}
void MapScene::finishRunAction(const int num)
{
	use[num] = 0;
}

void MapScene::finishAttack(const int tag1, const int tag2)
{

	if (tag1 != 2046)
	{
		object* solder1 = (object*)getChildByTag(tag1);
		//读取属性及计算
		if (tag2 != 2046)
		{
			object* solder2 = (object*)getChildByTag(tag2);
			solder2->blood--;
			float cur = (float)solder2->blood*10;
			solder2->Loading->setPercentage(cur);
			if (solder2->blood <= 0)
				solder2->enemy->setPosition(-2000, -2000);
		}
		else
		{
			heroPrint* hero = (heroPrint*)getChildByTag(2046);
			hero->blood--;
			if (hero->blood <= 0)
				hero->heroSprite->setPosition(960, 540);
		}
	 }
	if(tag1==2046)
	{
		CCLOG("**%d", tag2);
		heroPrint* hero = (heroPrint*)getChildByTag(2046);
		//读取属性及计算
		object* solder2 = (object*)getChildByTag(tag2);
		solder2->blood--;
		if(solder2->blood<=0)
			solder2->enemy->setPosition(-2000, -2000);
	}
}
void MapScene::EnterHelloWorldScene(Ref *pSenderBack)
{
	//全局重新初始化
	HeroID = 0;
	Gold = 10000;
	BuyWeaponNum = 0;
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f,HelloWorld::createScene()));
}


void MapScene::Shopcall(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		Button *tmp = (Button *)this->getChildByTag(50);
		tmp->setTouchEnabled(false);
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		Button *ShopBackButton = Button::create();
		ShopBackButton->loadTextures("ShopBack.png", "ShopBack.png", "");
		ShopBackButton->setPosition(Vec2(1665, 910));
		ShopBackButton->addTouchEventListener(CC_CALLBACK_2(MapScene::ShopBack, this));
		ShopBackButton->setScale(0.72f);
		addChild(ShopBackButton,14,66);

		Sprite *ShopBackGround;
		ShopBackGround = Sprite::create("Shopbackground.png");
		ShopBackGround->setPosition(Vec2(960,550));
		ShopBackGround->setScale(0.85f);
		ShopBackGround->setOpacity(230);
		addChild(ShopBackGround, 10, 60);

		//装备方案图标
		Sprite *EquipPlan = Sprite::create("plan.png");
		EquipPlan->setPosition(Vec2(280,200));
		EquipPlan->setScale(1.0f);
		addChild(EquipPlan, 10, 61);

		//左上角小部件
		Sprite *EquipLittle1 = Sprite::create("little.png");
		EquipLittle1->setPosition(Vec2(200,900));
		EquipLittle1->setScale(1.0f);
		addChild(EquipLittle1, 10, 62);

		//"商城"文字
		auto *chnString = Dictionary::createWithContentsOfFile("CHN_String.xml");
		const char *SHOP_string = ((String *)chnString->objectForKey("SHOP_Text"))->getCString();
		Label *SHOP = Label::create(SHOP_string, "Arial", 33);
		SHOP->setPosition(Vec2(300,900));
		addChild(SHOP, 10, 63);

		//武器一
		const char *Weapon1_string = ((String *)chnString->objectForKey("Weapon1_Text"))->getCString();
		Label *Weapon1String = Label::create(Weapon1_string, "Arial", 38);
		Weapon1String->setPosition(Vec2(750,750));
		addChild(Weapon1String, 12, 1010);

		Button * Weapon1=Button::create();
		Weapon1->loadTextures("weapon1.png", "weapon1.png", "");
		Weapon1->setPosition(Vec2(550, 750));
		Weapon1->setScale(0.8f);
		Weapon1->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy1, this));
		addChild(Weapon1, 12, 101);

		//武器二
		const char *Weapon2_string = ((String *)chnString->objectForKey("Weapon2_Text"))->getCString();
		Label *Weapon2String = Label::create(Weapon2_string, "Arial", 38);
		Weapon2String->setPosition(Vec2(1200, 750));
		addChild(Weapon2String, 12, 1020);

		Button * Weapon2=Button::create();
		Weapon2->loadTextures("weapon2.png", "weapon2.png", "");
		Weapon2->setPosition(Vec2(1000, 750));
		Weapon2->setScale(0.8f);
		Weapon2->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy2, this));
		addChild(Weapon2, 12, 102);

		//武器三
		const char *Weapon3_string = ((String *)chnString->objectForKey("Weapon3_Text"))->getCString();
		Label *Weapon3String = Label::create(Weapon3_string, "Arial", 38);
		Weapon3String->setPosition(Vec2(750, 550));
		addChild(Weapon3String, 12, 1030);

		Button * Weapon3=Button::create();
		Weapon3->loadTextures("weapon3.png", "weapon3.png", "");
		Weapon3->setPosition(Vec2(550, 550));
		Weapon3->setScale(0.8f);
		Weapon3->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy3, this));
		addChild(Weapon3, 12, 103);

		//武器四
		const char *Weapon4_string = ((String *)chnString->objectForKey("Weapon4_Text"))->getCString();
		Label *Weapon4String = Label::create(Weapon4_string, "Arial", 38);
		Weapon4String->setPosition(Vec2(1200, 550));
		addChild(Weapon4String, 12, 1040);

		Button * Weapon4=Button::create();
		Weapon4->loadTextures("weapon4.png", "weapon4.png", "");
		Weapon4->setPosition(Vec2(1000, 550));
		Weapon4->setScale(0.8f);
		Weapon4->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy4, this));
		addChild(Weapon4, 12, 104);
		
		//武器五
		const char *Weapon5_string = ((String *)chnString->objectForKey("Weapon5_Text"))->getCString();
		Label *Weapon5String = Label::create(Weapon5_string, "Arial", 38);
		Weapon5String->setPosition(Vec2(750, 350));
		addChild(Weapon5String, 12, 1050);

		Button * Weapon5=Button::create();
		Weapon5->loadTextures("weapon5.png", "weapon5.png", "");
		Weapon5->setPosition(Vec2(550, 350));
		Weapon5->setScale(0.8f);
		Weapon5->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy5, this));
		addChild(Weapon5, 12, 105);

		//武器六
		const char *Weapon6_string = ((String *)chnString->objectForKey("Weapon6_Text"))->getCString();
		Label *Weapon6String = Label::create(Weapon6_string, "Arial", 38);
		Weapon6String->setPosition(Vec2(1200, 350));
		addChild(Weapon6String, 12, 1060);

		Button * Weapon6=Button::create();
		Weapon6->loadTextures("weapon6.png", "weapon6.png", "");
		Weapon6->setPosition(Vec2(1000, 350));
		Weapon6->setScale(0.8f);
		Weapon6->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy6, this));
		addChild(Weapon6, 12, 106);


		Sprite *Equipment1 = Sprite::create("kong.png");
		Equipment1->setPosition(Vec2(500,200));
		Equipment1->setScale(0.8f);
		addChild(Equipment1, 12, 71);

		Sprite *Equipment2 = Sprite::create("kong.png");
		Equipment2->setPosition(Vec2(650,200));
		Equipment2->setScale(0.8f);
		addChild(Equipment2, 12, 72);

		Sprite *Equipment3 = Sprite::create("kong.png");
		Equipment3->setPosition(Vec2(800,200));
		Equipment3->setScale(0.8f);
		addChild(Equipment3, 12, 73);

		Sprite *Equipment4 = Sprite::create("kong.png");
		Equipment4->setPosition(Vec2(950,200));
		Equipment4->setScale(0.8f);
		addChild(Equipment4, 12, 74);

		Sprite *Equipment5 = Sprite::create("kong.png");
		Equipment5->setPosition(Vec2(1100,200));
		Equipment5->setScale(0.8f);
		addChild(Equipment5, 12, 75);

		Sprite *Equipment6 = Sprite::create("kong.png");
		Equipment6->setPosition(Vec2(1250,200));
		Equipment6->setScale(0.8f);
		addChild(Equipment6, 12, 76);

		for (int p = 1; p <= BuyWeaponNum; ++p)
		{
			Sprite *Tmp = (Sprite *)this->getChildByTag(40+p);
			Tmp->setVisible(true);
		}

	}
}



void MapScene::Shopbuy1(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		removeChildByTag(1011);
		removeChildByTag(1012);
		removeChildByTag(1013);
		removeChildByTag(1021);
		removeChildByTag(1022);
		removeChildByTag(1023);
		removeChildByTag(1031);
		removeChildByTag(1032);
		removeChildByTag(1033);
		removeChildByTag(1041);
		removeChildByTag(1042);
		removeChildByTag(1043);
		removeChildByTag(1051);
		removeChildByTag(1052);
		removeChildByTag(1053);
		removeChildByTag(1061);
		removeChildByTag(1062);
		removeChildByTag(1063);

		Sprite *Weapon1Detail;
		Weapon1Detail = Sprite::create("weapon1detail.png");
		Weapon1Detail->setPosition(Vec2(1653,593));
		Weapon1Detail->setScale(0.7f);
		addChild(Weapon1Detail, 13,1011);

		if (Gold >= 1740&&BuyWeaponNum<=5)
		{
			Button *BuyButton11=Button::create();
			BuyButton11->loadTextures("buy1.png","buy1.png","");
			BuyButton11->setPosition(Vec2(1650,180));
			BuyButton11->setScale(1.0f);
			BuyButton11->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit1, this));
			addChild(BuyButton11, 14, 1012);
		}

		if (Gold < 1740 || BuyWeaponNum >= 6)
		{
			Sprite *BuyButton12;
			BuyButton12 = Sprite::create("buy2.png");
			BuyButton12->setPosition(Vec2(1650, 180));
			BuyButton12->setScale(1.0f);
			addChild(BuyButton12, 14, 1013);
		}

	}
}

void MapScene::Shopbuy2(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		removeChildByTag(1011);
		removeChildByTag(1012);
		removeChildByTag(1013);
		removeChildByTag(1021);
		removeChildByTag(1022);
		removeChildByTag(1023);
		removeChildByTag(1031);
		removeChildByTag(1032);
		removeChildByTag(1033);
		removeChildByTag(1041);
		removeChildByTag(1042);
		removeChildByTag(1043);
		removeChildByTag(1051);
		removeChildByTag(1052);
		removeChildByTag(1053);
		removeChildByTag(1061);
		removeChildByTag(1062);
		removeChildByTag(1063);

		Sprite *Weapon2Detail;
		Weapon2Detail = Sprite::create("weapon2detail.png");
		Weapon2Detail->setPosition(Vec2(1650,590));
		Weapon2Detail->setScale(0.7f);
		addChild(Weapon2Detail, 13,1021);

		if (Gold >= 2140 && BuyWeaponNum <= 5)
		{
			Button *BuyButton21=Button::create();
			BuyButton21->loadTextures("buy1.png", "buy1.png", "");
			BuyButton21->setPosition(Vec2(1650, 180));
			BuyButton21->setScale(1.0f);
			BuyButton21->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit2, this));
			addChild(BuyButton21, 14, 1022);
		}

		if (Gold < 2140 || BuyWeaponNum >= 6)
		{
			Sprite *BuyButton22;
			BuyButton22 = Sprite::create("buy2.png");
			BuyButton22->setPosition(Vec2(1650, 180));
			BuyButton22->setScale(1.0f);
			addChild(BuyButton22, 14, 1023);
		}
	}
}

void MapScene::Shopbuy3(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		removeChildByTag(1011);
		removeChildByTag(1012);
		removeChildByTag(1013);
		removeChildByTag(1021);
		removeChildByTag(1022);
		removeChildByTag(1023);
		removeChildByTag(1031);
		removeChildByTag(1032);
		removeChildByTag(1033);
		removeChildByTag(1041);
		removeChildByTag(1042);
		removeChildByTag(1043);
		removeChildByTag(1051);
		removeChildByTag(1052);
		removeChildByTag(1053);
		removeChildByTag(1061);
		removeChildByTag(1062);
		removeChildByTag(1063);

		Sprite *Weapon3Detail;
		Weapon3Detail = Sprite::create("weapon3detail.png");
		Weapon3Detail->setPosition(Vec2(1653,590));
		Weapon3Detail->setScale(0.7f);
		addChild(Weapon3Detail, 13, 1031);


		if (Gold >= 2300 && BuyWeaponNum <= 5)
		{
			Button *BuyButton31=Button::create();
			BuyButton31->loadTextures("buy1.png", "buy1.png", "");
			BuyButton31->setPosition(Vec2(1650, 180));
			BuyButton31->setScale(1.0f);
			BuyButton31->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit3, this));
			addChild(BuyButton31, 14, 1032);
		}

		if (Gold < 2300 || BuyWeaponNum >= 6)
		{
			Sprite *BuyButton32;
			BuyButton32 = Sprite::create("buy2.png");
			BuyButton32->setPosition(Vec2(1650, 180));
			BuyButton32->setScale(1.0f);
			addChild(BuyButton32, 14, 1033);
		}
	}
}

void MapScene::Shopbuy4(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		removeChildByTag(1011);
		removeChildByTag(1012);
		removeChildByTag(1013);
		removeChildByTag(1021);
		removeChildByTag(1022);
		removeChildByTag(1023);
		removeChildByTag(1031);
		removeChildByTag(1032);
		removeChildByTag(1033);
		removeChildByTag(1041);
		removeChildByTag(1042);
		removeChildByTag(1043);
		removeChildByTag(1051);
		removeChildByTag(1052);
		removeChildByTag(1053);
		removeChildByTag(1061);
		removeChildByTag(1062);
		removeChildByTag(1063);

		Sprite *Weapon4Detail;
		Weapon4Detail = Sprite::create("weapon4detail.png");
		Weapon4Detail->setPosition(Vec2(1653,590));
		Weapon4Detail->setScale(0.7f);
		addChild(Weapon4Detail, 13, 1041);


		if (Gold >= 2990 && BuyWeaponNum <= 5)
		{
			Button *BuyButton41=Button::create();
			BuyButton41->loadTextures("buy1.png", "buy1.png", "");
			BuyButton41->setPosition(Vec2(1650, 180));
			BuyButton41->setScale(1.0f);
			BuyButton41->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit4, this));
			addChild(BuyButton41, 14, 1042);
		}

		if (Gold < 2990 || BuyWeaponNum >= 6)
		{
			Sprite *BuyButton42;
			BuyButton42 = Sprite::create("buy2.png");
			BuyButton42->setPosition(Vec2(1650, 180));
			BuyButton42->setScale(1.0f);
			addChild(BuyButton42, 14, 1043);
		}
	}
}

void MapScene::Shopbuy5(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		removeChildByTag(1011);
		removeChildByTag(1012);
		removeChildByTag(1013);
		removeChildByTag(1021);
		removeChildByTag(1022);
		removeChildByTag(1023);
		removeChildByTag(1031);
		removeChildByTag(1032);
		removeChildByTag(1033);
		removeChildByTag(1041);
		removeChildByTag(1042);
		removeChildByTag(1043);
		removeChildByTag(1051);
		removeChildByTag(1052);
		removeChildByTag(1053);
		removeChildByTag(1061);
		removeChildByTag(1062);
		removeChildByTag(1063);

		Sprite *Weapon5Detail;
		Weapon5Detail = Sprite::create("weapon5detail.png");
		Weapon5Detail->setPosition(Vec2(1653,590));
		Weapon5Detail->setScale(0.7f);
		addChild(Weapon5Detail, 13, 1051);


		if (Gold >= 900 && BuyWeaponNum <= 5)
		{
			Button *BuyButton51=Button::create();
			BuyButton51->loadTextures("buy1.png", "buy1.png", "");
			BuyButton51->setPosition(Vec2(1650, 180));
			BuyButton51->setScale(1.0f);
			BuyButton51->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit5, this));
			addChild(BuyButton51, 14, 1052);
		}

		if (Gold < 900 || BuyWeaponNum >= 6)
		{
			Sprite *BuyButton52;
			BuyButton52 = Sprite::create("buy2.png");
			BuyButton52->setPosition(Vec2(1650, 180));
			BuyButton52->setScale(1.0f);
			addChild(BuyButton52, 14, 1053);
		}
	}
}

void MapScene::Shopbuy6(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		removeChildByTag(1011);
		removeChildByTag(1012);
		removeChildByTag(1013);
		removeChildByTag(1021);
		removeChildByTag(1022);
		removeChildByTag(1023);
		removeChildByTag(1031);
		removeChildByTag(1032);
		removeChildByTag(1033);
		removeChildByTag(1041);
		removeChildByTag(1042);
		removeChildByTag(1043);
		removeChildByTag(1051);
		removeChildByTag(1052);
		removeChildByTag(1053);
		removeChildByTag(1061);
		removeChildByTag(1062);
		removeChildByTag(1063);

		Sprite *Weapon6Detail;
		Weapon6Detail = Sprite::create("weapon6detail.png");
		Weapon6Detail->setPosition(Vec2(1650,590));
		Weapon6Detail->setScale(0.7f);
		addChild(Weapon6Detail, 13, 1061);


		if (Gold >= 710 && BuyWeaponNum <= 5)
		{
			Button *BuyButton61=Button::create();
			BuyButton61->loadTextures("buy1.png", "buy1.png", "");
			BuyButton61->setPosition(Vec2(1650, 180));
			BuyButton61->setScale(1.0f);
			BuyButton61->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit6, this));
			addChild(BuyButton61, 14, 1062);
		}

		if (Gold < 710||BuyWeaponNum >= 6)
		{
			Sprite *BuyButton62;
			BuyButton62 = Sprite::create("buy2.png");
			BuyButton62->setPosition(Vec2(1650, 180));
			BuyButton62->setScale(1.0f);
			addChild(BuyButton62, 14, 1063);
		}
	}
}


//武器一确认购买
void MapScene::Buyit1(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED&&BuyWeaponNum<=5)
	{
		BuyWeaponNum++;
		Sprite *MyEquip1 = Sprite::create("weapon1.png");
		MyEquip1->setPosition(Vec2(350+BuyWeaponNum*150,200));
		MyEquip1->setScale(0.7f);
		addChild(MyEquip1, 14, 40 + BuyWeaponNum);
		Gold -= 1740;
		removeChildByTag(55);
		CCString *goldCCstring = CCString::createWithFormat("%d", Gold);
		std::string goldstring = goldCCstring->getCString();
		Label *GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
		GoldNum->setPosition(Vec2(50, 450));
		addChild(GoldNum, 9, 55);
		removeChildByTag(1011);
		removeChildByTag(1012);
		removeChildByTag(1013);
		removeChildByTag(1021);
		removeChildByTag(1022);
		removeChildByTag(1023);
		removeChildByTag(1031);
		removeChildByTag(1032);
		removeChildByTag(1033);
		removeChildByTag(1041);
		removeChildByTag(1042);
		removeChildByTag(1043);
		removeChildByTag(1051);
		removeChildByTag(1052);
		removeChildByTag(1053);
		removeChildByTag(1061);
		removeChildByTag(1062);
		removeChildByTag(1063);
	}
}

//武器二确认购买
void MapScene::Buyit2(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED&&BuyWeaponNum <= 5)
	{
		BuyWeaponNum++;
		Sprite *MyEquip2 = Sprite::create("weapon2.png");
		MyEquip2->setPosition(Vec2(350 + BuyWeaponNum * 150, 200));
		MyEquip2->setScale(0.7f);
		addChild(MyEquip2, 14, 40+BuyWeaponNum);
		Gold -= 2140;
		removeChildByTag(55);
		CCString *goldCCstring = CCString::createWithFormat("%d", Gold);
		std::string goldstring = goldCCstring->getCString();
		Label *GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
		GoldNum->setPosition(Vec2(50, 450));
		addChild(GoldNum, 9, 55);
		removeChildByTag(1011);
		removeChildByTag(1012);
		removeChildByTag(1013);
		removeChildByTag(1021);
		removeChildByTag(1022);
		removeChildByTag(1023);
		removeChildByTag(1031);
		removeChildByTag(1032);
		removeChildByTag(1033);
		removeChildByTag(1041);
		removeChildByTag(1042);
		removeChildByTag(1043);
		removeChildByTag(1051);
		removeChildByTag(1052);
		removeChildByTag(1053);
		removeChildByTag(1061);
		removeChildByTag(1062);
		removeChildByTag(1063);
	}
}

//武器三确认购买
void MapScene::Buyit3(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED&&BuyWeaponNum <= 5)
	{
		BuyWeaponNum++;
		Sprite *MyEquip3 = Sprite::create("weapon3.png");
		MyEquip3->setPosition(Vec2(350 + BuyWeaponNum * 150, 200));
		MyEquip3->setScale(0.7f);
		addChild(MyEquip3, 14, 40 + BuyWeaponNum);
		Gold -= 2300;
		removeChildByTag(55);
		CCString *goldCCstring = CCString::createWithFormat("%d", Gold);
		std::string goldstring = goldCCstring->getCString();
		Label *GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
		GoldNum->setPosition(Vec2(50, 450));
		addChild(GoldNum, 9, 55);
		removeChildByTag(1011);
		removeChildByTag(1012);
		removeChildByTag(1013);
		removeChildByTag(1021);
		removeChildByTag(1022);
		removeChildByTag(1023);
		removeChildByTag(1031);
		removeChildByTag(1032);
		removeChildByTag(1033);
		removeChildByTag(1041);
		removeChildByTag(1042);
		removeChildByTag(1043);
		removeChildByTag(1051);
		removeChildByTag(1052);
		removeChildByTag(1053);
		removeChildByTag(1061);
		removeChildByTag(1062);
		removeChildByTag(1063);
	}
}

//武器四确认购买
void MapScene::Buyit4(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED&&BuyWeaponNum <= 5)
	{
		BuyWeaponNum++;
		Sprite *MyEquip4 = Sprite::create("weapon4.png");
		MyEquip4->setPosition(Vec2(350 + BuyWeaponNum * 150, 200));
		MyEquip4->setScale(0.7f);
		addChild(MyEquip4, 14, 40 + BuyWeaponNum);
		Gold -= 2990;
		removeChildByTag(55);
		CCString *goldCCstring = CCString::createWithFormat("%d", Gold);
		std::string goldstring = goldCCstring->getCString();
		Label *GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
		GoldNum->setPosition(Vec2(50, 450));
		addChild(GoldNum, 9, 55);
		removeChildByTag(1011);
		removeChildByTag(1012);
		removeChildByTag(1013);
		removeChildByTag(1021);
		removeChildByTag(1022);
		removeChildByTag(1023);
		removeChildByTag(1031);
		removeChildByTag(1032);
		removeChildByTag(1033);
		removeChildByTag(1041);
		removeChildByTag(1042);
		removeChildByTag(1043);
		removeChildByTag(1051);
		removeChildByTag(1052);
		removeChildByTag(1053);
		removeChildByTag(1061);
		removeChildByTag(1062);
		removeChildByTag(1063);
	}
}

//武器五确认购买
void MapScene::Buyit5(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED&&BuyWeaponNum <= 5)
	{
		BuyWeaponNum++;
		CCString *BuyCCstring5 = CCString::createWithFormat("%d", BuyWeaponNum);
		std::string buystring5 = BuyCCstring5->getCString();
		Sprite *MyEquip5 = Sprite::create("weapon5.png");
		MyEquip5->setPosition(Vec2(350 + BuyWeaponNum * 150, 200));
		MyEquip5->setScale(0.7f);
		addChild(MyEquip5, 14, 40 + BuyWeaponNum);
		Gold -= 900;
		removeChildByTag(55);
		CCString *goldCCstring = CCString::createWithFormat("%d", Gold);
		std::string goldstring = goldCCstring->getCString();
		Label *GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
		GoldNum->setPosition(Vec2(50, 450));
		addChild(GoldNum, 9, 55);
		removeChildByTag(1011);
		removeChildByTag(1012);
		removeChildByTag(1013);
		removeChildByTag(1021);
		removeChildByTag(1022);
		removeChildByTag(1023);
		removeChildByTag(1031);
		removeChildByTag(1032);
		removeChildByTag(1033);
		removeChildByTag(1041);
		removeChildByTag(1042);
		removeChildByTag(1043);
		removeChildByTag(1051);
		removeChildByTag(1052);
		removeChildByTag(1053);
		removeChildByTag(1061);
		removeChildByTag(1062);
		removeChildByTag(1063);
	}
}

//武器六确认购买
void MapScene::Buyit6(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED&&BuyWeaponNum <= 5)
	{
		BuyWeaponNum++;
		CCString *BuyCCstring6 = CCString::createWithFormat("%d", BuyWeaponNum);
		std::string buystring6 = BuyCCstring6->getCString();
		Sprite *MyEquip6 = Sprite::create("weapon6.png");
		MyEquip6->setPosition(Vec2(350 + BuyWeaponNum * 150, 200));
		MyEquip6->setScale(0.7f);
		addChild(MyEquip6, 14, 40 + BuyWeaponNum);
		Gold -= 710;
		removeChildByTag(55);
		CCString *goldCCstring = CCString::createWithFormat("%d", Gold);
		std::string goldstring = goldCCstring->getCString();
		Label *GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
		GoldNum->setPosition(Vec2(50, 450));
		addChild(GoldNum, 9, 55);
		removeChildByTag(1011);
		removeChildByTag(1012);
		removeChildByTag(1013);
		removeChildByTag(1021);
		removeChildByTag(1022);
		removeChildByTag(1023);
		removeChildByTag(1031);
		removeChildByTag(1032);
		removeChildByTag(1033);
		removeChildByTag(1041);
		removeChildByTag(1042);
		removeChildByTag(1043);
		removeChildByTag(1051);
		removeChildByTag(1052);
		removeChildByTag(1053);
		removeChildByTag(1061);
		removeChildByTag(1062);
		removeChildByTag(1063);
	}
}

//商城返回
void MapScene::ShopBack(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		for (int p = 1; p <= BuyWeaponNum; ++p)
		{
			Sprite *Tmp = (Sprite *)this->getChildByTag(40 + p);
			Tmp->setVisible(false);
		}
		removeChildByTag(60);
		removeChildByTag(61);
		removeChildByTag(62);
		removeChildByTag(63);
		removeChildByTag(66);
		removeChildByTag(71);
		removeChildByTag(72);
		removeChildByTag(73);
		removeChildByTag(74);
		removeChildByTag(75);
		removeChildByTag(76);
		removeChildByTag(101);
		removeChildByTag(102);
		removeChildByTag(103);
		removeChildByTag(104);
		removeChildByTag(105);
		removeChildByTag(106);
		removeChildByTag(1010);
		removeChildByTag(1011);
		removeChildByTag(1012);
		removeChildByTag(1013);
		removeChildByTag(1020);
		removeChildByTag(1021);
		removeChildByTag(1022);
		removeChildByTag(1023);
		removeChildByTag(1030);
		removeChildByTag(1031);
		removeChildByTag(1032);
		removeChildByTag(1033);
		removeChildByTag(1040);
		removeChildByTag(1041);
		removeChildByTag(1042);
		removeChildByTag(1043);
		removeChildByTag(1050);
		removeChildByTag(1051);
		removeChildByTag(1052);
		removeChildByTag(1053);
		removeChildByTag(1060);
		removeChildByTag(1061);
		removeChildByTag(1062);
		removeChildByTag(1063);
		Button *tmp = (Button *)this->getChildByTag(50);
		tmp->setTouchEnabled(true);
	}
}
//使用removebytag至关重要，byTag还可以提供访问
