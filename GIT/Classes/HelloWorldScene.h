#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void update(float dt);
	void EnterHeroChooseScene(Ref *pSenderEnter);
    void CloseGame(cocos2d::Ref* pSenderClose);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
	ProgressTimer *Loading;
	LabelTTF *PEC;
};

#endif // __HELLOWORLD_SCENE_H__
