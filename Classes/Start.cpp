#include"Start.h"

Start::Start()
{

}

Start::~Start()
{

}

bool Start::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 排行按钮
	auto closeItem = MenuItemImage::create(
		"start/rank.png",
		"start/rank.png",
		CC_CALLBACK_1(Start::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));


	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	// 图片
	m_bg_img = Sprite::create("start/title.png");
	m_bg_img->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2+100));
	this->addChild(m_bg_img);

	// 创建分割图片
	m_flag = false;
	m_count = 0;
	m_particle_pic = ParticlePic::create("start/title.png");
	this->addChild(m_particle_pic);

	//创建按钮控件ControlButton
	Label* labelSprite = Label::createWithSystemFont("ControlButton", "bitfont", 30);
	ui::Scale9Sprite* bg1 = ui::Scale9Sprite::create("start/btn-play-normal.png");
	ui::Scale9Sprite* bg2 = ui::Scale9Sprite::create("start/btn-play-selected.png");
	ui::Scale9Sprite* bg3 = ui::Scale9Sprite::create("start/btn-play-disabled.png");

	m_play_btn = ControlButton::create(bg1);

	// 设置按钮被按下时的状态
	m_play_btn->setBackgroundSpriteForState(bg1, Control::State::NORMAL);
	m_play_btn->setBackgroundSpriteForState(bg2, Control::State::HIGH_LIGHTED);
	m_play_btn->setBackgroundSpriteForState(bg3, Control::State::DISABLED);
	m_play_btn->setAdjustBackgroundImage(false);
	m_play_btn->setPosition(Vec2(visibleSize.width / 2-150, 200));
	addChild(m_play_btn, 10);

	// 添加监听
	m_play_btn->addTargetWithActionForControlEvents(this, cccontrol_selector(Start::playBtnOnClick), extension::Control::EventType::TOUCH_DOWN);

	this->scheduleUpdate();

	//抖动效果
	CCShake* shake = CCShake::create(3, 15);
	m_bg_img->runAction(shake);

	//键盘注册
	auto key_listener = EventListenerKeyboard::create();
	key_listener->onKeyPressed = CC_CALLBACK_2(Start::onKeyPressed, this);
	key_listener->onKeyReleased = CC_CALLBACK_2(Start::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(key_listener, this);

	//背景音乐
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("vedio/start_background.mp3", true);

	return true;
}

void Start::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_K)
		start();
}

Scene* Start::createScene()
{
	auto scene = Scene::create();
	auto layer = Start::create();
	scene->addChild(layer);
	return scene;
}

void Start::update(float delta)
{
	m_flag = m_particle_pic->isComplete();
	if (!m_flag && m_count)
	{
		m_play_btn->setEnabled(false);
	}
	else if (m_flag)
	{
		unschedule(schedule_selector(Start::particlesUpdate));
		m_bg_img->setVisible(true);
		m_play_btn->setEnabled(true);
	}
}

//排行
void Start::menuCloseCallback(Ref* pSender)
{
	CCDirector::sharedDirector()->replaceScene(RankList::createScene(0, false));

}

void Start::particlesUpdate(float delta)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// 加载粒子
	m_currParticle = ParticleSystemQuad::create("start/particles4.plist");

	m_currParticle->retain();
	m_currParticle->setBlendAdditive(true);
	m_currParticle->setScale(0.8f);
	addChild(m_currParticle, 20);

	float x = visibleSize.width / 2 - 256 + rand_0_1() * 512;
	float y = visibleSize.height / 2 - 128 + rand_0_1() * 256;
	m_currParticle->setPosition(Point(x, y));
}//617*231

void Start::nextScene() {
	//更换场景
	CCDirector::sharedDirector()->replaceScene(Loading::createScene(GameData(0, 1000, 1000, 0)));
}

void Start::playBtnOnClick(Ref* pSender, extension::Control::EventType eventType)
{
	start();
}

void Start::start() {
	//开始音效
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->stopBackgroundMusic();
	// play a sound effect, just once.
	audio->playEffect("vedio/pressButton.wav", false, 1.0f, 1.0f, 1.0f);

	m_flag = false;
	m_count++;
	m_bg_img->setVisible(false);
	schedule(schedule_selector(Start::particlesUpdate), 0.5f, -1, 2.0f);
	m_particle_pic->run();
	CCDelayTime* delayTime = CCDelayTime::create(3);
	CCCallFunc *callFunND = CCCallFunc::create(this, callfunc_selector(Start::nextScene));
	CCSequence *action = CCSequence::create(delayTime, callFunND, NULL);
	this->runAction(action);
}


