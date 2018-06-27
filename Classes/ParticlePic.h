#pragma once
#ifndef __PARTICLE_PIC_H__
#define __PARTICLE_PIC_H__

#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d;
#define SLICES	100												// ��Ϊ 128*128Ƭ
#define PICSIZE 256
typedef struct
{
	float	life;												// ��������
	float	fade;												// ����˥���ٶ�
	float	r, g, b;											// ������ɫ
	float	x, y, z;											// ����λ��
	float	time;												// �����˶�������ʱ��
	float	timeg;
}PICS;

class ParticlePic : public Layer
{
public:
	ParticlePic();
	~ParticlePic();

	static ParticlePic* create(const char* pic);
	bool run();
	bool isComplete() { return m_complete; }

private:
	virtual bool init(const char* pic);
	void update(float delta);
	void setVisible(bool visible);
	void initPicTexture(const char* pic);
	bool drawPic();

private:
	PICS    m_particlespics[SLICES][SLICES];
	Sprite* m_sprites[SLICES][SLICES];
	int     m_wait;
	float   m_upcastSpeed ;			// �������׳�ʼ�ٶ�
	bool    m_showPic;				// ��ʾ
	bool    m_complete;				// �Ƿ񲥷Ž���					
};

#endif