#include "Boss.h"

Boss* Boss::create(int ID, float x, float y, int dir, int kind)
{
	Boss* pRet = new(std::nothrow) Boss();
	if (pRet && pRet->init(ID, x, y, dir, kind))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

//扇形弹幕
void Boss::attackType1() {
	//移动
	static int state=0;
	if (state == 0) {
		m_texture = Director::getInstance()->getTextureCache()->addImage("image/tank/bossnormal.png");
		Draw();
	}
	setSpeed(6);
	if (state==1&&this->getPositionX() > 160)
		MoveLeft();
	else if (state!=3&&this->getPositionX() < WINDOWWIDTH - 160) {
		state = 2;
		MoveRight();
	}
	else {
		state = 3;
		if (this->getPositionX() > WINDOWWIDTH / 2)
			MoveLeft();
		else {
			state = 0;
			attackType = 0;
		}
	}

	//每秒开火
	if (nowTime - lastTime2 == 0)
		return;
	lastTime2 = nowTime;
	Fire();
}

//追踪弹幕
void Boss::attackType2() {
	static int beginTime = nowTime;
	static int count = 0;
	if (count == 0) {
		m_texture = Director::getInstance()->getTextureCache()->addImage("image/tank/bossskill3.png");
		Draw();
	}
	if (count == 5) {
		attackType = 0;
		count = 0;
		return;
	}
	if (nowTime - beginTime) {
		beginTime = nowTime;
		Fire2();
		++count;
	}
	setSpeed(10);
	if (playerPosition.x - getPositionX() < 20)
		MoveLeft();
	else if (playerPosition.x - getPositionX() > 20)
		MoveRight();
}

//围住玩家
void Boss::attackType3() {
	static int beginTime=-1;
	if (beginTime == -1) {
		beginTime = nowTime;
		isAttack3 = true;
		m_texture = Director::getInstance()->getTextureCache()->addImage("image/tank/bosskill1.png");
		Draw();
	}
	if (nowTime - beginTime >= 3) {
		attackType = ATTACKTYPE2;
		beginTime = -1;
	}
}

void Boss::damage() {
	if (!isAlive)
		return;
	xp -= 1000;
	if (xp <= 0) {
		isAlive = false;
		Blast();
	}
}

bool Boss::init(int ID, float x, float y, int dir, int kind)
{
	if (!BaseObject::init())
	{
		return false;
	}

	maxXp = 30000;
	isAlive = true;
	isAttack3 = false;
	nowTime = 0;
	attackType = ATTACKTYPE1;
	xp = maxXp;
	setLife(1); // tank xlife
	setSpeed(TANKSPEED);   // tank speed
	setLevel(1);           // tank level
	setHindered(TANK_STAY);    // tank hindered
	m_moveRight = FALSE;
	m_moveLeft = FALSE;

	m_isMoving = false;

	// tank init ifo
	setID(ID);
	this->setPositionX(x);
	this->setPositionY(y);
	setDirection(dir);
	setKind(kind);

	m_texture = Director::getInstance()->getTextureCache()->addImage("image/tank/bossnormal.png");

	// 坦克初始状态
	m_sprite = Sprite::createWithTexture(m_texture, Rect(0, 0,  320,320));
	//m_sprite = Sprite::createWithTexture(m_texture, Rect(256, 256, 112, 112));
	m_rect = Rect(this->getPositionX()-160, this->getPositionY()-160, 320, 320);
	m_sprite->setPosition(Vec2::ZERO);
	m_sprite->setScale(TANKSIZE / 28);
	this->addChild(m_sprite);

	this->scheduleUpdate();
	return true;
}

void Boss::MoveLeft()
{
	m_moveLeft = true;
	m_moveRight = false;

	setDirection(TANK_LEFT);
}

void Boss::MoveRight()
{
	m_moveRight = true;
	m_moveLeft = false;

	setDirection(TANK_RIGHT);
}

void Boss::Fire()
{
	//炮击音效
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect("vedio/sfire.wav", false, 1.0f, 1.0f, 1.0f);

	//暂停游戏
	if (CCDirector::sharedDirector()->isPaused())
		return;
	Vec2 position;
	
	//左边子弹
	position = Vec2(this->getPositionX()-14, this->getPositionY() - 14);
	auto bullet = BossBullet1::create(position, 3, BULLET_LEFT);
	bulletList1.pushBack(bullet);            // 添加到子弹列表
	this->getParent()->addChild(bullet, 8);   // 添加到游戏场景

	//右边子弹
	position = Vec2(this->getPositionX()+14, this->getPositionY() - 14);
	bullet = BossBullet1::create(position, 3, BULLET_RIGHT);
	bulletList1.pushBack(bullet);            // 添加到子弹列表
	this->getParent()->addChild(bullet, 8);   // 添加到游戏场景

	 //下子弹
	position = Vec2(this->getPositionX(), this->getPositionY() - 14);
	bullet = BossBullet1::create(position, 3, BULLET_DOWN);
	bulletList1.pushBack(bullet);            // 添加到子弹列表
	this->getParent()->addChild(bullet, 8);   // 添加到游戏场景

	
}

void Boss::Fire2() {
	//炮击音效
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect("vedio/sfire.wav", false, 1.0f, 1.0f, 1.0f);

	//暂停游戏
	if (CCDirector::sharedDirector()->isPaused())
		return;
	Vec2 position;
											  //下子弹
	position = Vec2(this->getPositionX(), this->getPositionY() - 14);
	auto bullet = BossBullet2::create(position, 3, BULLET_DOWN);
	bulletList2.pushBack(bullet);            // 添加到子弹列表
	this->getParent()->addChild(bullet, 8);   // 添加到游戏场景

}

void Boss::Blast()
{
	this->setVisible(false);   // 坦克消失
	this->setLife(0);
	auto explode = Sprite::create("image/tank/dog.png");
	this->getParent()->addChild(explode);
	explode->setPosition(this->getPosition());  // 显示爆炸

	explode->runAction(Sequence::create(
		DelayTime::create(5.0f),
		FadeOut::create(1.0f),                  // 爆炸消失
		CallFunc::create(CC_CALLBACK_0(Boss::deleteObj, this, explode)),
		NULL
	));
}

void Boss::AI(int time,Vec2 position1,Vec2 position2) {

	//获取游戏时间，确定攻击模式
	nowTime = time;
	static int chosenPlayer = -1;

	//选跟踪敌人
	if (attackType == ATTACKTYPE2) {
		if (chosenPlayer == -1)
			chosenPlayer = rand() % 2;
		switch (chosenPlayer) {
		case 0:
			playerPosition = position1;
			break;
		case 1:
			playerPosition = position2;
			break;
		}
	}
	else 
		chosenPlayer = -1;

	if (attackType)
		return;

	switch (rand() % 3) {
	case 0:
		attackType = ATTACKTYPE1;
		break;
	case 1: {
		attackType = ATTACKTYPE2;
		break;
	}
	case 2:
		attackType = ATTACKTYPE3;
		break;
	}
}

void Boss::update(float t)
{
	//攻击
	switch (attackType) {
	case ATTACKTYPE1:
		attackType1();
		break;
	case ATTACKTYPE2:
		attackType2();
		break;
	case ATTACKTYPE3:
		attackType3();
		break;
	}

	//移动
	m_isMoving = m_moveLeft | m_moveRight;				 // 更新移动状态
	m_rect = Rect(this->getPositionX() - 32, this->getPositionY() - 32, 64, 64); // 更新rect

	//子弹飞行
	for (int i = 0; i < bulletList1.size(); i++)
	{
		auto nowBullet = bulletList1.at(i);
		if (nowBullet->getLife() <= 0)
		{
			bulletList1.eraseObject(nowBullet);
		}
	}
	for (int i = 0; i < bulletList2.size(); i++)
	{
		auto nowBullet = bulletList2.at(i);
		if (nowBullet->getLife() <= 0)
		{
			bulletList2.eraseObject(nowBullet);
		}
	}

	if (this->getLife() <= 0)
	{
		this->unscheduleUpdate();
	}

	//左右移动
	if (m_moveLeft)
	{
		if ((this->getPositionX() >= 14) && (this->getHindered() != TANK_LEFT))
		{
			setHindered(TANK_STAY);
			this->setPositionX(this->getPositionX() - this->getSpeed());
		}
	}
	if (m_moveRight)
	{
		if ((this->getPositionX() <= WINDOWWIDTH - 14) && (this->getHindered() != TANK_RIGHT))
		{
			setHindered(TANK_STAY);
			this->setPositionX(this->getPositionX() + this->getSpeed());
		}
	}
}

void Boss::Draw() {
	if (this->getLife()) {
		this->removeChild(m_sprite, true); // 重要：把前一个精灵移除,避免内存无法释放
		m_sprite = Sprite::createWithTexture(m_texture, Rect(0, 0, 320, 320));
		m_sprite->setScale(TANKSIZE / 28);
		this->addChild(m_sprite);          // 更新精灵图片
	}
}

void Boss::Stay(int dir)
{
	switch (dir)
	{
	case TANK_LEFT:
		m_moveLeft = false;
		this->setPositionX(this->getPositionX() + 1);
		break;
	case TANK_RIGHT:
		m_moveRight = false;
		this->setPositionX(this->getPositionX() - 1);
		break;
	}
}

void Boss::deleteObj(Sprite* obj)
{
	obj->removeFromParent();
	this->removeFromParent();

	//狗狗
}
