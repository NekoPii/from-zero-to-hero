#ifndef _HERO_MAKE_SURE_1_H_
#define _HERO_MAKE_SURE_1_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class HeroMakeSure1 : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void EnterMapScene(Ref *pSenderEnter);
	void ReturnHeroChoose(Ref *pSenderReturn);
	//void testcall(Ref *sender, Widget::TouchEventType controlevent);
	void initHero1Stand();
	cocos2d::Animate * createAnimateStand1(int num);
	// implement the "static create()" method manually
	CREATE_FUNC(HeroMakeSure1);

};

#endif // !_HERO_MAKE_SURE_1_H_