#pragma once
#ifndef __BASE_OBJECT_H__
#define __BASE_OBJECT_H__

#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d;

class BaseObject : public Sprite
{
public:
	BaseObject() {};
	virtual ~BaseObject() {};
	
	CREATE_FUNC(BaseObject);
	virtual bool init();

	// set and get
	int		getLife()      { return m_life ; };
	float	getSpeed()     { return m_speed; };
	int		getKind()      { return m_kind ; };
	int     getDirection() { return m_direction; };
	int     getLevel()     { return m_level;     };
	int     getID()        { return m_ID;        };
	int     getHindered()  { return m_hindered;  };

	void	setLife(int l)        { m_life = l;      };
	void	setSpeed(float s)     { m_speed = s;     };
	void    setDirection(float d) { m_direction = d; };
	void    setKind(int k)        { m_kind = k;      };
	void    setLevel(int lev)     { m_level = lev;   };
	void    setID(int id)         { m_ID = id;       };
	void    setHindered(int hindered)   { m_hindered = hindered; };
	
private:
	int   m_ID;             // OBJECT��ID
	float m_speed;			// OBJECT���ƶ��ٶ�
	int   m_life;			// OBJECT������ֵ(����Чֵ)
	int   m_direction;		// OBJECT�ƶ��ķ���
	int   m_kind;			// OBJECT����������,�ڲ�ͬ�����������в�ͬ����˼
	int   m_level;          // OBJECT�ȼ�
	int   m_hindered;       // OBJECT���ĸ��������Ƿ��ܵ��谭up.down.left.right
};
#endif