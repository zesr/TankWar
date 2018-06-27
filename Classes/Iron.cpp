#include "Iron.h"

Iron::Iron()
{

}

Iron::~Iron()
{

}

bool Iron::init(Vec2 position)
{
	if (!Sprite::init())
	{
		return false;
	}

	m_sprite = Sprite::create("image/map/iron.png",Rect(0, 0, 16, 16));
	
	this->addChild(m_sprite);
	m_sprite->setPosition(Vec2::ZERO);

	this->setPosition(position);
	m_rect = Rect(this->getPositionX() - 8, this->getPositionY() - 8, 16, 16);
	
	return true;
}

Iron* Iron::create(Vec2 position)
{
	Iron* pRet = new(std::nothrow) Iron();
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

void Iron::Blast()
{
	this->setVisible(false);   // Ìú¿éÏûÊ§
	this->removeFromParent();  
}

