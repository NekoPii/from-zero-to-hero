#include "proj.win32/MapScene.h"
#include "proj.win32/HeroMakeSure1.h"
#include "proj.win32/HeroMakeSure2.h"
#include "proj.win32/HeroMakeSure3.h"
#include "HelloWorldScene.h"
#include "GLOBAL.h"


USING_NS_CC;


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

	//µØÍ¼
	auto MAP = Sprite::create("map.jpg");
	MAP->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

	MAP->setScale(3.0f);
	addChild(MAP, 0);

	//²âÊÔ°´Å¥
	auto pBackButtonItem = MenuItemImage::create("Backbutton.png", "Backbutton.png", CC_CALLBACK_1(MapScene::EnterHelloWorldScene, this));
	auto BackButton = Menu::create(pBackButtonItem,NULL);
	BackButton->setPosition(Vec2(80,visibleSize.height-30));
	addChild(BackButton, 1);

	hero = new heroPrint();
	hero->initHeroSprite(8, Vec2(860, 540));
	addChild(hero);
	
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}
bool MapScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	hero->getDirection(hero->currentPosition, touch->getLocation());
	hero->heroMoveTo(touch->getLocation());
	return false;
}


void MapScene::EnterHelloWorldScene(Ref *pSenderBack)
{
	Director::getInstance()->replaceScene((HelloWorld::createScene()));
}