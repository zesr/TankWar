#pragma once
#include"Bullet.h"
#include "cocos2d.h"

class xBullet :public Bullet {

public:
	xBullet();
	~xBullet();

	bool init(Vec2 position, float speed, int dir);
	static xBullet* create(Vec2 position, float speed, int dir);
	void deleteObj(Sprite* obj);

	void goBack();

	int backTime = 2;
	int startTime;
	int hp;
	
};