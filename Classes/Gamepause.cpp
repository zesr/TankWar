#include "Gamepause.h"  
//����һ��CCrenderTexture   
//�൱��һ���������е���Ϸ�Ľ�ͼ��Ϊ�����ͣ�Ի���ı���   
//�����Ϳ��������ǶԻ�������Ϸ����֮�ϣ�һ����Ϸ���ж���������д�ġ�  
Scene* Gamepause::scene(CCRenderTexture* sqr)
{

	CCScene *scene = CCScene::create();
	Gamepause *layer = Gamepause::create();
	scene->addChild(layer, 1);
	//���Ӳ��֣�ʹ��Game�����н�ͼ��sqr����ͼƬ����Sprite  
	//����Sprite��ӵ�GamePause��������  
	//�õ����ڵĴ�С  
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCSprite *back_spr = CCSprite::createWithTexture(sqr->getSprite()->getTexture());
	back_spr->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2)); //����λ��,������������λ�á�  
	back_spr->setFlipY(true);            //��ת����ΪUI�����OpenGL���겻ͬ  
	back_spr->setColor(cocos2d::Color3B::GRAY); //ͼƬ��ɫ���ɫ  
	scene->addChild(back_spr);

	

	return scene;
}
bool Gamepause::init()
{

	if (!Scene::init())
	{
		return false;
	}
	//�õ����ڵĴ�С  
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	//ԭ������  
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

