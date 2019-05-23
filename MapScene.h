#ifndef _MAP_SCENE_H_
#define _MAP_SCENE_H_

#include "cocos2d.h"
#include "heroPrint.h"
#include "cocos-ext.h"
using namespace cocos2d::extension;
class MapScene : public cocos2d::Layer, public ScrollViewDelegate
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	heroPrint* hero;
	void update(float dt);
	void updatePosition(float dt);
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onKeyPressed(EventKeyboard::KeyCode keycode, Event* event);
	void EnterHelloWorldScene(Ref *pSenderBack);


	// implement the "static create()" method manually
	CREATE_FUNC(MapScene);

};

#endif // !_MAP_SCENE_H_