#pragma once
#ifndef __Sea_H__
#define __Sea_H__

#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d;

class Sea : public Sprite
{
public:
	Sea();
	~Sea();
	static Sea* create(Vec2 position);
	virtual bool init(Vec2 position);
	void Blast();

	// get
	Rect getRect() { return m_rect; };

private:
	Sprite* m_sprite;  // ש�龫��
	Rect m_rect;       // ��Χ��
};

#endif