#include"Enemy1.h"
#include "cocos2d.h"

Enemy1* Enemy1::create(int ID, float x, float y, int dir, int kind,int dir2)
{
	Enemy1* pRet = new(std::nothrow) Enemy1();
	if (pRet && pRet->init(ID, x, y, dir, kind, dir2))
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

bool Enemy1::init(int ID, float x, float y, int dir, int kind) {
	return true;
}
bool Enemy1::init(int ID, float x, float y, int dir, int kind,int dir2)
{
	if (!BaseObject::init())
	{
		return false;
	}

	lastX = x;
	lastTime1 = 0;
	fireDir = dir2;
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

	// 坦克初始状态
	m_sprite = Sprite::createWithTexture(m_texture, Rect(m_textureX - 14.0, m_textureY - 14.0, 28, 28));
	m_rect = Rect(this->getPositionX() - 16, this->getPositionY() - 16, 32, 32);
	m_sprite->setPosition(Vec2::ZERO);
	m_sprite->setScale(TANKSIZE / 28);
	this->addChild(m_sprite);

	this->schedule(schedule_selector(Enemy1::mTimer), 1.0f);//开启一个每1s执行一次的计时器  
	this->scheduleUpdate();
	return true;
}

void Enemy1::update(float t)
{
	//偷家
	if (getPosition().y > 14)
		MoveDown();
	else
		if (fireDir == TANK_RIGHT) {
			MoveRight();
			Fire(nt);
		}
		else {
			MoveLeft();
			Fire(nt);
		}

	m_isMoving = m_moveUp | m_moveDown | m_moveLeft | m_moveRight;				 // 更新移动状态
	m_rect = Rect(this->getPositionX() - 16, this->getPositionY() - 16, 32, 32); // 更新rect

	for (int i = 0; i < m_bulletList.size(); i++)
	{
		auto nowBullet = m_bulletList.at(i);
		if (nowBullet->getLife() <= 0)
		{
			m_bulletList.eraseObject(nowBullet);
		}
	}
	for (int i = 0; i < x_bulletList.size(); i++)
	{
		auto nowBullet = x_bulletList.at(i);
		if (nowBullet->getLife() <= 0)
		{
			x_bulletList.eraseObject(nowBullet);
		}
	}

	if (this->getLife() <= 0)
	{
		this->unscheduleUpdate();
	}
	if (m_moveUp)
	{
		m_textureY = 1 * 14;
		if ((this->getPositionY() <= WINDOWHEIGHT - 14) && (this->getHindered() != TANK_UP))
		{
			setHindered(TANK_STAY);
			this->setPositionY(this->getPositionY() + this->getSpeed());
		}
		Draw();
	}
	if (m_moveDown)
	{
		m_textureY = 5 * 14;
		if ((this->getPositionY() >= 14) && (this->getHindered() != TANK_DOWN))
		{
			setHindered(TANK_STAY);
			this->setPositionY(this->getPositionY() - this->getSpeed());
		}
		Draw();
	}
	if (m_moveLeft)
	{
		m_textureY = 7 * 14;
		if ((this->getPositionX() >= 14) && (this->getHindered() != TANK_LEFT))
		{
			setHindered(TANK_STAY);
			this->setPositionX(this->getPositionX() - this->getSpeed());
		}
		Draw();
	}
	if (m_moveRight)
	{
		m_textureY = 3 * 14;
		if ((this->getPositionX() <= WINDOWWIDTH - 14) && (this->getHindered() != TANK_RIGHT))
		{
			setHindered(TANK_STAY);
			this->setPositionX(this->getPositionX() + this->getSpeed());
		}
		Draw();
	}
}

