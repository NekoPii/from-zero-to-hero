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
#include "SimpleAudioEngine.h"
using namespace std;

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;

int MeKill = 0;
int MeDead = 0;
int YouKill = 0;
int YouDead = 0;
int current = 0;
int Range = 1;
int MaxBlood = 1000;
int Attack = 50;
int Defense = 2;
int AttackSpeed = 100;
int MoveSpeed = 100;


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
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//地图
	auto MAP = Sprite::create("map.jpg");
	MAP->setPosition(Vec2(0, 0));
	MAP->setScale(0.88f);
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
	_colliable->setVisible(false);
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
	Button* ShopButton = Button::create();
	ShopButton->loadTextures("shop.png", "shop.png", "");
	ShopButton->setPosition(Vec2(50, 500));
	ShopButton->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopcall, this));
	addChild(ShopButton, 9, 50);

	//战绩图标
	Button* ZhanjiButton = Button::create();
	ZhanjiButton->loadTextures("Zhanji.png", "Zhanji.png", "");
	ZhanjiButton->setPosition(Vec2(1750, 1030));
	ZhanjiButton->setScale(1.0f);
	ZhanjiButton->addTouchEventListener(CC_CALLBACK_2(MapScene::ZhanjiCall, this));
	addChild(ZhanjiButton, 9, 51);

	//击杀数
	CCString* TMeKillCCstring = CCString::createWithFormat("%d", MeKill);
	std::string TMeKillstring = TMeKillCCstring->getCString();
	Label* TMeKillNum = Label::createWithTTF(TMeKillstring, "fonts/Marker Felt.ttf", 32);
	TMeKillNum->setPosition(Vec2(1750, 1030));
	TMeKillNum->setScale(1.0f);
	addChild(TMeKillNum, 10, 52);

	//击杀图标
	Sprite* KillTag = Sprite::create("kill.png");
	KillTag->setPosition(Vec2(1700, 1030));
	KillTag->setScale(1.0f);
	addChild(KillTag, 10, 53);

	//死亡数
	CCString* TMeDeadCCstring = CCString::createWithFormat("%d", MeDead);
	std::string TMeDeadstring = TMeDeadCCstring->getCString();
	Label* TMeDeadNum = Label::createWithTTF(TMeDeadstring, "fonts/Marker Felt.ttf", 32);
	TMeDeadNum->setPosition(Vec2(1860, 1030));
	TMeDeadNum->setScale(1.0f);
	addChild(TMeDeadNum, 10, 54);

	//死亡图标
	Sprite* DeadTag = Sprite::create("die.png");
	DeadTag->setPosition(Vec2(1810, 1030));
	DeadTag->setScale(1.0f);
	addChild(DeadTag, 10, 56);

	//金币数字
	CCString* goldCCstring = CCString::createWithFormat("%d", MyGold);
	std::string goldstring = goldCCstring->getCString();
	Label* GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
	GoldNum->setPosition(Vec2(50, 450));
	addChild(GoldNum, 9, 55);

	frameCacheH = SpriteFrameCache::getInstance();
	frameCacheH->addSpriteFramesWithFile("heroh.plist", "heroh.png");
	frameCacheS = SpriteFrameCache::getInstance();
	frameCacheS->addSpriteFramesWithFile("heros.plist", "heros.png");
	frameCacheL = SpriteFrameCache::getInstance();
	frameCacheL->addSpriteFramesWithFile("levelup.plist", "levelup.png");
	frameCacheM = SpriteFrameCache::getInstance();
	frameCacheM->addSpriteFramesWithFile("magic.plist", "magic.png");
	LBS = Sprite::create("LBS.png");
	addChild(LBS, 3);
	hero = new heroPrint();
	hero->initHeroSprite(8, Vec2(860, 540));
	addChild(hero, 0, 2046);
	Friend.pushBack(hero->heroSprite);
	auto antiHero = object::createObject();
	addChild(antiHero, 0, 2047);
	antiHero->start(5, Vec2(1730, 1020));
	Enemy.pushBack(antiHero->enemy);




	myMagic = Sprite::create("magic_1_20.png");
	addChild(myMagic, 1);
	antiMagic = Sprite::create("magic_1_20.png");
	addChild(antiMagic, 1);


	//技能一
	Sprite* SKILL11 = Sprite::create("SKILL/skill1.png");
	SKILL11->setPosition(Vec2(origin.x + visibleSize.width * 0.55, origin.y + visibleSize.height * 0.1));
	SKILL11->setScale(0.25f);
	SKILL11->setVisible(true);
	addChild(SKILL11, 6, 901);
	Sprite* SKILL12 = Sprite::create("SKILL/skill1shade.png");
	SKILL12->setPosition(Vec2(origin.x + visibleSize.width * 0.55, origin.y + visibleSize.height * 0.1));
	SKILL12->setScale(0.25f);
	SKILL12->setVisible(false);
	addChild(SKILL12, 6, 902);
	auto SKILL13 = Sprite::create("SKILL/skill1.png");
	SKILL1TIME = ProgressTimer::create(SKILL13);
	SKILL1TIME->setScale(0.25f);
	SKILL1TIME->setType(kCCProgressTimerTypeRadial);
	SKILL1TIME->setPosition(Vec2(origin.x + visibleSize.width * 0.55, origin.y + visibleSize.height * 0.1));
	addChild(SKILL1TIME, 6, 903);

	//技能二
	Sprite* SKILL21 = Sprite::create("SKILL/skill2.png");
	SKILL21->setPosition(Vec2(origin.x + visibleSize.width * 0.6, origin.y + visibleSize.height * 0.1));
	SKILL21->setScale(0.25f);
	SKILL21->setVisible(true);
	addChild(SKILL21, 6, 904);
	Sprite* SKILL22 = Sprite::create("SKILL/skill2shade.png");
	SKILL22->setPosition(Vec2(origin.x + visibleSize.width * 0.6, origin.y + visibleSize.height * 0.1));
	SKILL22->setScale(0.25f);
	SKILL22->setVisible(false);
	addChild(SKILL22, 6, 905);
	auto SKILL23 = Sprite::create("SKILL/skill2.png");
	SKILL2TIME = ProgressTimer::create(SKILL23);
	SKILL2TIME->setScale(0.25f);
	SKILL2TIME->setType(kCCProgressTimerTypeRadial);
	SKILL2TIME->setPosition(Vec2(origin.x + visibleSize.width * 0.6, origin.y + visibleSize.height * 0.1));
	addChild(SKILL2TIME, 6, 906);


	//技能三
	Sprite* SKILL31 = Sprite::create("SKILL/skill3.png");
	SKILL31->setPosition(Vec2(origin.x + visibleSize.width * 0.65, origin.y + visibleSize.height * 0.1));
	SKILL31->setScale(0.25f);
	SKILL31->setVisible(true);
	addChild(SKILL31, 6, 907);
	Sprite* SKILL32 = Sprite::create("SKILL/skill3shade.png");
	SKILL32->setPosition(Vec2(origin.x + visibleSize.width * 0.65, origin.y + visibleSize.height * 0.1));
	SKILL32->setScale(0.25f);
	SKILL32->setVisible(false);
	addChild(SKILL32, 6, 908);
	auto SKILL33 = Sprite::create("SKILL/skill3.png");
	SKILL3TIME = ProgressTimer::create(SKILL33);
	SKILL3TIME->setScale(0.25f);
	SKILL3TIME->setType(kCCProgressTimerTypeRadial);
	SKILL3TIME->setPosition(Vec2(origin.x + visibleSize.width * 0.65, origin.y + visibleSize.height * 0.1));
	addChild(SKILL3TIME, 6, 909);

	//技能四
	Sprite* SKILL41 = Sprite::create("SKILL/skill4.png");
	SKILL41->setPosition(Vec2(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height * 0.1));
	SKILL41->setScale(0.25f);
	SKILL41->setVisible(true);
	addChild(SKILL41, 6, 910);
	Sprite* SKILL42 = Sprite::create("SKILL/skill4shade.png");
	SKILL42->setPosition(Vec2(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height * 0.1));
	SKILL42->setScale(0.25f);
	SKILL42->setVisible(false);
	addChild(SKILL42, 6, 911);
	auto SKILL43 = Sprite::create("SKILL/skill4.png");
	SKILL4TIME = ProgressTimer::create(SKILL43);
	SKILL4TIME->setScale(0.25f);
	SKILL4TIME->setType(kCCProgressTimerTypeRadial);
	SKILL4TIME->setPosition(Vec2(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height * 0.1));
	addChild(SKILL4TIME, 6, 912);

	//经验条
	Sprite* EXP = Sprite::create("EXP.png");
	EXPTIME = ProgressTimer::create(EXP);
	EXPTIME->setScale(1.753f);
	EXPTIME->setType(kCCProgressTimerTypeBar);
	EXPTIME->setMidpoint(Vec2(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.02));
	EXPTIME->setBarChangeRate(Vec2(1.0, 0));
	EXPTIME->setPercentage(100);
	addChild(EXPTIME, 10, 108);

	/*
	EXPpec = CCLabelTTF::create("100", "fonts / Marker Felt", 32);
	EXPpec->setColor(ccc3(0, 0, 0));
	EXPpec->setPosition(Vec2(origin.x + visibleSize.width*0.98, origin.y + visibleSize.height*0.02));
	addChild(EXPpec, 4,109);
	*/

	//个人信息背景
	Sprite* DetailBackGround = Sprite::create("DetailBackGround.png");
	DetailBackGround->setPosition(Vec2(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.1));
	DetailBackGround->setOpacity(220);
	DetailBackGround->setScale(0.45f);
	addChild(DetailBackGround, 5, 110);

	//头像
	Sprite* CharacterAvatar;
	std::string CharacterAvatarName = StringUtils::format("Hero%d_Avatar.png", MyHeroID);
	CharacterAvatar = Sprite::create(CharacterAvatarName);
	if (CharacterAvatarName == "Hero1_Avatar.png")
	{
		CharacterAvatar->setScale(0.2f);
	}
	else
	{
		CharacterAvatar->setScale(1.5f);
	}
	CharacterAvatar->setPosition(Vec2(origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.11));
	addChild(CharacterAvatar, 10, 111);

	//最大血量
	Sprite* MaxBloodsp = Sprite::create("MaxBlood.png");
	MaxBloodsp->setPosition(Vec2(origin.x + visibleSize.width * 0.37, origin.y + visibleSize.height * 0.15));
	MaxBloodsp->setScale(0.5f);
	addChild(MaxBloodsp, 6, 112);

	CCString* MaxBloodCCDataStr = CCString::createWithFormat("%d", MaxBlood);
	std::string MaxBloodDataStr = MaxBloodCCDataStr->getCString();
	Label* MaxBloodData = Label::createWithTTF(MaxBloodDataStr, "fonts/arial.ttf", 30);
	MaxBloodData->setPosition(Vec2(origin.x + visibleSize.width * 0.41, origin.y + visibleSize.height * 0.15));
	addChild(MaxBloodData, 6, 113);

	//攻击
	Sprite* Attacksp = Sprite::create("Attack.png");
	Attacksp->setPosition(Vec2(origin.x + visibleSize.width * 0.45, origin.y + visibleSize.height * 0.15));
	Attacksp->setScale(0.5f);
	addChild(Attacksp, 6, 114);

	CCString* AttackCCDataStr = CCString::createWithFormat("%d", Attack);
	std::string AttackDataStr = AttackCCDataStr->getCString();
	Label* AttackData = Label::createWithTTF(AttackDataStr, "fonts/arial.ttf", 30);
	AttackData->setPosition(Vec2(origin.x + visibleSize.width * 0.49, origin.y + visibleSize.height * 0.15));
	addChild(AttackData, 6, 115);

	//护甲
	Sprite* Defensesp = Sprite::create("Defense.png");
	Defensesp->setPosition(Vec2(origin.x + visibleSize.width * 0.37, origin.y + visibleSize.height * 0.1));
	Defensesp->setScale(0.5f);
	addChild(Defensesp, 6, 116);

	CCString* DefenseCCDataStr = CCString::createWithFormat("%d", Defense);
	std::string DefenseDataStr = DefenseCCDataStr->getCString();
	Label* DefenseData = Label::createWithTTF(DefenseDataStr, "fonts/arial.ttf", 30);
	DefenseData->setPosition(Vec2(origin.x + visibleSize.width * 0.41, origin.y + visibleSize.height * 0.1));
	addChild(DefenseData, 6, 117);

	//攻速
	Sprite* AttackSpeedsp = Sprite::create("AttackSpeed.png");
	AttackSpeedsp->setPosition(Vec2(origin.x + visibleSize.width * 0.45, origin.y + visibleSize.height * 0.1));
	AttackSpeedsp->setScale(0.5f);
	addChild(AttackSpeedsp, 6, 118);

	CCString* AttackSpeedCCDataStr = CCString::createWithFormat("%d%%", AttackSpeed);
	std::string AttackSpeedDataStr = AttackSpeedCCDataStr->getCString();
	Label* AttackSpeedData = Label::createWithTTF(AttackSpeedDataStr, "fonts/arial.ttf", 30);
	AttackSpeedData->setPosition(Vec2(origin.x + visibleSize.width * 0.49, origin.y + visibleSize.height * 0.1));
	addChild(AttackSpeedData, 6, 119);

	//移速
	Sprite* MoveSpeedsp = Sprite::create("MoveSpeed.png");
	MoveSpeedsp->setPosition(Vec2(origin.x + visibleSize.width * 0.37, origin.y + visibleSize.height * 0.05));
	MoveSpeedsp->setScale(0.4f);
	addChild(MoveSpeedsp, 6, 120);

	CCString* MoveSpeedCCDataStr = CCString::createWithFormat("%d%%", MoveSpeed);
	std::string MoveSpeedDataStr = MoveSpeedCCDataStr->getCString();
	Label* MoveSpeedData = Label::createWithTTF(MoveSpeedDataStr, "fonts/arial.ttf", 30);
	MoveSpeedData->setPosition(Vec2(origin.x + visibleSize.width * 0.41, origin.y + visibleSize.height * 0.05));
	addChild(MoveSpeedData, 6, 121);

	//
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto myKeyListener = EventListenerKeyboard::create();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(MapScene::onTouchBegan, this);
	myKeyListener->onKeyPressed = CC_CALLBACK_2(MapScene::onKeyPressed, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	dispatcher->addEventListenerWithSceneGraphPriority(myKeyListener, this);
	this->schedule(schedule_selector(MapScene::heroIn), 0.1f);
	this->schedule(schedule_selector(MapScene::Tower1), 0.1f);
	this->schedule(schedule_selector(MapScene::Tower2), 0.1f);
	this->scheduleOnce(schedule_selector(MapScene::soldersMake), 5.0f);
	this->scheduleOnce(schedule_selector(MapScene::soldersMake1), 35.0f);
	this->scheduleOnce(schedule_selector(MapScene::soldersMake2), 65.0f);
	this->scheduleOnce(schedule_selector(MapScene::soldersMake3), 95.0f);
	this->scheduleOnce(schedule_selector(MapScene::soldersMake4), 125.0f);
	this->scheduleOnce(schedule_selector(MapScene::soldersMake5), 155.0f);
	this->scheduleOnce(schedule_selector(MapScene::soldersMake6), 185.0f);
	this->scheduleOnce(schedule_selector(MapScene::soldersMake7), 215.0f);
	this->scheduleOnce(schedule_selector(MapScene::soldersMake8), 245.0f);
	this->scheduleOnce(schedule_selector(MapScene::soldersMake9), 275.0f);
	this->schedule(schedule_selector(MapScene::skillQ), 0.5f);
	this->schedule(schedule_selector(MapScene::skillW), 0.5f);
	this->schedule(schedule_selector(MapScene::skillE), 0.5f);
	this->schedule(schedule_selector(MapScene::skillR), 0.5f);
	this->schedule(schedule_selector(MapScene::soldersContrl), 0.5f);
	this->schedule(schedule_selector(MapScene::mark), 0.1f);
	this->schedule(schedule_selector(MapScene::anti), 0.8f);
	this->schedule(schedule_selector(MapScene::myMagicF), 0.05f);
	return true;
}
void MapScene::myMagicF(float dt)
{
	Vec2 hPos = hero->herosPosition();
	myMagic->setPosition(hPos);
	Vec2 aPos = Enemy.at(1)->getPosition();
	antiMagic->setPosition(aPos);
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


Animate* MapScene::createAnimateL()
{

	Vector <SpriteFrame*> frameArray;
	for (int i = 1; i <=15 ; i++)
	{
		auto* frame = frameCacheL->getSpriteFrameByName(String::createWithFormat("levelup%d", i)->getCString());
		frameArray.pushBack(frame);
	}
	Animation* animation = Animation::createWithSpriteFrames(frameArray);
	animation->setLoops(-1);
	animation->setDelayPerUnit(0.1f);
	return Animate::create(animation);
}
Animate* MapScene::createAnimateM( int id, int num,float t,int loop)
{

	Vector <SpriteFrame*> frameArray;
	for (int i = 1; i <= num; i++)
	{
		auto* frame = frameCacheM->getSpriteFrameByName(String::createWithFormat("magic_%d_%d.png", id, i)->getCString());
		frameArray.pushBack(frame);
	}
	Animation* animation = Animation::createWithSpriteFrames(frameArray);
	animation->setLoops(loop);
	animation->setDelayPerUnit(t);
	return Animate::create(animation);
}
Animate* MapScene::createAnimateM()
{

	Vector <SpriteFrame*> frameArray;
	
		auto* frame = frameCacheM->getSpriteFrameByName(String::createWithFormat("magic_%d_%d.png", 1, 20)->getCString());
		frameArray.pushBack(frame);
	
	Animation* animation = Animation::createWithSpriteFrames(frameArray);
	animation->setLoops(1);
	animation->setDelayPerUnit(0.1);
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
				if (distance <=hero->range)
				{
					int x = i+1022;
					if (i == 0)
					{
						current = 2045;
					}
					else if (i == 1)
					{
						current = 2047;
					}
					else
					{
						current = x;
					}
				//	CCLOG("%d*****+***", current);
					//hero->heroAttack(hero_position, soldersPostion);
					hero->heroSprite->stopAllActions();
					hero->heroSprite->runAction(createAnimateH(4,"att", 7, MyHeroID));
					if(i>1)
					attactEnemy(hero_position, soldersPostion, 49,2046,x);
					if (i == 0)
						attactEnemy(hero_position, soldersPostion, 49, 2046, 2045);
					if (i == 1)
					{attactEnemy(hero_position, soldersPostion, 49, 2046, 2047);}
					return false;
				}
				if (distance <= 150 && hero->R == 1&&i==1)
				{
					//计算
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
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (keycode==EventKeyboard::KeyCode::KEY_Q)
	{

		Sprite *tmp1 = (Sprite *)this->getChildByTag(901);
		tmp1->setVisible(false);
		Sprite *tmp2 = (Sprite *)this->getChildByTag(902);
		tmp2->setVisible(true);
		schedule(schedule_selector(MapScene::skill1), 0.1f);
		if (timeQ == 0)
		{
			hero->skill_Q = 1;
			hero->Q = 1;
			//hero->heroSprite->stopAllActions();
			CCLOG("QQQQ");
		}
	}
	if (keycode== EventKeyboard::KeyCode::KEY_W)
	{
		Sprite *tmp1 = (Sprite *)this->getChildByTag(904);
		tmp1->setVisible(false);
		Sprite *tmp2 = (Sprite *)this->getChildByTag(905);
		tmp2->setVisible(true);
		schedule(schedule_selector(MapScene::skill2), 0.1f);
		if (timeW == 0)
		{
			hero->skill_W = 1;
		}
	}
	if (keycode== EventKeyboard::KeyCode::KEY_E)
	{
		Sprite *tmp1 = (Sprite *)this->getChildByTag(907);
		tmp1->setVisible(false);
		Sprite *tmp2 = (Sprite *)this->getChildByTag(908);
		tmp2->setVisible(true);
		schedule(schedule_selector(MapScene::skill3), 0.1f);
		if (timeE == 0)
		{
			hero->skill_E = 1;
		}
	}
	if (keycode== EventKeyboard::KeyCode::KEY_R)
	{
		Sprite *tmp1 = (Sprite *)this->getChildByTag(910);
		tmp1->setVisible(false);
		Sprite *tmp2 = (Sprite *)this->getChildByTag(911);
		tmp2->setVisible(true);
		schedule(schedule_selector(MapScene::skill4), 0.1f);
		if (timeR == 0)
		{
			hero->skill_R = 1;
		}
		if (hero->R == 1)
			hero->R = 0;
	}
}

void MapScene::skill1(float dt)
{
	float Current1 = SKILL1TIME->getPercentage();
	if (Current1 <= 99)
	{
		Current1 += 1;
		SKILL1TIME->setPercentage(Current1);

	}
	else
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		unschedule(schedule_selector(MapScene::skill1));

		removeChildByTag(901);
		removeChildByTag(902);
		removeChildByTag(903);

		auto myKeyListener = EventListenerKeyboard::create();
		myKeyListener->onKeyPressed = CC_CALLBACK_2(MapScene::onKeyPressed, this);

		//技能一
		Sprite* SKILL11 = Sprite::create("SKILL/skill1.png");
		SKILL11->setPosition(Vec2(origin.x + visibleSize.width * 0.55, origin.y + visibleSize.height * 0.1));
		SKILL11->setScale(0.25f);
		SKILL11->setVisible(true);
		addChild(SKILL11, 6, 901);
		Sprite* SKILL12 = Sprite::create("SKILL/skill1shade.png");
		SKILL12->setPosition(Vec2(origin.x + visibleSize.width * 0.55, origin.y + visibleSize.height * 0.1));
		SKILL12->setScale(0.25f);
		SKILL12->setVisible(false);
		addChild(SKILL12, 6, 902);
		auto SKILL13 = Sprite::create("SKILL/skill1.png");
		SKILL1TIME = ProgressTimer::create(SKILL13);
		SKILL1TIME->setScale(0.25f);
		SKILL1TIME->setType(kCCProgressTimerTypeRadial);
		SKILL1TIME->setPosition(Vec2(origin.x + visibleSize.width * 0.55, origin.y + visibleSize.height * 0.1));
		addChild(SKILL1TIME, 6, 903);
	}
}

void MapScene::skill2(float dt)
{
	float Current2 = SKILL2TIME->getPercentage();
	if (Current2 <= 99.3333)
	{
		Current2 += 0.6667;
		SKILL2TIME->setPercentage(Current2);
	}
	else
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		unschedule(schedule_selector(MapScene::skill2));

		removeChildByTag(904);
		removeChildByTag(905);
		removeChildByTag(906);

		auto myKeyListener = EventListenerKeyboard::create();
		myKeyListener->onKeyPressed = CC_CALLBACK_2(MapScene::onKeyPressed, this);

		//技能二
		Sprite* SKILL21 = Sprite::create("SKILL/skill2.png");
		SKILL21->setPosition(Vec2(origin.x + visibleSize.width * 0.6, origin.y + visibleSize.height * 0.1));
		SKILL21->setScale(0.25f);
		SKILL21->setVisible(true);
		addChild(SKILL21, 6, 904);
		Sprite* SKILL22 = Sprite::create("SKILL/skill2shade.png");
		SKILL22->setPosition(Vec2(origin.x + visibleSize.width * 0.6, origin.y + visibleSize.height * 0.1));
		SKILL22->setScale(0.25f);
		SKILL22->setVisible(false);
		addChild(SKILL22, 6, 905);
		auto SKILL23 = Sprite::create("SKILL/skill2.png");
		SKILL2TIME = ProgressTimer::create(SKILL23);
		SKILL2TIME->setScale(0.25f);
		SKILL2TIME->setType(kCCProgressTimerTypeRadial);
		SKILL2TIME->setPosition(Vec2(origin.x + visibleSize.width * 0.6, origin.y + visibleSize.height * 0.1));
		addChild(SKILL2TIME, 6, 906);

	}
}

void MapScene::skill3(float dt)
{
	float Current3 = SKILL3TIME->getPercentage();
	if (Current3 <= 99.3333)
	{
		Current3 += 0.6667;
		SKILL3TIME->setPercentage(Current3);
	}
	else
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		unschedule(schedule_selector(MapScene::skill3));

		removeChildByTag(907);
		removeChildByTag(908);
		removeChildByTag(909);
		auto myKeyListener = EventListenerKeyboard::create();
		myKeyListener->onKeyPressed = CC_CALLBACK_2(MapScene::onKeyPressed, this);


		//技能三
		Sprite* SKILL31 = Sprite::create("SKILL/skill3.png");
		SKILL31->setPosition(Vec2(origin.x + visibleSize.width * 0.65, origin.y + visibleSize.height * 0.1));
		SKILL31->setScale(0.25f);
		SKILL31->setVisible(true);
		addChild(SKILL31, 6, 907);
		Sprite* SKILL32 = Sprite::create("SKILL/skill3shade.png");
		SKILL32->setPosition(Vec2(origin.x + visibleSize.width * 0.65, origin.y + visibleSize.height * 0.1));
		SKILL32->setScale(0.25f);
		SKILL32->setVisible(false);
		addChild(SKILL32, 6, 908);
		auto SKILL33 = Sprite::create("SKILL/skill3.png");
		SKILL3TIME = ProgressTimer::create(SKILL33);
		SKILL3TIME->setScale(0.25f);
		SKILL3TIME->setType(kCCProgressTimerTypeRadial);
		SKILL3TIME->setPosition(Vec2(origin.x + visibleSize.width * 0.65, origin.y + visibleSize.height * 0.1));
		addChild(SKILL3TIME, 6, 909);
	}
}

void MapScene::skill4(float dt)
{
	float Current4 = SKILL4TIME->getPercentage();
	if (Current4 <= 99.8333)
	{
		Current4 += 0.1667;
		SKILL4TIME->setPercentage(Current4);

	}
	else
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		unschedule(schedule_selector(MapScene::skill4));

		removeChildByTag(910);
		removeChildByTag(911);
		removeChildByTag(912);

		auto myKeyListener = EventListenerKeyboard::create();
		myKeyListener->onKeyPressed = CC_CALLBACK_2(MapScene::onKeyPressed, this);

		//技能四
		Sprite* SKILL41 = Sprite::create("SKILL/skill4.png");
		SKILL41->setPosition(Vec2(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height * 0.1));
		SKILL41->setScale(0.25f);
		SKILL41->setVisible(true);
		addChild(SKILL41, 6, 910);
		Sprite* SKILL42 = Sprite::create("SKILL/skill4shade.png");
		SKILL42->setPosition(Vec2(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height * 0.1));
		SKILL42->setScale(0.25f);
		SKILL42->setVisible(false);
		addChild(SKILL42, 6, 911);
		auto SKILL43 = Sprite::create("SKILL/skill4.png");
		SKILL4TIME = ProgressTimer::create(SKILL43);
		SKILL4TIME->setScale(0.25f);
		SKILL4TIME->setType(kCCProgressTimerTypeRadial);
		SKILL4TIME->setPosition(Vec2(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height * 0.1));
		addChild(SKILL4TIME, 6, 912);
	}
}


void MapScene::heroIn(float dt)
{
	Vec2 hero_position = hero->herosPosition();
	Vec2 tiledPositon = tiledpos(hero_position);
	int tileGid = _colliable->getTileGIDAt(tiledPositon);
	//CCLOG("%d     ,  ,   %d", tiledPositon.x, tiledPositon.y);
	if (tileGid > 0 && canMove != 2)
	{
		//CCLOG("%d     ,  ,   %d", tiledPositon.x, tiledPositon.y);
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
	int range = 300;
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
	int range = 300;
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
			if (n > 1)
			{
				attactEnemy(Tower1_position, objectPositon, 1, 2044, 1022 + n);
				return;
			}
			if (n == 1)
			{
				attactEnemy(Tower1_position, objectPositon, 1, 2044, 2047);
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
				if (i!=1)
				attactEnemy(Tower1_position, objectPositon, 1,2044,1022+i);
				else attactEnemy(Tower1_position, objectPositon, 1, 2044, 2047);
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
		solder2->start(5, Vec2(650, 450));
		auto solder3 = object::createObject();
		addChild(solder3, 0, 2050);
		solder3->start(4, Vec2(550, 550));
		Friend.pushBack(solder1->enemy);
		Friend.pushBack(solder2->enemy);
		Friend.pushBack(solder3->enemy);
		auto solder11 = object::createObject();
		addChild(solder11, 0, 1024);//1730 1020
		solder11->start(3, Vec2(1600, 900));
		auto solder22 = object::createObject();
		addChild(solder22, 0, 1025);
		solder22->start(5, Vec2(1650, 850));
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
		solder5->start(5, Vec2(650, 450));
		auto solder6 = object::createObject();
		addChild(solder6, 0, 2053);
		solder6->start(4, Vec2(550, 550));
		Friend.pushBack(solder4->enemy);
		Friend.pushBack(solder5->enemy);
		Friend.pushBack(solder6->enemy);
		auto solder44 = object::createObject();
		addChild(solder44, 0, 1027);//1730 1020
		solder44->start(3, Vec2(1600, 900));
		auto solder55 = object::createObject();
		addChild(solder55, 0, 1028);
		solder55->start(5, Vec2(1650, 850));
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
		solder8->start(5, Vec2(650, 450));
		auto solder9 = object::createObject();
		addChild(solder9, 0, 2056);
		solder9->start(4, Vec2(550, 550));
		Friend.pushBack(solder7->enemy);
		Friend.pushBack(solder8->enemy);
		Friend.pushBack(solder9->enemy);
		auto solder77 = object::createObject();
		addChild(solder77, 0, 1030);//1730 1020
		solder77->start(3, Vec2(1600, 900));
		auto solder88 = object::createObject();
		addChild(solder88, 0, 1031);
		solder88->start(5, Vec2(1650, 850));
		auto solder99 = object::createObject();
		addChild(solder99, 0, 1032);
		solder99->start(4, Vec2(1550, 950));
		Enemy.pushBack(solder77->enemy);
		Enemy.pushBack(solder88->enemy);
		Enemy.pushBack(solder99->enemy);
		time++;
	}
}
void MapScene::soldersMake3(float dt)
{
	auto s010 = object::createObject();
	addChild(s010, 0, 2057);
	s010->start(3, Vec2(600, 500));
	auto s011 = object::createObject();
	addChild(s011, 0, 2058);
	s011->start(5, Vec2(650, 450));
	auto s012 = object::createObject();
	addChild(s012, 0, 2059);
	s012->start(4, Vec2(550, 550));
	Friend.pushBack(s010->enemy);
	Friend.pushBack(s011->enemy);
	Friend.pushBack(s012->enemy);
	auto e010 = object::createObject();
	addChild(e010, 0, 1033);//1730 1020
	e010->start(3, Vec2(1600, 900));
	auto e011 = object::createObject();
	addChild(e011, 0, 1034);
	e011->start(5, Vec2(1650, 850));
	auto e012 = object::createObject();
	addChild(e012, 0, 1035);
	e012->start(4, Vec2(1550, 950));
	Enemy.pushBack(e010->enemy);
	Enemy.pushBack(e011->enemy);
	Enemy.pushBack(e012->enemy);
}
//////
void MapScene::soldersMake4(float dt)
{
	auto s013 = object::createObject();
	addChild(s013, 0, 2060);
	s013->start(3, Vec2(600, 500));
	auto s014 = object::createObject();
	addChild(s014, 0, 2061);
	s014->start(5, Vec2(650, 450));
	auto s015 = object::createObject();
	addChild(s015, 0, 2062);
	s015->start(4, Vec2(550, 550));
	Friend.pushBack(s013->enemy);
	Friend.pushBack(s014->enemy);
	Friend.pushBack(s015->enemy);
	auto e013 = object::createObject();
	addChild(e013, 0, 1036);//1730 1020
	e013->start(3, Vec2(1600, 900));
	auto e014 = object::createObject();
	addChild(e014, 0, 1037);
	e014->start(5, Vec2(1650, 850));
	auto e015 = object::createObject();
	addChild(e015, 0, 1038);
	e015->start(4, Vec2(1550, 950));
	Enemy.pushBack(e013->enemy);
	Enemy.pushBack(e014->enemy);
	Enemy.pushBack(e015->enemy);
}
void MapScene::soldersMake5(float dt)
{
	auto s016 = object::createObject();
	addChild(s016, 0, 2063);
	s016->start(3, Vec2(600, 500));
	auto s017 = object::createObject();
	addChild(s017, 0, 2064);
	s017->start(5, Vec2(650, 450));
	auto s018 = object::createObject();
	addChild(s018, 0, 2065);
	s018->start(4, Vec2(550, 550));
	Friend.pushBack(s016->enemy);
	Friend.pushBack(s017->enemy);
	Friend.pushBack(s018->enemy);
	auto e016 = object::createObject();
	addChild(e016, 0, 1039);//1730 1020
	e016->start(3, Vec2(1600, 900));
	auto e017 = object::createObject();
	addChild(e017, 0, 1040);
	e017->start(5, Vec2(1650, 850));
	auto e018 = object::createObject();
	addChild(e018, 0, 1041);
	e018->start(4, Vec2(1550, 950));
	Enemy.pushBack(e016->enemy);
	Enemy.pushBack(e017->enemy);
	Enemy.pushBack(e018->enemy);
}
void MapScene::soldersMake6(float dt)
{
	auto s019 = object::createObject();
	addChild(s019, 0, 2066);
	s019->start(3, Vec2(600, 500));
	auto s020 = object::createObject();
	addChild(s020, 0, 2067);
	s020->start(5, Vec2(650, 450));
	auto s021 = object::createObject();
	addChild(s021, 0, 2068);
	s021->start(4, Vec2(550, 550));
	Friend.pushBack(s019->enemy);
	Friend.pushBack(s020->enemy);
	Friend.pushBack(s021->enemy);
	auto e019 = object::createObject();
	addChild(e019, 0, 1042);//1730 1020
	e019->start(3, Vec2(1600, 900));
	auto e020 = object::createObject();
	addChild(e020, 0, 1043);
	e020->start(5, Vec2(1650, 850));
	auto e021 = object::createObject();
	addChild(e021, 0, 1044);
	e021->start(4, Vec2(1550, 950));
	Enemy.pushBack(e019->enemy);
	Enemy.pushBack(e020->enemy);
	Enemy.pushBack(e021->enemy);
}
void MapScene::soldersMake7(float dt)
{
	auto s022 = object::createObject();
	addChild(s022, 0, 2069);
	s022->start(3, Vec2(600, 500));
	auto s023 = object::createObject();
	addChild(s023, 0, 2070);
	s023->start(5, Vec2(650, 450));
	auto s024 = object::createObject();
	addChild(s024, 0, 2071);
	s024->start(4, Vec2(550, 550));
	Friend.pushBack(s022->enemy);
	Friend.pushBack(s023->enemy);
	Friend.pushBack(s024->enemy);
	auto e022 = object::createObject();
	addChild(e022, 0, 1045);//1730 1020
	e022->start(3, Vec2(1600, 900));
	auto e023 = object::createObject();
	addChild(e023, 0, 1046);
	e023->start(5, Vec2(1650, 850));
	auto e024 = object::createObject();
	addChild(e024, 0, 1047);
	e024->start(4, Vec2(1550, 950));
	Enemy.pushBack(e022->enemy);
	Enemy.pushBack(e023->enemy);
	Enemy.pushBack(e024->enemy);
}
void MapScene::soldersMake8(float dt)
{
	auto s025 = object::createObject();
	addChild(s025, 0, 2072);
	s025->start(3, Vec2(600, 500));
	auto s026 = object::createObject();
	addChild(s026, 0, 2073);
	s026->start(5, Vec2(650, 450));
	auto s027 = object::createObject();
	addChild(s027, 0, 2074);
	s027->start(4, Vec2(550, 550));
	Friend.pushBack(s025->enemy);
	Friend.pushBack(s026->enemy);
	Friend.pushBack(s027->enemy);
	auto e025 = object::createObject();
	addChild(e025, 0, 1048);//1730 1020
	e025->start(3, Vec2(1600, 900));
	auto e026 = object::createObject();
	addChild(e026, 0, 1049);
	e026->start(5, Vec2(1650, 850));
	auto e027 = object::createObject();
	addChild(e027, 0, 1050);
	e027->start(4, Vec2(1550, 950));
	Enemy.pushBack(e025->enemy);
	Enemy.pushBack(e026->enemy);
	Enemy.pushBack(e027->enemy);
}
void MapScene::soldersMake9(float dt)
{
	auto s028 = object::createObject();
	addChild(s028, 0, 2075);
	s028->start(3, Vec2(600, 500));
	auto s029 = object::createObject();
	addChild(s029, 0, 2076);
	s029->start(5, Vec2(650, 450));
	auto s030 = object::createObject();
	addChild(s030, 0, 2077);
	s030->start(4, Vec2(550, 550));
	Friend.pushBack(s028->enemy);
	Friend.pushBack(s029->enemy);
	Friend.pushBack(s030->enemy);
	auto e028 = object::createObject();
	addChild(e028, 0, 1051);//1730 1020
	e028->start(3, Vec2(1600, 900));
	auto e029 = object::createObject();
	addChild(e029, 0, 1052);
	e029->start(5, Vec2(1650, 850));
	auto e030 = object::createObject();
	addChild(e030, 0, 1053);
	e030->start(4, Vec2(1550, 950));
	Enemy.pushBack(e028->enemy);
	Enemy.pushBack(e029->enemy);
	Enemy.pushBack(e030->enemy);
}
void MapScene::anti(float dt)
{
	object* antiHero = (object*)getChildByTag(2047);
	auto aHero = Enemy.at(1);
	Vec2 antiPos = antiHero->enemy->getPosition();
	int minDis = 10000;
	int range = 300;
	int target=-1;
	auto bloodLine = antiHero->Loading;
	Vec2 destination = Vec2(500,420);
	for (int i = 0; i <Friend.size(); i++)
	{
		Vec2 ourPos = Friend.at(i)->getPosition();

		int distance = getDistance(antiPos, ourPos);
		if (minDis > distance)
		{
			minDis = distance;
			target = i;

		}
	}
	if (minDis > range)
	{
		auto * move = MoveTo::create(getDistance(antiPos,destination)/50, destination);
		auto* moveBlood = MoveTo::create(getDistance(antiPos, destination)/50,Vec2(destination.x,destination.y+50));
		aHero->stopAllActions();
		getDistance(antiPos, destination);
		bloodLine->stopAllActions();
		
		aHero->runAction(move);
		aHero->runAction(createAnimateH(2, "run", 7, 1));
		bloodLine->runAction(moveBlood);
	}
	else if (minDis > range / 2)
	{
		aHero->stopAllActions();
		bloodLine->stopAllActions();
		aHero->runAction(createAnimateH(2, "run", 7, 1));
		auto* Move = MoveTo::create(minDis / 50, Friend.at(target)->getPosition());
		auto* MoveBlood = MoveTo::create(minDis/50, Vec2(Friend.at(target)->getPosition().x, Friend.at(target)->getPosition().y + 50));
		aHero->runAction(Move);
		bloodLine->runAction(MoveBlood);
	}
	else
	{
		aHero->stopAllActions();
		aHero->runAction(createAnimateH(2, "att", 7, 1 ));
		bloodLine->stopAllActions();
		if (target > 1)
			attactEnemy(antiPos, Friend.at(target)->getPosition(), 5,2047, 1022+ target);
		if (target == 0)
			attactEnemy(antiPos, Friend.at(target)->getPosition(),5, 2047, 2044);
		if (target == 1)
			attactEnemy(antiPos, Friend.at(target)->getPosition(), 5, 2047, 2046);
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
				if (minDis <= solder->range)
				{
					a->stopAllActions();
					a->runAction(createAnimateS(4, "att", 8, 1,-1));
					bloodLine->stopAllActions();
					if(min>1) attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i,2046+i,1022+min);
					else if(min==1)  attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i, 2046 + i, 2047);
					else if(min==0) attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i, 2046 + i, 2045);
				}
				if (minDis <= solder->range+100 && minDis > solder->range)
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
				if (minDis > solder->range + 100)
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
				if (minDis <= solder->range )
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(4, "att", 14, 5,1));
					if (min >1) attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i, 2046 + i, 1022 + min);
					else if (min == 1)  attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i, 2046 + i, 2047);
					else if(min==0) attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i, 2046 + i, 2045);
				}
				if (minDis <= solder->range + 100 && minDis > solder->range)
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
				if (minDis > solder->range+100)
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
				if (minDis <= solder->range)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(4, "att", 14, 6,1));
					if (min >1) attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i, 2046 + i, 1022 + min);
					else if (min == 1)  attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i, 2046 + i, 2047);
					else if(min==0) attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i, 2046 + i, 2045);
				}
				if (minDis <= solder->range+100 && minDis > solder->range)
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
				if (minDis > solder->range+100)
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
		for (int i = 2; i < Enemy.size(); i++)
		{
			auto a = Enemy.at(i);
			Size s = a->getContentSize();
			//Rect rect = Rect(0, 0, s.width, s.height);
			object* solder = (object*)getChildByTag(1022 + i);
			auto bloodLine = solder->Loading;
			//CCLOG("**%d**", solder->blood);
			if (solder->blood <= 0)
				continue;
			Vec2 soldersPostion = a->getPosition();
			if ((i + 1) % 3 == 0)
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
				if (minDis <= solder->range+100 && minDis > solder->range)
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
				if (minDis <= solder->range)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(2, "att", 14, 4, 1));
					if (min > 1)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 1022 + i, 2046 + min);
					if(min==0)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 1022 + i, 2044);
					if(min==1)
					attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 1022 + i, 2046);
				}
				if (minDis > solder->range+100)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(2, "run", 8, 4,-1));
					a->runAction(sequence);
					bloodLine->runAction(moveBlood);
				}
			}
			if ((i + 1) % 3 == 1)
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
				if (minDis <= solder->range+100 && minDis > solder->range)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(2, "run", 8, 5, -1));
					float distance = getDistance(soldersPostion, Friend.at(min)->getPosition());
					auto* Move = MoveTo::create((float)distance / 50, Friend.at(min)->getPosition());
					auto* MoveBlood = MoveTo::create((float)distance / 50, Vec2(Friend.at(min)->getPosition().x, Friend.at(min)->getPosition().y + 50));
					a->runAction(Move);
					bloodLine->runAction(MoveBlood);
				}
				if (minDis <= solder->range)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(2, "att", 14, 5, 1));
					if (min > 1)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 1022 + i, 2046 + min);
					if (min == 0)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 1022 + i, 2044);
					if (min == 1)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 1022 + i, 2046);
				}
				if (minDis > solder->range+100)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(2, "run", 8, 5,-1));
					a->runAction(sequence);
					bloodLine->runAction(moveBlood);
				}
			}
			if ((i + 1) % 3 == 2)
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
				if (minDis <= solder->range+100 && minDis > solder->range)
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
				if (minDis <= solder->range)
				{
					a->stopAllActions();
					bloodLine->stopAllActions();
					a->runAction(createAnimateS(2, "att", 14, 6, 1));
					bloodLine->stopAllActions();
					if (min > 1)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 1022 + i, 2046 + min);
					if (min == 0)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 1022 + i, 2044);
					if (min == 1)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 1022 + i, 2046);
				}
				if (minDis > solder->range+100 )
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
	float n=1.0;
	if (tag1 != 2046)
	{
		object* solder = (object*)getChildByTag(tag1);
		n = 1 / solder->attack_speed;
	}
	if(tag1==2046)
	{
		n = 1 /hero->attack_speed;
	}
	auto delayTime = DelayTime::create(n);
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
void MapScene::skillQ(float dt)
{
	if (hero->skill_Q == 1 && timeQ != 20)
	{
		if (timeQ == 0)
			hero->speed = 150;
		timeQ++;
	}
	if (timeQ == 10)
	{
		hero->heroSprite->stopAllActions();
		hero->speed = 50;
	}
	if (timeQ == 20)
	{
		timeQ = 0;
		hero->skill_Q = 0;
		CCLOG("Q is ready");
	}
}
void MapScene::skillW(float dt)
{
	if (hero->skill_W == 1 && timeW != 30)
	{

		timeW++;
		hero->Armor = hero->Armor * 2;
		hero->magic_resistance = hero->magic_resistance * 2;
	}
	if (timeW == 1 && hero->skill_W == 1)
	{
		myMagic->runAction(createAnimateM(2, 10, 0.1, 1));
	}
	if (timeW == 6)
	{
	
		myMagic->runAction(createAnimateM());
		hero->Armor = hero->Armor / 2;
		hero->magic_resistance = hero->magic_resistance / 2;
	}
	if (timeW == 30)
	{
		timeW = 0;
		hero->skill_W = 0;
		CCLOG("W is ready");
	}
}
void MapScene::skillE(float dt)
{
	if (hero->skill_E == 1 && timeE != 30)
	{
		timeE++;
	}
	if (hero->skill_E == 1&&timeE <= 10)
	{
		Vec2 pos = hero->herosPosition();
		myMagic->runAction(createAnimateM(1, 20,0.025,1));
		for (int i = 1; i < Enemy.size(); i++)
		{
			auto a = Enemy.at(i);
			Vec2 Epos = a->getPosition();
			float dis = getDistance(pos, Epos);
			if (dis <= 150)
			{
				object* solder;
				if(i==1)
				solder = (object*)getChildByTag(2047);
				else
				solder = (object*)getChildByTag(1022+i);
				solder->blood -= (hero->level_E * 10 + 20);
				float cur = (((float)solder->blood / (float)solder->maxBlood) * 100);
				solder->Loading->setPercentage(cur);
				if (solder->blood <= 0)
				{
					if (solder->getTag() == 2047)
					{
						solder->blood = 10;
						solder->enemy->setPosition(Vec2(1460, 1030));
						solder->Loading->setPosition(Vec2(1460, 1080));
						solder->Loading->setPercentage(100.0);
					}
					else
					solder->enemy->setPosition(-2000, -2000);
					hero->exprience = hero->exprience + 10;
					if (hero->needExprience == hero->exprience)
					{
						hero->level++; hero->needExprience = hero->needExprience + 20; hero->exprience = 0; hero->skillNum = 1;
						hero->blood += 50;
						hero->maxBoold += 50;
						hero->Aggressivity += 5;
						hero->Armor += 20;
						hero->attack_speed += 0.2;
					}
				}
			}
		}
	}
	if (timeE == 30)
	{
		timeE = 0;
		hero->skill_E = 0;
		CCLOG("E is ready");
	}
}
void MapScene::skillR(float dt)
{

	if (hero->skill_R == 1 && timeR == 0)
	{
		Vec2 hPos = hero->herosPosition();
		Vec2 aPos = Enemy.at(1)->getPosition();
		float d = getDistance(hPos, aPos);
		if (d <= 300)
		{
			antiMagic->runAction(createAnimateM(6, 22, 0.1, 1));
		}
	}
	if (hero->skill_R == 1 && timeR == 5)
	{
		Vec2 hPos = hero->herosPosition();
		Vec2 aPos = Enemy.at(1)->getPosition();
		float d = getDistance(hPos, aPos);
		if (d <= 300)
		{
			
			object* e = (object*)getChildByTag(2047);
			e->blood = e->blood - 5000;
			float cur = (((float)e->blood / (float)e->maxBlood) * 100);
			e->Loading->setPercentage(cur);
			if (e->blood <= 0)
			{
				e->blood = 10;
				e->enemy->setPosition(Vec2(1460, 1030));
				e->Loading->setPosition(Vec2(1460, 1080));
				e->Loading->setPercentage(100.0);
			}
			antiMagic->runAction(createAnimateM());

		}
		
	}
	
	if (hero->skill_R == 1 && timeR != 120)
	{
		timeR++;
		hero->R = 1;
	}
	if (timeR == 120)
	{
		timeR = 0;
		hero->skill_R = 0;
		CCLOG("W is ready");
	}
}
void MapScene::finishAttack(const int tag1, const int tag2)
{

	if (tag1 != 2046)
	{
		object* solder1 = (object*)getChildByTag(tag1);
		//读取属性及计算
		if (tag2 != 2046)
		{
			object* solder1 = (object*)getChildByTag(tag1);
			object* solder2 = (object*)getChildByTag(tag2);
			solder2->blood = solder2->blood - solder1->Aggressivity * (1 - 0.01 * (solder2->Armor / 10));
			float cur = (((float)solder2->blood / (float)solder2->maxBlood) * 100);
			solder2->Loading->setPercentage(cur);
			if (solder2->blood <= 0)
			{
				if (tag2 == 2047)
				{
					solder2->blood = 10;
					solder2->enemy->setPosition(Vec2(1460, 1030));
					solder2->Loading->setPosition(Vec2(1460, 1080));
					solder2->Loading->setPercentage(100.0);
				}
				else
				{
					solder2->enemy->setPosition(-2000, -2000);
				}
				if (tag2 == 2047 || (tag2 > 1023 && tag2 < 2044)||tag2==2045)
					hero->exprience = hero->exprience + 10;
				if (hero->needExprience == hero->exprience)
				{
					hero->level++; hero->needExprience = hero->needExprience + 20; hero->exprience = 0; hero->skillNum = 1;
					hero->blood += 50;
					hero->maxBoold += 50;
					hero->Aggressivity += 5;
					hero->Armor += 20;
					hero->attack_speed += 0.2;
				}
			}
		}
		else
		{
			object* solder1 = (object*)getChildByTag(tag1);
			heroPrint* hero = (heroPrint*)getChildByTag(2046);
			hero->blood = hero->blood - solder1->Aggressivity * (1 - 0.01 * (hero->Armor / 10));
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
		if(timeQ<=10&&hero->Q==1)
		{ 
			solder2->blood = solder2->blood - (hero->Aggressivity+50+hero->level_Q*30) * (1 - 0.01 * (solder2->Armor / 10));
			hero->Q = 0;
		}
		else
		solder2->blood = solder2->blood - hero->Aggressivity * (1 - 0.01 * (solder2->Armor / 10));
		float cur = (((float)solder2->blood / (float)solder2->maxBlood) * 100);
		solder2->Loading->setPercentage(cur);
		if (solder2->blood <= 0)
		{
			if (tag2 == 2047)
			{
				solder2->blood = 10;
				solder2->enemy->setPosition(Vec2(1860, 1030));
				solder2->Loading->setPosition(Vec2(1860, 1080));
			}
			else
			{
				solder2->enemy->setPosition(-2000, -2000);
			}
			if(tag2==2047||(tag2>1023&&tag2<=2048))
			hero->exprience = hero->exprience + 10;
			if(hero->needExprience== hero->exprience)
			{
				hero->level++; hero->needExprience = hero->needExprience + 20; hero->exprience = 0; 
				hero->skillNum = 1;
				hero->blood+=10;
				hero->maxBoold+=10;
				hero->Aggressivity+=5;
				hero->Armor+=20;
				hero->attack_speed+=0.2;
			}
			MyGold = MyGold + solder2->gold;
			/*hero->level;
			hero->needExprience;
			hero->exprience;*/
		}
	//		solder2->enemy->setPosition(-2000, -2000);
	}
}
void MapScene::EnterHelloWorldScene(Ref* pSenderBack)
{
	//全局重新初始化
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	MyHeroID = 0;
	MyGold = 10000;
	MyBuyWeaponNum = 0;
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, HelloWorld::createScene()));
}

void MapScene::Shopcall(Ref* sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		SimpleAudioEngine::getInstance()->playEffect("Touch.wav");
		//战绩图标不可触发
		Button* tmp1 = (Button*)this->getChildByTag(51);
		tmp1->setTouchEnabled(false);
		//商城图标不可触发
		Button* tmp2 = (Button*)this->getChildByTag(50);
		tmp2->setTouchEnabled(false);

		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		Button* ShopBackButton = Button::create();
		ShopBackButton->loadTextures("ShopBack.png", "ShopBack.png", "");
		ShopBackButton->setPosition(Vec2(1665, 910));
		ShopBackButton->addTouchEventListener(CC_CALLBACK_2(MapScene::ShopBack, this));
		ShopBackButton->setScale(0.72f);
		addChild(ShopBackButton, 14, 64);

		Sprite* ShopBackGround;
		ShopBackGround = Sprite::create("Shopbackground.png");
		ShopBackGround->setPosition(Vec2(960, 550));
		ShopBackGround->setScale(0.85f);
		ShopBackGround->setOpacity(230);
		addChild(ShopBackGround, 10, 60);

		//装备方案图标
		Sprite* EquipPlan = Sprite::create("plan.png");
		EquipPlan->setPosition(Vec2(280, 200));
		EquipPlan->setScale(1.0f);
		addChild(EquipPlan, 10, 61);

		//左上角小部件
		Sprite* EquipLittle1 = Sprite::create("little.png");
		EquipLittle1->setPosition(Vec2(200, 900));
		EquipLittle1->setScale(1.0f);
		addChild(EquipLittle1, 10, 62);

		//"商城"文字
		auto* chnString = Dictionary::createWithContentsOfFile("CHN_String.xml");
		const char* SHOP_string = ((String*)chnString->objectForKey("SHOP_Text"))->getCString();
		Label* SHOP = Label::create(SHOP_string, "Arial", 33);
		SHOP->setPosition(Vec2(300, 900));
		addChild(SHOP, 10, 63);

		//武器一
		const char* Weapon1_string = ((String*)chnString->objectForKey("Weapon1_Text"))->getCString();
		Label* Weapon1String = Label::create(Weapon1_string, "Arial", 38);
		Weapon1String->setPosition(Vec2(750, 750));
		addChild(Weapon1String, 12, 1010);

		Button* Weapon1 = Button::create();
		Weapon1->loadTextures("weapon1.png", "weapon1.png", "");
		Weapon1->setPosition(Vec2(550, 750));
		Weapon1->setScale(0.8f);
		Weapon1->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy1, this));
		addChild(Weapon1, 12, 101);

		//武器二
		const char* Weapon2_string = ((String*)chnString->objectForKey("Weapon2_Text"))->getCString();
		Label* Weapon2String = Label::create(Weapon2_string, "Arial", 38);
		Weapon2String->setPosition(Vec2(1200, 750));
		addChild(Weapon2String, 12, 1020);

		Button* Weapon2 = Button::create();
		Weapon2->loadTextures("weapon2.png", "weapon2.png", "");
		Weapon2->setPosition(Vec2(1000, 750));
		Weapon2->setScale(0.8f);
		Weapon2->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy2, this));
		addChild(Weapon2, 12, 102);

		//武器三
		const char* Weapon3_string = ((String*)chnString->objectForKey("Weapon3_Text"))->getCString();
		Label* Weapon3String = Label::create(Weapon3_string, "Arial", 38);
		Weapon3String->setPosition(Vec2(750, 550));
		addChild(Weapon3String, 12, 1030);

		Button* Weapon3 = Button::create();
		Weapon3->loadTextures("weapon3.png", "weapon3.png", "");
		Weapon3->setPosition(Vec2(550, 550));
		Weapon3->setScale(0.8f);
		Weapon3->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy3, this));
		addChild(Weapon3, 12, 103);

		//武器四
		const char* Weapon4_string = ((String*)chnString->objectForKey("Weapon4_Text"))->getCString();
		Label* Weapon4String = Label::create(Weapon4_string, "Arial", 38);
		Weapon4String->setPosition(Vec2(1200, 550));
		addChild(Weapon4String, 12, 1040);

		Button* Weapon4 = Button::create();
		Weapon4->loadTextures("weapon4.png", "weapon4.png", "");
		Weapon4->setPosition(Vec2(1000, 550));
		Weapon4->setScale(0.8f);
		Weapon4->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy4, this));
		addChild(Weapon4, 12, 104);

		//武器五
		const char* Weapon5_string = ((String*)chnString->objectForKey("Weapon5_Text"))->getCString();
		Label* Weapon5String = Label::create(Weapon5_string, "Arial", 38);
		Weapon5String->setPosition(Vec2(750, 350));
		addChild(Weapon5String, 12, 1050);

		Button* Weapon5 = Button::create();
		Weapon5->loadTextures("weapon5.png", "weapon5.png", "");
		Weapon5->setPosition(Vec2(550, 350));
		Weapon5->setScale(0.8f);
		Weapon5->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy5, this));
		addChild(Weapon5, 12, 105);

		//武器六
		const char* Weapon6_string = ((String*)chnString->objectForKey("Weapon6_Text"))->getCString();
		Label* Weapon6String = Label::create(Weapon6_string, "Arial", 38);
		Weapon6String->setPosition(Vec2(1200, 350));
		addChild(Weapon6String, 12, 1060);

		Button* Weapon6 = Button::create();
		Weapon6->loadTextures("weapon6.png", "weapon6.png", "");
		Weapon6->setPosition(Vec2(1000, 350));
		Weapon6->setScale(0.8f);
		Weapon6->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy6, this));
		addChild(Weapon6, 12, 106);


		Sprite* Equipment1 = Sprite::create("kong.png");
		Equipment1->setPosition(Vec2(500, 200));
		Equipment1->setScale(0.8f);
		addChild(Equipment1, 12, 71);

		Sprite* Equipment2 = Sprite::create("kong.png");
		Equipment2->setPosition(Vec2(650, 200));
		Equipment2->setScale(0.8f);
		addChild(Equipment2, 12, 72);

		Sprite* Equipment3 = Sprite::create("kong.png");
		Equipment3->setPosition(Vec2(800, 200));
		Equipment3->setScale(0.8f);
		addChild(Equipment3, 12, 73);

		Sprite* Equipment4 = Sprite::create("kong.png");
		Equipment4->setPosition(Vec2(950, 200));
		Equipment4->setScale(0.8f);
		addChild(Equipment4, 12, 74);

		Sprite* Equipment5 = Sprite::create("kong.png");
		Equipment5->setPosition(Vec2(1100, 200));
		Equipment5->setScale(0.8f);
		addChild(Equipment5, 12, 75);

		Sprite* Equipment6 = Sprite::create("kong.png");
		Equipment6->setPosition(Vec2(1250, 200));
		Equipment6->setScale(0.8f);
		addChild(Equipment6, 12, 76);

		for (int p = 1; p <= MyBuyWeaponNum; ++p)
		{
			Sprite* Tmp = (Sprite*)this->getChildByTag(40 + p);
			Tmp->setVisible(true);
		}

	}
}

void MapScene::ZhanjiCall(Ref* sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		SimpleAudioEngine::getInstance()->playEffect("Touch.wav");
		//战绩图标不可触发
		Button* tmp1 = (Button*)this->getChildByTag(51);
		tmp1->setTouchEnabled(false);
		//商城图标不可触发
		Button* tmp2 = (Button*)this->getChildByTag(50);
		tmp2->setTouchEnabled(false);

		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		auto* chnString = Dictionary::createWithContentsOfFile("CHN_String.xml");
		const char* KillStr = ((String*)chnString->objectForKey("Kill_Text"))->getCString();
		const char* DeadStr = ((String*)chnString->objectForKey("Dead_Text"))->getCString();
		const char* ZhanjiStr = ((String*)chnString->objectForKey("Zhanji_Text"))->getCString();

		Button* ZhanjiBackButton = Button::create();
		ZhanjiBackButton->loadTextures("ShopBack.png", "ShopBack.png", "");
		ZhanjiBackButton->setPosition(Vec2(1665, 910));
		ZhanjiBackButton->addTouchEventListener(CC_CALLBACK_2(MapScene::ZhanjiBack, this));
		ZhanjiBackButton->setScale(0.72f);
		addChild(ZhanjiBackButton, 14, 80);

		Sprite* ZhanjiBackGround;
		ZhanjiBackGround = Sprite::create("Shopbackground.png");
		ZhanjiBackGround->setPosition(Vec2(960, 550));
		ZhanjiBackGround->setScale(0.85f);
		ZhanjiBackGround->setOpacity(230);
		addChild(ZhanjiBackGround, 10, 81);

		Sprite* LittleTag = Sprite::create("little.png");
		LittleTag->setPosition(Vec2(180, 900));
		addChild(LittleTag, 14, 82);


		Label* Zhanjilabel = Label::create(ZhanjiStr, "Arial", 34);
		Zhanjilabel->setPosition(Vec2(250, 900));
		addChild(Zhanjilabel, 14, 97);

		//我方头像
		Sprite* MeAvatar;
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
		MeAvatar->setPosition(Vec2(origin.x + visibleSize.width * 0.25, origin.y + visibleSize.height * 0.5));
		addChild(MeAvatar, 12, 83);

		//对方头像
		Sprite* YouAvatar;
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
		YouAvatar->setPosition(Vec2(origin.x + visibleSize.width * 0.725, origin.y + visibleSize.height * 0.5));
		addChild(YouAvatar, 12, 84);

		//我方空方案
		Sprite* MyEquipment1 = Sprite::create("kong.png");
		MyEquipment1->setPosition(Vec2(250, 300));
		MyEquipment1->setScale(0.6f);
		addChild(MyEquipment1, 12, 201);

		Sprite* MyEquipment2 = Sprite::create("kong.png");
		MyEquipment2->setPosition(Vec2(350, 300));
		MyEquipment2->setScale(0.6f);
		addChild(MyEquipment2, 12, 202);

		Sprite* MyEquipment3 = Sprite::create("kong.png");
		MyEquipment3->setPosition(Vec2(450, 300));
		MyEquipment3->setScale(0.6f);
		addChild(MyEquipment3, 12, 203);

		Sprite* MyEquipment4 = Sprite::create("kong.png");
		MyEquipment4->setPosition(Vec2(550, 300));
		MyEquipment4->setScale(0.6f);
		addChild(MyEquipment4, 12, 204);

		Sprite* MyEquipment5 = Sprite::create("kong.png");
		MyEquipment5->setPosition(Vec2(650, 300));
		MyEquipment5->setScale(0.6f);
		addChild(MyEquipment5, 12, 205);

		Sprite* MyEquipment6 = Sprite::create("kong.png");
		MyEquipment6->setPosition(Vec2(750, 300));
		MyEquipment6->setScale(0.6f);
		addChild(MyEquipment6, 12, 206);

		//对方空方案
		Sprite* YouEquipment1 = Sprite::create("kong.png");
		YouEquipment1->setPosition(Vec2(1150, 300));
		YouEquipment1->setScale(0.6f);
		addChild(YouEquipment1, 12, 207);

		Sprite* YouEquipment2 = Sprite::create("kong.png");
		YouEquipment2->setPosition(Vec2(1250, 300));
		YouEquipment2->setScale(0.6f);
		addChild(YouEquipment2, 12, 208);

		Sprite* YouEquipment3 = Sprite::create("kong.png");
		YouEquipment3->setPosition(Vec2(1350, 300));
		YouEquipment3->setScale(0.6f);
		addChild(YouEquipment3, 12, 209);

		Sprite* YouEquipment4 = Sprite::create("kong.png");
		YouEquipment4->setPosition(Vec2(1450, 300));
		YouEquipment4->setScale(0.6f);
		addChild(YouEquipment4, 12, 210);

		Sprite* YouEquipment5 = Sprite::create("kong.png");
		YouEquipment5->setPosition(Vec2(1550, 300));
		YouEquipment5->setScale(0.6f);
		addChild(YouEquipment5, 12, 211);

		Sprite* YouEquipment6 = Sprite::create("kong.png");
		YouEquipment6->setPosition(Vec2(1650, 300));
		YouEquipment6->setScale(0.6f);
		addChild(YouEquipment6, 12, 212);

		//设置我方装备可见
		for (int q = 1; q <= MyBuyWeaponNum; ++q)
		{
			Sprite* TMP = (Sprite*)this->getChildByTag(810 + q);
			if (TMP != nullptr)
			{
				TMP->setVisible(true);
			}
		}

		//设置对方装备可见
		/*for (int q = 1; q <= YouBuyWeaponNum; ++q)
		{
		Sprite *TMP = (Sprite *)this->getChildByTag();
		TMP->setVisible(true);
		}*/

		//我方金币
		auto MyGoldZhanji = Sprite::create("gold.png");
		MyGoldZhanji->setPosition(Vec2(450, 200));
		MyGoldZhanji->setScale(1.0f);
		addChild(MyGoldZhanji, 14, 85);

		CCString* MygoldCCstringZhanji = CCString::createWithFormat("%d", MyGold);
		std::string MygoldstringZhanji = MygoldCCstringZhanji->getCString();
		Label* MyGoldNumZhanji = Label::createWithTTF(MygoldstringZhanji, "fonts/Marker Felt.ttf", 32);
		MyGoldNumZhanji->setPosition(Vec2(520, 200));
		MyGoldNumZhanji->setScale(1.0f);
		addChild(MyGoldNumZhanji, 14, 86);

		//对方金币
		auto YouGoldZhanji = Sprite::create("gold.png");
		YouGoldZhanji->setPosition(Vec2(1350, 200));
		YouGoldZhanji->setScale(1.0f);
		addChild(YouGoldZhanji, 14, 87);

		CCString* YougoldCCstringZhanji = CCString::createWithFormat("%d", YourGold);
		std::string YougoldstringZhanji = YougoldCCstringZhanji->getCString();
		Label* YouGoldNumZhanji = Label::createWithTTF(YougoldstringZhanji, "fonts/Marker Felt.ttf", 32);
		YouGoldNumZhanji->setPosition(Vec2(1420, 200));
		YouGoldNumZhanji->setScale(1.0f);
		addChild(YouGoldNumZhanji, 14, 88);

		//我方击杀

		auto MeKillstr = Label::create(KillStr, "Arial", 36);
		MeKillstr->setPosition(Vec2(300, 800));
		addChild(MeKillstr, 14, 89);

		CCString* MeKillCCstring = CCString::createWithFormat("%d", MeKill);
		std::string MeKillstring = MeKillCCstring->getCString();
		Label* MeKillNum = Label::createWithTTF(MeKillstring, "fonts/Marker Felt.ttf", 35);
		MeKillNum->setPosition(Vec2(300, 750));
		MeKillNum->setScale(1.0f);
		addChild(MeKillNum, 14, 90);


		//我方死亡
		auto MeDeadstr = Label::create(DeadStr, "Arial", 36);
		MeDeadstr->setPosition(Vec2(700, 800));
		addChild(MeDeadstr, 14, 91);

		CCString* MeDeadCCstring = CCString::createWithFormat("%d", MeDead);
		std::string MeDeadstring = MeDeadCCstring->getCString();
		Label* MeDeadNum = Label::createWithTTF(MeDeadstring, "fonts/Marker Felt.ttf", 35);
		MeDeadNum->setPosition(Vec2(700, 750));
		MeDeadNum->setScale(1.0f);
		addChild(MeDeadNum, 14, 92);

		//对方击杀

		auto YouKillstr = Label::create(KillStr, "Arial", 36);
		YouKillstr->setPosition(Vec2(1200, 800));
		addChild(YouKillstr, 14, 93);

		CCString* YouKillCCstring = CCString::createWithFormat("%d", YouKill);
		std::string YouKillstring = YouKillCCstring->getCString();
		Label* YouKillNum = Label::createWithTTF(YouKillstring, "fonts/Marker Felt.ttf", 35);
		YouKillNum->setPosition(Vec2(1200, 750));
		YouKillNum->setScale(1.0f);
		addChild(YouKillNum, 14, 94);


		//对方死亡
		auto YouDeadstr = Label::create(DeadStr, "Arial", 36);
		YouDeadstr->setPosition(Vec2(1600, 800));
		addChild(YouDeadstr, 14, 95);

		CCString* YouDeadCCstring = CCString::createWithFormat("%d", YouDead);
		std::string YouDeadstring = YouDeadCCstring->getCString();
		Label* YouDeadNum = Label::createWithTTF(YouDeadstring, "fonts/Marker Felt.ttf", 35);
		YouDeadNum->setPosition(Vec2(1600, 750));
		YouDeadNum->setScale(1.0f);
		addChild(YouDeadNum, 14, 96);

	}
}

void MapScene::ZhanjiBack(Ref* sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		SimpleAudioEngine::getInstance()->playEffect("Touch.wav");
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
			Sprite* TMP = (Sprite*)this->getChildByTag(810 + j);
			TMP->setVisible(false);
		}
		//恢复商城可触发
		Button* tmp1 = (Button*)this->getChildByTag(50);
		tmp1->setTouchEnabled(true);
		//恢复战绩可触发
		Button* tmp2 = (Button*)this->getChildByTag(51);
		tmp2->setTouchEnabled(true);
	}
}

void MapScene::Shopbuy1(Ref* sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		SimpleAudioEngine::getInstance()->playEffect("Touch.wav");
		for (int j = 1; j <= 6; ++j)
		{
			for (int i = 1; i <= 3; ++i)
			{
				removeChildByTag(1000 + i + j * 10);
			}
		}

		Sprite* Weapon1Detail;
		Weapon1Detail = Sprite::create("weapon1detail.png");
		Weapon1Detail->setPosition(Vec2(1653, 593));
		Weapon1Detail->setScale(0.7f);
		addChild(Weapon1Detail, 13, 1011);

		if (MyGold >= 1740 && MyBuyWeaponNum <= 5)
		{
			Button* BuyButton11 = Button::create();
			BuyButton11->loadTextures("buy1.png", "buy1.png", "");
			BuyButton11->setPosition(Vec2(1650, 180));
			BuyButton11->setScale(1.0f);
			BuyButton11->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit1, this));
			addChild(BuyButton11, 14, 1012);
		}

		if (MyGold < 1740 || MyBuyWeaponNum >= 6)
		{
			Sprite* BuyButton12;
			BuyButton12 = Sprite::create("buy2.png");
			BuyButton12->setPosition(Vec2(1650, 180));
			BuyButton12->setScale(1.0f);
			addChild(BuyButton12, 14, 1013);
		}

	}
}

void MapScene::Shopbuy2(Ref* sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		SimpleAudioEngine::getInstance()->playEffect("Touch.wav");
		for (int j = 1; j <= 6; ++j)
		{
			for (int i = 1; i <= 3; ++i)
			{
				removeChildByTag(1000 + i + j * 10);
			}
		}

		Sprite* Weapon2Detail;
		Weapon2Detail = Sprite::create("weapon2detail.png");
		Weapon2Detail->setPosition(Vec2(1650, 590));
		Weapon2Detail->setScale(0.7f);
		addChild(Weapon2Detail, 13, 1021);

		if (MyGold >= 2140 && MyBuyWeaponNum <= 5)
		{
			Button* BuyButton21 = Button::create();
			BuyButton21->loadTextures("buy1.png", "buy1.png", "");
			BuyButton21->setPosition(Vec2(1650, 180));
			BuyButton21->setScale(1.0f);
			BuyButton21->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit2, this));
			addChild(BuyButton21, 14, 1022);
		}

		if (MyGold < 2140 || MyBuyWeaponNum >= 6)
		{
			Sprite* BuyButton22;
			BuyButton22 = Sprite::create("buy2.png");
			BuyButton22->setPosition(Vec2(1650, 180));
			BuyButton22->setScale(1.0f);
			addChild(BuyButton22, 14, 1023);
		}
	}
}

void MapScene::Shopbuy3(Ref* sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		SimpleAudioEngine::getInstance()->playEffect("Touch.wav");
		for (int j = 1; j <= 6; ++j)
		{
			for (int i = 1; i <= 3; ++i)
			{
				removeChildByTag(1000 + i + j * 10);
			}
		}

		Sprite* Weapon3Detail;
		Weapon3Detail = Sprite::create("weapon3detail.png");
		Weapon3Detail->setPosition(Vec2(1653, 590));
		Weapon3Detail->setScale(0.7f);
		addChild(Weapon3Detail, 13, 1031);


		if (MyGold >= 2300 && MyBuyWeaponNum <= 5)
		{
			Button* BuyButton31 = Button::create();
			BuyButton31->loadTextures("buy1.png", "buy1.png", "");
			BuyButton31->setPosition(Vec2(1650, 180));
			BuyButton31->setScale(1.0f);
			BuyButton31->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit3, this));
			addChild(BuyButton31, 14, 1032);
		}

		if (MyGold < 2300 || MyBuyWeaponNum >= 6)
		{
			Sprite* BuyButton32;
			BuyButton32 = Sprite::create("buy2.png");
			BuyButton32->setPosition(Vec2(1650, 180));
			BuyButton32->setScale(1.0f);
			addChild(BuyButton32, 14, 1033);
		}
	}
}

void MapScene::Shopbuy4(Ref* sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		SimpleAudioEngine::getInstance()->playEffect("Touch.wav");
		for (int j = 1; j <= 6; ++j)
		{
			for (int i = 1; i <= 3; ++i)
			{
				removeChildByTag(1000 + i + j * 10);
			}
		}
		Sprite* Weapon4Detail;
		Weapon4Detail = Sprite::create("weapon4detail.png");
		Weapon4Detail->setPosition(Vec2(1653, 590));
		Weapon4Detail->setScale(0.7f);
		addChild(Weapon4Detail, 13, 1041);


		if (MyGold >= 2990 && MyBuyWeaponNum <= 5)
		{
			Button* BuyButton41 = Button::create();
			BuyButton41->loadTextures("buy1.png", "buy1.png", "");
			BuyButton41->setPosition(Vec2(1650, 180));
			BuyButton41->setScale(1.0f);
			BuyButton41->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit4, this));
			addChild(BuyButton41, 14, 1042);
		}

		if (MyGold < 2990 || MyBuyWeaponNum >= 6)
		{
			Sprite* BuyButton42;
			BuyButton42 = Sprite::create("buy2.png");
			BuyButton42->setPosition(Vec2(1650, 180));
			BuyButton42->setScale(1.0f);
			addChild(BuyButton42, 14, 1043);
		}
	}
}

void MapScene::Shopbuy5(Ref* sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		SimpleAudioEngine::getInstance()->playEffect("Touch.wav");
		for (int j = 1; j <= 6; ++j)
		{
			for (int i = 1; i <= 3; ++i)
			{
				removeChildByTag(1000 + i + j * 10);
			}
		}

		Sprite* Weapon5Detail;
		Weapon5Detail = Sprite::create("weapon5detail.png");
		Weapon5Detail->setPosition(Vec2(1653, 590));
		Weapon5Detail->setScale(0.7f);
		addChild(Weapon5Detail, 13, 1051);


		if (MyGold >= 900 && MyBuyWeaponNum <= 5)
		{
			Button* BuyButton51 = Button::create();
			BuyButton51->loadTextures("buy1.png", "buy1.png", "");
			BuyButton51->setPosition(Vec2(1650, 180));
			BuyButton51->setScale(1.0f);
			BuyButton51->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit5, this));
			addChild(BuyButton51, 14, 1052);
		}

		if (MyGold < 900 || MyBuyWeaponNum >= 6)
		{
			Sprite* BuyButton52;
			BuyButton52 = Sprite::create("buy2.png");
			BuyButton52->setPosition(Vec2(1650, 180));
			BuyButton52->setScale(1.0f);
			addChild(BuyButton52, 14, 1053);
		}
	}
}

void MapScene::Shopbuy6(Ref* sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		SimpleAudioEngine::getInstance()->playEffect("Touch.wav");
		for (int j = 1; j <= 6; ++j)
		{
			for (int i = 1; i <= 3; ++i)
			{
				removeChildByTag(1000 + i + j * 10);
			}
		}
		Sprite* Weapon6Detail;
		Weapon6Detail = Sprite::create("weapon6detail.png");
		Weapon6Detail->setPosition(Vec2(1650, 590));
		Weapon6Detail->setScale(0.7f);
		addChild(Weapon6Detail, 13, 1061);


		if (MyGold >= 710 && MyBuyWeaponNum <= 5)
		{
			Button* BuyButton61 = Button::create();
			BuyButton61->loadTextures("buy1.png", "buy1.png", "");
			BuyButton61->setPosition(Vec2(1650, 180));
			BuyButton61->setScale(1.0f);
			BuyButton61->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit6, this));
			addChild(BuyButton61, 14, 1062);
		}

		if (MyGold < 710 || MyBuyWeaponNum >= 6)
		{
			Sprite* BuyButton62;
			BuyButton62 = Sprite::create("buy2.png");
			BuyButton62->setPosition(Vec2(1650, 180));
			BuyButton62->setScale(1.0f);
			addChild(BuyButton62, 14, 1063);
		}
	}
}


//武器一确认购买
void MapScene::Buyit1(Ref* sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED && MyBuyWeaponNum <= 5)
	{
		MyBuyWeaponNum++;
		SimpleAudioEngine::getInstance()->playEffect("GOLD.wav");
		Sprite* MyEquip1 = Sprite::create("weapon1.png");
		MyEquip1->setPosition(Vec2(350 + MyBuyWeaponNum * 150, 200));
		MyEquip1->setScale(0.7f);
		addChild(MyEquip1, 14, 40 + MyBuyWeaponNum);

		Sprite* MyEquip1Zhanji = Sprite::create("weapon1.png");
		MyEquip1Zhanji->setPosition(Vec2(150 + MyBuyWeaponNum * 100, 300));
		MyEquip1Zhanji->setScale(0.5f);
		MyEquip1Zhanji->setVisible(false);
		addChild(MyEquip1Zhanji, 14, 810 + MyBuyWeaponNum);

		MyGold -= 1740;
		removeChildByTag(55);
		CCString* goldCCstring = CCString::createWithFormat("%d", MyGold);
		std::string goldstring = goldCCstring->getCString();
		Label* GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
		GoldNum->setPosition(Vec2(50, 450));
		addChild(GoldNum, 9, 55);
		for (int j = 1; j <= 6; ++j)
		{
			for (int i = 1; i <= 3; ++i)
			{
				removeChildByTag(1000 + i + j * 10);
			}
		}
	}
}

//武器二确认购买
void MapScene::Buyit2(Ref* sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED && MyBuyWeaponNum <= 5)
	{
		MyBuyWeaponNum++;
		SimpleAudioEngine::getInstance()->playEffect("GOLD.wav");
		Sprite* MyEquip2 = Sprite::create("weapon2.png");
		MyEquip2->setPosition(Vec2(350 + MyBuyWeaponNum * 150, 200));
		MyEquip2->setScale(0.7f);
		addChild(MyEquip2, 14, 40 + MyBuyWeaponNum);

		Sprite* MyEquip2Zhanji = Sprite::create("weapon2.png");
		MyEquip2Zhanji->setPosition(Vec2(150 + MyBuyWeaponNum * 100, 300));
		MyEquip2Zhanji->setScale(0.5f);
		MyEquip2Zhanji->setVisible(false);
		addChild(MyEquip2Zhanji, 14, 810 + MyBuyWeaponNum);

		MyGold -= 2140;
		removeChildByTag(55);
		CCString* goldCCstring = CCString::createWithFormat("%d", MyGold);
		std::string goldstring = goldCCstring->getCString();
		Label* GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
		GoldNum->setPosition(Vec2(50, 450));
		addChild(GoldNum, 9, 55);
		for (int j = 1; j <= 6; ++j)
		{
			for (int i = 1; i <= 3; ++i)
			{
				removeChildByTag(1000 + i + j * 10);
			}
		}
	}
}

//武器三确认购买
void MapScene::Buyit3(Ref* sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED && MyBuyWeaponNum <= 5)
	{
		MyBuyWeaponNum++;
		SimpleAudioEngine::getInstance()->playEffect("GOLD.wav");
		Sprite* MyEquip3 = Sprite::create("weapon3.png");
		MyEquip3->setPosition(Vec2(350 + MyBuyWeaponNum * 150, 200));
		MyEquip3->setScale(0.7f);
		addChild(MyEquip3, 14, 40 + MyBuyWeaponNum);

		Sprite* MyEquip3Zhanji = Sprite::create("weapon3.png");
		MyEquip3Zhanji->setPosition(Vec2(150 + MyBuyWeaponNum * 100, 300));
		MyEquip3Zhanji->setScale(0.5f);
		MyEquip3Zhanji->setVisible(false);
		addChild(MyEquip3Zhanji, 14, 810 + MyBuyWeaponNum);

		MyGold -= 2300;
		removeChildByTag(55);
		CCString* goldCCstring = CCString::createWithFormat("%d", MyGold);
		std::string goldstring = goldCCstring->getCString();
		Label* GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
		GoldNum->setPosition(Vec2(50, 450));
		addChild(GoldNum, 9, 55);
		for (int j = 1; j <= 6; ++j)
		{
			for (int i = 1; i <= 3; ++i)
			{
				removeChildByTag(1000 + i + j * 10);
			}
		}
	}
}

//武器四确认购买
void MapScene::Buyit4(Ref* sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED && MyBuyWeaponNum <= 5)
	{
		MyBuyWeaponNum++;
		SimpleAudioEngine::getInstance()->playEffect("GOLD.wav");
		Sprite* MyEquip4 = Sprite::create("weapon4.png");
		MyEquip4->setPosition(Vec2(350 + MyBuyWeaponNum * 150, 200));
		MyEquip4->setScale(0.7f);
		addChild(MyEquip4, 14, 40 + MyBuyWeaponNum);

		Sprite* MyEquip4Zhanji = Sprite::create("weapon4.png");
		MyEquip4Zhanji->setPosition(Vec2(150 + MyBuyWeaponNum * 100, 300));
		MyEquip4Zhanji->setScale(0.5f);
		MyEquip4Zhanji->setVisible(false);
		addChild(MyEquip4Zhanji, 14, 810 + MyBuyWeaponNum);

		MyGold -= 2990;
		removeChildByTag(55);
		CCString* goldCCstring = CCString::createWithFormat("%d", MyGold);
		std::string goldstring = goldCCstring->getCString();
		Label* GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
		GoldNum->setPosition(Vec2(50, 450));
		addChild(GoldNum, 9, 55);
		for (int j = 1; j <= 6; ++j)
		{
			for (int i = 1; i <= 3; ++i)
			{
				removeChildByTag(1000 + i + j * 10);
			}
		}
	}
}

//武器五确认购买
void MapScene::Buyit5(Ref* sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED && MyBuyWeaponNum <= 5)
	{
		MyBuyWeaponNum++;
		SimpleAudioEngine::getInstance()->playEffect("GOLD.wav");
		CCString* BuyCCstring5 = CCString::createWithFormat("%d", MyBuyWeaponNum);
		std::string buystring5 = BuyCCstring5->getCString();
		Sprite* MyEquip5 = Sprite::create("weapon5.png");
		MyEquip5->setPosition(Vec2(350 + MyBuyWeaponNum * 150, 200));
		MyEquip5->setScale(0.7f);
		addChild(MyEquip5, 14, 40 + MyBuyWeaponNum);

		Sprite* MyEquip5Zhanji = Sprite::create("weapon5.png");
		MyEquip5Zhanji->setPosition(Vec2(150 + MyBuyWeaponNum * 100, 300));
		MyEquip5Zhanji->setScale(0.5f);
		MyEquip5Zhanji->setVisible(false);
		addChild(MyEquip5Zhanji, 14, 810 + MyBuyWeaponNum);

		MyGold -= 900;
		removeChildByTag(55);
		CCString* goldCCstring = CCString::createWithFormat("%d", MyGold);
		std::string goldstring = goldCCstring->getCString();
		Label* GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
		GoldNum->setPosition(Vec2(50, 450));
		addChild(GoldNum, 9, 55);
		for (int j = 1; j <= 6; ++j)
		{
			for (int i = 1; i <= 3; ++i)
			{
				removeChildByTag(1000 + i + j * 10);
			}
		}
	}
}

//武器六确认购买
void MapScene::Buyit6(Ref* sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED && MyBuyWeaponNum <= 5)
	{
		MyBuyWeaponNum++;
		SimpleAudioEngine::getInstance()->playEffect("GOLD.wav");
		CCString* BuyCCstring6 = CCString::createWithFormat("%d", MyBuyWeaponNum);
		std::string buystring6 = BuyCCstring6->getCString();
		Sprite* MyEquip6 = Sprite::create("weapon6.png");
		MyEquip6->setPosition(Vec2(350 + MyBuyWeaponNum * 150, 200));
		MyEquip6->setScale(0.7f);
		addChild(MyEquip6, 14, 40 + MyBuyWeaponNum);

		Sprite* MyEquip6Zhanji = Sprite::create("weapon6.png");
		MyEquip6Zhanji->setPosition(Vec2(150 + MyBuyWeaponNum * 100, 300));
		MyEquip6Zhanji->setScale(0.5f);
		MyEquip6Zhanji->setVisible(false);
		addChild(MyEquip6Zhanji, 14, 810 + MyBuyWeaponNum);

		MyGold -= 710;
		removeChildByTag(55);
		CCString* goldCCstring = CCString::createWithFormat("%d", MyGold);
		std::string goldstring = goldCCstring->getCString();
		Label* GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
		GoldNum->setPosition(Vec2(50, 450));
		addChild(GoldNum, 9, 55);
		for (int j = 1; j <= 6; ++j)
		{
			for (int i = 1; i <= 3; ++i)
			{
				removeChildByTag(1000 + i + j * 10);
			}
		}
	}
}

//商城返回
void MapScene::ShopBack(Ref* sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		SimpleAudioEngine::getInstance()->playEffect("Touch.wav");
		for (int p = 1; p <= MyBuyWeaponNum; ++p)
		{
			Sprite* Tmp = (Sprite*)this->getChildByTag(40 + p);
			Tmp->setVisible(false);
		}
		for (int i = 0; i <= 5; ++i)
		{
			if (i <= 3)
			{
				for (int j = 1; j <= 6; ++j)
				{
					removeChildByTag(1000 + i + j * 10);
				}
			}
			if (i <= 4)  removeChildByTag(60 + i);
			removeChildByTag(71 + i);
			removeChildByTag(101 + i);
		}
		//恢复商城可触发
		Button* tmp1 = (Button*)this->getChildByTag(50);
		tmp1->setTouchEnabled(true);
		//恢复战绩可触发
		Button* tmp2 = (Button*)this->getChildByTag(51);
		tmp2->setTouchEnabled(true);
	}
}
//使用removebytag至关重要，byTag还可以提供访问
