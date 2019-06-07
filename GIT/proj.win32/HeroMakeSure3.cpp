#include "proj.win32/MapScene.h"
#include "proj.win32/HeroChoose.h"
#include "proj.win32/HeroMakeSure3.h"
#include "GLOBAL.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
USING_NS_CC_EXT;


Scene *HeroMakeSure3::createScene()
{
	Scene *scene = Scene::create();
	HeroMakeSure3 *layer = HeroMakeSure3::create();
	scene->addChild(layer);
	return scene;
}

bool HeroMakeSure3::init()
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
	auto MakeSure3BackGround = Sprite::create("ChooseBackGround.jpg");
	MakeSure3BackGround->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	MakeSure3BackGround->setScale(3.0f);
	addChild(MakeSure3BackGround, 0);

	//人物图片3
	/*
	auto Hero_3Picture = Sprite::create("Startbutton.jpg");
	Hero_3Picture->setPosition(Vec2(origin.x + visibleSize.width / 2.0, origin.y + visibleSize.height / 2.0));
	Hero_3Picture->setScale(2.0f);
	addChild(Hero_3Picture,1);*/

	initHero3Stand();
	createAnimateStand3(7);

	//继续
	auto labelContinue3 = Label::create(ContinueGameStr, "Arial", 32);
	auto pContinueButtonItem3 = MenuItemImage::create("Startbutton.jpg", "Startbutton.jpg", CC_CALLBACK_1(HeroMakeSure3::EnterMapScene, this));
	auto ContinueButton3 = Menu::create(pContinueButtonItem3, NULL);
	ContinueButton3->setPosition(Vec2(origin.x + 2.0 *visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelContinue3->getContentSize().height));
	labelContinue3->setPosition(Vec2(origin.x + 2.0*visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelContinue3->getContentSize().height));
	addChild(labelContinue3, 2);
	addChild(ContinueButton3, 1);

	//重选
	auto labelReturn3 = Label::create(BackGameStr, "Arial", 32);
	auto pReturnButtonItem3 = MenuItemImage::create("Exitbutton.jpg", "Exitbutton.jpg", CC_CALLBACK_1(HeroMakeSure3::ReturnHeroChoose, this));
	auto ReturnButton3 = Menu::create(pReturnButtonItem3, NULL);
	ReturnButton3->setPosition(Vec2(origin.x + visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelReturn3->getContentSize().height));
	labelReturn3->setPosition(Vec2(origin.x + visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelReturn3->getContentSize().height));
	addChild(labelReturn3, 2);
	addChild(ReturnButton3, 1);
	
	return true;
}

void HeroMakeSure3::EnterMapScene(Ref *pSenderEnter)
{

	MyHeroID = 3;
	YourHeroID = 3;
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f,MapScene::createScene()));
}

void HeroMakeSure3::ReturnHeroChoose(Ref *pSenderReturn)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f,HeroChoose::createScene()));
}

void HeroMakeSure3::initHero3Stand()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto heroSpriteStand3 = Sprite::create("hero_3_stand55.png");
	heroSpriteStand3->setScale(4.0f);
	heroSpriteStand3->setPosition(Vec2(origin.x + visibleSize.width / 3.0, origin.y + visibleSize.height / 2.0));
	addChild(heroSpriteStand3, 1);
	heroSpriteStand3->runAction(this->createAnimateStand3(7));

}

Animate* HeroMakeSure3::createAnimateStand3(int num)
{
	auto* frameCacheStand3 = SpriteFrameCache::getInstance();
	frameCacheStand3->addSpriteFramesWithFile("hero_3_stand5.plist", "hero_3_stand5.png");
	Vector <SpriteFrame*> frameArrayStand3;

	for (int i = 1; i <= num; ++i)
	{
		auto* frameStand3 = frameCacheStand3->getSpriteFrameByName(String::createWithFormat("hero_3_stand5%d.png", i)->getCString());
		frameArrayStand3.pushBack(frameStand3);
	}
	Animation* animationStand3 = Animation::createWithSpriteFrames(frameArrayStand3);
	animationStand3->setLoops(-1);
	animationStand3->setDelayPerUnit(0.1f);
	return Animate::create(animationStand3);
}