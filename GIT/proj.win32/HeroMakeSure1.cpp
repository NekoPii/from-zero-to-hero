#include "proj.win32/MapScene.h"
#include "proj.win32/HeroChoose.h"
#include "proj.win32/HeroMakeSure1.h"
#include "GLOBAL.h"


USING_NS_CC;


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


	//人物图片1
	auto Hero_1Picture = Sprite::create("Startbutton.jpg");
	Hero_1Picture->setPosition(Vec2(origin.x + visibleSize.width / 2.0, origin.y + visibleSize.height / 2.0));
	Hero_1Picture->setScale(2.0f);
	addChild(Hero_1Picture,1);

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
	HeroID = "1";
	Director::getInstance()->replaceScene(MapScene::createScene());
}

void HeroMakeSure1::ReturnHeroChoose(Ref *pSenderReturn)
{
	Director::getInstance()->replaceScene(HeroChoose::createScene());
}
