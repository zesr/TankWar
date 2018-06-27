#pragma once
#ifndef __TANK_H__
#define __TANK_H__

#include "BaseObject.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Global.h"
#include "Bullet.h"
#include "SimpleAudioEngine.h"
#include "xBullet.h"

USING_NS_CC;
using namespace cocos2d;
using namespace cocostudio;
using namespace cocos2d::ui;

#define TANKSPEED    2    // TANK默认速度
#define TANKSIZE    32    // TANK尺寸大小
#define TANK_UP      1	  // TANK向上状态
#define TANK_DOWN    2    // TANK向下状态
#define TANK_LEFT    3    // TANK向左状态
#define TANK_RIGHT   4    // TANK向右状态
#define TANK_STAY    5    // TANK停止状态

class Tank : public BaseObject
{
public:
	Tank(){}

	static Tank* create(int ID, float x, float y, int dir, int kind);
	void MoveUP();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void Fire(float time);
	void Stay(int dir);
	void Blast();     // 爆炸时已自动设置life为0

	bool isMoving() { return m_isMoving; }
	Rect getRect() { return m_rect; }
	Vector<Bullet*> getBulletList() { return m_bulletList; }
	Vector<xBullet*> getXBulletList() { return x_bulletList; }
	int getXp() {
		return xp;
	}
	void addXp(int input = 1000) {
		xp += input;
	}

	void damage() {
		xp /= 2;
	}
	void setXp(int input) {
		xp=input;
	}

protected:
	/*virtual */bool init(int ID, float x, float y, int dir, int kind);
	void Draw();
	void update(float t);
	void deleteObj(Sprite* obj);
	
	int xp;
	int lastTime1;
	Sprite*     m_sprite;          // 图片精灵
	Texture2D*  m_texture;         // 保存texture
	Vector<Bullet*> m_bulletList;  // 子弹列表
	Vector<xBullet*> x_bulletList;  // 子弹列表

	bool  m_moveUp;             // 坦克往上移动
	bool  m_moveDown;           // 坦克往下移动
	bool  m_moveLeft;           // 坦克往左移动
	bool  m_moveRight;			// 坦克往右移动

	float m_frametime;			// 切换图片时间步长
	float m_temptime;			// 每经过temptime切换图片
	int   m_texchange;			// 坦克履带纹理切换控制

	Rect  m_rect;               // 坦克包围框
	bool  m_isMoving;           // 坦克正在移动

	int   m_textureX;           
	int   m_textureY;
};

#endif