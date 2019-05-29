#include "proj.win32/MapScene.h"
#include "proj.win32/HeroMakeSure1.h"
#include "proj.win32/HeroMakeSure2.h"
#include "proj.win32/HeroMakeSure3.h"
#include "HelloWorldScene.h"
#include "GLOBAL.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "deprecated/CCString.h"



USING_NS_CC;
USING_NS_CC_EXT;


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
	MAP->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	MAP->setScale(3.0f);
	auto * layer1 = Layer::create();
	layer1->setAnchorPoint(Vec2(0, 0));
	layer1->setPosition((Vec2(0, 0)));
	layer1->addChild(MAP);
	this->addChild(layer1, 0, 1);

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
	GoldNum->setPosition(Vec2(50,460));
	addChild(GoldNum, 9,55);



	hero = new heroPrint();
	hero->initHeroSprite(8, Vec2(860, 540));
	addChild(hero);
	
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto myKeyListener = EventListenerKeyboard::create();	
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	myKeyListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	dispatcher->addEventListenerWithSceneGraphPriority(myKeyListener, this);
	return true;
}
bool MapScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	hero->getDirection(hero->currentPosition, touch->getLocation());
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

void MapScene::EnterHelloWorldScene(Ref *pSenderBack)
{
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
		ShopBackButton->setPosition(Vec2(1660, 900));
		ShopBackButton->addTouchEventListener(CC_CALLBACK_2(MapScene::ShopBack, this));
		ShopBackButton->setScale(0.7f);
		addChild(ShopBackButton,14,66);

		Sprite *ShopBackGround;
		ShopBackGround = Sprite::create("Shopbackground.png");
		ShopBackGround->setPosition(Vec2(960,550));
		ShopBackGround->setScale(0.85f);
		ShopBackGround->setOpacity(230);
		addChild(ShopBackGround, 10, 60);

	
		Button * Weapon1=Button::create();
		Weapon1->loadTextures("weapon1.png", "weapon1.png", "");
		Weapon1->setPosition(Vec2(550, 750));
		Weapon1->setScale(0.8f);
		Weapon1->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy1, this));
		addChild(Weapon1, 12, 101);


		Button * Weapon2=Button::create();
		Weapon2->loadTextures("weapon2.png", "weapon2.png", "");
		Weapon2->setPosition(Vec2(1000, 750));
		Weapon2->setScale(0.8f);
		Weapon2->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy2, this));
		addChild(Weapon2, 12, 102);

		Button * Weapon3=Button::create();
		Weapon3->loadTextures("weapon3.png", "weapon3.png", "");
		Weapon3->setPosition(Vec2(550, 550));
		Weapon3->setScale(0.8f);
		Weapon3->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy3, this));
		addChild(Weapon3, 12, 103);

		Button * Weapon4=Button::create();
		Weapon4->loadTextures("weapon4.png", "weapon4.png", "");
		Weapon4->setPosition(Vec2(1000, 550));
		Weapon4->setScale(0.8f);
		Weapon4->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy4, this));
		addChild(Weapon4, 12, 104);
			
		Button * Weapon5=Button::create();
		Weapon5->loadTextures("weapon5.png", "weapon5.png", "");
		Weapon5->setPosition(Vec2(550, 350));
		Weapon5->setScale(0.8f);
		Weapon5->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy5, this));
		addChild(Weapon5, 12, 105);

		Button * Weapon6=Button::create();
		Weapon6->loadTextures("weapon6.png", "weapon6.png", "");
		Weapon6->setPosition(Vec2(1000, 350));
		Weapon6->setScale(0.8f);
		Weapon6->addTouchEventListener(CC_CALLBACK_2(MapScene::Shopbuy6, this));
		addChild(Weapon6, 12, 106);
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
		Weapon1Detail->setPosition(Vec2(1650,591));
		Weapon1Detail->setScale(0.7f);
		addChild(Weapon1Detail, 13,1011);

		if (Gold >= 1740)
		{
			Button *BuyButton11=Button::create();
			BuyButton11->loadTextures("buy1.png","buy1.png","");
			BuyButton11->setPosition(Vec2(1650,180));
			BuyButton11->setScale(1.0f);
			BuyButton11->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit1, this));
			addChild(BuyButton11, 14, 1012);
		}

		if (Gold < 1740)
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

		if (Gold >= 2140)
		{
			Button *BuyButton21=Button::create();
			BuyButton21->loadTextures("buy1.png", "buy1.png", "");
			BuyButton21->setPosition(Vec2(1650, 180));
			BuyButton21->setScale(1.0f);
			BuyButton21->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit2, this));
			addChild(BuyButton21, 14, 1022);
		}

		if (Gold < 2140)
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
		Weapon3Detail->setPosition(Vec2(1650,590));
		Weapon3Detail->setScale(0.7f);
		addChild(Weapon3Detail, 13, 1031);


		if (Gold >= 2300)
		{
			Button *BuyButton31=Button::create();
			BuyButton31->loadTextures("buy1.png", "buy1.png", "");
			BuyButton31->setPosition(Vec2(1650, 180));
			BuyButton31->setScale(1.0f);
			BuyButton31->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit3, this));
			addChild(BuyButton31, 14, 1032);
		}

		if (Gold < 2300)
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
		Weapon4Detail->setPosition(Vec2(1650,590));
		Weapon4Detail->setScale(0.7f);
		addChild(Weapon4Detail, 13, 1041);


		if (Gold >= 2990)
		{
			Button *BuyButton41=Button::create();
			BuyButton41->loadTextures("buy1.png", "buy1.png", "");
			BuyButton41->setPosition(Vec2(1650, 180));
			BuyButton41->setScale(1.0f);
			BuyButton41->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit4, this));
			addChild(BuyButton41, 14, 1042);
		}

		if (Gold < 2990)
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
		Weapon5Detail->setPosition(Vec2(1650,590));
		Weapon5Detail->setScale(0.7f);
		addChild(Weapon5Detail, 13, 1051);


		if (Gold >= 900)
		{
			Button *BuyButton51=Button::create();
			BuyButton51->loadTextures("buy1.png", "buy1.png", "");
			BuyButton51->setPosition(Vec2(1650, 180));
			BuyButton51->setScale(1.0f);
			BuyButton51->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit5, this));
			addChild(BuyButton51, 14, 1052);
		}

		if (Gold < 900)
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


		if (Gold >= 710)
		{
			Button *BuyButton61=Button::create();
			BuyButton61->loadTextures("buy1.png", "buy1.png", "");
			BuyButton61->setPosition(Vec2(1650, 180));
			BuyButton61->setScale(1.0f);
			BuyButton61->addTouchEventListener(CC_CALLBACK_2(MapScene::Buyit6, this));
			addChild(BuyButton61, 14, 1062);
		}

		if (Gold < 710)
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
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		Gold -= 1740;
		removeChildByTag(55);
		CCString *goldCCstring = CCString::createWithFormat("%d", Gold);
		std::string goldstring = goldCCstring->getCString();
		Label *GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
		GoldNum->setPosition(Vec2(50, 460));
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
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		Gold -= 2140;
		removeChildByTag(55);
		CCString *goldCCstring = CCString::createWithFormat("%d", Gold);
		std::string goldstring = goldCCstring->getCString();
		Label *GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
		GoldNum->setPosition(Vec2(50, 460));
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
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		Gold -= 2300;
		removeChildByTag(55);
		CCString *goldCCstring = CCString::createWithFormat("%d", Gold);
		std::string goldstring = goldCCstring->getCString();
		Label *GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
		GoldNum->setPosition(Vec2(50, 460));
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
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		Gold -= 2990;
		removeChildByTag(55);
		CCString *goldCCstring = CCString::createWithFormat("%d", Gold);
		std::string goldstring = goldCCstring->getCString();
		Label *GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
		GoldNum->setPosition(Vec2(50, 460));
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
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		Gold -= 900;
		removeChildByTag(55);
		CCString *goldCCstring = CCString::createWithFormat("%d", Gold);
		std::string goldstring = goldCCstring->getCString();
		Label *GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
		GoldNum->setPosition(Vec2(50, 460));
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
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		Gold -= 710;
		removeChildByTag(55);
		CCString *goldCCstring = CCString::createWithFormat("%d", Gold);
		std::string goldstring = goldCCstring->getCString();
		Label *GoldNum = Label::createWithTTF(goldstring, "fonts/Marker Felt.ttf", 32);
		GoldNum->setPosition(Vec2(50, 460));
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
		removeChildByTag(60);
		removeChildByTag(66);
		removeChildByTag(101);
		removeChildByTag(102);
		removeChildByTag(103);
		removeChildByTag(104);
		removeChildByTag(105);
		removeChildByTag(106);
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
		Button *tmp = (Button *)this->getChildByTag(50);
		tmp->setTouchEnabled(true);
	}
}
//使用removebytag至关重要，byTag还可以提供访问
