#pragma once
#include"Bullet.h"
#include "cocos2d.h"


//ÆÕÍ¨µ¯Ä»
class BossBullet1 :public Bullet {

public:
	BossBullet1();
	~BossBullet1();

	bool init(Vec2 position, float speed, int dir);
	static BossBullet1* create(Vec2 position, float speed, int dir);
	void deleteObj(Sprite* obj);
	void update(float delta);

};