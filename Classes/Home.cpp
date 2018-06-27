#include "Home.h"

Home::Home()
{
	hp = 5;
}

Home::~Home()
{

}

void Home::damage() {
	--hp;
	Texture2D* texture;
	switch (hp) {
	case 3:
		texture = Director::getInstance()->getTextureCache()->addImage("image/map/home3hp.png");
		changeImage(texture);
		break;
	case 1:
		texture = Director::getInstance()->getTextureCache()->addImage("image/map/home1hp.png");
		changeImage(texture);
		break;
	case 0:
		texture = Director::getInstance()->getTextureCache()->addImage("image/map/fail.png");
		changeImage(texture);
		break;
	}
}

bool Home::init(Vec2 position)
{
	if (!Sprite::init())
	{
		return false;
	}

	m_sprite = Sprite::create("image/map/home.png",Rect(0, 0, 32, 32));
	this->addChild(m_sprite);
	m_sprite->setPosition(Vec2::ZERO);

	this->setPosition(position);
	m_rect = Rect(this->getPositionX() - 16, this->getPositionY() - 16, 32, 32);
	
	return true;
}

Home* Home::create(Vec2 position)
{
	Home* pRet = new(std::nothrow) Home();
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

void Home::Blast()
{
	this->setVisible(false);   // ×©¿éÏûÊ§
	this->removeFromParent();  
}

