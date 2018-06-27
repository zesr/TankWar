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

static int tankcount = 0;     // ��¼��ǰ̹����
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

	// ʵ�ּ��̻ص�
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	
	float nowTime;

private:
	//��������
#define ENEMY1 201
#define ENEMY2 202


	//��ײ���
	void deadTankBullet(CocosDenshion::SimpleAudioEngine *audio);
	void forTankCollide(CocosDenshion::SimpleAudioEngine *audio);
	void boomerangCollide(Player *input, CocosDenshion::SimpleAudioEngine *audio);
	void bossCollide(CocosDenshion::SimpleAudioEngine *audio);
	void clearBlock(CocosDenshion::SimpleAudioEngine *audio);

	//��Ϸ�������
	void returnToTitle();
	void gameRestart();

	//��ͼ����
	void createBackGround();
	void drawMap1();
	void drawMap2();
	void drawBoss();
	void drawBossBG(Vec2 position);
	void drawBigBG(Vec2 position);

	//ʱ���¼
	void mTimer(float delta) {
		nowTime += delta;
	}
	
	void fail();
	void win();

	//͵�ҹ�������
	void monsterManager();

	//���ɱ��������AI
	void monsterManager1();

	//TMXLayer * m_mapLayer;
	Label *pauseLabel;//��ͣ��ǩ
	Door *door;
	Home *home;
	Boss *boss;
	LoadingBar *BossHp;//BossѪ��
	Label* xpLabel1;//����ֵ1
	Label* xpLabel2;//����ֵ2
	Label *timeLabel;//ʱ����ʾ
	bool noMonster;//�Ƿ�ˢ��
	bool isFail;
	bool isWin;
	int enemyCount;//���˱�ţ���������ID
	int lastTime;
	
	Vector<Brick*>  m_bgList;     // ש���б�
	Vector<Iron*>  m_irList;     // �����б�
	Vector<Sea*>  m_seaList;     // �����б�
	Vector<Tank*>   m_tankList;   // ̹���б�
	Vector<Enemy1*>   ene1List;   // ̹���б�
	Vector<Enemy2*>   ene2List;   // ̹���б�
	Player*           player1;    //�������
	Player*           player2;
	Vector<Tank*>	m_drawList;   // �ѻ��Ƶ�̹��
	Vector<Bullet*> bulletList;   // ��ɾ����̹�˵��ӵ�

	Vector<xBullet*> m_deleteXBulletList;   // ɾ���������б�
	Vector<Bullet*> m_deleteBulletList;   // ɾ���ӵ��б�
	Vector<Brick*>  m_deleteBrickList;    // ɾ��ש���б�
	Vector<Tank*>   m_deleteTankList;     // ɾ��̹���б�
	Vector<Iron*>   m_deleteIronList;     // ɾ��̹���б�
};
