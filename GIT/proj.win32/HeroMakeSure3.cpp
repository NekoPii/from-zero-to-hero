#include "proj.win32/MapScene.h"
#include "proj.win32/HeroChoose.h"
#include "proj.win32/HeroMakeSure3.h"
#include "GLOBAL.h"


USING_NS_CC;


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

	//切换界面背景
	auto MakeSure3BackGround = Sprite::create("ChooseBackGround.jpg");
	MakeSure3BackGround->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	MakeSure3BackGround->setScale(3.0f);
	addChild(MakeSure3BackGround, 0);

	//人物图片3
	auto Hero_3Picture = Sprite::create("Startbutton.jpg");
	Hero_3Picture->setPosition(Vec2(origin.x + visibleSize.width / 2.0, origin.y + visibleSize.height / 2.0));
	Hero_3Picture->setScale(2.0f);
	addChild(Hero_3Picture,1);

	//继续
	auto labelContinue3 = Label::createWithTTF("Continue", "fonts/Marker Felt.ttf", 32);
	auto pContinueButtonItem3 = MenuItemImage::create("Startbutton.jpg", "Startbutton.jpg", CC_CALLBACK_1(HeroMakeSure3::EnterMapScene, this));
	auto ContinueButton3 = Menu::create(pContinueButtonItem3, NULL);
	ContinueButton3->setPosition(Vec2(origin.x + 2.0 *visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelContinue3->getContentSize().height));
	labelContinue3->setPosition(Vec2(origin.x + 2.0*visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelContinue3->getContentSize().height));
	addChild(labelContinue3, 2);
	addChild(ContinueButton3, 1);

	//重选
	auto labelReturn3 = Label::createWithTTF("ReSelect", "fonts/Marker Felt.ttf", 32);
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
	HeroID = "3";
	Director::getInstance()->replaceScene(MapScene::createScene());
}

void HeroMakeSure3::ReturnHeroChoose(Ref *pSenderReturn)
{
	Director::getInstance()->replaceScene(HeroChoose::createScene());
}
