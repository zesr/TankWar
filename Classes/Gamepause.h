#ifndef __Gamepause__H__  
#define __Gamepause__H__  
#include "cocos2d.h"  
USING_NS_CC;
class Gamepause : public cocos2d::Scene
{
public:
	virtual bool init();
	static cocos2d::Scene* scene(CCRenderTexture* sqr);
	CREATE_FUNC(Gamepause);
	//¼ÌÐøÓÎÏ·  
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
private:
};

#endif // __Gamepause_H__  