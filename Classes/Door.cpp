#include "door.h"
#include "ui/CocosGUI.h"
Door::Door()
{

}

Door::~Door()
{

}

bool Door::init(Vec2 position)
{
	if (!Sprite::init())
	{
		return false;
	}

	m_sprite = Sprite::create("image/map/door.png",Rect(0, 0, 32, 32));

	this->addChild(m_sprite,1);
	m_sprite->setPosition(Vec2::ZERO);

	this->setPosition(position);
	m_rect = Rect(this->getPositionX() - 16, this->getPositionY() - 16, 32, 32);

	return true;
}

Door* Door::create(Vec2 position)
{
	Door* pRet = new(std::nothrow) Door();
	if (pRet && pRet->init(position))
	{
		pRet->autorelease();
		return pRet;
	} else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

void Door::Blast()
{
	this->setVisible(false);   // ×©¿éÏûÊ§
	this->removeFromParent();  
}

