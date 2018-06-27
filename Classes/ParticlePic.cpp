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
	
	m_upcastSpeed = 2.4f;	// �������׳�ʼ�ٶ�
	m_showPic = false;

	initPicTexture(pic);    // ��ʼ��ͼƬ
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
	m_wait = 0;  // ��ʼ�����Ʊ���
	m_complete = false;
	Size visibleSize = Director::getInstance()->getVisibleSize();

	for (int i = 0; i < SLICES; i++)							// ��ͼ����ָ�ΪSLICES��
	{
		for (int j = 0; j < SLICES; j++)					    // ��ͼ����ָ�ΪSLICES��
		{
			m_sprites[i][j]->setVisible(true);
			m_sprites[i][j]->setPosition(visibleSize.width / 2 - PICSIZE / 2 + (i * PICSIZE / SLICES), visibleSize.height / 2 + PICSIZE / 2 - (j * PICSIZE / SLICES));
			m_particlespics[i][j].life = 0.5f;								// ��ʼ������Ϊ0.2
			m_particlespics[i][j].fade = rand_0_1() / 10.0f + 0.003f;       // �������˥���ٶ�
			m_particlespics[i][j].r = 0.0f;									// ������ɫ��ɫ����
			m_particlespics[i][j].g = 255.0f;								// ������ɫ��ɫ����
			m_particlespics[i][j].b = 255.0f;								// ������ɫ��ɫ����
			m_particlespics[i][j].x = float(visibleSize.width / 2 - PICSIZE/2 + (i * PICSIZE / SLICES));    // x��λ��
			m_particlespics[i][j].y = float(visibleSize.height / 2 + PICSIZE / 2 - (j * PICSIZE / SLICES)); // Y��λ��
			m_particlespics[i][j].z = 0.0f;									// Z��λ��
			m_particlespics[i][j].time = 0.0f;								// �����˶�������ʱ��
			m_particlespics[i][j].timeg = rand_0_1() / 5.0f + 0.05f;		// ���ʱ��仯��λֵ

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
	m_wait += 1;	  // ���Ʊ����Լ�1

	float newHeight;

	for (int i = 0; i < SLICES; ++i)
	{
		for (int j = 0; j < SLICES; ++j)
		{

			if (m_particlespics[i][j].life<0)
			{
				m_sprites[i][j]->setVisible(false);

			}
			
			// �������ӵ�λ�õ�����
			m_particlespics[i][j].time += m_particlespics[i][j].timeg;	

			m_particlespics[i][j].r = 0.0f + rand()%256;									// ������ɫ��ɫ����
			m_particlespics[i][j].g = 255.0f - rand()%256;								// ������ɫ��ɫ����
			m_particlespics[i][j].b = 255.0f - rand()%256;								// ������ɫ��ɫ����
			
			// �������׹�ʽ
			newHeight = m_upcastSpeed*m_particlespics[i][j].time - 0.5f*0.98f*m_particlespics[i][j].time*m_particlespics[i][j].time;
			
			// 0.2*
			m_particlespics[i][j].y += newHeight;
			
			// ����ʱ����������ʼ˥��
			if (newHeight < 0.0f)
			{
				m_particlespics[i][j].life -= m_particlespics[i][j].fade;
			}

			// ���������ľ�
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

