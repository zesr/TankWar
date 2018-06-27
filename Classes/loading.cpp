#include"loading.h"

GameData in;

Loading::Loading()
{

}

Loading::~Loading()
{

}

bool Loading::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//确定需要显示的文字
	String temp;
	switch (in.mapType) {
	case 0:
		temp = "Chapter 1";
		break;
	case 1:
		temp = "Chapter 2";
		break;
	case 2:
		temp = "BOSS";
		break;

	}

	//按键提示
	auto myLabel = Label::createWithSystemFont("Press any key", "Arial", 32);
	myLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 3));
	this->addChild(myLabel);


	//显示当前关卡数
	myLabel = Label::createWithSystemFont(temp.getCString(), "Arial", 64);
	myLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(myLabel);


	//键盘注册
	auto key_listener = EventListenerKeyboard::create();
	key_listener->onKeyPressed = CC_CALLBACK_2(Loading::onKeyPressed, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(key_listener, this);


	return true;
}

void Loading::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	CCDirector::sharedDirector()->replaceScene(GameClient::createScene(in));
}


Scene* Loading::createScene(GameData gameData)
{
	in = gameData;
	auto scene = Scene::create();
	auto layer = Loading::create();
	scene->addChild(layer);
	return scene;
}