#pragma once
#include"Bullet.h"
#include "cocos2d.h"

//³´¼¦µ¯Ä»£¬´©Í¸
class BossBullet2 :public Bullet {

public:
	BossBullet2();
	~BossBullet2();

	bool init(Vec2 position, float speed, int dir);
	static BossBullet2* create(Vec2 position, float speed, int dir);
	void deleteObj(Sprite* obj);

};