#ifndef _MAP_SCENE_H_
#define _MAP_SCENE_H_

#include "cocos2d.h"
#include "heroPrint.h"
class MapScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	heroPrint* hero;
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	void EnterHelloWorldScene(Ref *pSenderBack);


	// implement the "static create()" method manually
	CREATE_FUNC(MapScene);

};

#endif // !_MAP_SCENE_H_