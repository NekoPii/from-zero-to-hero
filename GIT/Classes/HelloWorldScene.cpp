#include "HelloWorldScene.h"
#include "proj.win32/MapScene.h"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
	auto scene = Scene::create();
	auto layer = HelloWorld::create();
	scene->addChild(layer);
	return scene;

}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//StartButton
    auto labelStart = Label::createWithTTF("START", "fonts/Marker Felt.ttf", 24);
	auto pStartButtonItem = MenuItemImage::create("Startbutton.jpg", "Startbutton.jpg",CC_CALLBACK_1(HelloWorld::EnterMapScene, this));
	auto StartButton = Menu::create(pStartButtonItem, NULL);
	StartButton->setPosition(Vec2(origin.x + 2.0*visibleSize.width / 3.0, origin.y + visibleSize.height / 4.0 - labelStart->getContentSize().height));
	labelStart->setPosition(Vec2(origin.x + 2.0*visibleSize.width / 3.0, origin.y + visibleSize.height / 4.0 - labelStart->getContentSize().height));
	addChild(labelStart,2);
	addChild(StartButton,1);

	//ExitButton
	auto labelExit = Label::createWithTTF("EXIT", "fonts/Marker Felt.ttf", 24);
	auto pExitButtonItem = MenuItemImage::create("Exitbutton.jpg", "Exitbutton.jpg",CC_CALLBACK_1(HelloWorld::CloseGame, this));
	auto ExitButton = Menu::create(pExitButtonItem, NULL);
	ExitButton->setPosition(Vec2(origin.x + visibleSize.width / 3.0, origin.y + visibleSize.height / 4.0 - labelExit->getContentSize().height));
	labelExit->setPosition(Vec2(origin.x + visibleSize.width / 3.0, origin.y + visibleSize.height / 4.0 - labelExit->getContentSize().height));
	addChild(labelExit,2);
	addChild(ExitButton,1);

	//BackGround
    auto spriteBackGround = Sprite::create("WZRY_begin.jpg");
    spriteBackGround->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	spriteBackGround->setScale(1.0f);
    addChild(spriteBackGround, 0);

    return true;
}


//跳转到MAP
void HelloWorld::EnterMapScene(Ref *pSender)
{
	Director::getInstance()->replaceScene(TransitionRotoZoom::create(1.0f,MapScene::createScene()));
}

//关闭游戏
void HelloWorld::CloseGame(cocos2d::Ref* pSenderClose)
{
	Director::getInstance()->end();
}
