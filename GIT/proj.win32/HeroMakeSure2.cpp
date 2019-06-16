#include "proj.win32/MapScene.h"
#include "proj.win32/HeroChoose.h"
#include "proj.win32/HeroMakeSure2.h"
#include "GLOBAL.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;


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

	auto *chnString = Dictionary::createWithContentsOfFile("UI/CHN_String.xml");
	const char *ContinueGameStr = ((String *)chnString->objectForKey("Continue_Text"))->getCString();
	const char *BackGameStr = ((String *)chnString->objectForKey("Back_Text"))->getCString();

	frameCacheM = SpriteFrameCache::getInstance();
	frameCacheM->addSpriteFramesWithFile("magic.plist", "magic.png");
	frameCacheH = SpriteFrameCache::getInstance();
	frameCacheH->addSpriteFramesWithFile("heroh.plist", "heroh.png");

	//切换界面背景
	auto MakeSure1BackGround = Sprite::create("UI/ChooseBackGround.jpg");
	MakeSure1BackGround->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	MakeSure1BackGround->setScaleX(0.7f);
	MakeSure1BackGround->setScaleY(0.9f);
	addChild(MakeSure1BackGround, 0);


	Sprite *displayhero = Sprite::create("UI/MoveSpeed.png");
	displayhero->setPosition(Vec2(origin.x + visibleSize.width / 3.0, origin.y + visibleSize.height / 2.0));
	addChild(displayhero, 2);
	displayhero->runAction(createAnimateH(1, "stand", 7, 2));
	displayhero->setScale(4.0f);

	//继续
	auto labelContinue2 = Label::create(ContinueGameStr, "Arial", 32);
	auto pContinueButtonItem2 = MenuItemImage::create("UI/Startbutton.jpg", "UI/StartbuttonP.jpg", CC_CALLBACK_1(HeroMakeSure2::EnterMapScene, this));
	auto ContinueButton2 = Menu::create(pContinueButtonItem2, NULL);
	ContinueButton2->setPosition(Vec2(origin.x + 2.0 *visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelContinue2->getContentSize().height));
	labelContinue2->setPosition(Vec2(origin.x + 2.0*visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelContinue2->getContentSize().height));
	addChild(labelContinue2, 2);
	addChild(ContinueButton2, 1);

	//重选
	auto labelReturn2 = Label::create(BackGameStr, "Arial", 32);
	auto pReturnButtonItem2 = MenuItemImage::create("UI/Exitbutton.jpg", "UI/ExitbuttonP.jpg", CC_CALLBACK_1(HeroMakeSure2::ReturnHeroChoose, this));
	auto ReturnButton2 = Menu::create(pReturnButtonItem2, NULL);
	ReturnButton2->setPosition(Vec2(origin.x + visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelReturn2->getContentSize().height));
	labelReturn2->setPosition(Vec2(origin.x + visibleSize.width / 3.0, origin.y + visibleSize.height / 5.0 - labelReturn2->getContentSize().height));
	addChild(labelReturn2, 2);
	addChild(ReturnButton2, 1);

	//展示按钮一
	Button *display1 = Button::create();
	display1->loadTextures("SKILL/skill21.png", "SKILL/skill21.png", "");
	display1->setPosition(Vec2(origin.x + visibleSize.width*0.85, origin.y + visibleSize.height*0.8));
	display1->setScale(1.0f);
	display1->addTouchEventListener(CC_CALLBACK_2(HeroMakeSure2::displayskill1, this));
	addChild(display1, 2);

	//展示按钮二
	Button *display2 = Button::create();
	display2->loadTextures("SKILL/skill22.png", "SKILL/skill22.png", "");
	display2->setPosition(Vec2(origin.x + visibleSize.width*0.85, origin.y + visibleSize.height*0.6));
	display2->setScale(1.0f);
	display2->addTouchEventListener(CC_CALLBACK_2(HeroMakeSure2::displayskill2, this));
	addChild(display2, 2);

	//展示按钮三
	Button *display3 = Button::create();
	display3->loadTextures("SKILL/skill23.png", "SKILL/skill23.png", "");
	display3->setPosition(Vec2(origin.x + visibleSize.width*0.85, origin.y + visibleSize.height*0.4));
	display3->setScale(1.0f);
	display3->addTouchEventListener(CC_CALLBACK_2(HeroMakeSure2::displayskill3, this));
	addChild(display3, 2);

	//展示按钮四
	Button *display4 = Button::create();
	display4->loadTextures("SKILL/skill24.png", "SKILL/skill24.png", "");
	display4->setPosition(Vec2(origin.x + visibleSize.width*0.85, origin.y + visibleSize.height*0.2));
	display4->setScale(1.0f);
	display4->addTouchEventListener(CC_CALLBACK_2(HeroMakeSure2::displayskill4, this));
	addChild(display4, 2);

	return true;
}

void HeroMakeSure2::EnterMapScene(Ref *pSenderEnter)
{
	SimpleAudioEngine::getInstance()->playEffect("MUSIC/Touch.wav");
	MyHeroID = 2;
	YourHeroID = 2;
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f,MapScene::createScene()));
}

void HeroMakeSure2::ReturnHeroChoose(Ref *pSenderReturn)
{
	SimpleAudioEngine::getInstance()->playEffect("MUSIC/Touch.wav");
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, HeroChoose::createScene()));
}

Animate* HeroMakeSure2::createAnimateH(int direction, const char* action, int num, int id)
{

	Vector <SpriteFrame*> frameArray;
	for (int i = 1; i <= num; i++)
	{
		auto* frame = frameCacheH->getSpriteFrameByName(String::createWithFormat("hero_%d_%s%d%d.png", id, action, direction, i)->getCString());
		frameArray.pushBack(frame);
	}
	Animation* animation = Animation::createWithSpriteFrames(frameArray);
	animation->setLoops(-1);
	animation->setDelayPerUnit(0.1f);
	return Animate::create(animation);
}

Animate* HeroMakeSure2::createAnimateM(int id, int num, float t, int loop)
{

	Vector <SpriteFrame*> frameArray;
	for (int i = 1; i <= num; i++)
	{
		auto* frame = frameCacheM->getSpriteFrameByName(String::createWithFormat("magic_%d_%d.png", id, i)->getCString());
		frameArray.pushBack(frame);
	}
	Animation* animation = Animation::createWithSpriteFrames(frameArray);
	animation->setLoops(loop);
	animation->setDelayPerUnit(t);
	return Animate::create(animation);
}

void HeroMakeSure2::displayskill1(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		auto *chnString = Dictionary::createWithContentsOfFile("UI/CHN_String.xml");
		const char *skill1wordStr = ((String *)chnString->objectForKey("Skill21_Text"))->getCString();

		removeChildByTag(1);
		removeChildByTag(2);
		Sprite*displayskill = Sprite::create("UI/MoveSpeed.png");
		displayskill->setPosition(Vec2(origin.x + visibleSize.width / 3.0, origin.y + visibleSize.height*0.7));
		addChild(displayskill, 2, 1);
		displayskill->runAction(CCSequence::create(createAnimateM(12, 14, 0.05, 3),CCRemoveSelf::create(),NULL));
		displayskill->setScale(2.0f);

		Label* displayword = Label::create(skill1wordStr, "Arial", 34);
		displayword->setPosition(Vec2(origin.x + visibleSize.width / 2.0, origin.y + visibleSize.height / 4.0));
		addChild(displayword, 2, 2);
	}
}

void HeroMakeSure2::displayskill2(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		auto *chnString = Dictionary::createWithContentsOfFile("UI/CHN_String.xml");
		const char *skill2wordStr = ((String *)chnString->objectForKey("Skill22_Text"))->getCString();

		removeChildByTag(1);
		removeChildByTag(2);

		Sprite*displayskill = Sprite::create("UI/MoveSpeed.png");
		displayskill->setPosition(Vec2(origin.x + visibleSize.width*0.65, origin.y + visibleSize.height / 2.0));
		addChild(displayskill, 2, 1);
		displayskill->runAction(CCSequence::create(createAnimateM(4, 20, 0.05, 3),CCRemoveSelf::create(),NULL));
		displayskill->setScale(2.0f);

		Label* displayword = Label::create(skill2wordStr, "Arial", 34);
		displayword->setPosition(Vec2(origin.x + visibleSize.width / 2.0, origin.y + visibleSize.height / 4.0));
		addChild(displayword, 2, 2);
	}
}


void HeroMakeSure2::displayskill3(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		auto *chnString = Dictionary::createWithContentsOfFile("UI/CHN_String.xml");
		const char *skill3wordStr = ((String *)chnString->objectForKey("Skill23_Text"))->getCString();

		removeChildByTag(1);
		removeChildByTag(2);

		Label* displayword = Label::create(skill3wordStr, "Arial", 34);
		displayword->setPosition(Vec2(origin.x + visibleSize.width / 2.0, origin.y + visibleSize.height / 4.0));
		addChild(displayword, 2, 2);
	}
}

void HeroMakeSure2::displayskill4(Ref *sender, Widget::TouchEventType controlevent)
{
	if (controlevent == Widget::TouchEventType::ENDED)
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		auto *chnString = Dictionary::createWithContentsOfFile("UI/CHN_String.xml");
		const char *skill4wordStr = ((String *)chnString->objectForKey("Skill24_Text"))->getCString();

		removeChildByTag(1);
		removeChildByTag(2);

		Sprite*displayskill = Sprite::create("UI/MoveSpeed.png");
		displayskill->setPosition(Vec2(origin.x + visibleSize.width/3.0, origin.y + visibleSize.height*0.43));
		addChild(displayskill, 2, 1);
		displayskill->runAction(CCSequence::create(createAnimateM(10, 21, 0.05, 3),CCRemoveSelf::create(),NULL));
		displayskill->setScale(2.0f);

		Label* displayword = Label::create(skill4wordStr, "Arial", 34);
		displayword->setPosition(Vec2(origin.x + visibleSize.width / 2.0, origin.y + visibleSize.height / 4.0));
		addChild(displayword, 2, 2);
	}
}