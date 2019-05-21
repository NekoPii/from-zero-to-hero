#ifndef _HERO_MAKE_SURE_1_H_
#define _HERO_MAKE_SURE_1_H_

#include "cocos2d.h"

class HeroMakeSure1 : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void EnterMapScene(Ref *pSenderEnter);
	void ReturnHeroChoose(Ref *pSenderReturn);

	// implement the "static create()" method manually
	CREATE_FUNC(HeroMakeSure1);

};

#endif // !_HERO_MAKE_SURE_1_H_