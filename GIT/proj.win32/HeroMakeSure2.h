#ifndef _HERO_MAKE_SURE_2_H_
#define _HERO_MAKE_SURE_2_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;

class HeroMakeSure2 : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void EnterMapScene(Ref *pSenderEnter);
	void ReturnHeroChoose(Ref *pSenderReturn);
	void initHero2Stand();
	cocos2d::Animate * createAnimateStand2(int num);


	// implement the "static create()" method manually
	CREATE_FUNC(HeroMakeSure2);

};

#endif // !_HERO_MAKE_SURE_2_H_