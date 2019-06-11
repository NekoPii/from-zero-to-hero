#include "HelloWorldScene.h"
#include "proj.win32/HeroChoose.h"
#include "GLOBAL.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"


int MyHeroID = 0;
int YourHeroID = 0;
int MyGold = 10000;
int YourGold = 10000;
int MyBuyWeaponNum = 0;
int YouBuyWeaponNum = 0;

USING_NS_CC;
using namespace CocosDenshion;


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

	SimpleAudioEngine::getInstance()->preloadEffect("Touch.wav");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("BGM.wav");

	//StartButton
    auto labelStart = Label::create(StartGameStr, "Arial", 33);
	auto pStartButtonItem = MenuItemImage::create("Startbutton.jpg", "Startbutton.jpg",CC_CALLBACK_1(HelloWorld::EnterHeroChooseScene, this));
	auto StartButton = Menu::create(pStartButtonItem, NULL);
	StartButton->setPosition(Vec2(origin.x + 5.0*visibleSize.width / 6.0, origin.y + visibleSize.height /5.5));
	labelStart->setPosition(Vec2(origin.x + 2.2*visibleSize.width / 3.0, origin.y + visibleSize.height / 12));
	StartButton->setScale(1.2f);
	addChild(labelStart,2,20);
	addChild(StartButton,1,21);

	//ExitButton
	auto labelExit = Label::create(EndGameStr, "Arial", 33);
	auto pExitButtonItem = MenuItemImage::create("Exitbutton.jpg", "Exitbutton.jpg",CC_CALLBACK_1(HelloWorld::CloseGame, this));
	auto ExitButton = Menu::create(pExitButtonItem, NULL);
	ExitButton->setPosition(Vec2(origin.x + visibleSize.width / 3.0, origin.y + visibleSize.height / 5.5));
	labelExit->setPosition(Vec2(origin.x + visibleSize.width / 4.3, origin.y + visibleSize.height / 12));
	ExitButton->setScale(1.2f);
	addChild(labelExit,2,22);
	addChild(ExitButton,1,23);

	//BackGround
    auto spriteBackGround = Sprite::create("WZRY_begin.jpg");
    spriteBackGround->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	spriteBackGround->setScale(1.0f);
    addChild(spriteBackGround, 0);


    return true;
}

void HelloWorld::update(float dt)
{
	float Cur = Loading->getPercentage();
	Cur += 2.0f;
	Loading->setPercentage(Cur);

	if (Cur <= 100)
	{
		auto tmpstr = String::createWithFormat("%.0f%%", Cur);
		PEC->setString(tmpstr->getCString());
	}
	else
	{
		unscheduleUpdate();
		Sleep(1500);
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, HeroChoose::createScene()));
	}
}

//跳转到HeroChoose
void HelloWorld::EnterHeroChooseScene(Ref *pSenderEnter)
{
	SimpleAudioEngine::getInstance()->playEffect("Touch.wav");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("BGM.wav", true);

	removeChildByTag(20);
	removeChildByTag(21);
	removeChildByTag(22);
	removeChildByTag(23);
	Director::getInstance()->getTextureCache()->addImageAsync("weapon1.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("weapon2.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("weapon3.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("weapon4.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("weapon5.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("weapon6.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("weapon1detail.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("weapon2detail.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("weapon3detail.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("weapon4detail.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("weapon5detail.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("weapon6detail.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("ChooseBackGround.jpg", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("Exitbutton.jpg", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("Startbutton.jpg", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("Shopbackground.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("plan.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("little.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("map.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("shop.png", NULL);//20
	Director::getInstance()->getTextureCache()->addImageAsync("buy1.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("buy2.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("kong.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("Hero1_Avatar.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("Hero2_Avatar.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("Hero3_Avatar.png", NULL);
	Director::getInstance()->getTextureCache()->addImageAsync("BloodLine.png", NULL);
	SimpleAudioEngine::getInstance()->preloadEffect("GOLD.wav");


	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto progressSprite = Sprite::create("Progress.png");
	Loading = ProgressTimer::create(progressSprite);
	Loading->setType(kCCProgressTimerTypeBar);
	Loading->setPosition(Vec2(origin.x + visibleSize.width*0.5, origin.y + visibleSize.height*0.10));
	Loading->setScale(0.7f);
	Loading->setMidpoint(Vec2(0, 0));
	Loading->setBarChangeRate(Vec2(1.0, 0));
	Loading->setPercentage(0);
	addChild(Loading, 1);
	PEC = CCLabelTTF::create("0", "fonts / Marker Felt", 32);
	PEC->setColor(ccc3(0,0,0));
	PEC->setPosition(Vec2(origin.x + visibleSize.width*0.5, origin.y + visibleSize.height*0.06));
	addChild(PEC, 1);
	scheduleUpdate();
} 

//关闭游戏
void HelloWorld::CloseGame(cocos2d::Ref* pSenderClose)
{
	SimpleAudioEngine::getInstance()->playEffect("Touch.wav");
	Director::getInstance()->end();
}

