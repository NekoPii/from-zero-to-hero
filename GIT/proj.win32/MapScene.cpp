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

	//地图
	auto MAP = Sprite::create("map.jpg");
	MAP->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	MAP->setScale(3.0f);
	auto * layer1 = Layer::create();
	layer1->setAnchorPoint(Vec2(0, 0));
	layer1->setPosition((Vec2(0, 0)));
	layer1->addChild(MAP);
	this->addChild(layer1, 0, 1);

	//测试按钮
	auto pBackButtonItem = MenuItemImage::create("Backbutton.png", "Backbutton.png", CC_CALLBACK_1(MapScene::EnterHelloWorldScene, this));
	auto BackButton = Menu::create(pBackButtonItem,NULL);
	BackButton->setPosition(Vec2(80,visibleSize.height-30));
	addChild(BackButton, 1);

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
