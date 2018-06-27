#pragma once
#include "Enemy1.h"
#include "Tank.h"
class Enemy1 :public Tank {

#define MAXHP 2000

	bool init(int ID, float x, float y, int dir, int kind,int dir2);
	bool init(int ID, float x, float y, int dir, int kind);

	int fireDir;
	int lastX;
	int nt;
	void update(float t);
public:
	static Enemy1* create(int ID, float x, float y, int dir, int kind,int dir2);
	void mTimer(float input) {
		nt += input;
	}
};