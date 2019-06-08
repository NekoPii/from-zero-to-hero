#include "proj.win32/HeroChoose.h"
#include "proj.win32/HeroMakeSure1.h"
#include "proj.win32/HeroMakeSure2.h"
#include "proj.win32/HeroMakeSure3.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

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

	auto *chnString = Dictionary::createWithContentsOfFile("CHN_String.xml");
	const char *ChooseStr = ((String *)chnString->objectForKey("Choose_Text"))->getCString();
	
	auto labelChoose = Label::create(ChooseStr, "Arial", 60);
	labelChoose->setPosition(Vec2(origin.x+visibleSize.width/2.0,origin.y+visibleSize.height*0.8));
	addChild(labelChoose, 1);

	//返回按钮
	const char *BackStr = ((String *)chnString->objectForKey("Back_Text"))->getCString();
	auto labelBack = Label::create(BackStr, "Arial", 32);
	auto pBackButtonItem = MenuItemImage::create("Exitbutton.jpg", "Exitbutton.jpg", CC_CALLBACK_1(HeroChoose::EnterHelloWorldScene, this));
	auto BackButton = Menu::create(pBackButtonItem, NULL);
	BackButton->setPosition(Vec2(origin.x + visibleSize.width / 2.0, origin.y + visibleSize.height / 5.0 - labelBack->getContentSize().height));
	labelBack->setPosition(Vec2(origin.x + visibleSize.width / 2.0, origin.y + visibleSize.height / 5.0 - labelBack->getContentSize().height));
	addChild(labelBack, 2);
	addChild(BackButton, 1);
	
	//人物头像1
	auto pHero_1Avatar = MenuItemImage::create("Hero1_Avatar.png", "Hero1_Avatar.png", CC_CALLBACK_1(HeroChoose::OnRoleSelect1, this));
	auto Hero_1Avatar = Menu::create(pHero_1Avatar, NULL);
	Hero_1Avatar->setPosition(Vec2(-150, 180));
	Hero_1Avatar->setScale(0.3f);
	addChild(Hero_1Avatar, 1);

	//人物头像2
	auto pHero_2Avatar = MenuItemImage::create("Hero2_Avatar.png", "Hero2_Avatar.png", CC_CALLBACK_1(HeroChoose::OnRoleSelect2, this));
	auto Hero_2Avatar = Menu::create(pHero_2Avatar, NULL);
	Hero_2Avatar->setPosition(Vec2(1900,1100));
	Hero_2Avatar->setScale(2.0f);
	addChild(Hero_2Avatar, 1);


	//人物头像3
	auto pHero_3Avatar = MenuItemImage::create("Hero3_Avatar.png", "Hero3_Avatar.png", CC_CALLBACK_1(HeroChoose::OnRoleSelect3, this));
	auto Hero_3Avatar = Menu::create(pHero_3Avatar, NULL);
	Hero_3Avatar->setPosition(Vec2(2300,1100));
	Hero_3Avatar->setScale(2.0f);
	addChild(Hero_3Avatar, 1);


	return true;

}

//跳转到开始界面
void HeroChoose::EnterHelloWorldScene(Ref *pSenderBack)
{
	SimpleAudioEngine::getInstance()->playEffect("Touch.wav");
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f,HelloWorld::createScene()));
}


void HeroChoose::OnRoleSelect1(Ref *pSenderEnter)
{
	SimpleAudioEngine::getInstance()->playEffect("Touch.wav");
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f,HeroMakeSure1::createScene()));
}

void HeroChoose::OnRoleSelect2(Ref *pSenderEnter)
{
	SimpleAudioEngine::getInstance()->playEffect("Touch.wav");
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f,HeroMakeSure2::createScene()));
}

void HeroChoose::OnRoleSelect3(Ref *pSenderEnter)
{
	SimpleAudioEngine::getInstance()->playEffect("Touch.wav");
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f,HeroMakeSure3::createScene()));
}
