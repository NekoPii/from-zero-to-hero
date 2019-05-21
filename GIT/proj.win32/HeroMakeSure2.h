#ifndef _HERO_MAKE_SURE_2_H_
#define _HERO_MAKE_SURE_2_H_

#include "cocos2d.h"

class HeroMakeSure2 : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void EnterMapScene(Ref *pSenderEnter);
	void ReturnHeroChoose(Ref *pSenderReturn);


	// implement the "static create()" method manually
	CREATE_FUNC(HeroMakeSure2);

};

#endif // !_HERO_MAKE_SURE_2_H_