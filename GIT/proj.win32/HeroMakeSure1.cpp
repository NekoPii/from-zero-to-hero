#include "proj.win32/MapScene.h"
#include "proj.win32/HeroChoose.h"
#include "proj.win32/HeroMakeSure1.h"
#include "GLOBAL.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
USING_NS_CC_EXT;


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

	//切换界面背景
	auto MakeSure1BackGround = Sprite::create("ChooseBackGround.jpg");
	MakeSure1BackGround->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	MakeSure1BackGround->setScale(3.0f);
	addChild(MakeSure1BackGround, 0);


	//人物展示1
	/*auto Hero_1Picture = Sprite::create("Startbutton.jpg");
	Hero_1Picture->setPosition(Vec2(origin.x + visibleSize.width / 2.0, origin.y + visibleSize.height / 2.0));
	Hero_1Picture->setScale(2.0f);
	addChild(Hero_1Picture,1);*/


	initHero1Stand();
	createAnimateStand1(7);
	

	//技能1
	/*
	Button *test1 = Button::create();
	test1->loadTextures("Exitbutton.jpg", "Exitbutton.jpg", "");
	test1->setPosition(Vec2(400, 500));
	test1->addTouchEventListener(CC_CALLBACK_2(HeroMakeSure1::testcall, this));
	addChild(test1,2);*/


	//继续
	auto labelContinue1 = Label::createWithTTF("Continue", "fonts/Marker Felt.ttf", 32);
	auto pContinueButtonItem1 = MenuItemImage::create("Startbutton.jpg", "Startbutton.jpg", CC_CALLBACK_1(HeroMakeSure1::EnterMapScene, this));
	auto ContinueButton1 = Menu::create(pContinueButtonItem1, NULL);
	ContinueButton1->setPosition(Vec2(origin.x + 2.0 *visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelContinue1->getContentSize().height));
	labelContinue1->setPosition(Vec2(origin.x + 2.0*visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelContinue1->getContentSize().height));
	addChild(labelContinue1, 2);
	addChild(ContinueButton1, 1);

	//重选
	auto labelReturn1 = Label::createWithTTF("ReSelect", "fonts/Marker Felt.ttf", 32);
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
	HeroID = 1;
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f,MapScene::createScene()));
}

void HeroMakeSure1::ReturnHeroChoose(Ref *pSenderReturn)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f,HeroChoose::createScene()));
}

/*/Test
void HeroMakeSure1::testcall(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		auto testSp1 = Sprite::create("Exitbutton.jpg");
		addChild(testSp1, 5);
		testSp1->setPosition(Vec2(600, 500));
	}
}*/


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
