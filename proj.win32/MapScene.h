#ifndef _MAP_SCENE_H_
#define _MAP_SCENE_H_

#include "cocos2d.h"
#include "heroPrint.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include <vector>
using namespace std;

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class MapScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();


	TMXTiledMap* map;
	TMXLayer* _colliable;
	Vec2 touchLocation;
	int time = 0;
	int canMove;
	CCRect getRect(Node* pNode);
	virtual bool init();
	heroPrint* hero;
	void attactEnemy(Vec2 pos, Vec2 Epos, int num,int tag1,int tag2);
	void finishRunAction(const int num);
	void finishAttack(const int tag1, const int tag2);
	//void update(float dt);
	//void updatePosition(float dt);
	Sprite* LBS;
	void mark(float dt);
	void heroIn(float dt);
	void Tower1(float dt);
	void soldersMake(float dt);
	void soldersContrl(float dt);
	void Tower2(float dt);
	void soldersMake1(float dt);
	void soldersMake2(float dt);
	float getDistance(Vec2 me, Vec2 you);
	Sprite* chosenEnemy;
	Sprite* chosenFriend;
	Vector<Sprite*> Friend;
	Vector<Sprite*> Enemy;
	Vec2 tiledpos(Vec2 pos);
	virtual bool onTouchBegan(Touch* touch, Event* unused_event);
	virtual void onKeyPressed(EventKeyboard::KeyCode keycode, Event* event);
	void EnterHelloWorldScene(Ref* pSenderBack);
	vector<int>use;
	void Shopcall(Ref *sender, Widget::TouchEventType controlevent);
	void ShopBack(Ref *sender, Widget::TouchEventType controlevent);
	void Shopbuy1(Ref *sender, Widget::TouchEventType controlevent);
	void Shopbuy2(Ref *sender, Widget::TouchEventType controlevent);
	void Shopbuy3(Ref *sender, Widget::TouchEventType controlevent);
	void Shopbuy4(Ref *sender, Widget::TouchEventType controlevent);
	void Shopbuy5(Ref *sender, Widget::TouchEventType controlevent);
	void Shopbuy6(Ref *sender, Widget::TouchEventType controlevent);
	void Buyit1(Ref *sender, Widget::TouchEventType controlevent);
	void Buyit2(Ref *sender, Widget::TouchEventType controlevent);
	void Buyit3(Ref *sender, Widget::TouchEventType controlevent);
	void Buyit4(Ref *sender, Widget::TouchEventType controlevent);
	void Buyit5(Ref *sender, Widget::TouchEventType controlevent);
	void Buyit6(Ref *sender, Widget::TouchEventType controlevent);

	// implement the "static create()" method manually
	cocos2d::Animate* createAnimateH(int direction, const char* action, int num, int id);
	cocos2d::Animate* createAnimateS(int direction, const char* action, int num, int id,int loop);

	SpriteFrameCache* frameCacheH;
	SpriteFrameCache* frameCacheS;


	
//	void dieAni(object *die);
	CREATE_FUNC(MapScene);

private:
	Sprite *ShopBackGround;
	Button * Weapon1;
	Button * Weapon2;
	Button * Weapon3;
	Button * Weapon4;
	Button * Weapon5;
	Button * Weapon6;
	Button * ShopButton;
	Button * ShopBackButton;
	Sprite *GoldCost1;
	Sprite *GoldCost2;
	Sprite *GoldCost3;
	Sprite *GoldCost4;
	Sprite *GoldCost5;
	Sprite *GoldCost6;
	Sprite *Weapon1Detail;
	Sprite *Weapon2Detail;
	Sprite *Weapon3Detail;
	Sprite *Weapon4Detail;
	Sprite *Weapon5Detail;
	Sprite *Weapon6Detail;
	Sprite *BuyButton12;
	Sprite *BuyButton22;
	Sprite *BuyButton32;
	Sprite *BuyButton42;
	Sprite *BuyButton52;
	Sprite *BuyButton62;
	Button *BuyButton11;
	Button *BuyButton21;
	Button *BuyButton31;
	Button *BuyButton41;
	Button *BuyButton51;
	Button *BuyButton61;


};

#endif // !_MAP_SCENE_H_
