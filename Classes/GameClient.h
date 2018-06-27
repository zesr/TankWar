#pragma once

#include "cocos2d.h"
#include "Tank.h"
#include "Brick.h"
#include "Sea.h"
#include "Iron.h"
#include "Home.h"
#include"CCShake.h"
#include"GameClient.h"
#include "SimpleAudioEngine.h"
#include"Door.h"
#include"GameData.h"
#include<string>
#include"Player.h"
#include"xBullet.h"
#include "Bullet.h"
#include "Boss.h"
#include "Fail.h"
#include "Start.h"
#include "Enemy1.h"
#include "Enemy2.h"

USING_NS_CC;
using namespace cocos2d;

static int tankcount = 0;     // 记录当前坦克数
static int NET_TAG = 11111;

class GameClient : public Scene
{
public:
	GameClient();
	~GameClient();

	CREATE_FUNC(GameClient);
	virtual bool init();
	static Scene* createScene(GameData input = GameData(0, 1000, 1000, 0));
	void update(float delta);

	// 实现键盘回调
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	
	float nowTime;

private:
	//敌人类型
#define ENEMY1 201
#define ENEMY2 202


	//碰撞检测
	void deadTankBullet(CocosDenshion::SimpleAudioEngine *audio);
	void forTankCollide(CocosDenshion::SimpleAudioEngine *audio);
	void boomerangCollide(Player *input, CocosDenshion::SimpleAudioEngine *audio);
	void bossCollide(CocosDenshion::SimpleAudioEngine *audio);
	void clearBlock(CocosDenshion::SimpleAudioEngine *audio);

	//游戏界面操作
	void returnToTitle();
	void gameRestart();

	//地图绘制
	void createBackGround();
	void drawMap1();
	void drawMap2();
	void drawBoss();
	void drawBossBG(Vec2 position);
	void drawBigBG(Vec2 position);

	//时间记录
	void mTimer(float delta) {
		nowTime += delta;
	}
	
	void fail();
	void win();

	//偷家怪物生成
	void monsterManager();

	//玩家杀手生成与AI
	void monsterManager1();

	//TMXLayer * m_mapLayer;
	Label *pauseLabel;//暂停标签
	Door *door;
	Home *home;
	Boss *boss;
	LoadingBar *BossHp;//Boss血条
	Label* xpLabel1;//经验值1
	Label* xpLabel2;//经验值2
	Label *timeLabel;//时间显示
	bool noMonster;//是否刷怪
	bool isFail;
	bool isWin;
	int enemyCount;//敌人编号，用于设置ID
	int lastTime;
	
	Vector<Brick*>  m_bgList;     // 砖块列表
	Vector<Iron*>  m_irList;     // 铁块列表
	Vector<Sea*>  m_seaList;     // 铁块列表
	Vector<Tank*>   m_tankList;   // 坦克列表
	Vector<Enemy1*>   ene1List;   // 坦克列表
	Vector<Enemy2*>   ene2List;   // 坦克列表
	Player*           player1;    //两个玩家
	Player*           player2;
	Vector<Tank*>	m_drawList;   // 已绘制的坦克
	Vector<Bullet*> bulletList;   // 被删除的坦克的子弹

	Vector<xBullet*> m_deleteXBulletList;   // 删除回旋镖列表
	Vector<Bullet*> m_deleteBulletList;   // 删除子弹列表
	Vector<Brick*>  m_deleteBrickList;    // 删除砖块列表
	Vector<Tank*>   m_deleteTankList;     // 删除坦克列表
	Vector<Iron*>   m_deleteIronList;     // 删除坦克列表
};
