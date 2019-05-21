#include "proj.win32/MapScene.h"
#include "proj.win32/HeroChoose.h"
#include "proj.win32/HeroMakeSure2.h"
#include "GLOBAL.h"


USING_NS_CC;


Scene *HeroMakeSure2::createScene()
{
	Scene *scene = Scene::create();
	HeroMakeSure2 *layer = HeroMakeSure2::create();
	scene->addChild(layer);
	return scene;
}

bool HeroMakeSure2::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//切换界面背景
	auto MakeSure2BackGround = Sprite::create("ChooseBackGround.jpg");
	MakeSure2BackGround->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	MakeSure2BackGround->setScale(3.0f);
	addChild(MakeSure2BackGround, 0);


	//人物图片2
	auto Hero_2Picture = Sprite::create("Exitbutton.jpg");
	Hero_2Picture->setPosition(Vec2(origin.x + visibleSize.width / 2.0, origin.y + visibleSize.height / 2.0));
	Hero_2Picture->setScale(2.0f);
	addChild(Hero_2Picture,1);

	//继续
	auto labelContinue2 = Label::createWithTTF("Continue", "fonts/Marker Felt.ttf", 32);
	auto pContinueButtonItem2 = MenuItemImage::create("Startbutton.jpg", "Startbutton.jpg", CC_CALLBACK_1(HeroMakeSure2::EnterMapScene, this));
	auto ContinueButton2 = Menu::create(pContinueButtonItem2, NULL);
	ContinueButton2->setPosition(Vec2(origin.x + 2.0 *visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelContinue2->getContentSize().height));
	labelContinue2->setPosition(Vec2(origin.x + 2.0*visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelContinue2->getContentSize().height));
	addChild(labelContinue2, 2);
	addChild(ContinueButton2, 1);

	//重选
	auto labelReturn2 = Label::createWithTTF("ReSelect", "fonts/Marker Felt.ttf", 32);
	auto pReturnButtonItem2 = MenuItemImage::create("Exitbutton.jpg", "Exitbutton.jpg", CC_CALLBACK_1(HeroMakeSure2::ReturnHeroChoose, this));
	auto ReturnButton2 = Menu::create(pReturnButtonItem2, NULL);
	ReturnButton2->setPosition(Vec2(origin.x + visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelReturn2->getContentSize().height));
	labelReturn2->setPosition(Vec2(origin.x + visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelReturn2->getContentSize().height));
	addChild(labelReturn2, 2);
	addChild(ReturnButton2, 1);

	return true;
}

void HeroMakeSure2::EnterMapScene(Ref *pSenderEnter)
{
	HeroID = "2";
	Director::getInstance()->replaceScene(MapScene::createScene());
}

void HeroMakeSure2::ReturnHeroChoose(Ref *pSenderReturn)
{
	Director::getInstance()->replaceScene(HeroChoose::createScene());
}
