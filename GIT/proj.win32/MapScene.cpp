#include "proj.win32/MapScene.h"
#include "HelloWorldScene.h"

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
	auto MAP = Sprite::create("HelloWorld.png");
	MAP->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	MAP->setScale(1.0f);
	addChild(MAP, 0);

	//²âÊÔ°´Å¥
	auto labelBack = Label::createWithTTF("BACK", "fonts/Marker Felt.ttf", 24);
	auto pBackButtonItem = MenuItemImage::create("Exitbutton.jpg", "Exitbutton.jpg", CC_CALLBACK_1(MapScene::EnterHelloWorldScene, this));
	auto BackButton = Menu::create(pBackButtonItem,NULL);
	BackButton->setPosition(Vec2(origin.x + visibleSize.width /2.0, origin.y + visibleSize.height / 4.0-labelBack->getContentSize().height));
	labelBack->setPosition(Vec2(origin.x + visibleSize.width / 2.0, origin.y + visibleSize.height / 4.0 - labelBack->getContentSize().height));
	addChild(BackButton, 1);
	addChild(labelBack, 2);

	return true;
}

void MapScene::EnterHelloWorldScene(Ref *pSenderBack)
{
	Director::getInstance()->replaceScene(TransitionRotoZoom::create(1.0f,HelloWorld::createScene()));
}