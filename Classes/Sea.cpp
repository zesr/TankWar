#include "Sea.h"

Sea::Sea()
{

}

Sea::~Sea()
{

}

bool Sea::init(Vec2 position)
{
	if (!Sprite::init())
	{
		return false;
	}

	m_sprite = Sprite::create("Image/map/sea.png",Rect(0, 0, 16, 16));
	
	this->addChild(m_sprite);
	m_sprite->setPosition(Vec2::ZERO);

	this->setPosition(position);
	m_rect = Rect(this->getPositionX() - 8, this->getPositionY() - 8, 16, 16);
	
	return true;
}

Sea* Sea::create(Vec2 position)
{
	Sea* pRet = new(std::nothrow) Sea();
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

void Sea::Blast()
{
	this->setVisible(false);   // ×©¿éÏûÊ§
	this->removeFromParent();  
}

