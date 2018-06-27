#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "GameData.h"
#include "GameClient.h"

USING_NS_CC;
using namespace cocos2d;

class Loading : public Layer
{
public:
	Loading();
	~Loading();

	virtual bool init();
	CREATE_FUNC(Loading);
	static Scene* createScene(GameData);
	void Loading::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

private:
};
