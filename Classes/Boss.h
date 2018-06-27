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

#define TANKSPEED    1    // TANKĬ���ٶ�
#define TANKSIZE    32    // TANK�ߴ��С
#define TANK_LEFT    3    // TANK����״̬
#define TANK_RIGHT   4    // TANK����״̬
#define TANK_STAY    5    // TANKֹͣ״̬
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
	void Blast();     // ��ըʱ���Զ�����lifeΪ0

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
	bool  m_moveLeft;           // ̹�������ƶ�
	bool  m_moveRight;			// ̹�������ƶ�
	int lastTime2;
	int nowTime;

	int xp;
	Rect  m_rect;               // ̹�˰�Χ��
	bool  m_isMoving;           // ̹�������ƶ�
	bool isAlive;
	int attackType;

	bool init(int ID, float x, float y, int dir, int kind);
	void update(float t);
	void deleteObj(Sprite* obj);
	void Draw();

	Sprite*     m_sprite;          // ͼƬ����
	Texture2D*  m_texture;         // ����texture
	Vector<BossBullet1*> bulletList1;  // �ӵ��б�
	Vector<BossBullet2*> bulletList2;  // �ӵ��б�

	void attackType1();
	void attackType2();
	void attackType3();
};
