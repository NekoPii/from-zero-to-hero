#ifndef _HERO_CHOOSE_H_
#define _HERO_CHOOSE_H_

#include "cocos2d.h"

using namespace cocos2d::ui;

class HeroChoose : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void EnterHelloWorldScene(Ref *pSenderBack);
	void OnRoleSelect1(Ref *pSenderEnter);
	void OnRoleSelect2(Ref *pSenderEnter);
	void OnRoleSelect3(Ref *pSenderEnter);
	CREATE_FUNC(HeroChoose);

};

#endif // !_HERO_CHOOSE_H_
