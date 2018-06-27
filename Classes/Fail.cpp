#include"Fail.h"

Fail::Fail()
{

}

Fail::~Fail()
{

}

bool Fail::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	// Í¼Æ¬
	m_bg_img = Sprite::create("image/UI/Fail.png");
	m_bg_img->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 100));
	this->addChild(m_bg_img);

	auto myLabel = Label::createWithSystemFont("Press K to retry", "Arial", 64);
	myLabel->setPosition(Vec2(visibleSize.width / 2 - 400, visibleSize.height / 2 - 400));
	this->addChild(myLabel);

	//¼üÅÌ×¢²á
	auto key_listener = EventListenerKeyboard::create();
	key_listener->onKeyPressed = CC_CALLBACK_2(Fail::onKeyPressed, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(key_listener, this);

	//±³¾°ÒôÀÖ
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->stopBackgroundMusic();
	audio->playBackgroundMusic("vedio/fail.wav", true);

	return true;
}

void Fail::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	nextScene();
}

Scene* Fail::createScene()
{
	auto scene = Scene::create();
	auto layer = Fail::create();
	scene->addChild(layer);
	return scene;
}

void Fail::nextScene() {
	//»Øµ½Ô­À´µÄ³¡¾°
	Director::getInstance()->popScene();
}