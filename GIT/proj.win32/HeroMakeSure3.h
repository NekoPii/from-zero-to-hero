#ifndef _HERO_MAKE_SURE_3_H_
#define _HERO_MAKE_SURE_3_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;

class HeroMakeSure3 : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void EnterMapScene(Ref *pSenderEnter);
	void ReturnHeroChoose(Ref *pSenderReturn);
	void initHero3Stand();
	cocos2d::Animate * createAnimateStand3(int num);


	// implement the "static create()" method manually
	CREATE_FUNC(HeroMakeSure3);

};

#endif // !_HERO_MAKE_SURE_3_H_