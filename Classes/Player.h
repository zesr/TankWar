#pragma once
#include "Tank.h"
#include"xBullet.h"
class Player :public Tank {
public:
	static Player * create(int ID, float x, float y, int dir, int kind);

	void startRush();
	void stopRush();
	bool isRush;
	void xFire(float time);
	void Blast();
};