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
#include "SimpleAudioEngine.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;

int MeKill = 0;
int MeDead = 0;
int YouKill = 0;
int YouDead = 0;


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
	addChild(protect1, 0, 3);
	protect1->start(1, Vec2(1730, 1020));
	Enemy.pushBack(protect1->enemy);
	chosenEnemy = NULL;
	auto protect2 = object::createObject();
	addChild(protect2, 0, 4);
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
	ShopButton->addTouchEventListener(CC_CALLBACK_2(MapScene::ShopCall, this));
	addChild(ShopButton, 9,50); 

	//战绩图标
	Button *ZhanjiButton = Button::create();
	ZhanjiButton->loadTextures("Zhanji.png", "Zhanji.png", "");
	ZhanjiButton->setPosition(Vec2(1750,1030));
	ZhanjiButton->setScale(1.0f);
	ZhanjiButton->addTouchEventListener(CC_CALLBACK_2(MapScene::ZhanjiCall, this));
	addChild(ZhanjiButton, 9,51);

	//击杀数
	CCString *TMeKillCCstring = CCString::createWithFormat("%d", MeKill);
	std::string TMeKillstring = TMeKillCCstring->getCString();
	Label *TMeKillNum = Label::createWithTTF(TMeKillstring, "fonts/Marker Felt.ttf", 32);
	TMeKillNum->setPosition(Vec2(1750, 1030));
	TMeKillNum->setScale(1.0f);
	addChild(TMeKillNum, 10, 52);

	//击杀图标
	Sprite *KillTag = Sprite::create("kill.png");
	KillTag->setPosition(Vec2(1700,1030));
	KillTag->setScale(1.0f);
	addChild(KillTag,10, 53);

	//死亡数
	CCString *TMeDeadCCstring = CCString::createWithFormat("%d", MeDead);
	std::string TMeDeadstring = TMeDeadCCstring->getCString();
	Label *TMeDeadNum = Label::createWithTTF(TMeDeadstring, "fonts/Marker Felt.ttf", 32);
	TMeDeadNum->setPosition(Vec2(1860, 1030));
	TMeDeadNum->setScale(1.0f);
	addChild(TMeDeadNum, 10, 54);
	
	//死亡图标
	Sprite *DeadTag = Sprite::create("die.png");
	DeadTag->setPosition(Vec2(1810,1030));
	DeadTag->setScale(1.0f);
	addChild(DeadTag, 10, 56);

	//金币数字
	CCString *goldCCstring=CCString::createWithFormat("%d",MyGold);
	std::string goldstring = goldCCstring->getCString();
	Label *GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
	GoldNum->setPosition(Vec2(50,450));
	addChild(GoldNum, 9,55);

	hero = new heroPrint();
	hero->initHeroSprite(8, Vec2(860, 540));
	addChild(hero);
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
	this->schedule(schedule_selector(MapScene::soldersContrl), 0.5f);
	return true;
}
bool MapScene::onTouchBegan(Touch* touch, Event* unused_event)
{

	object* protect1 = (object*)getChildByTag(3);
	int c = protect1->getTag();
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
					hero->heroAttack(hero_position, soldersPostion);
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
	object* protect1 = (object*)getChildByTag(3);
	Vec2 Tower1_position = protect1->enemy->getPosition();
	int range = 400;
	if (chosenEnemy)
	{
		Vec2 objectPositon = chosenEnemy->getPosition();
		if (getDistance(Tower1_position, objectPositon) <= range)
		{
			protect1->attactEnemy(Tower1_position, objectPositon, 0);
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
				protect1->attactEnemy(Tower1_position, objectPositon, 0);
			}
		}
	}
}
void MapScene::Tower2(float dt)
{
	object* protect2 = (object*)getChildByTag(4);
	Vec2 Tower1_position = protect2->enemy->getPosition();
	int range = 400;
	if (chosenFriend)
	{
		Vec2 objectPositon = chosenFriend->getPosition();
		if (getDistance(Tower1_position, objectPositon) <= range)
		{
			protect2->attactEnemy(Tower1_position, objectPositon, 1);
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
				protect2->attactEnemy(Tower1_position, objectPositon, 1);
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
		addChild(solder11, 0, 2051);//1730 1020
		solder11->start(4, Vec2(1600, 900));
		auto solder22 = object::createObject();
		addChild(solder22, 0, 2052);
		solder22->start(4, Vec2(1650, 850));
		auto solder33 = object::createObject();
		addChild(solder33, 0, 2053);
		solder33->start(4, Vec2(1550, 950));
		Enemy.pushBack(solder11->enemy);
		Enemy.pushBack(solder22->enemy);
		Enemy.pushBack(solder33->enemy);
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
			Vec2 soldersPostion = a->getPosition();
			if ((i + 1) % 3 == 0)
			{
				float distance = getDistance(soldersPostion, Vec2(1600, 900));
				auto* move = MoveTo::create((float)distance / 50, Vec2(1600, 900));
				auto* sequence = Sequence::create(move, NULL);
				int min; float minDis = 200000;
				for (int n = 0; n < Enemy.size(); n++)
				{
					auto b = Enemy.at(n);
					Size t = b->getContentSize();
					float minDistance = getDistance(soldersPostion, b->getPosition());
					CCLOG("*%f*", minDistance);
					if (minDis > minDistance)
					{
						minDis = minDistance;
						min = n;
					}
				}
				CCLOG("                        %d", min);
				if (minDis <= 100)
				{
					a->stopAllActions();
					solder->attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i);
				}
				if (minDis <= 200 && minDis > 100)
				{
					a->stopAllActions();
					float Distance = getDistance(soldersPostion, Enemy.at(min)->getPosition());
					auto* Move = MoveTo::create((float)Distance / 50, Enemy.at(min)->getPosition());
					a->runAction(Move);

				}
				if (minDis > 200)
				{
					a->stopAllActions();
					a->runAction(sequence);
				}
			}
			if ((i + 1) % 3 == 1)
			{
				float distance = getDistance(soldersPostion, Vec2(1650, 850));
				auto* move = MoveTo::create((float)distance / 50, Vec2(1650, 850));
				auto* sequence = Sequence::create(move, NULL);
				/*auto finishAction()
				{
					->stopAllActions();
				};*/
				//auto funcall1 = CallFunc::create(CC_CALLBACK_0(finishAction, this));
				int min; float minDis = 200000;
				for (int n = 0; n < Enemy.size(); n++)
				{
					auto b = Enemy.at(n);
					Size t = b->getContentSize();
					float minDistance = getDistance(soldersPostion, b->getPosition());
					CCLOG("*%f*", minDistance);
					if (minDis > minDistance)
					{
						minDis = minDistance;
						min = n;
					}
				}
				CCLOG("                        %d", min);
				if (minDis <= 100)
				{
					a->stopAllActions();
					solder->attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i);
				}
				if (minDis <= 200 && minDis > 100)
				{
					a->stopAllActions();
					float Distance = getDistance(soldersPostion, Enemy.at(min)->getPosition());
					auto* Move = MoveTo::create((float)Distance / 50, Enemy.at(min)->getPosition());
					a->runAction(Move);

				}
				if (minDis > 200)
				{
					a->stopAllActions();
					a->runAction(sequence);
				}
			}
			if ((i + 1) % 3 == 2)
			{
				float distance = getDistance(soldersPostion, Vec2(1550, 950));
				auto* move = MoveTo::create((float)distance / 50, Vec2(1550, 950));
				auto* sequence = Sequence::create(move, NULL);
				/*auto finishAction()
				{
					->stopAllActions();
				};*/
				//auto funcall1 = CallFunc::create(CC_CALLBACK_0(finishAction, this));
				int min; float minDis = 200000;
				for (int n = 0; n < Enemy.size(); n++)
				{
					auto b = Enemy.at(n);
					Size t = b->getContentSize();
					float minDistance = getDistance(soldersPostion, b->getPosition());
					CCLOG("*%f*", minDistance);
					if (minDis > minDistance)
					{
						minDis = minDistance;
						min = n;
					}
				}
				CCLOG("                        %d", min);
				if (minDis <= 100)
				{
					a->stopAllActions();
					solder->attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i);
				}
				if (minDis <= 200 && minDis > 100)
				{
					a->stopAllActions();
					float Distance = getDistance(soldersPostion, Enemy.at(min)->getPosition());
					auto* Move = MoveTo::create((float)Distance / 50, Enemy.at(min)->getPosition());
					a->runAction(Move);

				}
				if (minDis > 200)
				{
					a->stopAllActions();
					a->runAction(sequence);
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
			object* solder = (object*)getChildByTag(2050 + i);
			Vec2 soldersPostion = a->getPosition();
			if ((i + 2) % 3 == 0)
			{
				float distance = getDistance(Vec2(600, 500), soldersPostion);
				auto* move = MoveTo::create((float)distance / 50, Vec2(600, 500));
				auto* sequence = Sequence::create(move, NULL);
				/*auto finishAction()
				{
					->stopAllActions();
				};*/
				//auto funcall1 = CallFunc::create(CC_CALLBACK_0(finishAction, this));
				int min; float minDis = 200000;
				for (int n = 0; n < Friend.size(); n++)
				{
					auto b = Friend.at(n);
					Size t = b->getContentSize();
					float minDistance = getDistance(soldersPostion, b->getPosition());
					CCLOG("*%f*", minDistance);
					if (minDis > minDistance)
					{
						minDis = minDistance;
						min = n;
					}
				}
				CCLOG("                        %d", min);
				if (minDis <= 200 && minDis > 100)
				{
					a->stopAllActions();
					float Distance = getDistance(soldersPostion, Friend.at(min)->getPosition());
					auto* Move = MoveTo::create((float)Distance / 50, Friend.at(min)->getPosition());
					a->runAction(Move);

				}
				if (minDis <= 100)
				{
					a->stopAllActions();
					solder->attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4);
				}
				if (minDis > 200)
				{
					a->stopAllActions();
					a->runAction(sequence);
				}
			}
			if ((i + 2) % 3 == 1)
			{
				float Distance = getDistance(Vec2(650, 450), soldersPostion);
				auto* move = MoveTo::create((float)Distance / 50, Vec2(650, 450));
				auto* sequence = Sequence::create(move, NULL);
				/*auto finishAction()
				{
					->stopAllActions();
				};*/
				//auto funcall1 = CallFunc::create(CC_CALLBACK_0(finishAction, this));
				int min; float minDis = 200000;
				for (int n = 0; n < Friend.size(); n++)
				{
					auto b = Friend.at(n);
					Size t = b->getContentSize();
					float minDistance = getDistance(soldersPostion, b->getPosition());
					CCLOG("*%f*", minDistance);
					if (minDis > minDistance)
					{
						minDis = minDistance;
						min = n;
					}
				}
				CCLOG("                        %d", min);
				if (minDis <= 200 && minDis > 100)
				{
					a->stopAllActions();
					float distance = getDistance(soldersPostion, Friend.at(min)->getPosition());
					auto* Move = MoveTo::create((float)distance / 50, Friend.at(min)->getPosition());
					a->runAction(Move);
				}
				if (minDis <= 100)
				{
					a->stopAllActions();
					solder->attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4);
				}
				if (minDis > 200)
				{
					a->stopAllActions();
					a->runAction(sequence);
				}
			}
			if ((i + 2) % 3 == 2)
			{
				float distance = getDistance(Vec2(550, 550), soldersPostion);
				auto* move = MoveTo::create((float)distance / 50, Vec2(550, 550));
				auto* sequence = Sequence::create(move, NULL);
				/*auto finishAction()
				{
					->stopAllActions();
				};*/
				//auto funcall1 = CallFunc::create(CC_CALLBACK_0(finishAction, this));
				int min; float minDis = 200000;
				for (int n = 0; n < Friend.size(); n++)
				{
					auto b = Friend.at(n);
					Size t = b->getContentSize();
					float minDistance = getDistance(soldersPostion, b->getPosition());
					CCLOG("*%f*", minDistance);
					if (minDis > minDistance)
					{
						minDis = minDistance;
						min = n;
					}
				}
				CCLOG("                        %d", min);
				if (minDis <= 200 && minDis > 100)
				{
					a->stopAllActions();
					float Distance = getDistance(soldersPostion, Friend.at(min)->getPosition());
					auto* Move = MoveTo::create((float)Distance / 50, Friend.at(min)->getPosition());
					a->runAction(Move);
				}
				if (minDis <= 100)
				{
					a->stopAllActions();
					solder->attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4);
				}
				if (minDis > 200)
				{
					a->stopAllActions();
					a->runAction(sequence);
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

void MapScene::EnterHelloWorldScene(Ref *pSenderBack)
{
	//全局重新初始化

	MyHeroID = 0;
	MyGold = 10000;
	MyBuyWeaponNum = 0;
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f,HelloWorld::createScene()));
}



void MapScene::ShopCall(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		//战绩图标不可触发
		Button *tmp1 = (Button *)this->getChildByTag(51);
		tmp1->setTouchEnabled(false);
		//商城图标不可触发
		Button *tmp2 = (Button *)this->getChildByTag(50);
		tmp2->setTouchEnabled(false);

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

		for (int p = 1; p <= MyBuyWeaponNum; ++p)

		{
			Sprite *Tmp = (Sprite *)this->getChildByTag(40+p);
			Tmp->setVisible(true);
		}

	}
}


void MapScene::ZhanjiCall(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		//战绩图标不可触发
		Button *tmp1 = (Button *)this->getChildByTag(51);
		tmp1->setTouchEnabled(false);
		//商城图标不可触发
		Button *tmp2 = (Button *)this->getChildByTag(50);
		tmp2->setTouchEnabled(false);

		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		auto *chnString = Dictionary::createWithContentsOfFile("CHN_String.xml");
		const char *KillStr = ((String *)chnString->objectForKey("Kill_Text"))->getCString();
		const char *DeadStr = ((String *)chnString->objectForKey("Dead_Text"))->getCString();
		const char *ZhanjiStr= ((String *)chnString->objectForKey("Zhanji_Text"))->getCString();

		Button *ZhanjiBackButton = Button::create();
		ZhanjiBackButton->loadTextures("ShopBack.png", "ShopBack.png", "");
		ZhanjiBackButton->setPosition(Vec2(1665, 910));
		ZhanjiBackButton->addTouchEventListener(CC_CALLBACK_2(MapScene::ZhanjiBack, this));
		ZhanjiBackButton->setScale(0.72f);
		addChild(ZhanjiBackButton, 14, 80);

		Sprite *ZhanjiBackGround;
		ZhanjiBackGround = Sprite::create("Shopbackground.png");
		ZhanjiBackGround->setPosition(Vec2(960, 550));
		ZhanjiBackGround->setScale(0.85f);
		ZhanjiBackGround->setOpacity(230);
		addChild(ZhanjiBackGround, 10, 81);

		Sprite *LittleTag = Sprite::create("little.png");
		LittleTag->setPosition(Vec2(180,900));
		addChild(LittleTag, 14, 82);


		Label *Zhanjilabel = Label::create(ZhanjiStr,"Arial" ,34 );
		Zhanjilabel->setPosition(Vec2(250, 900));
		addChild(Zhanjilabel, 14, 97);

		//我方头像
		Sprite *MeAvatar;
		std::string MeAvatarName = StringUtils::format("Hero%d_Avatar.png", MyHeroID);
		MeAvatar = Sprite::create(MeAvatarName);
		if (MeAvatarName == "Hero1_Avatar.png")
		{
			MeAvatar->setScale(0.3f);
		}
		else
		{
			MeAvatar->setScale(2.0f);
		}
		MeAvatar->setPosition(Vec2(origin.x + visibleSize.width*0.25, origin.y + visibleSize.height*0.5));
		addChild(MeAvatar, 12, 83);

		//对方头像
		Sprite *YouAvatar;
		std::string YouAvatarName = StringUtils::format("Hero%d_Avatar.png", YourHeroID);
		YouAvatar = Sprite::create(YouAvatarName);
		if (YouAvatarName == "Hero1_Avatar.png")
		{
			YouAvatar->setScale(0.3f);
		}
		else
		{
			YouAvatar->setScale(2.0f);
		}
		YouAvatar->setPosition(Vec2(origin.x + visibleSize.width*0.725, origin.y + visibleSize.height*0.5));
		addChild(YouAvatar, 12, 84);

		//我方空方案
		Sprite *MyEquipment1 = Sprite::create("kong.png");
		MyEquipment1->setPosition(Vec2(250, 300));
		MyEquipment1->setScale(0.6f);
		addChild(MyEquipment1, 12, 201);

		Sprite *MyEquipment2 = Sprite::create("kong.png");
		MyEquipment2->setPosition(Vec2(350, 300));
		MyEquipment2->setScale(0.6f);
		addChild(MyEquipment2, 12, 202);

		Sprite *MyEquipment3 = Sprite::create("kong.png");
		MyEquipment3->setPosition(Vec2(450, 300));
		MyEquipment3->setScale(0.6f);
		addChild(MyEquipment3, 12, 203);

		Sprite *MyEquipment4 = Sprite::create("kong.png");
		MyEquipment4->setPosition(Vec2(550, 300));
		MyEquipment4->setScale(0.6f);
		addChild(MyEquipment4, 12, 204);

		Sprite *MyEquipment5 = Sprite::create("kong.png");
		MyEquipment5->setPosition(Vec2(650, 300));
		MyEquipment5->setScale(0.6f);
		addChild(MyEquipment5, 12, 205);

		Sprite *MyEquipment6 = Sprite::create("kong.png");
		MyEquipment6->setPosition(Vec2(750, 300));
		MyEquipment6->setScale(0.6f);
		addChild(MyEquipment6, 12, 206);

		//对方空方案
		Sprite *YouEquipment1 = Sprite::create("kong.png");
		YouEquipment1->setPosition(Vec2(1150, 300));
		YouEquipment1->setScale(0.6f);
		addChild(YouEquipment1, 12, 207);

		Sprite *YouEquipment2 = Sprite::create("kong.png");
		YouEquipment2->setPosition(Vec2(1250, 300));
		YouEquipment2->setScale(0.6f);
		addChild(YouEquipment2, 12, 208);

		Sprite *YouEquipment3 = Sprite::create("kong.png");
		YouEquipment3->setPosition(Vec2(1350, 300));
		YouEquipment3->setScale(0.6f);
		addChild(YouEquipment3, 12, 209);

		Sprite *YouEquipment4 = Sprite::create("kong.png");
		YouEquipment4->setPosition(Vec2(1450, 300));
		YouEquipment4->setScale(0.6f);
		addChild(YouEquipment4, 12, 210);

		Sprite *YouEquipment5 = Sprite::create("kong.png");
		YouEquipment5->setPosition(Vec2(1550, 300));
		YouEquipment5->setScale(0.6f);
		addChild(YouEquipment5, 12, 211);

		Sprite *YouEquipment6 = Sprite::create("kong.png");
		YouEquipment6->setPosition(Vec2(1650, 300));
		YouEquipment6->setScale(0.6f);
		addChild(YouEquipment6, 12, 212);

		//设置我方装备可见
		for (int q = 1; q <= MyBuyWeaponNum; ++q)
		{
			Sprite *TMP = (Sprite *)this->getChildByTag(810+q);
			TMP->setVisible(true);
		}

		//设置对方装备可见
		/*for (int q = 1; q <= YouBuyWeaponNum; ++q)
		{
			Sprite *TMP = (Sprite *)this->getChildByTag();
			TMP->setVisible(true);
		}*/

		//我方金币
		auto MyGoldZhanji = Sprite::create("gold.png");
		MyGoldZhanji->setPosition(Vec2(450,200));
		MyGoldZhanji->setScale(1.0f);
		addChild(MyGoldZhanji, 14, 85);

		CCString *MygoldCCstringZhanji = CCString::createWithFormat("%d", MyGold);
		std::string MygoldstringZhanji = MygoldCCstringZhanji->getCString();
		Label *MyGoldNumZhanji = Label::createWithTTF(MygoldstringZhanji, "fonts/Marker Felt.ttf", 32);
		MyGoldNumZhanji->setPosition(Vec2(520,200));
		MyGoldNumZhanji->setScale(1.0f);
		addChild(MyGoldNumZhanji, 14, 86);

		//对方金币
		auto YouGoldZhanji = Sprite::create("gold.png");
		YouGoldZhanji->setPosition(Vec2(1350, 200));
		YouGoldZhanji->setScale(1.0f);
		addChild(YouGoldZhanji, 14, 87);

		CCString *YougoldCCstringZhanji = CCString::createWithFormat("%d", YourGold);
		std::string YougoldstringZhanji = YougoldCCstringZhanji->getCString();
		Label *YouGoldNumZhanji = Label::createWithTTF(YougoldstringZhanji, "fonts/Marker Felt.ttf", 32);
		YouGoldNumZhanji->setPosition(Vec2(1420, 200));
		YouGoldNumZhanji->setScale(1.0f);
		addChild(YouGoldNumZhanji, 14, 88);

		//我方击杀

		auto MeKillstr = Label::create(KillStr, "Arial", 36);
		MeKillstr->setPosition(Vec2(300,800));
		addChild(MeKillstr, 14, 89);

		CCString *MeKillCCstring = CCString::createWithFormat("%d", MeKill);
		std::string MeKillstring = MeKillCCstring->getCString();
		Label *MeKillNum = Label::createWithTTF(MeKillstring, "fonts/Marker Felt.ttf", 35);
		MeKillNum->setPosition(Vec2(300, 750));
		MeKillNum->setScale(1.0f);
		addChild(MeKillNum, 14, 90);

		
		//我方死亡
		auto MeDeadstr = Label::create(DeadStr, "Arial", 36);
		MeDeadstr->setPosition(Vec2(700,800));
		addChild(MeDeadstr, 14, 91);

		CCString *MeDeadCCstring = CCString::createWithFormat("%d",MeDead);
		std::string MeDeadstring = MeDeadCCstring->getCString();
		Label *MeDeadNum = Label::createWithTTF(MeDeadstring, "fonts/Marker Felt.ttf", 35);
		MeDeadNum->setPosition(Vec2(700, 750));
		MeDeadNum->setScale(1.0f);
		addChild(MeDeadNum, 14, 92);

		//对方击杀

		auto YouKillstr = Label::create(KillStr, "Arial", 36);
		YouKillstr->setPosition(Vec2(1200, 800));
		addChild(YouKillstr, 14, 93);

		CCString *YouKillCCstring = CCString::createWithFormat("%d", YouKill);
		std::string YouKillstring = YouKillCCstring->getCString();
		Label *YouKillNum = Label::createWithTTF(YouKillstring, "fonts/Marker Felt.ttf", 35);
		YouKillNum->setPosition(Vec2(1200, 750));
		YouKillNum->setScale(1.0f);
		addChild(YouKillNum, 14, 94);


		//对方死亡
		auto YouDeadstr = Label::create(DeadStr, "Arial", 36);
		YouDeadstr->setPosition(Vec2(1600, 800));
		addChild(YouDeadstr, 14, 95);

		CCString *YouDeadCCstring = CCString::createWithFormat("%d", YouDead);
		std::string YouDeadstring = YouDeadCCstring->getCString();
		Label *YouDeadNum = Label::createWithTTF(YouDeadstring, "fonts/Marker Felt.ttf", 35);
		YouDeadNum->setPosition(Vec2(1600, 750));
		YouDeadNum->setScale(1.0f);
		addChild(YouDeadNum, 14, 96);

	}
}

void MapScene::ZhanjiBack(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		for (int i = 0; i <= 17; ++i)
		{
			removeChildByTag(80 + i);
		}
		for (int i = 1; i <= 12; ++i)
		{
			removeChildByTag(200 + i);
		}
		for (int j = 1; j <= MyBuyWeaponNum; ++j)
		{
			Sprite *TMP = (Sprite *)this->getChildByTag(810 + j);
			TMP->setVisible(false);
		}
		//恢复商城可触发
		Button *tmp1 = (Button *)this->getChildByTag(50);
		tmp1->setTouchEnabled(true);
		//恢复战绩可触发
		Button *tmp2 = (Button *)this->getChildByTag(51);
		tmp2->setTouchEnabled(true);
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

		if (MyGold >= 1740&&MyBuyWeaponNum<=5)

		{
			Button *BuyButton11=Button::create();
			BuyButton11->loadTextures("buy1.png","buy1.png","");
			BuyButton11->setPosition(Vec2(1650,180));
			BuyButton11->setScale(1.0f);
			BuyButton11->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit1, this));
			addChild(BuyButton11, 14, 1012);
		}

		if (MyGold < 1740 || MyBuyWeaponNum >= 6)

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

		if (MyGold >= 2140 && MyBuyWeaponNum <= 5)

		{
			Button *BuyButton21=Button::create();
			BuyButton21->loadTextures("buy1.png", "buy1.png", "");
			BuyButton21->setPosition(Vec2(1650, 180));
			BuyButton21->setScale(1.0f);
			BuyButton21->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit2, this));
			addChild(BuyButton21, 14, 1022);
		}

		if (MyGold < 2140 || MyBuyWeaponNum >= 6)

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

		if (MyGold >= 2300 && MyBuyWeaponNum <= 5)

		{
			Button *BuyButton31=Button::create();
			BuyButton31->loadTextures("buy1.png", "buy1.png", "");
			BuyButton31->setPosition(Vec2(1650, 180));
			BuyButton31->setScale(1.0f);
			BuyButton31->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit3, this));
			addChild(BuyButton31, 14, 1032);
		}

		if (MyGold < 2300 || MyBuyWeaponNum >= 6)

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

		if (MyGold >= 2990 && MyBuyWeaponNum <= 5)

		{
			Button *BuyButton41=Button::create();
			BuyButton41->loadTextures("buy1.png", "buy1.png", "");
			BuyButton41->setPosition(Vec2(1650, 180));
			BuyButton41->setScale(1.0f);
			BuyButton41->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit4, this));
			addChild(BuyButton41, 14, 1042);
		}

		if (MyGold < 2990 || MyBuyWeaponNum >= 6)

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

		if (MyGold >= 900 && MyBuyWeaponNum <= 5)

		{
			Button *BuyButton51=Button::create();
			BuyButton51->loadTextures("buy1.png", "buy1.png", "");
			BuyButton51->setPosition(Vec2(1650, 180));
			BuyButton51->setScale(1.0f);
			BuyButton51->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit5, this));
			addChild(BuyButton51, 14, 1052);
		}

		if (MyGold < 900 || MyBuyWeaponNum >= 6)

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

		if (MyGold >= 710 && MyBuyWeaponNum <= 5)

		{
			Button *BuyButton61=Button::create();
			BuyButton61->loadTextures("buy1.png", "buy1.png", "");
			BuyButton61->setPosition(Vec2(1650, 180));
			BuyButton61->setScale(1.0f);
			BuyButton61->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit6, this));
			addChild(BuyButton61, 14, 1062);
		}

		if (MyGold < 710||MyBuyWeaponNum >= 6)

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
	if (controlevent == Widget::TouchEventType::ENDED&&MyBuyWeaponNum<=5)
	{
		MyBuyWeaponNum++;
		SimpleAudioEngine::getInstance()->playEffect("GOLD.wav");
		Sprite *MyEquip1 = Sprite::create("weapon1.png");
		MyEquip1->setPosition(Vec2(350+MyBuyWeaponNum*150,200));
		MyEquip1->setScale(0.7f);
		addChild(MyEquip1, 14, 40 + MyBuyWeaponNum);

		Sprite *MyEquip1Zhanji = Sprite::create("weapon1.png");
		MyEquip1Zhanji->setPosition(Vec2(150 + MyBuyWeaponNum * 100, 300));
		MyEquip1Zhanji->setScale(0.5f);
		MyEquip1Zhanji->setVisible(false);
		addChild(MyEquip1Zhanji, 14, 810 + MyBuyWeaponNum);

		MyGold -= 1740;
		removeChildByTag(55);
		CCString *goldCCstring = CCString::createWithFormat("%d", MyGold);
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
	if (controlevent == Widget::TouchEventType::ENDED&&MyBuyWeaponNum <= 5)
	{
		MyBuyWeaponNum++;
		SimpleAudioEngine::getInstance()->playEffect("GOLD.wav");
		Sprite *MyEquip2 = Sprite::create("weapon2.png");
		MyEquip2->setPosition(Vec2(350 + MyBuyWeaponNum * 150, 200));
		MyEquip2->setScale(0.7f);
		addChild(MyEquip2, 14, 40+MyBuyWeaponNum);

		Sprite *MyEquip2Zhanji = Sprite::create("weapon2.png");
		MyEquip2Zhanji->setPosition(Vec2(150 + MyBuyWeaponNum * 100, 300));
		MyEquip2Zhanji->setScale(0.5f);
		MyEquip2Zhanji->setVisible(false);
		addChild(MyEquip2Zhanji, 14, 810 + MyBuyWeaponNum);

		MyGold -= 2140;
		removeChildByTag(55);
		CCString *goldCCstring = CCString::createWithFormat("%d", MyGold);
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
	if (controlevent == Widget::TouchEventType::ENDED&&MyBuyWeaponNum <= 5)
	{
		MyBuyWeaponNum++;
		SimpleAudioEngine::getInstance()->playEffect("GOLD.wav");
		Sprite *MyEquip3 = Sprite::create("weapon3.png");
		MyEquip3->setPosition(Vec2(350 + MyBuyWeaponNum * 150, 200));
		MyEquip3->setScale(0.7f);
		addChild(MyEquip3, 14, 40 + MyBuyWeaponNum);

		Sprite *MyEquip3Zhanji = Sprite::create("weapon3.png");
		MyEquip3Zhanji->setPosition(Vec2(150 + MyBuyWeaponNum * 100, 300));
		MyEquip3Zhanji->setScale(0.5f);
		MyEquip3Zhanji->setVisible(false);
		addChild(MyEquip3Zhanji, 14, 810 + MyBuyWeaponNum);

		MyGold -= 2300;
		removeChildByTag(55);
		CCString *goldCCstring = CCString::createWithFormat("%d", MyGold);
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
	if (controlevent == Widget::TouchEventType::ENDED&&MyBuyWeaponNum <= 5)
	{
		MyBuyWeaponNum++;
		SimpleAudioEngine::getInstance()->playEffect("GOLD.wav");
		Sprite *MyEquip4 = Sprite::create("weapon4.png");
		MyEquip4->setPosition(Vec2(350 + MyBuyWeaponNum * 150, 200));
		MyEquip4->setScale(0.7f);
		addChild(MyEquip4, 14, 40 + MyBuyWeaponNum);

		Sprite *MyEquip4Zhanji = Sprite::create("weapon4.png");
		MyEquip4Zhanji->setPosition(Vec2(150 + MyBuyWeaponNum * 100, 300));
		MyEquip4Zhanji->setScale(0.5f);
		MyEquip4Zhanji->setVisible(false);
		addChild(MyEquip4Zhanji, 14, 810 + MyBuyWeaponNum);

		MyGold -= 2990;
		removeChildByTag(55);
		CCString *goldCCstring = CCString::createWithFormat("%d", MyGold);
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
	if (controlevent == Widget::TouchEventType::ENDED&&MyBuyWeaponNum <= 5)
	{
		MyBuyWeaponNum++;
		SimpleAudioEngine::getInstance()->playEffect("GOLD.wav");
		Sprite *MyEquip5 = Sprite::create("weapon5.png");
		MyEquip5->setPosition(Vec2(350 + MyBuyWeaponNum * 150, 200));
		MyEquip5->setScale(0.7f);
		addChild(MyEquip5, 14, 40 + MyBuyWeaponNum);

		Sprite *MyEquip5Zhanji = Sprite::create("weapon5.png");
		MyEquip5Zhanji->setPosition(Vec2(150 + MyBuyWeaponNum * 100, 300));
		MyEquip5Zhanji->setScale(0.5f);
		MyEquip5Zhanji->setVisible(false);
		addChild(MyEquip5Zhanji, 14, 810 + MyBuyWeaponNum);

		MyGold -= 900;
		removeChildByTag(55);
		CCString *goldCCstring = CCString::createWithFormat("%d", MyGold);
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
	if (controlevent == Widget::TouchEventType::ENDED&&MyBuyWeaponNum <= 5)
	{
		MyBuyWeaponNum++;
		SimpleAudioEngine::getInstance()->playEffect("GOLD.wav");
		Sprite *MyEquip6 = Sprite::create("weapon6.png");
		MyEquip6->setPosition(Vec2(350 + MyBuyWeaponNum * 150, 200));
		MyEquip6->setScale(0.7f);
		addChild(MyEquip6, 14, 40 + MyBuyWeaponNum);

		Sprite *MyEquip6Zhanji = Sprite::create("weapon6.png");
		MyEquip6Zhanji->setPosition(Vec2(150 + MyBuyWeaponNum * 100, 300));
		MyEquip6Zhanji->setScale(0.5f);
		MyEquip6Zhanji->setVisible(false);
		addChild(MyEquip6Zhanji, 14, 810 + MyBuyWeaponNum);

		MyGold -= 710;
		removeChildByTag(55);
		CCString *goldCCstring = CCString::createWithFormat("%d", MyGold);
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
		for (int p = 1; p <= MyBuyWeaponNum; ++p)

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
		//恢复商城可触发
		Button *tmp1 = (Button *)this->getChildByTag(50);
		tmp1->setTouchEnabled(true);
		//恢复战绩可触发
		Button *tmp2 = (Button *)this->getChildByTag(51);
		tmp2->setTouchEnabled(true);
	}
}
//使用removebytag至关重要，byTag还可以提供访问
