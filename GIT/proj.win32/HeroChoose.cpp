#include "proj.win32/HeroChoose.h"
#include "proj.win32/HeroMakeSure1.h"
#include "proj.win32/HeroMakeSure2.h"
#include "proj.win32/HeroMakeSure3.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene *HeroChoose::createScene()
{
	Scene *scene = Scene::create();
	HeroChoose *layer = HeroChoose::create();
	scene->addChild(layer);
	return scene;
}

bool HeroChoose::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//切换界面背景
	auto ChooseBackGround = Sprite::create("ChooseBackGround.jpg");
	ChooseBackGround->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	ChooseBackGround->setScale(3.0f);
	addChild(ChooseBackGround, 0);

	//返回按钮
	auto labelBack = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 28);
	auto pBackButtonItem = MenuItemImage::create("Exitbutton.jpg", "Exitbutton.jpg", CC_CALLBACK_1(HeroChoose::EnterHelloWorldScene, this));
	auto BackButton = Menu::create(pBackButtonItem, NULL);
	BackButton->setPosition(Vec2(origin.x + visibleSize.width / 2.0, origin.y + visibleSize.height / 5.0 - labelBack->getContentSize().height));
	labelBack->setPosition(Vec2(origin.x + visibleSize.width / 2.0, origin.y + visibleSize.height / 5.0 - labelBack->getContentSize().height));
	addChild(labelBack, 2);
	addChild(BackButton, 1);
	
	//人物头像1
	auto pHero_1Avatar = MenuItemImage::create("Startbutton.jpg", "Startbutton.jpg", CC_CALLBACK_1(HeroChoose::OnRoleSelect1, this));
	auto Hero_1Avatar = Menu::create(pHero_1Avatar, NULL);
	Hero_1Avatar->setPosition(Vec2(origin.x + visibleSize.width / 4.0, origin.y + visibleSize.height / 2.0));
	Hero_1Avatar->setScale(1.0f);
	addChild(Hero_1Avatar, 1);

	//人物头像2
	auto pHero_2Avatar = MenuItemImage::create("Exitbutton.jpg", "Exitbutton.jpg", CC_CALLBACK_1(HeroChoose::OnRoleSelect2, this));
	auto Hero_2Avatar = Menu::create(pHero_2Avatar, NULL);
	Hero_2Avatar->setPosition(Vec2(origin.x + 2.0*visibleSize.width / 4.0, origin.y + visibleSize.height / 2.0));
	Hero_2Avatar->setScale(1.0f);
	addChild(Hero_2Avatar, 1);


	//人物头像3
	auto pHero_3Avatar = MenuItemImage::create("Startbutton.jpg", "Startbutton.jpg", CC_CALLBACK_1(HeroChoose::OnRoleSelect3, this));
	auto Hero_3Avatar = Menu::create(pHero_3Avatar, NULL);
	Hero_3Avatar->setPosition(Vec2(origin.x + 3.0*visibleSize.width / 4.0, origin.y + visibleSize.height / 2.0));
	Hero_3Avatar->setScale(1.0f);
	addChild(Hero_3Avatar, 1);

	return true;

}

//跳转到开始界面
void HeroChoose::EnterHelloWorldScene(Ref *pSenderBack)
{
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}


void HeroChoose::OnRoleSelect1(Ref *pSenderEnter)
{
	Director::getInstance()->replaceScene(HeroMakeSure1::createScene());
}

void HeroChoose::OnRoleSelect2(Ref *pSenderEnter)
{
	Director::getInstance()->replaceScene(HeroMakeSure2::createScene());
}

void HeroChoose::OnRoleSelect3(Ref *pSenderEnter)
{
	Director::getInstance()->replaceScene(HeroMakeSure3::createScene());
}
