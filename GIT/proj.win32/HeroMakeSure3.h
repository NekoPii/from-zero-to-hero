#ifndef _HERO_MAKE_SURE_3_H_
#define _HERO_MAKE_SURE_3_H_

#include "cocos2d.h"

class HeroMakeSure3 : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void EnterMapScene(Ref *pSenderEnter);
	void ReturnHeroChoose(Ref *pSenderReturn);


	// implement the "static create()" method manually
	CREATE_FUNC(HeroMakeSure3);

};

#endif // !_HERO_MAKE_SURE_3_H_