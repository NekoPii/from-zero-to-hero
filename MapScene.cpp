#include "proj.win32/MapScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;

float x, y;
Scene *MapScene::createScene()
{
	Scene *scene = Scene::create();
	MapScene *layer = MapScene::create();
	scene->addChild(layer);
	return scene;
}
/*void MapScene::view_move()
{
	Size visableSize = CCSizeMake(1920, 1080); //屏幕大小
	Size mysize = CCSizeMake(3*1920/2, 3*1080/2);       //容器大小

//创建容器、设置大小
	LayerColor* scrollLayer = LayerColor::create(ccc4(255, 255, 255, 255));
	scrollLayer->setContentSize(mysize);

	//容器中的东西
	Sprite* MAP = Sprite::create("map.jpg");
	MAP->setScale(3.0f);
	MAP->setPosition(ccp(3*1920 / 2.0, 3*1080 / 2.0));
	scrollLayer->addChild(MAP);


	//创建滚动视图CCScrollView
	ScrollView * scrollView = ScrollView::create();
	this->addChild(scrollView, 0, 2);

	//属性设置
	scrollView->setContainer(scrollLayer);  //设置容器
	scrollView->setViewSize(visableSize); //可视区域大小
	scrollView->setBounceable(false);        //是否具有弹性
	scrollView->setDelegate(this);
}*/
bool MapScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//地图
	auto MAP = Sprite::create("map.jpg");
	MAP->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	MAP->setScale(1.0f);
	auto* layer1 = Layer::create();
	layer1->setAnchorPoint(Vec2(0, 0));
	layer1->setPosition((Vec2(0, 0)));
	layer1->addChild(MAP);
	this->addChild(layer1,0, 1);

	//测试按钮
	auto pBackButtonItem = MenuItemImage::create("Backbutton.png", "Backbutton.png", CC_CALLBACK_1(MapScene::EnterHelloWorldScene, this));
	auto BackButton = Menu::create(pBackButtonItem,NULL);
	BackButton->setPosition(Vec2(80,visibleSize.height-30));
	addChild(BackButton, 1);
	hero = new heroPrint();
	hero->initHeroSprite( 8, Vec2(860,540));
	addChild(hero, 2, 3);

	
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto myKeyListener = EventListenerKeyboard::create();	//当键盘被按下时响应	
	/*myKeyListener->onKeyPressed = [](EventKeyboard::KeyCode keycode, cocos2d::Event *event)	
	{		CCLOG("key is pressed,keycode is %d", keycode);	};*/
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	myKeyListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//scheduleUpdate();
	dispatcher->addEventListenerWithSceneGraphPriority(myKeyListener, this);
	return true;
}
void MapScene::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event) {
	Vec2 hero_position = hero->herosPosition();
	//Sprite* MAP = (Sprite*)getChildByTag(1);
	Layer* layer1 = (Layer*)getChildByTag(1);
	Vec2 layer_position = layer1->getPosition();
	CCLOG("%f %f", hero_position.x, hero_position.y);
	if (keycode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
		CCLOG("按下了：上方向键");
		hero->heroSprite->setPosition(Vec2(hero_position.x, hero_position.y-100));
		layer1->setPosition(Vec2(layer_position.x, layer_position.y-100));
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
		CCLOG("按下了：左方向键");
		hero->heroSprite->setPosition(Vec2(hero_position.x+100, hero_position.y ));
		layer1->setPosition(Vec2(layer_position.x+100, layer_position.y ));
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
		CCLOG("按下了：右方向键");
		hero->heroSprite->setPosition(Vec2(hero_position.x-100, hero_position.y));
		layer1->setPosition(Vec2(layer_position.x-100, layer_position.y ));
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
		hero->heroSprite->setPosition(Vec2(hero_position.x , hero_position.y+100));
		layer1->setPosition(Vec2(layer_position.x , layer_position.y+100));
		CCLOG("按下了：下方向键");
	}
}
bool MapScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	float Position_x=hero->getDistance_x(hero->currentPosition, touch->getLocation());
	float Position_y = hero->getDistance_x(hero->currentPosition, touch->getLocation());
	CCLOG("%f %f", Position_x, Position_y);
	float distance = hero->getDistance(hero->currentPosition, touch->getLocation());
	hero->getDirection(hero->currentPosition, touch->getLocation());
	hero->heroMoveTo(touch->getLocation());
	Point position = touch->getLocation();
	CCLOG("%f,%f", position.x, position.y);
	Layer* layer1 = (Layer*)getChildByTag(1);
	//MoveBy* moveby = MoveBy::create((float)distance / 200, Point(Position_x, Position_y));
	//layer1->runAction(moveby);
	
	/*auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size mapSize = Size(2362 * 3, 1532 * 3);
	Vec2 hero_position = hero->getPosition();
	Vec2 layerPosition = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	if (x >= visibleSize.width / 2 && x <= mapSize.width - visibleSize.width / 2)
	{
		layerPosition.x = visibleSize.width / 2 - x;
	}
	if (x >= mapSize.width - visibleSize.width / 2)
	{
		layerPosition.x = visibleSize.width / 2 - mapSize.width;
	}
	if (y >= visibleSize.height && y <= mapSize.height - visibleSize.height / 2)
	{
		layerPosition.y = visibleSize.height / 2 - y;
	}
	if (y >= mapSize.height - visibleSize.height / 2) { layerPosition.y = visibleSize.height / 2 - mapSize.height; }
	layer1->setPosition(layerPosition);*/
	return false;
}
void MapScene::update(float dt)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size mapSize = Size(2362 , 1532 );
	CCLOG("%f,%f", x, y);
	Vec2 layerPosition = Vec2(origin.x + visibleSize.width/2 , origin.y + visibleSize.height/2 );
	if (x >= visibleSize.width / 2 && x <= mapSize.width - visibleSize.width / 2)
	{
		layerPosition.x = visibleSize.width / 2 - x;
	}
	if (x >= mapSize.width - visibleSize.width / 2)
	{
		layerPosition.x = visibleSize.width / 2 - mapSize.width;
	}
	if (y >= visibleSize.height && y <= mapSize.height - visibleSize.height / 2)
	{
		layerPosition.y = visibleSize.height / 2 - y;
	}
	if (y >= mapSize.height - visibleSize.height / 2) { layerPosition.y = visibleSize.height / 2 - mapSize.height; }
	Layer* layer1=(Layer*) getChildByTag(1);
	layer1->setPosition(layerPosition);
}
/*void MapScene::setViewPointCenter(Point position)
{
	//视窗大小
	Size winSize = Director::getInstance()->getWinSize();
	//地图大小
	Size mapSize = Size(2362 * 3, 1532 * 3);

	//让hero与mainLayer的左右边界的相对位置在半个屏幕间不滚动地图，所以英雄与Layer的相对位置在[winSize.w/2,mapSize.w-winSize.w/2]的范围才发生
	//获得镜头移动的范围(即地图会滚动的范围)
	float scrollX;
	if (position.x > winSize.width / 2)
		 scrollX = position.x;
	else
		 scrollX = winSize.width / 2;
	if (scrollX > mapSize.width - winSize.width / 2)
		 scrollX = mapSize.width - winSize.width / 2;
	else
		 scrollX = scrollX;
	float scrollY;
	if (position.y > winSize.height / 2)
		scrollY = position.y;
	else
		 scrollY = winSize.height / 2;
	if (scrollY > mapSize.height - winSize.height / 2)
		scrollY = mapSize.height - winSize.height / 2;
	else
		float scrollY = scrollY;

	//镜头移动（地图滚动）时的坐标范围(在未运行是x,y是不确定值，所以算是范围，当运行时x,y为定值，此时就是镜头相对于GameLayer世界移动的距离【如果镜头可以移动的话】)
	Point cameraNeedMoveSize = ccp(scrollX, scrollY);

	//镜头相对于Layer的坐标位置
	Point centerOfView = ccp(CENTER.x, CENTER.y);

	//由于镜头是静止的，所以移动Layer等同于移动镜头
	Point viewPoint = ccpSub(centerOfView, cameraNeedMoveSize);

	//设置Layer移动的目标坐标
	 layer1->setPosition(viewPoint);

}*/

void MapScene::EnterHelloWorldScene(Ref *pSenderBack)
{
	Director::getInstance()->replaceScene((HelloWorld::createScene()));
}