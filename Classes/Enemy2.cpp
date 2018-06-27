

#include "Enemy2.h"

Enemy2* Enemy2::create(int ID, float x, float y, int dir, int kind)
{
	Enemy2* pRet = new(std::nothrow) Enemy2();
	if (pRet && pRet->init(ID, x, y, dir, kind))
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

bool Enemy2::init(int ID, float x, float y, int dir, int kind)
{
	if (!BaseObject::init())
	{
		return false;
	}

	lastTime1 = 0;
	xp = MAXHP;
	setLife(1); // tank xlife
	setSpeed(TANKSPEED);   // tank speed
	setLevel(1);           // tank level
	setHindered(TANK_STAY);    // tank hindered
	m_moveRight = FALSE;
	m_moveLeft = FALSE;

	m_isMoving = false;

	// tank init ifo
	setID(ID);
	this->setPositionX(x);
	this->setPositionY(y);
	setDirection(dir);
	setKind(kind);

	m_texture = Director::getInstance()->getTextureCache()->addImage("image/tank/enemy.png");

	// ̹�˳�ʼ״̬
	m_sprite = Sprite::createWithTexture(m_texture, Rect(m_textureX - 14.0, m_textureY - 14.0, 28, 28));
	m_rect = Rect(this->getPositionX() - 16, this->getPositionY() - 16, 32, 32);
	m_sprite->setPosition(Vec2::ZERO);
	m_sprite->setScale(TANKSIZE / 28);
	this->addChild(m_sprite);

	this->scheduleUpdate();
	return true;
}
