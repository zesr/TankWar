#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include"ParticlePic.h"
#include "SimpleAudioEngine.h"
#include"CCShake.h"
#include"GameClient.h"
#include "RankList.h"
#include "loading.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d;
using namespace cocostudio;
using namespace cocos2d::ui;

class Start : public Layer
{
public:
	Start();
	~Start();

	virtual bool init();
	CREATE_FUNC(Start);
	static Scene* createScene();

	void update(float delta);

	void menuCloseCallback(Ref* pSender);
	void playBtnOnClick(Ref* pSender, extension::Control::EventType eventType);

	void particlesUpdate(float delta);

private:
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void start();
	void nextScene();
	ParticlePic * m_particle_pic;
	ParticleSystemQuad* m_currParticle;
	bool		        m_flag;
	int			        m_count;
	Sprite*             m_bg_img;
	ControlButton*      m_play_btn;
};

