#include "HelloWorldScene.h"
#include "proj.win32/HeroChoose.h"
#include "GLOBAL.h"

std::string HeroID = "0";

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

	auto *chnString = Dictionary::createWithContentsOfFile("CHN_String.xml");
	const char *StartGameStr = ((String *)chnString->objectForKey("StartGame_Text"))->getCString();
	const char *EndGameStr = ((String *)chnString->objectForKey("EndGame_Text"))->getCString();

	//StartButton
    auto labelStart = Label::create(StartGameStr, "Arial", 33);
	auto pStartButtonItem = MenuItemImage::create("Startbutton.jpg", "Startbutton.jpg",CC_CALLBACK_1(HelloWorld::EnterHeroChooseScene, this));
	auto StartButton = Menu::create(pStartButtonItem, NULL);
	StartButton->setPosition(Vec2(origin.x + 5.0*visibleSize.width / 6.0, origin.y + visibleSize.height /5.5));
	labelStart->setPosition(Vec2(origin.x + 2.2*visibleSize.width / 3.0, origin.y + visibleSize.height / 12));
	StartButton->setScale(1.2f);
	addChild(labelStart,2);
	addChild(StartButton,1);

	//ExitButton
	auto labelExit = Label::create(EndGameStr, "Arial", 33);
	auto pExitButtonItem = MenuItemImage::create("Exitbutton.jpg", "Exitbutton.jpg",CC_CALLBACK_1(HelloWorld::CloseGame, this));
	auto ExitButton = Menu::create(pExitButtonItem, NULL);
	ExitButton->setPosition(Vec2(origin.x + visibleSize.width / 3.0, origin.y + visibleSize.height / 5.5));
	labelExit->setPosition(Vec2(origin.x + visibleSize.width / 4.3, origin.y + visibleSize.height / 12));
	ExitButton->setScale(1.2f);
	addChild(labelExit,2);
	addChild(ExitButton,1);

	//BackGround
    auto spriteBackGround = Sprite::create("WZRY_begin.jpg");
    spriteBackGround->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	spriteBackGround->setScale(1.0f);
    addChild(spriteBackGround, 0);

    return true;
}


//跳转到HeroChoose
void HelloWorld::EnterHeroChooseScene(Ref *pSenderEnter)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f,HeroChoose::createScene()));
}

//关闭游戏
void HelloWorld::CloseGame(cocos2d::Ref* pSenderClose)
{
	Director::getInstance()->end();
}
