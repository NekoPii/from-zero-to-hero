#ifndef _MAP_SCENE_H_
#define _MAP_SCENE_H_

#include "cocos2d.h"

class MapScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void EnterHelloWorldScene(Ref *pSenderBack);


	// implement the "static create()" method manually
	CREATE_FUNC(MapScene);

};

#endif // !_MAP_SCENE_H_