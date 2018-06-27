#include"player.h"
#include "cocos2d.h"
 


Player* Player::create(int ID, float x, float y, int dir, int kind)
{
	Player* pRet = new(std::nothrow) Player();
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


//������
void Player::xFire(float time) {
	if (time - lastTime1 < 0.5)
		return;

	//�ڻ���Ч
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect("vedio/sfire.wav", false, 1.0f, 1.0f, 1.0f);

	//��ͣ��Ϸ
	if (CCDirector::sharedDirector()->isPaused())
		return;
	Vec2 position;
	switch (this->getDirection())
	{
	case TANK_UP:
		position = Vec2(this->getPositionX(), this->getPositionY() + 14);
		break;
	case TANK_DOWN:
		position = Vec2(this->getPositionX(), this->getPositionY() - 14);
		break;
	case TANK_LEFT:
		position = Vec2(this->getPositionX() - 14, this->getPositionY());
		break;
	case TANK_RIGHT:
		position = Vec2(this->getPositionX() + 14, this->getPositionY());
		break;
	}
	auto bullet = xBullet::create(position, 3, this->getDirection());
	x_bulletList.pushBack(bullet);            // ��ӵ��ӵ��б�
	if(this->getParent()!=nullptr)
		this->getParent()->addChild(bullet, 8);   // ��ӵ���Ϸ����


}


Texture2D*  saveTexture;         // ����texture

//������
void Player::startRush() {

	setSpeed(getSpeed() * 3);
	isRush = true;

	//̹�˱�ɺ�ɫ��
	saveTexture = m_texture;
	m_texture = Director::getInstance()->getTextureCache()->addImage("image/tank/TankSpeedUp.png");

	//����Ч��
	auto emitter = ParticleFire::create();
	emitter->setPosition(Vec2(0, 0));
	addChild(emitter, 10, 0);

	//�жϳ�̷���
	switch (getDirection()) {
	case TANK_LEFT:
		MoveLeft();
		break;
	case TANK_RIGHT:
		MoveRight();
		break;
	case TANK_DOWN:
		MoveDown();
		break;
	case TANK_UP:
		MoveUP();
		break;
	}
}

void Player::stopRush() {
	m_texture = saveTexture;

	auto g=getChildByTag(0);
	this->removeChild(g,1);

	setSpeed(getSpeed() / 3);
	isRush = false;
}

void Player::Blast() {
	Tank::Blast();
	isRush = false;
}
