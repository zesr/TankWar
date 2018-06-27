#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace cocos2d;

class Fail : public Layer
{
public:
	Fail();
	~Fail();

	virtual bool init();
	CREATE_FUNC(Fail);
	static Scene* createScene();

private:
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void nextScene();
	Sprite* m_bg_img;
};
