#pragma once
#ifndef __PARTICLE_PIC_H__
#define __PARTICLE_PIC_H__

#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d;
#define SLICES	100												// 分为 128*128片
#define PICSIZE 256
typedef struct
{
	float	life;												// 粒子生命
	float	fade;												// 粒子衰减速度
	float	r, g, b;											// 粒子颜色
	float	x, y, z;											// 粒子位置
	float	time;												// 上抛运动经历的时间
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
	float   m_upcastSpeed ;			// 粒子上抛初始速度
	bool    m_showPic;				// 显示
	bool    m_complete;				// 是否播放结束					
};

#endif