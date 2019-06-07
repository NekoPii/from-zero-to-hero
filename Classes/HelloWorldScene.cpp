#include "HelloWorldScene.h"
#include "proj.win32/HeroChoose.h"
#include "GLOBAL.h"

int HeroID = 0;
int Gold = 10000;
int BuyWeaponNum = 0;

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
	Cur += 20.0f;
	Loading->setPercentage(Cur);

	if (Cur <= 100)
	{
		auto tmpstr = String::createWithFormat("%.2f%%", Cur);
		PEC->setString(tmpstr->getCString());
	}
	else
	{
		unscheduleUpdate();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, HeroChoose::createScene()));
	}
}

//跳转到HeroChoose
void HelloWorld::EnterHeroChooseScene(Ref *pSenderEnter)
{
	removeChildByTag(20);
	removeChildByTag(21);
	removeChildByTag(22);
	removeChildByTag(23);
	CCTextureCache::sharedTextureCache()->addImageAsync("weapon1.png",NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("weapon2.png", NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("weapon3.png", NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("weapon4.png", NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("weapon5.png", NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("weapon6.png", NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("weapon1detail.png", NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("weapon1detai2.png", NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("weapon1detai3.png", NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("weapon1detai4.png", NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("weapon1detai5.png", NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("weapon1detai6.png", NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("ChooseBackGround.png", NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("Exitbutton.jpg", NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("Startbutton.jpg", NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("Shopbackground.png", NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("plan.png", NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("little.png", NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("shop.png", NULL);
	CCTextureCache::sharedTextureCache()->addImageAsync("map.png", NULL);


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
	Director::getInstance()->end();
}
