#pragma once

#include "BaseObject.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Global.h"
#include "SimpleAudioEngine.h"
#include"BossBullet1.h"
#include"BossBullet2.h"

USING_NS_CC;
using namespace cocos2d;
using namespace cocostudio;
using namespace cocos2d::ui;

#define TANKSPEED    1    // TANK默认速度
#define TANKSIZE    32    // TANK尺寸大小
#define TANK_LEFT    3    // TANK向左状态
#define TANK_RIGHT   4    // TANK向右状态
#define TANK_STAY    5    // TANK停止状态
#define ATTACKTYPE1 6
#define ATTACKTYPE2 7
#define ATTACKTYPE3 8

class Boss : public BaseObject
{
public:
	Boss() {}

	bool isAttack3;
	int maxXp;

	static Boss* create(int ID, float x, float y, int dir, int kind);
	void MoveLeft();
	void MoveRight();
	void Fire();
	void Fire2();
	void Stay(int dir);
	void Blast();     // 爆炸时已自动设置life为0

	bool isMoving() { return m_isMoving; }
	Rect getRect() { return m_rect; }
	Vector<BossBullet1*> getBulletList1() { return bulletList1; }
	Vector<BossBullet2*> getBulletList2() { return bulletList2; }

	int getXp() {
		return xp;
	}
	void damage();
	void setXp(int input) {
		xp = input;
	}

	void AI(int time, Vec2 position1, Vec2 position2);

protected:

	Vec2 playerPosition;
	bool  m_moveLeft;           // 坦克往左移动
	bool  m_moveRight;			// 坦克往右移动
	int lastTime2;
	int nowTime;

	int xp;
	Rect  m_rect;               // 坦克包围框
	bool  m_isMoving;           // 坦克正在移动
	bool isAlive;
	int attackType;

	bool init(int ID, float x, float y, int dir, int kind);
	void update(float t);
	void deleteObj(Sprite* obj);
	void Draw();

	Sprite*     m_sprite;          // 图片精灵
	Texture2D*  m_texture;         // 保存texture
	Vector<BossBullet1*> bulletList1;  // 子弹列表
	Vector<BossBullet2*> bulletList2;  // 子弹列表

	void attackType1();
	void attackType2();
	void attackType3();
};
