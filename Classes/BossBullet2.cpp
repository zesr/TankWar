#include"BossBullet2.h"

BossBullet2::BossBullet2() {
}
BossBullet2::~BossBullet2() {}






BossBullet2* BossBullet2::create(Vec2 position, float speed, int dir)
{
	BossBullet2* pRet = new(std::nothrow) BossBullet2();
	if (pRet && pRet->init(position, speed, dir))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool BossBullet2::init(Vec2 position, float speed, int dir)
{
	if (!BaseObject::init())
	{
		return false;
	}

	m_texture = Director::getInstance()->getTextureCache()->addImage("image/tank/BossBullet2.png");
	m_sprite = Sprite::createWithTexture(m_texture, Rect(0, 0, 64, 64));

	m_sprite->setPosition(Vec2::ZERO);

	this->addChild(m_sprite, 10);

	this->setLife(1);
	this->setPosition(position);
	this->setDirection(dir);
	this->setSpeed(speed);
	this->scheduleUpdate();

	m_rect = Rect(position.x - 8, position.y - 8, 16, 16);  // rect

	//ÅÚµ¯ËÙ¶È
	setSpeed(10);

	return true;
}

void BossBullet2::deleteObj(Sprite* obj)
{
	obj->removeFromParent();
	this->removeFromParent();
}
