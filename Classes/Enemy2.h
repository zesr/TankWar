#pragma once
#include "Tank.h"
#include "Player.h"
class Enemy2 :public Tank {

#define MAXHP 2000

public:
	Player* target;
	static Enemy2 *Enemy2::create(int ID, float x, float y, int dir, int kind);
	bool init(int ID, float x, float y, int dir, int kind);
};