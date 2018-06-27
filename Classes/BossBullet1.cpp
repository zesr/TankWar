#include"BossBullet1.h"

BossBullet1::BossBullet1() {
}
BossBullet1::~BossBullet1() {}






BossBullet1* BossBullet1::create(Vec2 position, float speed, int dir)
{
	BossBullet1* pRet = new(std::nothrow) BossBullet1();
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

bool BossBullet1::init(Vec2 position, float speed, int dir)
{
	if (!BaseObject::init())
	{
		return false;
	}

	m_texture = Director::getInstance()->getTextureCache()->addImage("image/tank/bossbullet1.png");
	m_sprite = Sprite::createWithTexture(m_texture, Rect(0, 0, 16, 16));

	m_sprite->setPosition(Vec2::ZERO);

	this->addChild(m_sprite, 10);

	this->setLife(1);
	this->setPosition(position);
	this->setDirection(dir);
	this->setSpeed(speed);
	this->scheduleUpdate();

	m_rect = Rect(position.x - 8, position.y - 8, 16, 16);  // rect

	return true;
}


void BossBullet1::update(float delta)
{
	m_rect = Rect(getPositionX() - 4, getPositionY() - 4, 16, 16); // ¸üÐÂrect
	if (this->getLife() <= 0)
	{
		this->unscheduleUpdate();
	}
	if (this->getDirection() == BULLET_DOWN && this->getLife())
	{
		if (this->getPositionY() >= 10)
			this->setPositionY(this->getPositionY() - this->getSpeed());
		else
			Blast();
	}
	if (this->getDirection() == BULLET_LEFT && this->getLife())
	{
		if (this->getPositionY() >= 10)
			this->setPositionY(this->getPositionY() - this->getSpeed());
		else
			Blast();

		if (this->getPositionX() <= WINDOWWIDTH + 10)
			this->setPositionX(this->getPositionX() - this->getSpeed());
		else
			Blast();
	}
	if (this->getDirection() == BULLET_RIGHT && this->getLife())
	{
		if (this->getPositionY() >= 10)
			this->setPositionY(this->getPositionY() - this->getSpeed());
		else
			Blast();

		if (this->getPositionX() <= WINDOWWIDTH - 10)
			this->setPositionX(this->getPositionX() + this->getSpeed());
		else
			Blast();
	}
}

void BossBullet1::deleteObj(Sprite* obj)
{
	obj->removeFromParent();
	this->removeFromParent();
}
