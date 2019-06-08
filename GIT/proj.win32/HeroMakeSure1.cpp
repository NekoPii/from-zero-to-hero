#include "proj.win32/MapScene.h"
#include "proj.win32/HeroChoose.h"
#include "proj.win32/HeroMakeSure1.h"
#include "GLOBAL.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;

Scene *HeroMakeSure1::createScene()
{
	Scene *scene = Scene::create();
	HeroMakeSure1 *layer = HeroMakeSure1::create();
	scene->addChild(layer);
	return scene;
}

bool HeroMakeSure1::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto *chnString = Dictionary::createWithContentsOfFile("CHN_String.xml");
	const char *ContinueGameStr = ((String *)chnString->objectForKey("Continue_Text"))->getCString();
	const char *BackGameStr = ((String *)chnString->objectForKey("Back_Text"))->getCString();

	//切换界面背景
	auto MakeSure1BackGround = Sprite::create("ChooseBackGround.jpg");
	MakeSure1BackGround->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	MakeSure1BackGround->setScale(3.0f);
	addChild(MakeSure1BackGround, 0);

	initHero1Stand();
	createAnimateStand1(7);
	

	//继续
	auto labelContinue1 = Label::create(ContinueGameStr, "Arial", 32);
	auto pContinueButtonItem1 = MenuItemImage::create("Startbutton.jpg", "Startbutton.jpg", CC_CALLBACK_1(HeroMakeSure1::EnterMapScene, this));
	auto ContinueButton1 = Menu::create(pContinueButtonItem1, NULL);
	ContinueButton1->setPosition(Vec2(origin.x + 2.0 *visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelContinue1->getContentSize().height));
	labelContinue1->setPosition(Vec2(origin.x + 2.0*visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelContinue1->getContentSize().height));
	addChild(labelContinue1, 2);
	addChild(ContinueButton1, 1);

	//重选
	auto labelReturn1 = Label::create(BackGameStr, "Arial", 32);
	auto pReturnButtonItem1 = MenuItemImage::create("Exitbutton.jpg", "Exitbutton.jpg", CC_CALLBACK_1(HeroMakeSure1::ReturnHeroChoose, this));
	auto ReturnButton1 = Menu::create(pReturnButtonItem1, NULL);
	ReturnButton1->setPosition(Vec2(origin.x + visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelReturn1->getContentSize().height));
	labelReturn1->setPosition(Vec2(origin.x + visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelReturn1->getContentSize().height));
	addChild(labelReturn1, 2);
	addChild(ReturnButton1, 1);

	return true;
}

void HeroMakeSure1::EnterMapScene(Ref *pSenderEnter)
{
	SimpleAudioEngine::getInstance()->playEffect("Touch.wav");
	MyHeroID = 1;
	YourHeroID = 1;
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f,MapScene::createScene()));
}

void HeroMakeSure1::ReturnHeroChoose(Ref *pSenderReturn)
{
	SimpleAudioEngine::getInstance()->playEffect("Touch.wav");
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f,HeroChoose::createScene()));
}

void HeroMakeSure1::initHero1Stand()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto heroSpriteStand1 = Sprite::create("hero_1_stand5.png");
	heroSpriteStand1->setScale(4.0f);
	heroSpriteStand1->setPosition(Vec2(origin.x + visibleSize.width / 3.0, origin.y + visibleSize.height / 2.0));
	addChild(heroSpriteStand1,1);
	heroSpriteStand1->runAction(this->createAnimateStand1(7));

}

Animate* HeroMakeSure1::createAnimateStand1(int num)
{
	auto* frameCacheStand1 = SpriteFrameCache::getInstance();
	frameCacheStand1->addSpriteFramesWithFile("hero_1_stand5.plist", "hero_1_stand5.png");
	Vector <SpriteFrame*> frameArrayStand1;

	for (int i = 1; i <= num; ++i)
	{
		auto* frameStand1 = frameCacheStand1->getSpriteFrameByName(String::createWithFormat("hero_1_stand5%d.png",i)->getCString());
		frameArrayStand1.pushBack(frameStand1);
	}
	Animation* animationStand1 = Animation::createWithSpriteFrames(frameArrayStand1);
	animationStand1->setLoops(-1);
	animationStand1->setDelayPerUnit(0.1f);
	return Animate::create(animationStand1);
}
