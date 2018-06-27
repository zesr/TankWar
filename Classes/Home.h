#pragma once
#ifndef __Home_H__
#define __Home_H__

#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d;

class Home : public Sprite
{
public:
	Home();
	~Home();
	static Home* create(Vec2 position);
	virtual bool init(Vec2 position);
	void Blast();

	// get
	Rect getRect() { return m_rect; };
	void damage();
	short getHp() { return hp; }
	void changeImage(Texture2D* input) {
		m_sprite->setTexture(input);
	}

private:
	short hp;
	Sprite* m_sprite;  // ×©¿é¾«Áé
	Rect m_rect;       // °üÎ§¿ò
};

#endif