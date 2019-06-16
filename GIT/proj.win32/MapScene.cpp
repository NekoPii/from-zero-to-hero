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
int NowBlood = MaxBlood;
int Attack = 50;
int MagicAttack = 0;
int Defense = 2;
int AttackSpeed = 0;
int MoveSpeed = 100;
int win = -1;


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
	for (int i = 0; i < 500; i++)
	{
		int n = 0;
		use.push_back(n);
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//地图
	auto MAP = Sprite::create("MAP/map.jpg");
	MAP->setPosition(Vec2(0, 0));
	MAP->setScale(0.88f);
	map = TMXTiledMap::create("mapCan/map5.tmx");
	MAP->setAnchorPoint(Point(0, 0));
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
	protect1->start(1, Vec2(1430, 920));
	Enemy.pushBack(protect1->enemy);
	chosenEnemy = NULL;
	auto protect2 = object::createObject();
	addChild(protect2, 0, 2044);
	protect2->start(2, Vec2(550, 470));
	Friend.pushBack(protect2->enemy);
	chosenFriend = NULL;

	//商城图标
	Button* ShopButton = Button::create();
	ShopButton->loadTextures("UI/shop.png", "UI/shopP.png", "");
	ShopButton->setPosition(Vec2(50, 500));
	ShopButton->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopcall, this));
	addChild(ShopButton, 9, 50);

	//战绩图标
	Button* ZhanjiButton = Button::create();
	ZhanjiButton->loadTextures("UI/Zhanji.png", "UI/ZhanjiP.png", "");
	ZhanjiButton->setPosition(Vec2(960, 1030));
	ZhanjiButton->setScale(1.0f);
	ZhanjiButton->addTouchEventListener(CC_CALLBACK_2(MapScene::ZhanjiCall, this));
	addChild(ZhanjiButton, 9, 51);

	//击杀数
	CCString* TMeKillCCstring = CCString::createWithFormat("%d", MeKill);
	std::string TMeKillstring = TMeKillCCstring->getCString();
	Label* TMeKillNum = Label::createWithTTF(TMeKillstring, "fonts/Marker Felt.ttf", 32);
	TMeKillNum->setPosition(Vec2(960, 1030));
	TMeKillNum->setScale(1.0f);
	addChild(TMeKillNum, 10, 52);

	//击杀图标
	Sprite* KillTag = Sprite::create("UI/kill.png");
	KillTag->setPosition(Vec2(910, 1030));
	KillTag->setScale(1.0f);
	addChild(KillTag, 10, 53);

	//死亡数
	CCString* TMeDeadCCstring = CCString::createWithFormat("%d", MeDead);
	std::string TMeDeadstring = TMeDeadCCstring->getCString();
	Label* TMeDeadNum = Label::createWithTTF(TMeDeadstring, "fonts/Marker Felt.ttf", 32);
	TMeDeadNum->setPosition(Vec2(1070, 1030));
	TMeDeadNum->setScale(1.0f);
	addChild(TMeDeadNum, 10, 54);

	//死亡图标
	Sprite* DeadTag = Sprite::create("UI/die.png");
	DeadTag->setPosition(Vec2(1020, 1030));
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
	LBS = Sprite::create("UI/LBS.png");
	addChild(LBS, 3);
	hero = new heroPrint();
	hero->initHeroSprite(8, Vec2(860, 540));
	addChild(hero, 0, 2046);
	Friend.pushBack(hero->heroSprite);
	auto antiHero = object::createObject();
	addChild(antiHero, 0, 2047);
	antiHero->start(6, Vec2(1730, 1020));
	Enemy.pushBack(antiHero->enemy);

	myMagic = Sprite::create("magic_1_20.png");
	addChild(myMagic, 1);
	antiMagic = Sprite::create("magic_1_20.png");
	addChild(antiMagic, 1);

	//技能一
	Sprite* SKILL11 = Sprite::create(String::createWithFormat("SKILL/skill%d1.png", MyHeroID)->getCString());
	SKILL11->setPosition(Vec2(origin.x + visibleSize.width * 0.55, origin.y + visibleSize.height * 0.1));
	SKILL11->setScale(0.75f);
	SKILL11->setVisible(true);
	addChild(SKILL11, 6, 901);
	Sprite* SKILL12 = Sprite::create(String::createWithFormat("SKILL/skill%d1shade.png", MyHeroID)->getCString());
	SKILL12->setPosition(Vec2(origin.x + visibleSize.width * 0.55, origin.y + visibleSize.height * 0.1));
	SKILL12->setScale(0.75f);
	SKILL12->setVisible(false);
	addChild(SKILL12, 6, 902);
	auto SKILL13 = Sprite::create(String::createWithFormat("SKILL/skill%d1.png", MyHeroID)->getCString());
	SKILL1TIME = ProgressTimer::create(SKILL13);
	SKILL1TIME->setScale(0.75f);
	SKILL1TIME->setType(kCCProgressTimerTypeRadial);
	SKILL1TIME->setPosition(Vec2(origin.x + visibleSize.width * 0.55, origin.y + visibleSize.height * 0.1));
	addChild(SKILL1TIME, 6, 903);

	//技能二
	Sprite* SKILL21 = Sprite::create(String::createWithFormat("SKILL/skill%d2.png", MyHeroID)->getCString());
	SKILL21->setPosition(Vec2(origin.x + visibleSize.width * 0.6, origin.y + visibleSize.height * 0.1));
	SKILL21->setScale(0.75f);
	SKILL21->setVisible(true);
	addChild(SKILL21, 6, 904);
	Sprite* SKILL22 = Sprite::create(String::createWithFormat("SKILL/skill%d2shade.png", MyHeroID)->getCString());
	SKILL22->setPosition(Vec2(origin.x + visibleSize.width * 0.6, origin.y + visibleSize.height * 0.1));
	SKILL22->setScale(0.75f);
	SKILL22->setVisible(false);
	addChild(SKILL22, 6, 905);
	auto SKILL23 = Sprite::create(String::createWithFormat("SKILL/skill%d2.png", MyHeroID)->getCString());
	SKILL2TIME = ProgressTimer::create(SKILL23);
	SKILL2TIME->setScale(0.75f);
	SKILL2TIME->setType(kCCProgressTimerTypeRadial);
	SKILL2TIME->setPosition(Vec2(origin.x + visibleSize.width * 0.6, origin.y + visibleSize.height * 0.1));
	addChild(SKILL2TIME, 6, 906);


	//技能三
	Sprite* SKILL31 = Sprite::create(String::createWithFormat("SKILL/skill%d3.png", MyHeroID)->getCString());
	SKILL31->setPosition(Vec2(origin.x + visibleSize.width * 0.65, origin.y + visibleSize.height * 0.1));
	SKILL31->setScale(0.75f);
	SKILL31->setVisible(true);
	addChild(SKILL31, 6, 907);
	Sprite* SKILL32 = Sprite::create(String::createWithFormat("SKILL/skill%d3shade.png", MyHeroID)->getCString());
	SKILL32->setPosition(Vec2(origin.x + visibleSize.width * 0.65, origin.y + visibleSize.height * 0.1));
	SKILL32->setScale(0.75f);
	SKILL32->setVisible(false);
	addChild(SKILL32, 6, 908);
	auto SKILL33 = Sprite::create(String::createWithFormat("SKILL/skill%d3.png", MyHeroID)->getCString());
	SKILL3TIME = ProgressTimer::create(SKILL33);
	SKILL3TIME->setScale(0.75f);
	SKILL3TIME->setType(kCCProgressTimerTypeRadial);
	SKILL3TIME->setPosition(Vec2(origin.x + visibleSize.width * 0.65, origin.y + visibleSize.height * 0.1));
	addChild(SKILL3TIME, 6, 909);

	//技能四
	Sprite* SKILL41 = Sprite::create(String::createWithFormat("SKILL/skill%d4.png", MyHeroID)->getCString());
	SKILL41->setPosition(Vec2(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height * 0.1));
	SKILL41->setScale(0.75f);
	SKILL41->setVisible(true);
	addChild(SKILL41, 6, 910);
	Sprite* SKILL42 = Sprite::create(String::createWithFormat("SKILL/skill%d4shade.png", MyHeroID)->getCString());
	SKILL42->setPosition(Vec2(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height * 0.1));
	SKILL42->setScale(0.75f);
	SKILL42->setVisible(false);
	addChild(SKILL42, 6, 911);
	auto SKILL43 = Sprite::create(String::createWithFormat("SKILL/skill%d4.png", MyHeroID)->getCString());
	SKILL4TIME = ProgressTimer::create(SKILL43);
	SKILL4TIME->setScale(0.75f);
	SKILL4TIME->setType(kCCProgressTimerTypeRadial);
	SKILL4TIME->setPosition(Vec2(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height * 0.1));
	addChild(SKILL4TIME, 6, 912);

	//经验条
	Sprite* EXP = Sprite::create("UI/EXP.png");
	EXPTIME = ProgressTimer::create(EXP);
	EXPTIME->setScale(0.88f);
	EXPTIME->setType(kCCProgressTimerTypeBar);
	EXPTIME->setMidpoint(Vec2(0,0));
	EXPTIME->setBarChangeRate(Vec2(1.0, 0));
	EXPTIME->setPercentage(0);
	EXPTIME->setPosition(Vec2(960,20));
	addChild(EXPTIME, 10, 108);


	//个人信息背景
	Sprite* DetailBackGround = Sprite::create("UI/DetailBackGround.png");
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
	Sprite* MaxBloodsp = Sprite::create("UI/MaxBlood.png");
	MaxBloodsp->setPosition(Vec2(origin.x + visibleSize.width * 0.37, origin.y + visibleSize.height * 0.15));
	MaxBloodsp->setScale(0.5f);
	addChild(MaxBloodsp, 6, 112);

	CCString* MaxBloodCCDataStr = CCString::createWithFormat("%d", MaxBlood);
	std::string MaxBloodDataStr = MaxBloodCCDataStr->getCString();
	Label* MaxBloodData = Label::createWithTTF(MaxBloodDataStr, "fonts/arial.ttf", 30);
	MaxBloodData->setPosition(Vec2(origin.x + visibleSize.width * 0.41, origin.y + visibleSize.height * 0.15));
	addChild(MaxBloodData, 6, 113);

	//攻击
	Sprite* Attacksp = Sprite::create("UI/Attack.png");
	Attacksp->setPosition(Vec2(origin.x + visibleSize.width * 0.37, origin.y + visibleSize.height * 0.1));
	Attacksp->setScale(0.5f);
	addChild(Attacksp, 6, 114);

	CCString* AttackCCDataStr = CCString::createWithFormat("%d", Attack);
	std::string AttackDataStr = AttackCCDataStr->getCString();
	Label* AttackData = Label::createWithTTF(AttackDataStr, "fonts/arial.ttf", 30);
	AttackData->setPosition(Vec2(origin.x + visibleSize.width * 0.41, origin.y + visibleSize.height * 0.1));
	addChild(AttackData, 6, 115);

	//魔法攻击
	Sprite* MagicAttacksp = Sprite::create("UI/MagicAttack.png");
	MagicAttacksp->setPosition(Vec2(origin.x + visibleSize.width * 0.45, origin.y + visibleSize.height * 0.1));
	MagicAttacksp->setScale(0.5f);
	addChild(MagicAttacksp, 6, 122);

	CCString* MagicAttackCCDataStr = CCString::createWithFormat("%d", MagicAttack);
	std::string MagicAttackDataStr = MagicAttackCCDataStr->getCString();
	Label* MagicAttackData = Label::createWithTTF(MagicAttackDataStr, "fonts/arial.ttf", 30);
	MagicAttackData->setPosition(Vec2(origin.x + visibleSize.width * 0.49, origin.y + visibleSize.height * 0.1));
	addChild(MagicAttackData, 6, 123);


	//护甲
	Sprite* Defensesp = Sprite::create("UI/Defense.png");
	Defensesp->setPosition(Vec2(origin.x + visibleSize.width * 0.45, origin.y + visibleSize.height * 0.15));
	Defensesp->setScale(0.5f);
	addChild(Defensesp, 6, 116);

	CCString* DefenseCCDataStr = CCString::createWithFormat("%d", Defense);
	std::string DefenseDataStr = DefenseCCDataStr->getCString();
	Label* DefenseData = Label::createWithTTF(DefenseDataStr, "fonts/arial.ttf", 30);
	DefenseData->setPosition(Vec2(origin.x + visibleSize.width * 0.49, origin.y + visibleSize.height * 0.15));
	addChild(DefenseData, 6, 117);

	//攻速
	Sprite* AttackSpeedsp = Sprite::create("UI/AttackSpeed.png");
	AttackSpeedsp->setPosition(Vec2(origin.x + visibleSize.width * 0.45, origin.y + visibleSize.height * 0.05));
	AttackSpeedsp->setScale(0.5f);
	addChild(AttackSpeedsp, 6, 118);

	CCString* AttackSpeedCCDataStr = CCString::createWithFormat("%d%%", AttackSpeed);
	std::string AttackSpeedDataStr = AttackSpeedCCDataStr->getCString();
	Label* AttackSpeedData = Label::createWithTTF(AttackSpeedDataStr, "fonts/arial.ttf", 30);
	AttackSpeedData->setPosition(Vec2(origin.x + visibleSize.width * 0.49, origin.y + visibleSize.height * 0.05));
	addChild(AttackSpeedData, 6, 119);

	//移速
	Sprite* MoveSpeedsp = Sprite::create("UI/MoveSpeed.png");
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
	this->schedule(schedule_selector(MapScene::soldersMake), 20.0f);
	this->schedule(schedule_selector(MapScene::skillQ), 0.5f);
	this->schedule(schedule_selector(MapScene::skillW), 0.5f);
	this->schedule(schedule_selector(MapScene::skillE), 0.5f);
	this->schedule(schedule_selector(MapScene::skillR), 0.5f);
	this->schedule(schedule_selector(MapScene::soldersContrl), 0.5f);
	this->schedule(schedule_selector(MapScene::mark), 0.05f);
	this->schedule(schedule_selector(MapScene::anti), 0.8f);
	this->schedule(schedule_selector(MapScene::myMagicF), 0.05f);
	this->schedule(schedule_selector(MapScene::refreshAb), 0.5f);
	this->schedule(schedule_selector(MapScene::quanshui), 1.0f);
	this->schedule(schedule_selector(MapScene::increaseGold), 3.0f);
	this->schedule(schedule_selector(MapScene::rebuild), 1.0f);
	this->schedule(schedule_selector(MapScene::rebuildme), 1.0f);
	return true;
}
void MapScene::rebuild(float dt)
{

	object* anti = (object*)getChildByTag(2047);

	if (anti->blood <= 0)
	{
		re = 0;
		anti->blood = 1;
		MeKill++;
		YouDead++;
		CCString *tmpCCMeKill = CCString::createWithFormat("%d", MeKill);
		std::string tmpMeKill = tmpCCMeKill->getCString();
		Label* tmpdata1 = (Label *)this->getChildByTag(52);
		tmpdata1->setString(tmpMeKill);
	}

	if (re >= 0)
	{
		if (re > 0) removeChildByTag(11113);
		re++;
		CCString* AttackCCDataStr = CCString::createWithFormat("Anti-Enemy Will Be Reborn In %d Second", 10 - re);
		std::string AttackDataStr = AttackCCDataStr->getCString();
		Label* AttackDat = Label::createWithTTF(AttackDataStr, "fonts/arial.ttf", 40);
		AttackDat->setPosition(Vec2(960, 900));
		AttackDat->setColor(ccc3(0, 0, 0));
		addChild(AttackDat, 6, 11113);
	}
	if (re == 10)
	{
		anti->blood = anti->maxBlood;
		anti->enemy->setPosition(1500, 800);
		anti->Loading->setPosition(1500, 800);
		removeChildByTag(11113);
		re = -1;
	}
	//	CCLOG("%d++", re);
}
void MapScene::rebuildme(float dt)
{
	if (hero->blood <= 0)
	{
		rd = 0;
		hero->blood = 1;
		hero->heroSprite->stopAllActions();
		hero->heroSprite->setPosition(1,1);
		hero->heroSprite->setVisible(false);
		YouKill++;
		MeDead++;
		CCString *tmpCCYouKill = CCString::createWithFormat("%d", MeDead);
		std::string tmpYouKill = tmpCCYouKill->getCString();
		Label* tmpdata1 = (Label *)this->getChildByTag(54);
		tmpdata1->setString(tmpYouKill);
	}
	if (rd >= 0)
	{
		if (rd > 0) removeChildByTag(11114);
		rd++;
		CCString* AttackCCDataStr = CCString::createWithFormat("You Will Be Reborn In %d Second", 10- rd);
		std::string AttackDataStr = AttackCCDataStr->getCString();
		Label* AttackDat = Label::createWithTTF(AttackDataStr, "fonts/arial.ttf", 40);
		AttackDat->setColor(ccc3(0,0,0));
		AttackDat->setPosition(Vec2(960, 960));
		addChild(AttackDat, 6, 11114);
	}
	if (rd == 10)
	{
		hero->blood = hero->maxBlood;
		hero->heroSprite->setPosition(500, 400);
		rd = -1;
		removeChildByTag(11114);
		hero->heroSprite->setVisible(true);
		//	anti->Loading->setPosition(1500, 800);
	}
	CCLOG("%d++", rd);
}

void MapScene::refreshAb(float dt)
{
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Attack = hero->Aggressivity;
	Defense = hero->Armor;
	CCLOG("%d##%f", hero->Aggressivity, hero->attack_speed);
	AttackSpeed = hero->attack_speed * 100;
	NowBlood = hero->maxBlood;
	MoveSpeed = hero->speed;
	MagicAttack = hero->magic;
	
	//金币
	CCString* goldCCstring = CCString::createWithFormat("%d", MyGold);
	std::string goldstring = goldCCstring->getCString();
	Label *tmpgold = (Label *)this->getChildByTag(55);
	tmpgold->setString(goldstring);

	//经验条
	ProgressTimer* tmpexp = (ProgressTimer*)this->getChildByTag(108);
	/*if (lup == 0)
	{
		tmp->setPercentage(20);
		lup = 1;
		CCLOG("************************");
	}*/
	int a = ((float)hero->exprience / (float)hero->needExprience) * 100;
	tmpexp->setPercentage(a);
	CCLOG("%d==",a);

	//
	hero->expcir->setPercentage(a);
	CCString* levCCStr = CCString::createWithFormat("%d", hero->level);
	std::string levStr = levCCStr->getCString();
	hero->lev->setString(levStr);

	object* tmpanti = (object*)getChildByTag(2047);
	int b= ((float)tmpanti->nowexp / (float)tmpanti->maxexp) * 100;
	tmpanti->expcir->setPercentage(b);
	CCLOG("%d---", tmpanti->level);
	CCString* alevCCStr = CCString::createWithFormat("%d", tmpanti->level);
	std::string alevStr = alevCCStr->getCString();
	tmpanti->lev->setString(alevStr);
	/*
	//现在血量
	CCString* nowBloodCCDataStr = CCString::createWithFormat("%d", NowBlood);
	std::string nowBloodDataStr = nowBloodCCDataStr->getCString();
	Label* tmpnowBloodData = (Label*)this->getChildByTag(112001);
	tmpnowBloodData->setString(nowBloodDataStr);
	*/
	//总血量
	CCString* MaxBloodCCDataStr = CCString::createWithFormat("%d", NowBlood);
	std::string MaxBloodDataStr = MaxBloodCCDataStr->getCString();
	Label* tmpMaxBloodData = (Label *)this->getChildByTag(113);
	tmpMaxBloodData->setString(MaxBloodDataStr);
	
	//攻击
	CCString* AttackCCDataStr = CCString::createWithFormat("%d", Attack);
	std::string AttackDataStr = AttackCCDataStr->getCString();
	Label* tmpAttackData = (Label *)this->getChildByTag(115);
	tmpAttackData->setString(AttackDataStr);

	//护甲
	CCString* DefenseCCDataStr = CCString::createWithFormat("%d", Defense);
	std::string DefenseDataStr = DefenseCCDataStr->getCString();
	Label* tmpDefenseData = (Label *)this->getChildByTag(117);
	tmpDefenseData->setString(DefenseDataStr);

	//攻速
	CCString* AttackSpeedCCDataStr = CCString::createWithFormat("%d%%", AttackSpeed);
	std::string AttackSpeedDataStr = AttackSpeedCCDataStr->getCString();
	Label* tmpAttackSpeedData = (Label *)this->getChildByTag(119);
	tmpAttackSpeedData->setString(AttackSpeedDataStr);

	//移速
	CCString* MoveSpeedCCDataStr = CCString::createWithFormat("%d%%", MoveSpeed);
	std::string MoveSpeedDataStr = MoveSpeedCCDataStr->getCString();
	Label* tmpMoveSpeedData = (Label *)this->getChildByTag(121);
	tmpMoveSpeedData->setString(MoveSpeedDataStr);

	//法术攻击
	CCString* MagicAttackCCDataStr = CCString::createWithFormat("%d", MagicAttack);
	std::string MagicAttackDataStr = MagicAttackCCDataStr->getCString();
	Label *tmpMagicAttackData = (Label *)this->getChildByTag(123);
	tmpMagicAttackData->setString(MagicAttackDataStr);
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
	hero->Loading->setPosition(Vec2(hero->herosPosition().x, hero->herosPosition().y + 50));
	hero->expcir->setPosition(Vec2(hero->herosPosition().x - 80, hero->herosPosition().y + 50));
	hero->lev->setPosition(Vec2(hero->herosPosition().x - 80, hero->herosPosition().y + 50));
	hero->Loading->setPercentage((float)hero->blood / (float)hero->maxBlood * 100);
	object* tmpanti = (object*)getChildByTag(2047);
	tmpanti->expcir->setPosition(Vec2(tmpanti->enemy->getPosition().x - 80, tmpanti->enemy->getPosition().y + 50));
	tmpanti->lev->setPosition(tmpanti->enemy->getPosition().x - 80,tmpanti-> enemy->getPosition().y + 50);
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
		auto* frame = frameCacheL->getSpriteFrameByName(String::createWithFormat("levelup%d.png", i)->getCString());
		frameArray.pushBack(frame);
	}
	Animation* animation = Animation::createWithSpriteFrames(frameArray);
	animation->setLoops(1);
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
	if (hero->blood <= 2) return false;
	object* protect1 = (object*)getChildByTag(2045);
	int c = protect1->getTag();
	current = 0;
	CCLOG("%d", c);
	//Vec2 solders1=
	hero->getDirection(hero->currentPosition, touch->getLocation());
	CCLOG("%f    %f", touch->getLocation().x, touch->getLocation().y);
	Vec2 tiledPositon = tiledpos(touch->getLocation());
	int tileGid = _colliable->getTileGIDAt(tiledPositon);
	Vec2 hero_position = hero->herosPosition();
	if (hero->E == 1 && MyHeroID == 2)
	{
		float k; float x; float y;
		x = (touch->getLocation().x - hero_position.x) * 100 / sqrt((touch->getLocation().x - hero_position.x) * (touch->getLocation().x - hero_position.x)
			+ (touch->getLocation().y - hero_position.y) * (touch->getLocation().y - hero_position.y));
		y = (touch->getLocation().y - hero_position.y) * 100 / sqrt((touch->getLocation().x - hero_position.x) * (touch->getLocation().x - hero_position.x)
			+ (touch->getLocation().y - hero_position.y) * (touch->getLocation().y - hero_position.y));
		hero->heroSprite->setPosition(hero_position.x + x, hero_position.y + y);
		schedule(schedule_selector(MapScene::skill3), 0.1f);
		hero->E = 2;
		CCLOG("%d", hero->E);
		return false;
	}
	for (int i = 0; i < Enemy.size(); i++)
	{
		auto a = Enemy.at(i);
		Size s = a->getContentSize();
		if (i == 0)
		{
			s.width = 114;
			s.height = 150;
		}
		Rect rect = Rect(0, 0, s.width, s.height);
		Vec2 soldersPostion = a->getPosition();
		if (touch->getLocation().x <= soldersPostion.x + s.width / 2 && touch->getLocation().x >= soldersPostion.x - s.width / 2)
		{
			if (touch->getLocation().y <= soldersPostion.y + s.height / 2 && touch->getLocation().y >= soldersPostion.y - s.height / 2)
			{
				CCLOG("1");
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
					current = i + 4094;
				}
				hero->E_object = current;
				float distance = hero->getDistance(hero_position, soldersPostion);
				if (hero->W == 1 && MyHeroID == 3)
				{
					if (distance <= 200)
					{
						object* tar;
						Sprite* ta = Sprite::create("magic_1_20.png");
						addChild(ta);
						ta->setPosition(soldersPostion);
						ta->runAction(createAnimateM(5, 24, 0.048, 1));
						schedule(schedule_selector(MapScene::skill2), 0.1f);
						hero->W = 2;
						CCLOG("%d", hero->W);
						return false;
					}
				}
				if (hero->W == 1 && MyHeroID == 2)
				{
					object* tar;
					if (i > 1)
						tar = (object*)getChildByTag(4094 + i);
					if (i == 0)
						tar = (object*)getChildByTag(2045);
					if (i == 1)
						tar = (object*)getChildByTag(2047);
					Sprite * ta = Sprite::create("magic_1_20.png");
					addChild(ta);
					ta->setPosition(hero->herosPosition());
					Vec2 dir(100 * (tar->enemy->getPosition().x - hero->herosPosition().x), 100 * (tar->enemy->getPosition().y - hero->herosPosition().y));
					auto mov = MoveBy::create(getDistance(Vec2(0, 0), dir) / 1500, dir);
					ta->runAction(createAnimateM(4, 20, 0.048, 1));
					ta->runAction(mov);
					for (int i = 1; i < Enemy.size(); i++)
					{
						auto a = Enemy.at(i);
						Vec2 bit = a->getPosition();
						float d = (dir.y * bit.x - dir.x * bit.x + dir.x * hero->herosPosition().y - dir.y * hero->herosPosition().x) / sqrt(dir.x * dir.x + dir.y * dir.y);
						if (-50 < d < 50)
						{
							object* solder;
							if (i == 1)
								solder = (object*)getChildByTag(2047);
							else
								solder = (object*)getChildByTag(4094 + i);
							solder->blood -= (hero->level * 10 + (hero->Aggressivity - 50) * 0.3 + 30) * (1 - solder->Armor / 1000);
							float cur = (((float)solder->blood / (float)solder->maxBlood) * 100);
							solder->Loading->setPercentage(cur);
							if (solder->blood <= 0&&solder->used==0)
							{
								solder->used = 1;
								if (solder->getTag() == 2047)
								{
								//	solder->blood = 10;
									solder->used = 0;
									solder->enemy->setPosition(Vec2(-1460, -1030));
									solder->Loading->setPosition(Vec2(-1460, -1080));
									solder->Loading->setPercentage(100.0);
								}
								else
									solder->enemy->setPosition(-2000, -2000);
								hero->exprience = hero->exprience + 10;
								if (hero->needExprience == hero->exprience)
								{
									lup = 0;
									hero->level++; hero->needExprience = hero->needExprience + 20; hero->exprience = 0; hero->skillNum = 1;
									hero->blood += 50;
									hero->maxBlood += 50;
									hero->Aggressivity += 5;
									hero->Armor += 20;
									hero->attack_speed += 0.2;
								}
								MyGold += solder->gold;
							}
						}

					}
					schedule(schedule_selector(MapScene::skill2), 0.1f);
					hero->W = 2;
					CCLOG("%d", hero->W);
					return false;
				}
				if (distance <= hero->range)
				{
					int x = i + 4094;

					//	CCLOG("%d*****+***", current);
						//hero->heroAttack(hero_position, soldersPostion);
					hero->heroSprite->stopAllActions();
					hero->heroSprite->runAction(createAnimateH(4, "att", 7, MyHeroID));
					if (i > 1)
						attactEnemy(hero_position, soldersPostion, 49, 2046, x);
					if (i == 0)
						attactEnemy(hero_position, soldersPostion, 49, 2046, 2045);
					if (i == 1)
					{
						attactEnemy(hero_position, soldersPostion, 49, 2046, 2047);
					}
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
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (keycode == EventKeyboard::KeyCode::KEY_Q)
	{

		Sprite* tmp1 = (Sprite*)this->getChildByTag(901);
		tmp1->setVisible(false);
		Sprite* tmp2 = (Sprite*)this->getChildByTag(902);
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
	if (keycode == EventKeyboard::KeyCode::KEY_W)
	{
		if (MyHeroID == 3 && timeW == 0 && hero->W == 0)
		{
			hero->skill_W = 1;
			Sprite* tmp1 = (Sprite*)this->getChildByTag(904);
			tmp1->setVisible(false);
			Sprite* tmp2 = (Sprite*)this->getChildByTag(905);
			tmp2->setVisible(true);
			return;
		}
		if (MyHeroID == 3 && timeW == 0 && hero->W == 1)
		{
			hero->skill_W = 0;
			hero->W = 0;
			Sprite* tmp1 = (Sprite*)this->getChildByTag(904);
			tmp1->setVisible(true);
			Sprite* tmp2 = (Sprite*)this->getChildByTag(905);
			tmp2->setVisible(false);
			return;
		}
		if (MyHeroID == 2 && timeW == 0 && hero->W == 0)
		{
			hero->skill_W = 1;
			Sprite* tmp1 = (Sprite*)this->getChildByTag(904);
			tmp1->setVisible(false);
			Sprite* tmp2 = (Sprite*)this->getChildByTag(905);
			tmp2->setVisible(true);
			return;
		}
		if (MyHeroID == 2 && timeW == 0 && hero->W == 1)
		{
			hero->skill_W = 0;
			hero->W = 0;
			Sprite* tmp1 = (Sprite*)this->getChildByTag(904);
			tmp1->setVisible(true);
			Sprite* tmp2 = (Sprite*)this->getChildByTag(905);
			tmp2->setVisible(false);
			return;
		}
		Sprite* tmp1 = (Sprite*)this->getChildByTag(904);
		tmp1->setVisible(false);
		Sprite* tmp2 = (Sprite*)this->getChildByTag(905);
		tmp2->setVisible(true);
		schedule(schedule_selector(MapScene::skill2), 0.1f);
		if (timeW == 0)
		{
			hero->skill_W = 1;
		}
	}
	if (keycode == EventKeyboard::KeyCode::KEY_E)
	{
		if (MyHeroID == 2 && timeE == 0 && hero->E == 0)
		{
			hero->skill_E = 1;
			Sprite* tmp1 = (Sprite*)this->getChildByTag(907);
			tmp1->setVisible(false);
			Sprite* tmp2 = (Sprite*)this->getChildByTag(908);
			tmp2->setVisible(true);
			return;
		}
		if (MyHeroID == 2 && timeE == 0 && hero->E == 1)
		{
			hero->skill_E = 0;
			hero->E = 0;
			Sprite* tmp1 = (Sprite*)this->getChildByTag(907);
			tmp1->setVisible(true);
			Sprite* tmp2 = (Sprite*)this->getChildByTag(908);
			tmp2->setVisible(false);
			return;
		}
		Vec2 hPos = hero->herosPosition();
		Vec2 aPos = Enemy.at(1)->getPosition();
		float d = getDistance(hPos, aPos);
		if (MyHeroID == 3 && d >= 200)
		{
			return;
		}
		Sprite* tmp1 = (Sprite*)this->getChildByTag(907);
		tmp1->setVisible(false);
		Sprite* tmp2 = (Sprite*)this->getChildByTag(908);
		tmp2->setVisible(true);
		schedule(schedule_selector(MapScene::skill3), 0.1f);
		if (timeE == 0)
		{
			hero->skill_E = 1;
		}
	}
	if (keycode == EventKeyboard::KeyCode::KEY_R)
	{
		object* tar;
		float dis;
		Vec2 hPos = hero->herosPosition();
		Vec2 aPos = Enemy.at(1)->getPosition();
		float d = getDistance(hPos, aPos);
		if (MyHeroID == 1 && d >= 150)
			return;
		if (MyHeroID == 3 && current != 0)
		{
			tar = (object*)getChildByTag(current);
			dis = getDistance(hPos, tar->enemy->getPosition());
		}
		else if(MyHeroID == 3 && current == 0)
			return;
		if (MyHeroID == 3 && dis >= 250)
			return;
		Sprite * tmp1 = (Sprite*)this->getChildByTag(910);
		tmp1->setVisible(false);
		Sprite * tmp2 = (Sprite*)this->getChildByTag(911);
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
	int m; int n;
	if (MyHeroID == 1)
	{
		m = 99;
		n = 1;
	}
	if (MyHeroID == 2)
	{
		m = 95;
		n = 5;
	}
	if (MyHeroID == 3)
	{
		m = 99;
		n = 1;
	}
	if (Current1 <= m)
	{
		Current1 += n;
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
		Sprite* SKILL11 = Sprite::create(String::createWithFormat("SKILL/skill%d1.png", MyHeroID)->getCString());
		SKILL11->setPosition(Vec2(origin.x + visibleSize.width * 0.55, origin.y + visibleSize.height * 0.1));
		SKILL11->setScale(0.75f);
		SKILL11->setVisible(true);
		addChild(SKILL11, 6, 901);
		Sprite* SKILL12 = Sprite::create(String::createWithFormat("SKILL/skill%d1shade.png", MyHeroID)->getCString());
		SKILL12->setPosition(Vec2(origin.x + visibleSize.width * 0.55, origin.y + visibleSize.height * 0.1));
		SKILL12->setScale(0.75f);
		SKILL12->setVisible(false);
		addChild(SKILL12, 6, 902);
		auto SKILL13 = Sprite::create(String::createWithFormat("SKILL/skill%d1.png", MyHeroID)->getCString());
		SKILL1TIME = ProgressTimer::create(SKILL13);
		SKILL1TIME->setScale(0.75f);
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
		Sprite* SKILL21 = Sprite::create(String::createWithFormat("SKILL/skill%d2.png", MyHeroID)->getCString());
		SKILL21->setPosition(Vec2(origin.x + visibleSize.width * 0.6, origin.y + visibleSize.height * 0.1));
		SKILL21->setScale(0.75f);
		SKILL21->setVisible(true);
		addChild(SKILL21, 6, 904);
		Sprite* SKILL22 = Sprite::create(String::createWithFormat("SKILL/skill%d2shade.png", MyHeroID)->getCString());
		SKILL22->setPosition(Vec2(origin.x + visibleSize.width * 0.6, origin.y + visibleSize.height * 0.1));
		SKILL22->setScale(0.75f);
		SKILL22->setVisible(false);
		addChild(SKILL22, 6, 905);
		auto SKILL23 = Sprite::create(String::createWithFormat("SKILL/skill%d2.png", MyHeroID)->getCString());
		SKILL2TIME = ProgressTimer::create(SKILL23);
		SKILL2TIME->setScale(0.75f);
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
		Sprite* SKILL31 = Sprite::create(String::createWithFormat("SKILL/skill%d3.png", MyHeroID)->getCString());
		SKILL31->setPosition(Vec2(origin.x + visibleSize.width * 0.65, origin.y + visibleSize.height * 0.1));
		SKILL31->setScale(0.75f);
		SKILL31->setVisible(true);
		addChild(SKILL31, 6, 907);
		Sprite* SKILL32 = Sprite::create(String::createWithFormat("SKILL/skill%d3shade.png", MyHeroID)->getCString());
		SKILL32->setPosition(Vec2(origin.x + visibleSize.width * 0.65, origin.y + visibleSize.height * 0.1));
		SKILL32->setScale(0.75f);
		SKILL32->setVisible(false);
		addChild(SKILL32, 6, 908);
		auto SKILL33 = Sprite::create(String::createWithFormat("SKILL/skill%d3.png", MyHeroID)->getCString());
		SKILL3TIME = ProgressTimer::create(SKILL33);
		SKILL3TIME->setScale(0.75f);
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
		Sprite* SKILL41 = Sprite::create(String::createWithFormat("SKILL/skill%d4.png", MyHeroID)->getCString());
		SKILL41->setPosition(Vec2(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height * 0.1));
		SKILL41->setScale(0.75f);
		SKILL41->setVisible(true);
		addChild(SKILL41, 6, 910);
		Sprite* SKILL42 = Sprite::create(String::createWithFormat("SKILL/skill%d4shade.png", MyHeroID)->getCString());
		SKILL42->setPosition(Vec2(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height * 0.1));
		SKILL42->setScale(0.75f);
		SKILL42->setVisible(false);
		addChild(SKILL42, 6, 911);
		auto SKILL43 = Sprite::create(String::createWithFormat("SKILL/skill%d4.png", MyHeroID)->getCString());
		SKILL4TIME = ProgressTimer::create(SKILL43);
		SKILL4TIME->setScale(0.75f);
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
	if (protect1->blood <= 0)
	{
		win = 0;
		GameOver();
	}

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
	if (protect2->blood <= 0)
	{
		win = 1;
		GameOver();
	}
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
				attactEnemy(Tower1_position, objectPositon, 1, 2044, 4094 + n);
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
				attactEnemy(Tower1_position, objectPositon, 1,2044,4094+i);
				else attactEnemy(Tower1_position, objectPositon, 1, 2044, 2047);
				return;
			}
		}
	}
}
void MapScene::soldersMake(float dt)
{
		auto solder1 = object::createObject();
		addChild(solder1, 0, 2048+time*3);
		solder1->start(3, Vec2(650, 550));
		auto solder2 = object::createObject();
		addChild(solder2, 0, 2049+time * 3);
		solder2->start(5, Vec2(750, 450));
		auto solder3 = object::createObject();
		addChild(solder3, 0, 2050+ time * 3);
		solder3->start(4, Vec2(550, 650));
		Friend.pushBack(solder1->enemy);
		Friend.pushBack(solder2->enemy);
		Friend.pushBack(solder3->enemy);
		auto solder11 = object::createObject();
		addChild(solder11, 0, 4096+ time * 3);
		solder11->start(3, Vec2(1400, 900));
		auto solder22 = object::createObject();
		addChild(solder22, 0, 4097 + time * 3);
		solder22->start(5, Vec2(1500, 800));
		auto solder33 = object::createObject();
		addChild(solder33, 0, 4098+ time * 3);
		solder33->start(4, Vec2(1300, 1000));
		Enemy.pushBack(solder11->enemy);
		Enemy.pushBack(solder22->enemy);
		Enemy.pushBack(solder33->enemy);
		time++;
		CCLOG("&&%d&&", time);
}
void MapScene::anti(float dt)
{
	
	object* antiHero = (object*)getChildByTag(2047);
	antiHero->T++;
	if (antiHero->kongzhi == 1||antiHero->T<=20)
	{
		antiHero->enemy->stopAllActions(); return;
	}
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
			attactEnemy(antiPos, Friend.at(target)->getPosition(), 5,2047, 2046+ target);
		if (target == 0)
			attactEnemy(antiPos, Friend.at(target)->getPosition(),5, 2047, 2044);
		if (target == 1)
			attactEnemy(antiPos, Friend.at(target)->getPosition(), 5, 2047, 2046);
	}
	//战士
	if (YourHeroID == 1)
	{
		//泣血
		if (YourGold >= 1740 && YouBuyWeaponNum == 0)
		{
			YouBuyWeaponNum++;
			YourGold -= 1740;
			Sprite* YouEquip1Zhanji = Sprite::create("SHOP/weapon1.png");
			YouEquip1Zhanji->setPosition(Vec2(1050 + YouBuyWeaponNum * 100, 300));
			YouEquip1Zhanji->setScale(0.5f);
			YouEquip1Zhanji->setVisible(false);
			addChild(YouEquip1Zhanji, 14, 820 + YouBuyWeaponNum);
			antiHero->greedy += 0.25;
			object *a = (object *)this->getChildByTag(2047);
			a->Aggressivity += 100;
		}
		//破魔
		if (YourGold >= 2000 && YouBuyWeaponNum ==1)
		{
			YouBuyWeaponNum++;
			YourGold -= 2000;
			Sprite* YouEquip2Zhanji = Sprite::create("SHOP/weapon2.png");
			YouEquip2Zhanji->setPosition(Vec2(1050 + YouBuyWeaponNum * 100, 300));
			YouEquip2Zhanji->setScale(0.5f);
			YouEquip2Zhanji->setVisible(false);
			addChild(YouEquip2Zhanji, 14, 820 + YouBuyWeaponNum);
			object *a = (object *)this->getChildByTag(2047);
			a->Aggressivity += 100;
			a->Armor += 100;
			
		}
		//攻速鞋
		if (YourGold >= 710 && YouBuyWeaponNum == 2)
		{
			YouBuyWeaponNum++;
			YourGold -= 710;
			Sprite* YouEquip3Zhanji = Sprite::create("SHOP/weapon6.png");
			YouEquip3Zhanji->setPosition(Vec2(1050 + YouBuyWeaponNum * 100, 300));
			YouEquip3Zhanji->setScale(0.5f);
			YouEquip3Zhanji->setVisible(false);
			addChild(YouEquip3Zhanji, 14, 820 + YouBuyWeaponNum);
			object *a = (object *)this->getChildByTag(2047);
			a->attack_speed += 0.3;
			a->speed += 60;
		}
		//霸者重装
		if (YourGold >= 2070 && YouBuyWeaponNum == 3)
		{
			YouBuyWeaponNum++;
			YourGold -= 2070;
			Sprite* YouEquip4Zhanji = Sprite::create("SHOP/weapon5.png");
			YouEquip4Zhanji->setPosition(Vec2(1050 + YouBuyWeaponNum * 100, 300));
			YouEquip4Zhanji->setScale(0.5f);
			YouEquip4Zhanji->setVisible(false);
			addChild(YouEquip4Zhanji, 14, 820 + YouBuyWeaponNum);
			object *a = (object *)this->getChildByTag(2047);
			a->maxBlood += 2000;
			a->blood += 2000;
			a->NaiBlood =1;
		}
		//霸者重装
		if (YourGold >= 2070 && YouBuyWeaponNum == 4)
		{
			YouBuyWeaponNum++;
			YourGold -= 2070;
			Sprite* YouEquip5Zhanji = Sprite::create("SHOP/weapon5.png");
			YouEquip5Zhanji->setPosition(Vec2(1050 + YouBuyWeaponNum * 100, 300));
			YouEquip5Zhanji->setScale(0.5f);
			YouEquip5Zhanji->setVisible(false);
			addChild(YouEquip5Zhanji, 14, 820 + YouBuyWeaponNum);
			object *a = (object *)this->getChildByTag(2047);
			a->maxBlood += 2000;
			a->blood += 2000;
			a->NaiBlood = 1;
		}
		//破魔
		if (YourGold >= 2000 &&YouBuyWeaponNum == 5)
		{
			YouBuyWeaponNum++;
			YourGold -= 2000;
			Sprite* YouEquip6Zhanji = Sprite::create("SHOP/weapon2.png");
			YouEquip6Zhanji->setPosition(Vec2(1050 + YouBuyWeaponNum * 100, 300));
			YouEquip6Zhanji->setScale(0.5f);
			YouEquip6Zhanji->setVisible(false);
			addChild(YouEquip6Zhanji, 14, 820 + YouBuyWeaponNum);
			object *a = (object *)this->getChildByTag(2047);
			a->Aggressivity += 100;
			a->Armor += 100;

		}
	}
	//射手
	if (YourHeroID == 2)
	{
		//泣血
		if (YourGold >= 1740 && YouBuyWeaponNum == 0)
		{
			YouBuyWeaponNum++;
			YourGold -= 1740;
			Sprite* YouEquip1Zhanji = Sprite::create("SHOP/weapon1.png");
			YouEquip1Zhanji->setPosition(Vec2(1050 + YouBuyWeaponNum * 100, 300));
			YouEquip1Zhanji->setScale(0.5f);
			YouEquip1Zhanji->setVisible(false);
			addChild(YouEquip1Zhanji, 14, 820 + YouBuyWeaponNum);
			antiHero->greedy += 0.25;
			object *a = (object *)this->getChildByTag(2047);
			a->Aggressivity += 100;
		}
		//攻速鞋
		if (YourGold >= 710 && YouBuyWeaponNum == 1)
		{
			YouBuyWeaponNum++;
			YourGold -= 710;
			Sprite* YouEquip2Zhanji = Sprite::create("SHOP/weapon6.png");
			YouEquip2Zhanji->setPosition(Vec2(1050 + YouBuyWeaponNum * 100, 300));
			YouEquip2Zhanji->setScale(0.5f);
			YouEquip2Zhanji->setVisible(false);
			addChild(YouEquip2Zhanji, 14, 820 + YouBuyWeaponNum);
			object *a = (object *)this->getChildByTag(2047);
			a->attack_speed += 0.3;
			a->speed += 60;
		}
		//破魔
		if (YourGold >= 2000 && YouBuyWeaponNum == 2)
		{
			YouBuyWeaponNum++;
			YourGold -= 2000;
			Sprite* YouEquip3Zhanji = Sprite::create("SHOP/weapon2.png");
			YouEquip3Zhanji->setPosition(Vec2(1050 + YouBuyWeaponNum * 100, 300));
			YouEquip3Zhanji->setScale(0.5f);
			YouEquip3Zhanji->setVisible(false);
			addChild(YouEquip3Zhanji, 14, 820 + YouBuyWeaponNum);
			object *a = (object *)this->getChildByTag(2047);
			a->Aggressivity += 100;
			a->Armor += 100;

		}
		//霸者重装
		if (YourGold >= 2070 && YouBuyWeaponNum == 3)
		{
			YouBuyWeaponNum++;
			YourGold -= 2070;
			Sprite* YouEquip4Zhanji = Sprite::create("SHOP/weapon5.png");
			YouEquip4Zhanji->setPosition(Vec2(1050 + YouBuyWeaponNum * 100, 300));
			YouEquip4Zhanji->setScale(0.5f);
			YouEquip4Zhanji->setVisible(false);
			addChild(YouEquip4Zhanji, 14, 820 + YouBuyWeaponNum);
			object *a = (object *)this->getChildByTag(2047);
			a->maxBlood += 2000;
			a->blood += 2000;
			a->NaiBlood = 1;
		}
		//破魔
		if (YourGold >= 2000 && YouBuyWeaponNum == 4)
		{
			YouBuyWeaponNum++;
			YourGold -= 2000;
			Sprite* YouEquip5Zhanji = Sprite::create("SHOP/weapon2.png");
			YouEquip5Zhanji->setPosition(Vec2(1050 + YouBuyWeaponNum * 100, 300));
			YouEquip5Zhanji->setScale(0.5f);
			YouEquip5Zhanji->setVisible(false);
			addChild(YouEquip5Zhanji, 14, 820 + YouBuyWeaponNum);
			object *a = (object *)this->getChildByTag(2047);
			a->Aggressivity += 100;
			a->Armor += 100;

		}
		//泣血
		if (YourGold >= 1740 && YouBuyWeaponNum == 5)
		{
			YouBuyWeaponNum++;
			YourGold -= 1740;
			Sprite* YouEquip6Zhanji = Sprite::create("SHOP/weapon1.png");
			YouEquip6Zhanji->setPosition(Vec2(1050 + YouBuyWeaponNum * 100, 300));
			YouEquip6Zhanji->setScale(0.5f);
			YouEquip6Zhanji->setVisible(false);
			addChild(YouEquip6Zhanji, 14, 820 + YouBuyWeaponNum);
			antiHero->greedy += 0.25;
			object *a = (object *)this->getChildByTag(2047);
			a->Aggressivity += 100;
		}
	}
	//法师
	if (YourHeroID == 3)
	{
		//博学
		if (YourGold >= 2300 && YouBuyWeaponNum == 0)
		{
			YouBuyWeaponNum++;
			YourGold -= 2300;
			Sprite* YouEquip1Zhanji = Sprite::create("SHOP/weapon3.png");
			YouEquip1Zhanji->setPosition(Vec2(1050 + YouBuyWeaponNum * 100, 300));
			YouEquip1Zhanji->setScale(0.5f);
			YouEquip1Zhanji->setVisible(false);
			addChild(YouEquip1Zhanji, 14, 820 + YouBuyWeaponNum);
			object *a = (object *)this->getChildByTag(2047);
			a->magic *= 1.35;
			a->magic += 240;
		}
		//贤者
		if (YourGold >= 2990 && YouBuyWeaponNum == 1)
		{
			YouBuyWeaponNum++;
			YourGold -= 2990;
			Sprite* YouEquip2Zhanji = Sprite::create("SHOP/weapon4.png");
			YouEquip2Zhanji->setPosition(Vec2(1050 + YouBuyWeaponNum * 100, 300));
			YouEquip2Zhanji->setScale(0.5f);
			YouEquip2Zhanji->setVisible(false);
			addChild(YouEquip2Zhanji, 14, 820 + YouBuyWeaponNum);
			object *a = (object *)this->getChildByTag(2047);
			a->maxBlood += 1400;
			a->blood += 1400;
			a->magic += 400;
		}
		//攻速鞋
		if (YourGold >= 710 && YouBuyWeaponNum == 2)
		{
			YouBuyWeaponNum++;
			YourGold -= 710;
			Sprite* YouEquip3Zhanji = Sprite::create("SHOP/weapon6.png");
			YouEquip3Zhanji->setPosition(Vec2(1050 + YouBuyWeaponNum * 100, 300));
			YouEquip3Zhanji->setScale(0.5f);
			YouEquip3Zhanji->setVisible(false);
			addChild(YouEquip3Zhanji, 14, 820 + YouBuyWeaponNum);
			object *a = (object *)this->getChildByTag(2047);
			a->attack_speed += 0.3;
			a->speed += 60;
		}
		//贤者
		if (YourGold >= 2990 && YouBuyWeaponNum == 3)
		{
			YouBuyWeaponNum++;
			YourGold -= 2990;
			Sprite* YouEquip4Zhanji = Sprite::create("SHOP/weapon4.png");
			YouEquip4Zhanji->setPosition(Vec2(1050 + YouBuyWeaponNum * 100, 300));
			YouEquip4Zhanji->setScale(0.5f);
			YouEquip4Zhanji->setVisible(false);
			addChild(YouEquip4Zhanji, 14, 820 + YouBuyWeaponNum);
			object *a = (object *)this->getChildByTag(2047);
			a->maxBlood += 1400;
			a->blood += 1400;
			a->magic += 400;
		}
		//博学
		if (YourGold >= 2300 && YouBuyWeaponNum == 4)
		{
			YouBuyWeaponNum++;
			YourGold -= 2300;
			Sprite* YouEquip5Zhanji = Sprite::create("SHOP/weapon3.png");
			YouEquip5Zhanji->setPosition(Vec2(1050 + YouBuyWeaponNum * 100, 300));
			YouEquip5Zhanji->setScale(0.5f);
			YouEquip5Zhanji->setVisible(false);
			addChild(YouEquip5Zhanji, 14, 820 + YouBuyWeaponNum);
			object *a = (object *)this->getChildByTag(2047);
			a->magic *= 1.35;
			a->magic += 240;
		}
		//霸者重装
		if (YourGold >= 2070 && YouBuyWeaponNum == 5)
		{
			YouBuyWeaponNum++;
			YourGold -= 2070;
			Sprite* YouEquip4Zhanji = Sprite::create("SHOP/weapon5.png");
			YouEquip4Zhanji->setPosition(Vec2(1050 + YouBuyWeaponNum * 100, 300));
			YouEquip4Zhanji->setScale(0.5f);
			YouEquip4Zhanji->setVisible(false);
			addChild(YouEquip4Zhanji, 14, 820 + YouBuyWeaponNum);
			object *a = (object *)this->getChildByTag(2047);
			a->maxBlood += 2000;
			a->blood += 2000;
			a->NaiBlood = 1;
		}
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
					a->runAction(createAnimateS(4, "att", 8, 4,-1));
					bloodLine->stopAllActions();
					if(min>1) attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i,2046+i,4094+min);
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
					if (min >1) attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i, 2046 + i, 4094 + min);
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
					if (min >1) attactEnemy(soldersPostion, Enemy.at(min)->getPosition(), i, 2046 + i, 4094 + min);
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
			object* solder = (object*)getChildByTag(4094 + i);
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
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 4094 + i, 2046 + min);
					if(min==0)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 4094 + i, 2044);
					if(min==1)
					attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 4094 + i, 2046);
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
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 4094 + i, 2046 + min);
					if (min == 0)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 4094 + i, 2044);
					if (min == 1)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 4094 + i, 2046);
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
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 4094 + i, 2046 + min);
					if (min == 0)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 4094 + i, 2044);
					if (min == 1)
						attactEnemy(soldersPostion, Friend.at(min)->getPosition(), i + 4, 4094 + i, 2046);
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
void MapScene::increaseGold(float dt)
{
	MyGold += 5;
	YourGold += 5;
}
void MapScene::skillQ(float dt)
{
	if (MyHeroID == 2)
	{
		if (timeQ == 0 && hero->skill_Q == 1)
		{
			if (hero->zQ == 1) hero->zQ = 0;
			else hero->zQ = 1;

		}
		if (hero->zQ == 1)
		{
			myMagic->runAction(createAnimateM(12, 14, 0.03, 1));
			hero->range = 100;
			if (timeQ == 0 && hero->skill_Q == 1)
			{
				hero->attack_speed += (0.5 + hero->level * 0.05);
				hero->Aggressivity += (20 + hero->level * 2);
			}
		}
		if (hero->zQ == 0)
		{
			hero->range = 200 + hero->level * 10;
			if (timeQ == 0 && hero->skill_Q == 1)
			{
				hero->attack_speed -= (0.5 + hero->level * 0.05);
				hero->Aggressivity -= (20 + hero->level * 2);
				myMagic->runAction(createAnimateM());
			}
		}
		if (hero->skill_Q == 1 && timeQ != 4) timeQ++;
		if (timeQ == 4)
		{
			timeQ = 0;
			hero->skill_Q = 0;
		}
		CCLOG("%d&&%f", hero->Aggressivity, hero->attack_speed);
	}
	if (MyHeroID == 1)
	{
		if (hero->skill_Q == 1 && timeQ != 20)
		{
			if (timeQ == 0)
			{
				hero->speed = 150;
				myMagic->runAction(createAnimateM(13, 13, 0.1, 1));
			}
			timeQ++;
		}
		if (timeQ == 10)
		{
			myMagic->runAction(createAnimateM());
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
	if (MyHeroID == 3)
	{
		bool e = 0;
		if (hero->skill_Q == 1 && timeQ == 0)
		{
			Vec2 pos = hero->herosPosition();
			for (int i = 1; i < Enemy.size(); i++)
			{
				auto a = Enemy.at(i);
				Vec2 Epos = a->getPosition();
				float dis = getDistance(pos, Epos);
				if (dis <= 150)
				{
					object* solder;
					if (i == 1)
					{
						solder = (object*)getChildByTag(2047);
						solder->Armor = solder->Armor * 0.7;
						e = 1;
					}
					else
						solder = (object*)getChildByTag(4094 + i);
					solder->blood -= (hero->level * 15 + 50 + hero->magic * 0.5) * (1 - solder->Armor / 1000);
					solder->Armor = solder->Armor - 50;
					float cur = (((float)solder->blood / (float)solder->maxBlood) * 100);
					solder->Loading->setPercentage(cur);
					if (solder->blood <= 0&&solder->used==0)
					{
						solder->used = 1;
						if (solder->getTag() == 2047)
						{
						//	solder->blood = 10;
							solder->used = 0;
							solder->enemy->setPosition(Vec2(-1460, -1030));
							solder->Loading->setPosition(Vec2(-1460, -1080));
							solder->Loading->setPercentage(100.0);
						}
						else
							solder->enemy->setPosition(-2000, -2000);
						hero->exprience += 10;
						MyGold += solder->gold;
						if (hero->needExprience == hero->exprience)
						{
							lup = 0;
							hero->level++; hero->needExprience = hero->needExprience + 20; hero->exprience = 0; hero->skillNum = 1;
							hero->blood += 50;
							hero->maxBlood += 50;
							hero->Aggressivity += 5;
							hero->Armor += 20;
							hero->attack_speed += 0.2;
						}
					}
				}
			}
			myMagic->setScale(3.0);
			myMagic->runAction(createAnimateM(5, 24, 0.03, 1));
			myMagic->runAction(createAnimateM());
		}
		if (hero->skill_Q == 1 && timeQ == 10); 
		{
			object* solder = (object*)getChildByTag(2047);
			if (e == 1)
				solder->magic_resistance = (solder->magic_resistance + 50) / 0.7;
		}
		if (hero->skill_Q == 1 && timeQ != 20)
		{
			timeQ++;
		}
		if (timeQ == 20)
		{
			timeQ = 0;
			hero->skill_Q = 0;
			CCLOG("Q is ready");
		}
	}
}
void MapScene::skillW(float dt)
{
	if (MyHeroID == 2)
	{
		if (hero->skill_W == 1 && timeW == 0 && hero->W != 2)
		{
			hero->W = 1;
		}
		if (hero->skill_W == 1 && timeW != 30 && hero->W == 2)
		{
			timeW++;
		}
		if (timeW == 30)
		{
			timeW = 0;
			hero->skill_W = 0;
			hero->W = 0;
			CCLOG("W is ready");
		}
	}
	if (MyHeroID == 1)
	{
		if (hero->skill_W == 1 && timeW != 30)
		{

			timeW++;
		}
		if (timeW == 1 && hero->skill_W == 1)
		{
			hero->Armor = hero->Armor * (1.5 + 0.05 * hero->level);

			myMagic->runAction(createAnimateM(2, 10, 0.1, 1));
		}
		if (timeW == 6)
		{

			myMagic->runAction(createAnimateM());
			hero->Armor = hero->Armor / 2;
		}
		if (timeW == 30)
		{
			timeW = 0;
			hero->skill_W = 0;
			CCLOG("W is ready");
		}
	}
	if (MyHeroID == 3)
	{
		if (hero->skill_W == 1 && timeW == 0 && hero->W != 2)
		{
			hero->W = 1;
		}
		if (hero->skill_W == 1 && timeW <= 10 && hero->W == 2)
		{
			for (int i = 1; i < Enemy.size(); i++)
			{
				object* so = (object*)getChildByTag(hero->E_object);
				object* tar;
				auto a = Enemy.at(i);
				if (i > 1)
					tar = (object*)getChildByTag(4094 + i);
				if (i == 0)
					tar = (object*)getChildByTag(2045);
				if (i == 1)
					tar = (object*)getChildByTag(2047);
				float distance1 = getDistance(so->enemy->getPosition(), a->getPosition());
				if (distance1 <= 150)
				{
					tar->blood -= (hero->level * 5 + hero->magic * 0.2 + 20) * (1 - tar->Armor / 1000);
					float cur = (((float)tar->blood / (float)tar->maxBlood) * 100);
					tar->Loading->setPercentage(cur);
					if (tar->blood <= 0 && tar->used == 0)
					{
						tar->used = 1;
						if (tar->getTag() == 2047)
						{
						//	tar->blood = 10;
							tar->used = 0;
							tar->enemy->setPosition(Vec2(-1460, -1030));
							tar->Loading->setPosition(Vec2(-1460, -1080));
							tar->Loading->setPercentage(100.0);
						}
						else
							tar->enemy->setPosition(-2000, -2000);
						hero->exprience = hero->exprience + 10;
						if (hero->needExprience == hero->exprience)
						{
							lup = 0;
							hero->level++; hero->needExprience = hero->needExprience + 20; hero->exprience = 0; hero->skillNum = 1;
							hero->blood += 50;
							hero->maxBlood += 50;
							hero->Aggressivity += 5;
							hero->Armor += 20;
							hero->attack_speed += 0.2;
						}
						MyGold += tar->gold;
					}
				}

			}
		}
		if (hero->skill_W == 1 && timeW != 30 && hero->W == 2)
		{
			timeW++;
		}
		if (timeW == 30)
		{
			timeW = 0;
			hero->skill_W = 0;
			hero->W = 0;
			CCLOG("W is ready");
		}
	}
}
void MapScene::skillE(float dt)
{
	if (MyHeroID == 2)
	{
		if (hero->skill_E == 1 && timeE == 0&&hero->E!=2)
		{
			hero->E = 1;
		}
		if (hero->skill_E == 1 && timeE != 4&& hero->E == 2)
		{
			timeE++;
		}
		if (timeE == 4)
		{
			timeE = 0;
			hero->skill_E = 0;
			hero->E = 0;
			CCLOG("E is ready");
		}
	
	}
	if (MyHeroID == 1)
	{

		if (hero->skill_E == 1 && timeE != 30)
		{
			timeE++;
		}
		if (hero->skill_E == 1 && timeE <= 10)
		{
			Vec2 pos = hero->herosPosition();
			myMagic->runAction(createAnimateM(1, 20, 0.025, 1));
			for (int i = 1; i < Enemy.size(); i++)
			{
				auto a = Enemy.at(i);
				Vec2 Epos = a->getPosition();
				float dis = getDistance(pos, Epos);
				if (dis <= 150)
				{
					object* solder;
					if (i == 1)
						solder = (object*)getChildByTag(2047);
					else
						solder = (object*)getChildByTag(4094 + i);
					solder->blood -= (hero->level * 2.5 + 20 + (hero->Aggressivity - 50) * 0.2) * (1 - solder->Armor / 1000);
					float cur = (((float)solder->blood / (float)solder->maxBlood) * 100);
					solder->Loading->setPercentage(cur);
					if (solder->blood <= 0 && solder->used == 0)
					{
						solder->used = 1;
						if (solder->getTag() == 2047)
						{
						//	solder->blood = 10;
							solder->used = 0;
							solder->enemy->setPosition(Vec2(-1460, -1030));
							solder->Loading->setPosition(Vec2(-1460, -1080));
							solder->Loading->setPercentage(100.0);
						}
						else
							solder->enemy->setPosition(-2000, -2000);
						hero->exprience += 10;
						MyGold += solder->gold;
						if (hero->needExprience == hero->exprience)
						{
							lup = 0;
							hero->level++; hero->needExprience = hero->needExprience + 20; hero->exprience = 0; hero->skillNum = 1;
							hero->blood += 50;
							hero->maxBlood += 50;
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
		}
	}
	if (MyHeroID == 3)
	{
		if (hero->skill_E == 1 && timeE == 0)
		{
			Vec2 hPos = hero->herosPosition();
			Vec2 aPos = Enemy.at(1)->getPosition();
			float d = getDistance(hPos, aPos);
			if (d <= 200)
			{
				antiMagic->runAction(createAnimateM(6, 17, 0.04, 1));
			}
		}
		if (hero->skill_E == 1 && timeE == 2)
		{
			Vec2 hPos = hero->herosPosition();
			Vec2 aPos = Enemy.at(1)->getPosition();
			float d = getDistance(hPos, aPos);
			if (d <= 200)
			{
				object* e = (object*)getChildByTag(2047);
				e->blood = e->blood - (50 + hero->level * 10 + hero->magic * 0.5) * (1 - e->Armor / 1000);
				e->kongzhi = 1;
				float cur = (((float)e->blood / (float)e->maxBlood) * 100);
				e->Loading->setPercentage(cur);
				if (e->blood <= 0)
				{
			//		e->blood = 10;
					e->enemy->setPosition(Vec2(-1460, -1030));
					e->Loading->setPosition(Vec2(-1460, -1080));
					e->Loading->setPercentage(100.0);
					hero->exprience = hero->exprience + 10;
					if (hero->needExprience == hero->exprience)
					{
						lup = 0;
						hero->level++; hero->needExprience = hero->needExprience + 20; hero->exprience = 0;
						hero->skillNum = 1;
						hero->blood += 10;
						hero->maxBlood += 10;
						hero->Aggressivity += 5;
						hero->Armor += 20;
						hero->attack_speed += 0.2;
					}
					CCLOG("8*&%d&*8", hero->level);
					MyGold = MyGold + e->gold;
				}
				antiMagic->runAction(createAnimateM());
			}
		}
		if (hero->skill_E == 1 && timeE == 6)
		{
			object* e = (object*)getChildByTag(2047);
			e->kongzhi = 0;
		}
		if (hero->skill_E == 1 && timeE != 30)
		{
			timeE++;
		}
		if (timeE == 30)
		{
			timeE = 0;
			hero->skill_E = 0;
			CCLOG("E is ready");
		}
	}
}
void MapScene::skillR(float dt)
{
	if (MyHeroID == 2)
	{
		if (hero->skill_R == 1 && timeR == 0)
		{
			myMagic->runAction(createAnimateM(10, 21, 0.05, 8));
			hero->Aggressivity += 10;
			hero->attack_speed += 0.2 * hero->level;
			hero->speed += 40;
		}
		if (hero->skill_R == 1 && timeR == 16)
		{
			hero->heroSprite->stopAllActions();
			hero->Aggressivity -= 10;
			hero->attack_speed -= 0.2 * hero->level;
			hero->speed -= 40;
			myMagic->runAction(createAnimateM());
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
			CCLOG("R is ready");
		}
	}
	if (MyHeroID == 1)
	{
		if (hero->skill_R == 1 && timeR == 0)
		{
			Vec2 hPos = hero->herosPosition();
			Vec2 aPos = Enemy.at(1)->getPosition();
			float d = getDistance(hPos, aPos);
			if (d <= 150)
			{
				antiMagic->runAction(createAnimateM(11, 18, 0.04, 1));
			}
		}
		if (hero->skill_R == 1 && timeR == 2)
		{
			Vec2 hPos = hero->herosPosition();
			Vec2 aPos = Enemy.at(1)->getPosition();
			float d = getDistance(hPos, aPos);
			if (d <= 150)
			{
				object* e = (object*)getChildByTag(2047);
				e->blood = e->blood - (hero->level * 10 + (e->maxBlood - e->blood) * 0.33) * (1 - e->Armor / 1000);
				float cur = (((float)e->blood / (float)e->maxBlood) * 100);
				e->Loading->setPercentage(cur);
				if (e->blood <= 0)
				{
					hero->exprience = hero->exprience + 10;
					if (hero->needExprience == hero->exprience)
					{
						lup = 0;
						hero->level++; hero->needExprience += 20; hero->exprience = 0;
						hero->skillNum++;
						hero->blood += 10;
						hero->maxBlood += 10;
						hero->Aggressivity += 5;
						hero->Armor += 20;
						hero->attack_speed += 0.2;
					}
					MyGold = MyGold + e->gold;
				//	e->blood = e->maxBlood;
					e->enemy->setPosition(Vec2(-1460, -1030));
					e->Loading->setPosition(Vec2(-1460, -1080));
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
		}
	}
	if (MyHeroID == 3)
	{
		if (current == 0 && hero->skill_R == 1)
		{
			timeR = 0;
			hero->skill_R = 0;
			return;
		}
		object* tar = (object*)getChildByTag(current);
		Sprite* ta = Sprite::create("magic_1_20.png");
		ta->setScale(3.0);
		addChild(ta);
		if (hero->skill_R == 1 && timeR == 0)
		{
			Vec2 pos = tar->enemy->getPosition();
			ta->setPosition(pos);
			auto sequence = Sequence::create(createAnimateM(9, 19, 0.07, 1), createAnimateM(), NULL);
			ta->runAction(sequence);
			for (int i = 1; i < Enemy.size(); i++)
			{
				auto a = Enemy.at(i);
				Vec2 Epos = a->getPosition();
				float dis = getDistance(pos, Epos);
				if (dis <= 150)
				{
					hero->R_dis = 1;
					object* solder;
					if (i == 1)
						solder = (object*)getChildByTag(2047);
					else
						solder = (object*)getChildByTag(4094 + i);
					solder->blood -= (hero->level * 20 + 80 + hero->magic * 0.8) * (1 - solder->magic_resistance / 1000);
					float cur = (((float)solder->blood / (float)solder->maxBlood) * 100);
					solder->Loading->setPercentage(cur);
					if (solder->blood <= 0 && solder->used == 0)
					{
						solder->used = 1;
						if (solder->getTag() == 2047)
						{
					//		solder->blood = 10;
							solder->used = 0;
							solder->enemy->setPosition(Vec2(-1460, -1030));
							solder->Loading->setPosition(Vec2(-1460, -1080));
							solder->Loading->setPercentage(100.0);
						}
						else
							solder->enemy->setPosition(-2000, -2000);
						hero->exprience = hero->exprience + 10;
						if (hero->needExprience == hero->exprience)
						{
							lup = 0;
							hero->level++; hero->needExprience = hero->needExprience + 20; hero->exprience = 0;
							hero->skillNum = 1;
							hero->blood += 10;
							hero->maxBlood += 10;
							hero->Aggressivity += 5;
							hero->Armor += 20;
							hero->attack_speed += 0.2;
						}
						CCLOG("8*&%d&*8", hero->level);
						MyGold = MyGold + solder->gold;

					}
				}
			}
		}
		if (hero->skill_R == 1 && timeR != 60)
		{
			timeR++;
		}
		if (timeR == 60 && hero->skill_R == 1)
		{
			timeR = 0;
			hero->skill_R = 0;
			hero->R_dis = 0;
			CCLOG("R is ready");
		}
	}
}
void MapScene::finishAttack(const int tag1, const int tag2)
{
	if (tag1 != 2046)
	{
		object* solder1 = (object*)getChildByTag(tag1);
		if (tag2 != 2046)
		{
			object* solder1 = (object*)getChildByTag(tag1);
			object* solder2 = (object*)getChildByTag(tag2);
			solder2->blood = solder2->blood - solder1->Aggressivity * (1 - 0.01 * (solder2->Armor / 10));
			if(tag1==2047)
			{
				CCLOG("%d***", solder1->Aggressivity);
			}
			float cur = (((float)solder2->blood / (float)solder2->maxBlood) * 100);
			solder2->Loading->setPercentage(cur);
			if (solder2->blood <= 0 && solder2->used == 0)
			{
				solder2->used = 1;
				if (tag2 == 2047)
				{
				//	solder2->blood = 10;
					solder2->used = 0;
					solder2->enemy->setPosition(Vec2(-1460, -1030));
					solder2->Loading->setPosition(Vec2(-1460, -1080));
					solder2->Loading->setPercentage(100.0);
				}
				else
				{
					solder2->enemy->setPosition(-2000, -2000);
				}
				if (tag2 == 2047 || tag2 > 4094 || tag2 == 2045)
				{
					hero->exprience += 10;
					hero->times++;
					CCLOG("puls   puls puls   %d", hero->times);
				}
				if (hero->needExprience <= hero->exprience)
				{
					lup = 0;
					hero->level++;
					hero->needExprience = hero->needExprience + 20; hero->exprience = 0; hero->skillNum = 1;
					hero->blood += 50;
					hero->maxBlood += 50;
					hero->Aggressivity += 5;
					hero->Armor += 20;
					hero->attack_speed += 0.2;
				}
				if ((tag2 >= 2044 && tag2 <= 3000))
				{
					object* tmpanti = (object*)getChildByTag(2047);
					tmpanti->nowexp = tmpanti->nowexp + 10;
					if (tmpanti->maxexp <= tmpanti->nowexp)
					{

						tmpanti->level++;
						tmpanti->maxexp = tmpanti->maxexp + 20;
						tmpanti->nowexp = 0;
						tmpanti->blood += 10;
						tmpanti->maxBlood += 10;
						tmpanti->Aggressivity += 5;
						tmpanti->Armor += 20;
						tmpanti->attack_speed += 0.2;
					}
				}
				CCLOG("8*&%d&*8", hero->level);
				CCLOG("$%d$", hero->exprience);
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
	if (tag1 == 2046)
	{
		CCLOG("**%d", tag2);
		heroPrint* hero = (heroPrint*)getChildByTag(2046);
		object* solder2 = (object*)getChildByTag(tag2);
		if (timeQ <= 10 && hero->Q == 1 && MyHeroID == 1)
		{
			solder2->blood = solder2->blood - (hero->Aggressivity + 50 + hero->level * 15) * (1 - 0.01 * (solder2->Armor / 10));
			if (hero->greedy > 0)
			{
				hero->blood += (hero->Aggressivity + 50 + hero->level * 15) * (1 - 0.01 * (solder2->Armor / 10))*hero->greedy;
				if (hero->blood >= hero->maxBlood)
					hero ->blood = hero->maxBlood;
			}
			hero->Q = 0;
		}
		else
		{
			solder2->blood = solder2->blood - hero->Aggressivity * (1 - 0.01 * (solder2->Armor / 10));
			hero->blood += hero->Aggressivity * (1 - 0.01 * (solder2->Armor / 10))*hero->greedy;
			if (hero->blood >= hero->maxBlood)
				hero->blood = hero->maxBlood;
		}
		CCLOG("##%d", solder2->blood);
		float cur = (((float)solder2->blood / (float)solder2->maxBlood) * 100);
		solder2->Loading->setPercentage(cur);
		if (solder2->blood <= 0&&solder2->used==0 )
		{
			solder2->used = 1;
			if (tag2 == 2047)
			{
			//	solder2->blood = 10;
				solder2->used = 0;
				solder2->enemy->setPosition(Vec2(-1860, -1030));
				solder2->Loading->setPosition(Vec2(-1860, -1080));
			}
			else
			{
				solder2->used = 1;
				solder2->enemy->setPosition(-2000, -2000);
			}
			if (tag2 == 2047 || (tag2 > 4094))
				hero->exprience = hero->exprience + 10;
			if (hero->needExprience == hero->exprience)
			{
				lup = 0;
				hero->level++; hero->needExprience = hero->needExprience + 20; hero->exprience = 0;
				hero->skillNum = 1;
				hero->blood += 10;
				hero->maxBlood += 10;
				hero->Aggressivity += 5;
				hero->Armor += 20;
				hero->attack_speed += 0.2;
			}
			
			CCLOG("8*&%d&*8", hero->level);
			MyGold = MyGold + solder2->gold;
		}
	}
}
void MapScene::quanshui(float dt)
{//391 127
	if (myle != hero->level)
	{
		myle = hero->level;
		myMagic->runAction(createAnimateL());
	}
	object *t = (object*)getChildByTag(2047);
	if (yourle != t->level)
	{
		yourle = t->level;
		antiMagic->runAction(createAnimateL());
	}
	Vec2 pos = hero->heroSprite->getPosition();
	if (getDistance(pos, Vec2(391, 127)) <= 150)
	{
		if (hero->blood < hero->maxBlood)
		{
			hero->blood += 50;
			if (hero->blood >= hero->maxBlood)
				hero->blood = hero->maxBlood;
		}
	}
	object* solder = (object*)getChildByTag(2047);
	if (hero->NaiBlood == 1&&hero->blood<hero->maxBlood)
		hero->blood += hero->maxBlood;
	if (solder->NaiBlood == 1 && solder->blood < solder->maxBlood)
		solder->blood += solder->maxBlood;
}
void MapScene::EnterHelloWorldScene(Ref* pSenderBack)
{
	//全局重新初始化
	SimpleAudioEngine::getInstance()->playEffect("MUSIC/Touch.wav");
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	MyHeroID = 0;
	MyGold = 0;
	MyBuyWeaponNum = 0;
	YourHeroID = 0;
	YourGold = 0;
	YouBuyWeaponNum = 0;
	MeDead = 0;
	MeKill = 0;
	YouDead = 0;
	YouKill = 0;
	current = 0;

	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, HelloWorld::createScene()));
}

void MapScene::Shopcall(Ref* sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		SimpleAudioEngine::getInstance()->playEffect("MUSIC/Touch.wav");
		//战绩图标不可触发
		Button* tmp1 = (Button*)this->getChildByTag(51);
		tmp1->setTouchEnabled(false);
		//商城图标不可触发
		Button* tmp2 = (Button*)this->getChildByTag(50);
		tmp2->setTouchEnabled(false);

		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		Button* ShopBackButton = Button::create();
		ShopBackButton->loadTextures("UI/ShopBack.png", "UI/ShopBack.png", "");
		ShopBackButton->setPosition(Vec2(1665, 910));
		ShopBackButton->addTouchEventListener(CC_CALLBACK_2(MapScene::ShopBack, this));
		ShopBackButton->setScale(0.72f);
		addChild(ShopBackButton, 14, 64);

		Sprite* ShopBackGround;
		ShopBackGround = Sprite::create("UI/Shopbackground.png");
		ShopBackGround->setPosition(Vec2(960, 550));
		ShopBackGround->setScale(0.85f);
		ShopBackGround->setOpacity(230);
		addChild(ShopBackGround, 10, 60);

		//装备方案图标
		Sprite* EquipPlan = Sprite::create("UI/plan.png");
		EquipPlan->setPosition(Vec2(280, 200));
		EquipPlan->setScale(1.0f);
		addChild(EquipPlan, 10, 61);

		//左上角小部件
		Sprite* EquipLittle1 = Sprite::create("UI/little.png");
		EquipLittle1->setPosition(Vec2(200, 900));
		EquipLittle1->setScale(1.0f);
		addChild(EquipLittle1, 10, 62);

		//"商城"文字
		auto* chnString = Dictionary::createWithContentsOfFile("UI/CHN_String.xml");
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
		Weapon1->loadTextures("SHOP/weapon1.png", "SHOP/weapon1.png", "");
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
		Weapon2->loadTextures("SHOP/weapon2.png", "SHOP/weapon2.png", "");
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
		Weapon3->loadTextures("SHOP/weapon3.png", "SHOP/weapon3.png", "");
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
		Weapon4->loadTextures("SHOP/weapon4.png", "SHOP/weapon4.png", "");
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
		Weapon5->loadTextures("SHOP/weapon5.png", "SHOP/weapon5.png", "");
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
		Weapon6->loadTextures("SHOP/weapon6.png", "SHOP/weapon6.png", "");
		Weapon6->setPosition(Vec2(1000, 350));
		Weapon6->setScale(0.8f);
		Weapon6->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy6, this));
		addChild(Weapon6, 12, 106);


		Sprite* Equipment1 = Sprite::create("UI/kong.png");
		Equipment1->setPosition(Vec2(500, 200));
		Equipment1->setScale(0.8f);
		addChild(Equipment1, 12, 71);

		Sprite* Equipment2 = Sprite::create("UI/kong.png");
		Equipment2->setPosition(Vec2(650, 200));
		Equipment2->setScale(0.8f);
		addChild(Equipment2, 12, 72);

		Sprite* Equipment3 = Sprite::create("UI/kong.png");
		Equipment3->setPosition(Vec2(800, 200));
		Equipment3->setScale(0.8f);
		addChild(Equipment3, 12, 73);

		Sprite* Equipment4 = Sprite::create("UI/kong.png");
		Equipment4->setPosition(Vec2(950, 200));
		Equipment4->setScale(0.8f);
		addChild(Equipment4, 12, 74);

		Sprite* Equipment5 = Sprite::create("UI/kong.png");
		Equipment5->setPosition(Vec2(1100, 200));
		Equipment5->setScale(0.8f);
		addChild(Equipment5, 12, 75);

		Sprite* Equipment6 = Sprite::create("UI/kong.png");
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
		SimpleAudioEngine::getInstance()->playEffect("MUSIC/Touch.wav");
		//战绩图标不可触发
		Button* tmp1 = (Button*)this->getChildByTag(51);
		tmp1->setTouchEnabled(false);
		//商城图标不可触发
		Button* tmp2 = (Button*)this->getChildByTag(50);
		tmp2->setTouchEnabled(false);

		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		auto* chnString = Dictionary::createWithContentsOfFile("UI/CHN_String.xml");
		const char* KillStr = ((String*)chnString->objectForKey("Kill_Text"))->getCString();
		const char* DeadStr = ((String*)chnString->objectForKey("Dead_Text"))->getCString();
		const char* ZhanjiStr = ((String*)chnString->objectForKey("Zhanji_Text"))->getCString();

		Button* ZhanjiBackButton = Button::create();
		ZhanjiBackButton->loadTextures("UI/ShopBack.png", "UI/ShopBack.png", "");
		ZhanjiBackButton->setPosition(Vec2(1665, 910));
		ZhanjiBackButton->addTouchEventListener(CC_CALLBACK_2(MapScene::ZhanjiBack, this));
		ZhanjiBackButton->setScale(0.72f);
		addChild(ZhanjiBackButton, 14, 80);

		Sprite* ZhanjiBackGround;
		ZhanjiBackGround = Sprite::create("UI/Shopbackground.png");
		ZhanjiBackGround->setPosition(Vec2(960, 550));
		ZhanjiBackGround->setScale(0.85f);
		ZhanjiBackGround->setOpacity(230);
		addChild(ZhanjiBackGround, 10, 81);

		Sprite* LittleTag = Sprite::create("UI/little.png");
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
		Sprite* MyEquipment1 = Sprite::create("UI/kong.png");
		MyEquipment1->setPosition(Vec2(250, 300));
		MyEquipment1->setScale(0.6f);
		addChild(MyEquipment1, 12, 201);

		Sprite* MyEquipment2 = Sprite::create("UI/kong.png");
		MyEquipment2->setPosition(Vec2(350, 300));
		MyEquipment2->setScale(0.6f);
		addChild(MyEquipment2, 12, 202);

		Sprite* MyEquipment3 = Sprite::create("UI/kong.png");
		MyEquipment3->setPosition(Vec2(450, 300));
		MyEquipment3->setScale(0.6f);
		addChild(MyEquipment3, 12, 203);

		Sprite* MyEquipment4 = Sprite::create("UI/kong.png");
		MyEquipment4->setPosition(Vec2(550, 300));
		MyEquipment4->setScale(0.6f);
		addChild(MyEquipment4, 12, 204);

		Sprite* MyEquipment5 = Sprite::create("UI/kong.png");
		MyEquipment5->setPosition(Vec2(650, 300));
		MyEquipment5->setScale(0.6f);
		addChild(MyEquipment5, 12, 205);

		Sprite* MyEquipment6 = Sprite::create("UI/kong.png");
		MyEquipment6->setPosition(Vec2(750, 300));
		MyEquipment6->setScale(0.6f);
		addChild(MyEquipment6, 12, 206);

		//对方空方案
		Sprite* YouEquipment1 = Sprite::create("UI/kong.png");
		YouEquipment1->setPosition(Vec2(1150, 300));
		YouEquipment1->setScale(0.6f);
		addChild(YouEquipment1, 12, 207);

		Sprite* YouEquipment2 = Sprite::create("UI/kong.png");
		YouEquipment2->setPosition(Vec2(1250, 300));
		YouEquipment2->setScale(0.6f);
		addChild(YouEquipment2, 12, 208);

		Sprite* YouEquipment3 = Sprite::create("UI/kong.png");
		YouEquipment3->setPosition(Vec2(1350, 300));
		YouEquipment3->setScale(0.6f);
		addChild(YouEquipment3, 12, 209);

		Sprite* YouEquipment4 = Sprite::create("UI/kong.png");
		YouEquipment4->setPosition(Vec2(1450, 300));
		YouEquipment4->setScale(0.6f);
		addChild(YouEquipment4, 12, 210);

		Sprite* YouEquipment5 = Sprite::create("UI/kong.png");
		YouEquipment5->setPosition(Vec2(1550, 300));
		YouEquipment5->setScale(0.6f);
		addChild(YouEquipment5, 12, 211);

		Sprite* YouEquipment6 = Sprite::create("UI/kong.png");
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
		for (int q = 1; q <= YouBuyWeaponNum; ++q)
		{
			Sprite *TMP = (Sprite *)this->getChildByTag(820+q);
			if (TMP != nullptr)
			{
				TMP->setVisible(true);
			}
		}

		//我方金币
		auto MyGoldZhanji = Sprite::create("UI/gold.png");
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
		auto YouGoldZhanji = Sprite::create("UI/gold.png");
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
		SimpleAudioEngine::getInstance()->playEffect("MUSIC/Touch.wav");
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
		for (int j = 1; j <= YouBuyWeaponNum; ++j)
		{
			Sprite* TMP = (Sprite*)this->getChildByTag(820 + j);
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
		SimpleAudioEngine::getInstance()->playEffect("MUSIC/Touch.wav");
		for (int j = 1; j <= 6; ++j)
		{
			for (int i = 1; i <= 3; ++i)
			{
				removeChildByTag(1000 + i + j * 10);
			}
		}

		Sprite* Weapon1Detail;
		Weapon1Detail = Sprite::create("SHOP/weapon1detail.png");
		Weapon1Detail->setPosition(Vec2(1653, 593));
		Weapon1Detail->setScale(0.7f);
		addChild(Weapon1Detail, 13, 1011);

		if (MyGold >= 1740 && MyBuyWeaponNum <= 5)
		{
			Button* BuyButton11 = Button::create();
			BuyButton11->loadTextures("UI/buy1.png", "UI/buy1P.png", "");
			BuyButton11->setPosition(Vec2(1650, 180));
			BuyButton11->setScale(1.0f);
			BuyButton11->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit1, this));
			addChild(BuyButton11, 14, 1012);
		}

		if (MyGold < 1740 || MyBuyWeaponNum >= 6)
		{
			Sprite* BuyButton12;
			BuyButton12 = Sprite::create("UI/buy2.png");
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
		SimpleAudioEngine::getInstance()->playEffect("MUSIC/Touch.wav");
		for (int j = 1; j <= 6; ++j)
		{
			for (int i = 1; i <= 3; ++i)
			{
				removeChildByTag(1000 + i + j * 10);
			}
		}

		Sprite* Weapon2Detail;
		Weapon2Detail = Sprite::create("SHOP/weapon2detail.png");
		Weapon2Detail->setPosition(Vec2(1650, 590));
		Weapon2Detail->setScale(0.7f);
		addChild(Weapon2Detail, 13, 1021);

		if (MyGold >= 2140 && MyBuyWeaponNum <= 5)
		{
			Button* BuyButton21 = Button::create();
			BuyButton21->loadTextures("UI/buy1.png", "UI/buy1P.png", "");
			BuyButton21->setPosition(Vec2(1650, 180));
			BuyButton21->setScale(1.0f);
			BuyButton21->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit2, this));
			addChild(BuyButton21, 14, 1022);
		}

		if (MyGold < 2140 || MyBuyWeaponNum >= 6)
		{
			Sprite* BuyButton22;
			BuyButton22 = Sprite::create("UI/buy2.png");
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
		SimpleAudioEngine::getInstance()->playEffect("MUSIC/Touch.wav");
		for (int j = 1; j <= 6; ++j)
		{
			for (int i = 1; i <= 3; ++i)
			{
				removeChildByTag(1000 + i + j * 10);
			}
		}

		Sprite* Weapon3Detail;
		Weapon3Detail = Sprite::create("SHOP/weapon3detail.png");
		Weapon3Detail->setPosition(Vec2(1653, 590));
		Weapon3Detail->setScale(0.7f);
		addChild(Weapon3Detail, 13, 1031);


		if (MyGold >= 2300 && MyBuyWeaponNum <= 5)
		{
			Button* BuyButton31 = Button::create();
			BuyButton31->loadTextures("UI/buy1.png", "UI/buy1P.png", "");
			BuyButton31->setPosition(Vec2(1650, 180));
			BuyButton31->setScale(1.0f);
			BuyButton31->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit3, this));
			addChild(BuyButton31, 14, 1032);
		}

		if (MyGold < 2300 || MyBuyWeaponNum >= 6)
		{
			Sprite* BuyButton32;
			BuyButton32 = Sprite::create("UI/buy2.png");
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
		SimpleAudioEngine::getInstance()->playEffect("MUSIC/Touch.wav");
		for (int j = 1; j <= 6; ++j)
		{
			for (int i = 1; i <= 3; ++i)
			{
				removeChildByTag(1000 + i + j * 10);
			}
		}
		Sprite* Weapon4Detail;
		Weapon4Detail = Sprite::create("SHOP/weapon4detail.png");
		Weapon4Detail->setPosition(Vec2(1653, 590));
		Weapon4Detail->setScale(0.7f);
		addChild(Weapon4Detail, 13, 1041);


		if (MyGold >= 2990 && MyBuyWeaponNum <= 5)
		{
			Button* BuyButton41 = Button::create();
			BuyButton41->loadTextures("UI/buy1.png", "UI/buy1P.png", "");
			BuyButton41->setPosition(Vec2(1650, 180));
			BuyButton41->setScale(1.0f);
			BuyButton41->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit4, this));
			addChild(BuyButton41, 14, 1042);
		}

		if (MyGold < 2990 || MyBuyWeaponNum >= 6)
		{
			Sprite* BuyButton42;
			BuyButton42 = Sprite::create("UI/buy2.png");
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
		SimpleAudioEngine::getInstance()->playEffect("MUSIC/Touch.wav");
		for (int j = 1; j <= 6; ++j)
		{
			for (int i = 1; i <= 3; ++i)
			{
				removeChildByTag(1000 + i + j * 10);
			}
		}

		Sprite* Weapon5Detail;
		Weapon5Detail = Sprite::create("SHOP/weapon5detail.png");
		Weapon5Detail->setPosition(Vec2(1658, 590));
		Weapon5Detail->setScale(0.7f);
		addChild(Weapon5Detail, 13, 1051);


		if (MyGold >= 900 && MyBuyWeaponNum <= 5)
		{
			Button* BuyButton51 = Button::create();
			BuyButton51->loadTextures("UI/buy1.png", "UI/buy1P.png", "");
			BuyButton51->setPosition(Vec2(1650, 180));
			BuyButton51->setScale(1.0f);
			BuyButton51->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit5, this));
			addChild(BuyButton51, 14, 1052);
		}

		if (MyGold < 900 || MyBuyWeaponNum >= 6)
		{
			Sprite* BuyButton52;
			BuyButton52 = Sprite::create("UI/buy2.png");
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
		SimpleAudioEngine::getInstance()->playEffect("MUSIC/Touch.wav");
		for (int j = 1; j <= 6; ++j)
		{
			for (int i = 1; i <= 3; ++i)
			{
				removeChildByTag(1000 + i + j * 10);
			}
		}
		Sprite* Weapon6Detail;
		Weapon6Detail = Sprite::create("SHOP/weapon6detail.png");
		Weapon6Detail->setPosition(Vec2(1650, 590));
		Weapon6Detail->setScale(0.7f);
		addChild(Weapon6Detail, 13, 1061);


		if (MyGold >= 710 && MyBuyWeaponNum <= 5)
		{
			Button* BuyButton61 = Button::create();
			BuyButton61->loadTextures("UI/buy1.png", "UI/buy1P.png", "");
			BuyButton61->setPosition(Vec2(1650, 180));
			BuyButton61->setScale(1.0f);
			BuyButton61->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit6, this));
			addChild(BuyButton61, 14, 1062);
		}

		if (MyGold < 710 || MyBuyWeaponNum >= 6)
		{
			Sprite* BuyButton62;
			BuyButton62 = Sprite::create("UI/buy2.png");
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
		hero->Aggressivity += 100;
		hero->greedy += 0.25;
		MyBuyWeaponNum++;
		SimpleAudioEngine::getInstance()->playEffect("MUSIC/GOLD.wav");
		Sprite* MyEquip1 = Sprite::create("SHOP/weapon1.png");
		MyEquip1->setPosition(Vec2(350 + MyBuyWeaponNum * 150, 200));
		MyEquip1->setScale(0.7f);
		addChild(MyEquip1, 14, 40 + MyBuyWeaponNum);

		Sprite* MyEquip1Zhanji = Sprite::create("SHOP/weapon1.png");
		MyEquip1Zhanji->setPosition(Vec2(150 + MyBuyWeaponNum * 100, 300));
		MyEquip1Zhanji->setScale(0.5f);
		MyEquip1Zhanji->setVisible(false);
		addChild(MyEquip1Zhanji, 14, 810 + MyBuyWeaponNum);

		MyGold -= 1740;
		CCString* goldCCstring = CCString::createWithFormat("%d", MyGold);
		std::string goldstring = goldCCstring->getCString();
		Label* tmpGoldNum = (Label*)this->getChildByTag(55);
		tmpGoldNum->setString(goldstring);
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
		hero->Aggressivity += 100;
		hero->Armor += 100;
		MyBuyWeaponNum++;
		SimpleAudioEngine::getInstance()->playEffect("MUSIC/GOLD.wav");
		Sprite* MyEquip2 = Sprite::create("SHOP/weapon2.png");
		MyEquip2->setPosition(Vec2(350 + MyBuyWeaponNum * 150, 200));
		MyEquip2->setScale(0.7f);
		addChild(MyEquip2, 14, 40 + MyBuyWeaponNum);

		Sprite* MyEquip2Zhanji = Sprite::create("SHOP/weapon2.png");
		MyEquip2Zhanji->setPosition(Vec2(150 + MyBuyWeaponNum * 100, 300));
		MyEquip2Zhanji->setScale(0.5f);
		MyEquip2Zhanji->setVisible(false);
		addChild(MyEquip2Zhanji, 14, 810 + MyBuyWeaponNum);

		MyGold -= 2140;
		CCString* goldCCstring = CCString::createWithFormat("%d", MyGold);
		std::string goldstring = goldCCstring->getCString();
		Label* tmpGoldNum = (Label*)this->getChildByTag(55);
		tmpGoldNum->setString(goldstring);
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
		hero->magic *= 1.35;
		hero->magic += 240;
		MyBuyWeaponNum++;
		SimpleAudioEngine::getInstance()->playEffect("MUSIC/GOLD.wav");
		Sprite* MyEquip3 = Sprite::create("SHOP/weapon3.png");
		MyEquip3->setPosition(Vec2(350 + MyBuyWeaponNum * 150, 200));
		MyEquip3->setScale(0.7f);
		addChild(MyEquip3, 14, 40 + MyBuyWeaponNum);

		Sprite* MyEquip3Zhanji = Sprite::create("SHOP/weapon3.png");
		MyEquip3Zhanji->setPosition(Vec2(150 + MyBuyWeaponNum * 100, 300));
		MyEquip3Zhanji->setScale(0.5f);
		MyEquip3Zhanji->setVisible(false);
		addChild(MyEquip3Zhanji, 14, 810 + MyBuyWeaponNum);

		MyGold -= 2300;
		CCString* goldCCstring = CCString::createWithFormat("%d", MyGold);
		std::string goldstring = goldCCstring->getCString();
		Label* tmpGoldNum = (Label*)this->getChildByTag(55);
		tmpGoldNum->setString(goldstring);
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
		hero->magic += 400;
		hero->maxBlood += 1400;
		MyBuyWeaponNum++;
		SimpleAudioEngine::getInstance()->playEffect("MUSIC/GOLD.wav");
		Sprite* MyEquip4 = Sprite::create("SHOP/weapon4.png");
		MyEquip4->setPosition(Vec2(350 + MyBuyWeaponNum * 150, 200));
		MyEquip4->setScale(0.7f);
		addChild(MyEquip4, 14, 40 + MyBuyWeaponNum);

		Sprite* MyEquip4Zhanji = Sprite::create("SHOP/weapon4.png");
		MyEquip4Zhanji->setPosition(Vec2(150 + MyBuyWeaponNum * 100, 300));
		MyEquip4Zhanji->setScale(0.5f);
		MyEquip4Zhanji->setVisible(false);
		addChild(MyEquip4Zhanji, 14, 810 + MyBuyWeaponNum);

		MyGold -= 2990;
		CCString* goldCCstring = CCString::createWithFormat("%d", MyGold);
		std::string goldstring = goldCCstring->getCString();
		Label* tmpGoldNum = (Label*)this->getChildByTag(55);
		tmpGoldNum->setString(goldstring);
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
		hero->maxBlood += 2000;
		hero->NaiBlood =1;
		MyBuyWeaponNum++;
		SimpleAudioEngine::getInstance()->playEffect("MUSIC/GOLD.wav");
		CCString* BuyCCstring5 = CCString::createWithFormat("%d", MyBuyWeaponNum);
		std::string buystring5 = BuyCCstring5->getCString();
		Sprite* MyEquip5 = Sprite::create("SHOP/weapon5.png");
		MyEquip5->setPosition(Vec2(350 + MyBuyWeaponNum * 150, 200));
		MyEquip5->setScale(0.7f);
		addChild(MyEquip5, 14, 40 + MyBuyWeaponNum);

		Sprite* MyEquip5Zhanji = Sprite::create("SHOP/weapon5.png");
		MyEquip5Zhanji->setPosition(Vec2(150 + MyBuyWeaponNum * 100, 300));
		MyEquip5Zhanji->setScale(0.5f);
		MyEquip5Zhanji->setVisible(false);
		addChild(MyEquip5Zhanji, 14, 810 + MyBuyWeaponNum);

		MyGold -= 900;
		CCString* goldCCstring = CCString::createWithFormat("%d", MyGold);
		std::string goldstring = goldCCstring->getCString();
		Label* tmpGoldNum = (Label*)this->getChildByTag(55);
		tmpGoldNum->setString(goldstring);
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
		hero->speed += 60;
		hero->attack_speed += 0.3;
		MyBuyWeaponNum++;
		SimpleAudioEngine::getInstance()->playEffect("MUSIC/GOLD.wav");
		CCString* BuyCCstring6 = CCString::createWithFormat("%d", MyBuyWeaponNum);
		std::string buystring6 = BuyCCstring6->getCString();
		Sprite* MyEquip6 = Sprite::create("SHOP/weapon6.png");
		MyEquip6->setPosition(Vec2(350 + MyBuyWeaponNum * 150, 200));
		MyEquip6->setScale(0.7f);
		addChild(MyEquip6, 14, 40 + MyBuyWeaponNum);

		Sprite* MyEquip6Zhanji = Sprite::create("SHOP/weapon6.png");
		MyEquip6Zhanji->setPosition(Vec2(150 + MyBuyWeaponNum * 100, 300));
		MyEquip6Zhanji->setScale(0.5f);
		MyEquip6Zhanji->setVisible(false);
		addChild(MyEquip6Zhanji, 14, 810 + MyBuyWeaponNum);

		MyGold -= 710;
		CCString* goldCCstring = CCString::createWithFormat("%d", MyGold);
		std::string goldstring = goldCCstring->getCString();
		Label* tmpGoldNum = (Label*)this->getChildByTag(55);
		tmpGoldNum->setString(goldstring);
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
		SimpleAudioEngine::getInstance()->playEffect("MUSIC/Touch.wav");
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

//游戏结束
void MapScene::GameOver()
{
	if (win == 0)//我方获胜
	{
		//战绩图标不可触发
		Button *tmp1 = (Button *)this->getChildByTag(51);
		tmp1->setTouchEnabled(false);
		//商城图标不可触发
		Button *tmp2 = (Button *)this->getChildByTag(50);
		tmp2->setTouchEnabled(false);

		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//停止一切人物行为（包括小兵野怪）
		stopAllActions();

		//一切监听器不再生效
		/*
		myKeyListener->release();
		listener->release();
		*/
		this->unscheduleAllSelectors();
		_eventDispatcher->removeAllEventListeners();

		//移除详细信息
		for (int b = 0; b <= 13; ++b)
		{
			if (b <= 6) removeChildByTag(50 + b);
			if (b <= 11) removeChildByTag(901 + b);
			removeChildByTag(108 + b);
		}

		//只有返回按钮
		auto pBackButtonItem = MenuItemImage::create("UI/Startbutton.jpg", "UI/StartbuttonP.jpg", CC_CALLBACK_1(MapScene::EnterHelloWorldScene, this));
		pBackButtonItem->setScale(1.5f);
		auto BackButton = Menu::create(pBackButtonItem, NULL);
		BackButton->setPosition(Vec2(960, 200));
		BackButton->setOpacity(200);
		addChild(BackButton, 200);

		auto *chnString = Dictionary::createWithContentsOfFile("UI/CHN_String.xml");
		const char *backStr = ((String *)chnString->objectForKey("EndGame_Text"))->getCString();
		Label *backword = Label::create(backStr, "Arial", 32);
		backword->setPosition(Vec2(960, 200));
		addChild(backword, 200);


		Sprite *ZhanjiBackGround;
		ZhanjiBackGround = Sprite::create("UI/FinalBackGround.png");
		ZhanjiBackGround->setPosition(Vec2(origin.x + visibleSize.width*0.5, origin.y + visibleSize.height*0.5));
		ZhanjiBackGround->setScaleY(2.6f);
		ZhanjiBackGround->setScaleX(3.2f);
		ZhanjiBackGround->setOpacity(230);
		addChild(ZhanjiBackGround, 10, 81);

		//胜利图标
		Sprite *WinGame = Sprite::create("UI/victory.png");
		WinGame->setPosition(Vec2(origin.x + visibleSize.width*0.5, origin.y + visibleSize.height*0.75));
		WinGame->setScale(0.6f);
		addChild(WinGame, 10, 82);


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
		Sprite *MyEquipment1 = Sprite::create("UI/kong.png");
		MyEquipment1->setPosition(Vec2(250, 300));
		MyEquipment1->setScale(0.6f);
		addChild(MyEquipment1, 12, 201);

		Sprite *MyEquipment2 = Sprite::create("UI/kong.png");
		MyEquipment2->setPosition(Vec2(350, 300));
		MyEquipment2->setScale(0.6f);
		addChild(MyEquipment2, 12, 202);

		Sprite *MyEquipment3 = Sprite::create("UI/kong.png");
		MyEquipment3->setPosition(Vec2(450, 300));
		MyEquipment3->setScale(0.6f);
		addChild(MyEquipment3, 12, 203);

		Sprite *MyEquipment4 = Sprite::create("UI/kong.png");
		MyEquipment4->setPosition(Vec2(550, 300));
		MyEquipment4->setScale(0.6f);
		addChild(MyEquipment4, 12, 204);

		Sprite *MyEquipment5 = Sprite::create("UI/kong.png");
		MyEquipment5->setPosition(Vec2(650, 300));
		MyEquipment5->setScale(0.6f);
		addChild(MyEquipment5, 12, 205);

		Sprite *MyEquipment6 = Sprite::create("UI/kong.png");
		MyEquipment6->setPosition(Vec2(750, 300));
		MyEquipment6->setScale(0.6f);
		addChild(MyEquipment6, 12, 206);

		//对方空方案
		Sprite *YouEquipment1 = Sprite::create("UI/kong.png");
		YouEquipment1->setPosition(Vec2(1150, 300));
		YouEquipment1->setScale(0.6f);
		addChild(YouEquipment1, 12, 207);

		Sprite *YouEquipment2 = Sprite::create("UI/kong.png");
		YouEquipment2->setPosition(Vec2(1250, 300));
		YouEquipment2->setScale(0.6f);
		addChild(YouEquipment2, 12, 208);

		Sprite *YouEquipment3 = Sprite::create("UI/kong.png");
		YouEquipment3->setPosition(Vec2(1350, 300));
		YouEquipment3->setScale(0.6f);
		addChild(YouEquipment3, 12, 209);

		Sprite *YouEquipment4 = Sprite::create("UI/kong.png");
		YouEquipment4->setPosition(Vec2(1450, 300));
		YouEquipment4->setScale(0.6f);
		addChild(YouEquipment4, 12, 210);

		Sprite *YouEquipment5 = Sprite::create("UI/kong.png");
		YouEquipment5->setPosition(Vec2(1550, 300));
		YouEquipment5->setScale(0.6f);
		addChild(YouEquipment5, 12, 211);

		Sprite *YouEquipment6 = Sprite::create("UI/kong.png");
		YouEquipment6->setPosition(Vec2(1650, 300));
		YouEquipment6->setScale(0.6f);
		addChild(YouEquipment6, 12, 212);

		//设置我方装备可见
		for (int q = 1; q <= MyBuyWeaponNum; ++q)
		{
			Sprite *TMP = (Sprite *)this->getChildByTag(810 + q);
			if (TMP != nullptr)
			{
				TMP->setVisible(true);
			}
		}

		//设置对方装备可见
		for (int q = 1; q <= YouBuyWeaponNum; ++q)
		{

			Sprite *TMP = (Sprite *)this->getChildByTag(820 + q);
			if (TMP != nullptr)
			{
				TMP->setVisible(true);
			}
		}

		//我方金币
		auto MyGoldZhanji = Sprite::create("UI/gold.png");
		MyGoldZhanji->setPosition(Vec2(450, 200));
		MyGoldZhanji->setScale(1.0f);
		addChild(MyGoldZhanji, 14, 85);

		CCString *MygoldCCstringZhanji = CCString::createWithFormat("%d", MyGold);
		std::string MygoldstringZhanji = MygoldCCstringZhanji->getCString();
		Label *MyGoldNumZhanji = Label::createWithTTF(MygoldstringZhanji, "fonts/Marker Felt.ttf", 32);
		MyGoldNumZhanji->setPosition(Vec2(520, 200));
		MyGoldNumZhanji->setScale(1.0f);
		addChild(MyGoldNumZhanji, 14, 86);

		//对方金币
		auto YouGoldZhanji = Sprite::create("UI/gold.png");
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

		auto MeKillsp = Sprite::create("UI/kill.png");
		MeKillsp->setPosition(Vec2(300, 550));
		addChild(MeKillsp, 14, 89);

		CCString *MeKillCCstring = CCString::createWithFormat("%d", MeKill);
		std::string MeKillstring = MeKillCCstring->getCString();
		Label *MeKillNum = Label::createWithTTF(MeKillstring, "fonts/Marker Felt.ttf", 35);
		MeKillNum->setPosition(Vec2(300, 470));
		MeKillNum->setScale(1.0f);
		addChild(MeKillNum, 14, 90);


		//我方死亡
		auto MeDeadsp = Sprite::create("UI/die.png");
		MeDeadsp->setPosition(Vec2(700, 550));
		addChild(MeDeadsp, 14, 91);

		CCString *MeDeadCCstring = CCString::createWithFormat("%d", MeDead);
		std::string MeDeadstring = MeDeadCCstring->getCString();
		Label *MeDeadNum = Label::createWithTTF(MeDeadstring, "fonts/Marker Felt.ttf", 35);
		MeDeadNum->setPosition(Vec2(700, 470));
		MeDeadNum->setScale(1.0f);
		addChild(MeDeadNum, 14, 92);

		//对方击杀

		auto YouKillsp = Sprite::create("UI/kill.png");
		YouKillsp->setPosition(Vec2(1200, 550));
		addChild(YouKillsp, 14, 93);

		CCString *YouKillCCstring = CCString::createWithFormat("%d", YouKill);
		std::string YouKillstring = YouKillCCstring->getCString();
		Label *YouKillNum = Label::createWithTTF(YouKillstring, "fonts/Marker Felt.ttf", 35);
		YouKillNum->setPosition(Vec2(1200, 470));
		YouKillNum->setScale(1.0f);
		addChild(YouKillNum, 14, 94);


		//对方死亡
		auto YouDeadsp = Sprite::create("UI/die.png");
		YouDeadsp->setPosition(Vec2(1600, 550));
		addChild(YouDeadsp, 14, 95);

		CCString *YouDeadCCstring = CCString::createWithFormat("%d", YouDead);
		std::string YouDeadstring = YouDeadCCstring->getCString();
		Label *YouDeadNum = Label::createWithTTF(YouDeadstring, "fonts/Marker Felt.ttf", 35);
		YouDeadNum->setPosition(Vec2(1600, 470));
		YouDeadNum->setScale(1.0f);
		addChild(YouDeadNum, 14, 96);
	}
	if (win == 1)//对方获胜
	{
		//战绩图标不可触发
		Button *tmp1 = (Button *)this->getChildByTag(51);
		tmp1->setTouchEnabled(false);
		//商城图标不可触发
		Button *tmp2 = (Button *)this->getChildByTag(50);
		tmp2->setTouchEnabled(false);

		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//停止一切人物行为（包括小兵野怪）
		stopAllActions();

		//一切监听器不再生效
		/*
		myKeyListener->release();
		listener->release();
		*/
		this->unscheduleAllSelectors();
		_eventDispatcher->removeAllEventListeners();

		//移除详细信息
		for (int b = 0; b <= 13; ++b)
		{
			if (b <= 6) removeChildByTag(50 + b);
			if (b <= 11) removeChildByTag(901 + b);
			removeChildByTag(108 + b);
		}

		//只有返回按钮

		auto pBackButtonItem = MenuItemImage::create("UI/Startbutton.jpg", "UI/StartbuttonP.jpg", CC_CALLBACK_1(MapScene::EnterHelloWorldScene, this));
		pBackButtonItem->setScale(1.5f);
		auto BackButton = Menu::create(pBackButtonItem, NULL);
		BackButton->setPosition(Vec2(960, 200));
		BackButton->setOpacity(200);
		addChild(BackButton, 200);

		auto *chnString = Dictionary::createWithContentsOfFile("UI/CHN_String.xml");
		const char *backStr = ((String *)chnString->objectForKey("EndGame_Text"))->getCString();
		Label *backword = Label::create(backStr, "Arial", 32);
		backword->setPosition(Vec2(960, 200));
		addChild(backword, 200);

		Sprite *ZhanjiBackGround;
		ZhanjiBackGround = Sprite::create("UI/FinalBackGround.png");
		ZhanjiBackGround->setPosition(Vec2(origin.x + visibleSize.width*0.5, origin.y + visibleSize.height*0.5));
		ZhanjiBackGround->setScaleY(2.6f);
		ZhanjiBackGround->setScaleX(3.2f);
		ZhanjiBackGround->setOpacity(230);
		addChild(ZhanjiBackGround, 10, 81);

		//失败图标
		Sprite *WinGame = Sprite::create("UI/defeat.png");
		WinGame->setPosition(Vec2(origin.x + visibleSize.width*0.5, origin.y + visibleSize.height*0.85));
		WinGame->setScale(1.5f);
		addChild(WinGame, 10, 82);


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
		Sprite *MyEquipment1 = Sprite::create("UI/kong.png");
		MyEquipment1->setPosition(Vec2(250, 300));
		MyEquipment1->setScale(0.6f);
		addChild(MyEquipment1, 12, 201);

		Sprite *MyEquipment2 = Sprite::create("UI/kong.png");
		MyEquipment2->setPosition(Vec2(350, 300));
		MyEquipment2->setScale(0.6f);
		addChild(MyEquipment2, 12, 202);

		Sprite *MyEquipment3 = Sprite::create("UI/kong.png");
		MyEquipment3->setPosition(Vec2(450, 300));
		MyEquipment3->setScale(0.6f);
		addChild(MyEquipment3, 12, 203);

		Sprite *MyEquipment4 = Sprite::create("UI/kong.png");
		MyEquipment4->setPosition(Vec2(550, 300));
		MyEquipment4->setScale(0.6f);
		addChild(MyEquipment4, 12, 204);

		Sprite *MyEquipment5 = Sprite::create("UI/kong.png");
		MyEquipment5->setPosition(Vec2(650, 300));
		MyEquipment5->setScale(0.6f);
		addChild(MyEquipment5, 12, 205);

		Sprite *MyEquipment6 = Sprite::create("UI/kong.png");
		MyEquipment6->setPosition(Vec2(750, 300));
		MyEquipment6->setScale(0.6f);
		addChild(MyEquipment6, 12, 206);

		//对方空方案
		Sprite *YouEquipment1 = Sprite::create("UI/kong.png");
		YouEquipment1->setPosition(Vec2(1150, 300));
		YouEquipment1->setScale(0.6f);
		addChild(YouEquipment1, 12, 207);

		Sprite *YouEquipment2 = Sprite::create("UI/kong.png");
		YouEquipment2->setPosition(Vec2(1250, 300));
		YouEquipment2->setScale(0.6f);
		addChild(YouEquipment2, 12, 208);

		Sprite *YouEquipment3 = Sprite::create("UI/kong.png");
		YouEquipment3->setPosition(Vec2(1350, 300));
		YouEquipment3->setScale(0.6f);
		addChild(YouEquipment3, 12, 209);

		Sprite *YouEquipment4 = Sprite::create("UI/kong.png");
		YouEquipment4->setPosition(Vec2(1450, 300));
		YouEquipment4->setScale(0.6f);
		addChild(YouEquipment4, 12, 210);

		Sprite *YouEquipment5 = Sprite::create("UI/kong.png");
		YouEquipment5->setPosition(Vec2(1550, 300));
		YouEquipment5->setScale(0.6f);
		addChild(YouEquipment5, 12, 211);

		Sprite *YouEquipment6 = Sprite::create("UI/kong.png");
		YouEquipment6->setPosition(Vec2(1650, 300));
		YouEquipment6->setScale(0.6f);
		addChild(YouEquipment6, 12, 212);

		//设置我方装备可见
		for (int q = 1; q <= MyBuyWeaponNum; ++q)
		{
			Sprite *TMP = (Sprite *)this->getChildByTag(810 + q);
			if (TMP != nullptr)
			{
				TMP->setVisible(true);
			}
		}

		//设置对方装备可见
		for (int q = 1; q <= YouBuyWeaponNum; ++q)
		{

			Sprite *TMP = (Sprite *)this->getChildByTag(820 + q);
			if (TMP != nullptr)
			{
				TMP->setVisible(true);
			}
		}

		//我方金币
		auto MyGoldZhanji = Sprite::create("UI/gold.png");
		MyGoldZhanji->setPosition(Vec2(450, 200));
		MyGoldZhanji->setScale(1.0f);
		addChild(MyGoldZhanji, 14, 85);

		CCString *MygoldCCstringZhanji = CCString::createWithFormat("%d", MyGold);
		std::string MygoldstringZhanji = MygoldCCstringZhanji->getCString();
		Label *MyGoldNumZhanji = Label::createWithTTF(MygoldstringZhanji, "fonts/Marker Felt.ttf", 32);
		MyGoldNumZhanji->setPosition(Vec2(520, 200));
		MyGoldNumZhanji->setScale(1.0f);
		addChild(MyGoldNumZhanji, 14, 86);

		//对方金币
		auto YouGoldZhanji = Sprite::create("UI/gold.png");
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

		auto MeKillsp = Sprite::create("UI/kill.png");
		MeKillsp->setPosition(Vec2(300, 550));
		addChild(MeKillsp, 14, 89);

		CCString *MeKillCCstring = CCString::createWithFormat("%d", MeKill);
		std::string MeKillstring = MeKillCCstring->getCString();
		Label *MeKillNum = Label::createWithTTF(MeKillstring, "fonts/Marker Felt.ttf", 35);
		MeKillNum->setPosition(Vec2(300, 470));
		MeKillNum->setScale(1.0f);
		addChild(MeKillNum, 14, 90);


		//我方死亡
		auto MeDeadsp = Sprite::create("UI/die.png");
		MeDeadsp->setPosition(Vec2(700, 550));
		addChild(MeDeadsp, 14, 91);

		CCString *MeDeadCCstring = CCString::createWithFormat("%d", MeDead);
		std::string MeDeadstring = MeDeadCCstring->getCString();
		Label *MeDeadNum = Label::createWithTTF(MeDeadstring, "fonts/Marker Felt.ttf", 35);
		MeDeadNum->setPosition(Vec2(700, 470));
		MeDeadNum->setScale(1.0f);
		addChild(MeDeadNum, 14, 92);

		//对方击杀

		auto YouKillsp = Sprite::create("UI/kill.png");
		YouKillsp->setPosition(Vec2(1200, 550));
		addChild(YouKillsp, 14, 93);

		CCString *YouKillCCstring = CCString::createWithFormat("%d", YouKill);
		std::string YouKillstring = YouKillCCstring->getCString();
		Label *YouKillNum = Label::createWithTTF(YouKillstring, "fonts/Marker Felt.ttf", 35);
		YouKillNum->setPosition(Vec2(1200, 470));
		YouKillNum->setScale(1.0f);
		addChild(YouKillNum, 14, 94);


		//对方死亡
		auto YouDeadsp = Sprite::create("UI/die.png");
		YouDeadsp->setPosition(Vec2(1600, 550));
		addChild(YouDeadsp, 14, 95);

		CCString *YouDeadCCstring = CCString::createWithFormat("%d", YouDead);
		std::string YouDeadstring = YouDeadCCstring->getCString();
		Label *YouDeadNum = Label::createWithTTF(YouDeadstring, "fonts/Marker Felt.ttf", 35);
		YouDeadNum->setPosition(Vec2(1600, 470));
		YouDeadNum->setScale(1.0f);
		addChild(YouDeadNum, 14, 96);
	}
}
