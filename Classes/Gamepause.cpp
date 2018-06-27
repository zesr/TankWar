#include "Gamepause.h"  
//传入一个CCrenderTexture   
//相当于一个正在运行的游戏的截图作为这个暂停对话框的背景   
//这样就看起来像是对话框在游戏界面之上，一般游戏当中都是这样子写的。  
Scene* Gamepause::scene(CCRenderTexture* sqr)
{

	CCScene *scene = CCScene::create();
	Gamepause *layer = Gamepause::create();
	scene->addChild(layer, 1);
	//增加部分：使用Game界面中截图的sqr纹理图片创建Sprite  
	//并将Sprite添加到GamePause场景层中  
	//得到窗口的大小  
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCSprite *back_spr = CCSprite::createWithTexture(sqr->getSprite()->getTexture());
	back_spr->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2)); //放置位置,这个相对于中心位置。  
	back_spr->setFlipY(true);            //翻转，因为UI坐标和OpenGL坐标不同  
	back_spr->setColor(cocos2d::Color3B::GRAY); //图片颜色变灰色  
	scene->addChild(back_spr);

	

	return scene;
}
bool Gamepause::init()
{

	if (!Scene::init())
	{
		return false;
	}
	//得到窗口的大小  
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	//原点坐标  
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	auto key_listener = EventListenerKeyboard::create();
	key_listener->onKeyReleased = CC_CALLBACK_2(Gamepause::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(key_listener, this);

	return true;
}

void Gamepause::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if(keyCode== EventKeyboard::KeyCode::KEY_P)
		CCDirector::sharedDirector()->popScene();
}

