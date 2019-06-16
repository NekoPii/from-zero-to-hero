#ifndef _HERO_MAKE_SURE_3_H_
#define _HERO_MAKE_SURE_3_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class HeroMakeSure3 : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void EnterMapScene(Ref *pSenderEnter);
	void ReturnHeroChoose(Ref *pSenderReturn);
	void displayskill1(Ref *sender, Widget::TouchEventType controlevent);
	void displayskill2(Ref *sender, Widget::TouchEventType controlevent);
	void displayskill3(Ref *sender, Widget::TouchEventType controlevent);
	void displayskill4(Ref *sender, Widget::TouchEventType controlevent);
	//void testcall(Ref *sender, Widget::TouchEventType controlevent);
	cocos2d::Animate* createAnimateH(int direction, const char* action, int num, int id);
	cocos2d::Animate* createAnimateM(int id, int num, float t, int loop);
	// implement the "static create()" method manually
	CREATE_FUNC(HeroMakeSure3);

private:
	Button *display1;
	Button *display2;
	Button *display3;
	Button *display4;
	SpriteFrameCache* frameCacheM;
	SpriteFrameCache* frameCacheH;

};

#endif // !_HERO_MAKE_SURE_3_H_