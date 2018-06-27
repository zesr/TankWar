#include"xBullet.h"

xBullet::xBullet() {
	startTime = 0;
}
xBullet::~xBullet(){}






xBullet* xBullet::create(Vec2 position, float speed, int dir)
{
	xBullet* pRet = new(std::nothrow) xBullet();
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

bool xBullet::init(Vec2 position, float speed, int dir)
{
	if (!BaseObject::init())
	{
		return false;
	}

	//类型是回旋镖，记录开始时间来计算回旋时间，记录可以打破的砖块个数
	hp = 4;

	m_texture = Director::getInstance()->getTextureCache()->addImage("image/tank/xbullet.png");
	m_sprite = Sprite::createWithTexture(m_texture, Rect(0, 0, 32, 32));

	m_sprite->setPosition(Vec2::ZERO);

	this->addChild(m_sprite, 10);

	this->setLife(1);
	this->setPosition(position);
	this->setDirection(dir);
	this->setSpeed(speed);
	this->scheduleUpdate();

	m_rect = Rect(position.x - 16, position.y - 16, 32, 32);  // rect

	//转
	this->runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 180.0f)));

	return true;
}

void xBullet::deleteObj(Sprite* obj)
{
	obj->removeFromParent();
	this->removeFromParent();
}

void xBullet::goBack() {
	backTime = 999999;
	switch (getDirection()) {
	case BULLET_UP:
		setDirection(BULLET_DOWN);
		break;
	case BULLET_DOWN:
		setDirection(BULLET_UP);
		break;
	case BULLET_LEFT:
		setDirection(BULLET_RIGHT);
		break;
	case BULLET_RIGHT:
		setDirection(BULLET_LEFT);
		break;
	}
}
