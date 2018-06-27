#include "ParticlePic.h"

//************************************
// Method:    PriticlePic
// FullName:  PriticlePic::PriticlePic
// Access:    public 
// Returns:   
// Qualifier:
//************************************
ParticlePic::ParticlePic()
{

}

//************************************
// Method:    ~PriticlePic
// FullName:  PriticlePic::~PriticlePic
// Access:    public 
// Returns:   
// Qualifier:
//************************************
ParticlePic::~ParticlePic()
{

}

//************************************
// Method:    create
// FullName:  PriticlePic::create
// Access:    public static 
// Returns:   PriticlePic*
// Qualifier:
// Parameter: const char * pic
//************************************
ParticlePic* ParticlePic::create(const char* pic)
{
	ParticlePic* pRet = new(std::nothrow) ParticlePic();
	if (pRet && pRet->init(pic))
	{
		pRet->autorelease();
		return pRet;
	} else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

//************************************
// Method:    init
// FullName:  PriticlePic::init
// Access:    virtual private 
// Returns:   bool
// Qualifier:
// Parameter: const char * pic
//************************************
bool ParticlePic::init(const char* pic)
{
	if (!Layer::init())
	{
		return false;
	}
	
	m_upcastSpeed = 2.4f;	// 粒子上抛初始速度
	m_showPic = false;

	initPicTexture(pic);    // 初始化图片
	//this->scheduleUpdate();
	return true;
}

//************************************
// Method:    update
// FullName:  PriticlePic::update
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: float delta
//************************************
void ParticlePic::update(float delta)
{
	drawPic();
}

//************************************
// Method:    addParticles
// FullName:  ParticlePic::addParticles
// Access:    public 
// Returns:   bool
// Qualifier:
//************************************
bool ParticlePic::run()
{
	m_wait = 0;  // 初始化控制变量
	m_complete = false;
	Size visibleSize = Director::getInstance()->getVisibleSize();

	for (int i = 0; i < SLICES; i++)							// 贴图纵向分割为SLICES行
	{
		for (int j = 0; j < SLICES; j++)					    // 贴图横向分割为SLICES列
		{
			m_sprites[i][j]->setVisible(true);
			m_sprites[i][j]->setPosition(visibleSize.width / 2 - PICSIZE / 2 + (i * PICSIZE / SLICES), visibleSize.height / 2 + PICSIZE / 2 - (j * PICSIZE / SLICES));
			m_particlespics[i][j].life = 0.5f;								// 初始化生命为0.2
			m_particlespics[i][j].fade = rand_0_1() / 10.0f + 0.003f;       // 随机生命衰减速度
			m_particlespics[i][j].r = 0.0f;									// 粒子颜色红色分量
			m_particlespics[i][j].g = 255.0f;								// 粒子颜色绿色分量
			m_particlespics[i][j].b = 255.0f;								// 粒子颜色蓝色分量
			m_particlespics[i][j].x = float(visibleSize.width / 2 - PICSIZE/2 + (i * PICSIZE / SLICES));    // x轴位置
			m_particlespics[i][j].y = float(visibleSize.height / 2 + PICSIZE / 2 - (j * PICSIZE / SLICES)); // Y轴位置
			m_particlespics[i][j].z = 0.0f;									// Z轴位置
			m_particlespics[i][j].time = 0.0f;								// 上抛运动经历的时间
			m_particlespics[i][j].timeg = rand_0_1() / 5.0f + 0.05f;		// 随机时间变化单位值

			m_sprites[i][j]->setColor(Color3B(m_particlespics[i][j].r, m_particlespics[i][j].g, m_particlespics[i][j].b));

		}
	}
	this->scheduleUpdate();
	return true;
}

//************************************
// Method:    initPicTexture
// FullName:  ParticlePic::initPicTexture
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void ParticlePic::initPicTexture(const char* pic)
{
	Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(pic);
	for (int i = 0; i < SLICES; i++)
	{
		for (int j = 0; j < SLICES;j++)
		{
			m_sprites[i][j] = Sprite::createWithTexture(texture, Rect(i * PICSIZE / SLICES, j * PICSIZE / SLICES, PICSIZE / SLICES, PICSIZE / SLICES));
			addChild(m_sprites[i][j]);
			m_sprites[i][j]->setAnchorPoint(Point::ZERO);
		}
	}
}

//************************************
// Method:    drawPic
// FullName:  ParticlePic::drawPic
// Access:    public 
// Returns:   bool
// Qualifier:
//************************************
bool ParticlePic::drawPic()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	bool flag = false;

	if (m_wait % 300 >= 0 && m_wait % 300 <= 154)						
	{
		m_showPic = false;
	}
	if (m_wait % 300 >= 155 && m_wait % 300 <= 299)						
	{
		m_showPic = true;

	}
	m_wait += 1;	  // 控制变量自加1

	float newHeight;

	for (int i = 0; i < SLICES; ++i)
	{
		for (int j = 0; j < SLICES; ++j)
		{

			if (m_particlespics[i][j].life<0)
			{
				m_sprites[i][j]->setVisible(false);

			}
			
			// 更新粒子的位置等属性
			m_particlespics[i][j].time += m_particlespics[i][j].timeg;	

			m_particlespics[i][j].r = 0.0f + rand()%256;									// 粒子颜色红色分量
			m_particlespics[i][j].g = 255.0f - rand()%256;								// 粒子颜色绿色分量
			m_particlespics[i][j].b = 255.0f - rand()%256;								// 粒子颜色蓝色分量
			
			// 自由上抛公式
			newHeight = m_upcastSpeed*m_particlespics[i][j].time - 0.5f*0.98f*m_particlespics[i][j].time*m_particlespics[i][j].time;
			
			// 0.2*
			m_particlespics[i][j].y += newHeight;
			
			// 下落时粒子生命开始衰减
			if (newHeight < 0.0f)
			{
				m_particlespics[i][j].life -= m_particlespics[i][j].fade;
			}

			// 粒子生命耗尽
			if (m_showPic && m_particlespics[i][j].life < 0.0f)					
			{
				this->setVisible(false);
				m_complete = true;
				this->unscheduleUpdate();
				return true;
			}
			m_sprites[i][j]->setPosition(m_particlespics[i][j].x, m_particlespics[i][j].y);
			m_sprites[i][j]->setColor(Color3B(m_particlespics[i][j].r, m_particlespics[i][j].g, m_particlespics[i][j].b));
		}
	}
	return false;
}

//************************************
// Method:    setVisible
// FullName:  ParticlePic::setVisible
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: bool visible
//************************************
void ParticlePic::setVisible(bool visible)
{
	for (int i = 0; i < SLICES; ++i)
	{
		for (int j = 0; j < SLICES; ++j)
		{
			if (m_sprites[i][j]->isVisible())
			{
				m_sprites[i][j]->setVisible(visible);
			}
		}
	}
}

