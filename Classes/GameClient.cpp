#include "GameClient.h"
GameClient::GameClient()
{
	home = nullptr;
	door = nullptr;
	boss = nullptr;
	isFail = false;
	isWin = false;
	noMonster = false;
}

GameClient::~GameClient()
{
}
GameData ini;

bool GameClient::init()
{
	if (!Scene::init())
	{
		return false;
	}

	enemyCount = 0;
	nowTime = ini.time;
	// ����
	createBackGround();

	// ���
	player2 = Player::create(111, WINDOWWIDTH / 2 + 100, 100, 1, 2);
	player1 = Player::create(110, WINDOWWIDTH / 2 - 100, 100, 1, 1);
	player1->setXp(ini.player1Xp);
	player2->setXp(ini.player2Xp);
	m_tankList.pushBack(player1);
	m_tankList.pushBack(player2);

	// ��ײ���
	this->scheduleUpdate();

	// �����¼�
	auto key_listener = EventListenerKeyboard::create();
	key_listener->onKeyPressed = CC_CALLBACK_2(GameClient::onKeyPressed, this);
	key_listener->onKeyReleased = CC_CALLBACK_2(GameClient::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(key_listener, this);

	this->addChild(player1, 2);
	this->addChild(player2, 2);
	m_drawList.pushBack(player1); // �������ټ��룬��ΪID�ɷ���������
	m_drawList.pushBack(player2);

	//������
	auto bar = Sprite::create("image/UI/timg.png");
	bar->setPosition(30 * 16, 38 * 16);
	this->addChild(bar, 0);

	//����ֵ1
	xpLabel1 = Label::createWithSystemFont(std::to_string(player1->getXp()), "Arial", 25);
	xpLabel1->setPosition(15 * 16, 38 * 16);
	this->addChild(xpLabel1);
	////����ֵ2
	xpLabel2 = Label::createWithSystemFont(std::to_string(player2->getXp()), "Arial", 25);
	xpLabel2->setPosition(41 * 16, 38 * 16);
	this->addChild(xpLabel2);

	//ʱ��
	this->schedule(schedule_selector(GameClient::mTimer), 0.01f);//����һ��ÿ1sִ��һ�εļ�ʱ��  
	timeLabel = Label::createWithSystemFont(std::to_string(nowTime), "Arial", 25);
	timeLabel->setPosition(25 * 16, 38 * 16);
	this->addChild(timeLabel);

	if (!ini.bgmResume) {
		//����bgm
		auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
		switch (ini.mapType) {
		case 0:
			audio->playBackgroundMusic("vedio/fbgm.mp3", true);
			break;
		case 1:
			audio->playBackgroundMusic("vedio/bgm2.wav", true);
			break;
		case 2:
			audio->playBackgroundMusic("vedio/BossBgm.wav", true);
			break;
		}
	}


	//������
	if (boss) {
		BossHp = LoadingBar::create("image/UI/LoadingBar.png");
		BossHp->setPosition(Vec2(30 * 16, 35 * 16 + 10));
		BossHp->setDirection(LoadingBar::Direction::RIGHT);
		float p = float(boss->getXp()) / float(boss->maxXp);
		BossHp->setPercent(p * 100);
		this->addChild(BossHp);
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	pauseLabel = Label::createWithSystemFont("PAUSE", "Arial", 64);
	pauseLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(pauseLabel);
	pauseLabel->setVisible(false);
	// set the direction of the loading bars progress


	return true;
}

//���ƻ����ŵ�ͼ

//���ƹ���
//͵��
void GameClient::monsterManager() {
	
	//�Ƿ���Ҫˢ��
	if (ini.mapType==2||noMonster || m_tankList.size() >= 6)
		return;

	if (nowTime == ini.time) {
		lastTime = ini.time;
	}
	//һ�����һ����3Sˢһ��
	if (nowTime - lastTime > 1 && int(nowTime) % 4 == 3) {
		lastTime = nowTime;
		Enemy1 *enemy;
		//��ͬ�ؿ�ˢ�ֵ㲻ͬ
		if (ini.mapType == 0)
			enemy = Enemy1::create(112 + enemyCount++, 4 * 16, 34 * 16, 2, ENEMY1, 4);
		else
			enemy = Enemy1::create(enemyCount++ + 112, 1 * 16, 34 * 16, 2, ENEMY1, 4);
		ene1List.pushBack(enemy);
		m_tankList.pushBack(enemy);
	}
}
//���ɱ��
void GameClient::monsterManager1() {
	//boss�ز���Ҫ
	if (ini.mapType == 2)
		return;
	//Ai�ж���������Ҳ��ҹ������
	for (auto g : ene2List) {
		if (g->target->getXp() < 1000 && g->target == player1)
			g->target = player2;
		if (g->target->getXp() < 1000 && g->target == player2)
			g->target = player1;

		auto targetPlayer = g->target;
		if (abs(g->getPosition().x - targetPlayer->getPosition().x) < 14)
			if (g->getPosition().y - targetPlayer->getPosition().y > 0) {
				g->MoveDown();
				g->Fire(nowTime);
			}
			else {
				g->MoveRight();
				g->Fire(nowTime);
			}
		else if (abs(g->getPosition().y - targetPlayer->getPosition().y) < 14)
			if (g->getPosition().x - targetPlayer->getPosition().x > 0) {
				g->MoveLeft();
				g->Fire(nowTime);
			}
			else {
				g->MoveUP();
				g->Fire(nowTime);
			}
			//����ϰ���
		else if (g->getPosition().y - targetPlayer->getPosition().y > 0 && g->getHindered() != TANK_DOWN)
			g->MoveDown();
		else if (g->getPosition().y - targetPlayer->getPosition().y < 0 && g->getHindered() != TANK_UP)
			g->MoveUP();
		else if (g->getPosition().x - targetPlayer->getPosition().x > 0 && g->getHindered() != TANK_LEFT)
			g->MoveLeft();
		else  if (g->getPosition().x - targetPlayer->getPosition().x < 0 && g->getHindered() != TANK_RIGHT)
			g->MoveRight();
	}

	//����Ҫ����/����̫����
	if (noMonster || m_tankList.size() > 7)
		return;

	//ÿ����һ��
	if (nowTime - lastTime > 1 && int(nowTime) % 4 == 1) {
		lastTime = nowTime;
		Enemy2 *enemy;

		//ˢ�ֵ�
		if (ini.mapType == 0)
			enemy = Enemy2::create(112 + enemyCount++, 55 * 16, 34 * 16, 2, ENEMY2);
		else
			enemy = Enemy2::create(enemyCount++ + 112, 58 * 16, 34 * 16, 2, ENEMY2);

		//ɱ��ѡ��һ������
		Player *targetPlayer;
		srand(time(NULL));
		if (rand() % 2)
			targetPlayer = player1;
		else
			targetPlayer = player2;
		enemy->target = targetPlayer;

		m_tankList.pushBack(enemy);
		ene2List.pushBack(enemy);
	}
}

//����������
void GameClient::returnToTitle() {
	auto myScene = Start::createScene();
	ini.bgmResume = true;
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, myScene, Color3B(0, 255, 255)));
}


Scene* GameClient::createScene(GameData input)
{
	ini = input;
	auto scene = Scene::create();
	auto layer = GameClient::create();
	scene->addChild(layer);
	return scene;
}

//����̹�˵��ӵ���ײ���
void GameClient::deadTankBullet(CocosDenshion::SimpleAudioEngine *audio) {
	int i = 0;
	for (auto const &bullet : bulletList)
	{
		if (bullet->death)
			continue;
		// �ӵ���ǽ
		for (int k = 0; k < m_bgList.size(); k++)
		{
			auto brick = m_bgList.at(k);
			if (bullet->getRect().intersectsRect(brick->getRect()))
			{
				// �ӵ�����
				m_deleteBulletList.pushBack(bullet);

				// ש������
				m_deleteBrickList.pushBack(brick);
				bullet->death = true;
				break;
			}
		}
		if (bullet->death)
			continue;

		// �ӵ�������
		for (int k = 0; k < m_irList.size(); k++)
		{
			auto iron = m_irList.at(k);
			if (bullet->getRect().intersectsRect(iron->getRect())) {
				// �ӵ�����
				m_deleteBulletList.pushBack(bullet);
				bullet->death = true;
				break;
			}
		}
		if (bullet->death)
			continue;
		//�ӵ����ӵ�֮�����ײ
		for (int k = i + 1; k < bulletList.size(); k++)
		{
			auto bullet2 = bulletList.at(k);
			if (bullet->getRect().intersectsRect(bullet2->getRect()))
			{
				audio->setBackgroundMusicVolume(0.1);
				audio->playEffect("vedio/bulletBoom.wav", false);
				// �����ӵ�����
				m_deleteBulletList.pushBack(bullet);
				m_deleteBulletList.pushBack(bullet2);
				bullet2->death = true;
				bullet->death = true;
				break;
			}
		}
		if (bullet->death)
			continue;
		++i;

		//�ӵ���̹��
		for (auto const &g : m_tankList) {
			if (bullet->getRect().intersectsRect(g->getRect()))
			{
				// �ӵ�����
				m_deleteBulletList.pushBack(bullet);
				//̹������
				g->damage();
				if (g->getXp() < 1000)
					m_deleteTankList.pushBack(g);
				bullet->death = true;
				break;
			}
		}
		if (bullet->death)
			continue;
		//�ӵ��ͼ�
		if (home != nullptr)
			if (bullet->getRect().intersectsRect(home->getRect()))
			{
				m_deleteBulletList.pushBack(bullet);
				home->damage();
				if (home->getHp() <= 0) {
					player1->setXp(0);
					player2->setXp(0);
				}
				continue;
			}
		if (bullet->death)
			continue;
		// �����ӵ���boss
		if (boss != nullptr) {
			if (bullet->getRect().intersectsRect(boss->getRect())) {
				m_deleteBulletList.pushBack(bullet);
				boss->damage();
				float p = float(boss->getXp()) / float(boss->maxXp);
				BossHp->setPercent(p * 100);
				bullet->death = true;
				continue;
			}
			if (bullet->death)
				continue;
		}
	}
	clearBlock(audio);

}
//��������ײ�ͼ��
void GameClient::boomerangCollide(Player *input, CocosDenshion::SimpleAudioEngine *audio) {
	//�ӵ���ש��
	int i = 0;
	for (auto const &bullet : input->getXBulletList()) {
		if (bullet->death)
			continue;
		if (bullet->startTime == 0) {
			bullet->startTime = nowTime;
		}
		else {
			if (nowTime >= bullet->startTime + bullet->backTime)
				bullet->goBack();
		}
		// �ӵ���ǽ
		for (int k = 0; k < m_bgList.size(); k++)
		{
			auto brick = m_bgList.at(k);
			if (bullet->getRect().intersectsRect(brick->getRect()))
			{
				// ש������
				if (bullet->hp-- > 0)
					m_deleteBrickList.pushBack(brick);
			}

		}

		//�ӵ�������
		for (int k = 0; k < m_irList.size(); k++)
		{
			auto iron = m_irList.at(k);
			if (bullet->getRect().intersectsRect(iron->getRect())) {
				// �ӵ�����

				if (bullet->hp-- > 0)
					m_deleteIronList.pushBack(iron);
			}
		}


		//�ӵ����ӵ�����
		for (int k = i++ + 1; k < m_tankList.size(); k++)
		{
			auto bulletList = m_tankList.at(k)->getBulletList();
			for (int d = 0; d < bulletList.size(); ++d)
				if (bullet->getRect().intersectsRect(bulletList.at(d)->getRect()))
				{
					auto bullet2 = bulletList.at(d);
					if (bullet->hp-- > 0) {
						bulletList.eraseObject(bullet2);
						bullet2->Blast();
						audio->setBackgroundMusicVolume(0.1);
						audio->playEffect("vedio/bulletBoom.wav", false);
					}
				}
		}

		//�ӵ��ͼ�
		if (home != nullptr)
			if (bullet->getRect().intersectsRect(home->getRect()) && bullet->hp-- > 0)
			{

				m_deleteBulletList.pushBack(bullet);
				home->damage();
				if (home->getHp() <= 0) {
					player1->setXp(0);
					player2->setXp(0);
				}
			}

		//�����ڶ�boss
		if (boss != nullptr) {
			if (bullet->getRect().intersectsRect(boss->getRect()) && bullet->hp-- > 0) {
				boss->damage();
				float p = float(boss->getXp()) / float(boss->maxXp);
				BossHp->setPercent(p * 100);
			}
		}
		if (bullet->hp <= 0)
			m_deleteXBulletList.pushBack(bullet);
	}
	clearBlock(audio);
}
//boss������ײ
void GameClient::bossCollide(CocosDenshion::SimpleAudioEngine *audio) {
	int i = 0;
	for (auto const &bullet : boss->getBulletList1())
	{
		if (bullet->death)
			continue;
		// ����
		for (int k = 0; k < m_bgList.size(); k++)
		{
			auto brick = m_bgList.at(k);
			if (bullet->getRect().intersectsRect(brick->getRect()))
			{
				// �ӵ�����
				m_deleteBulletList.pushBack(bullet);

				// ש������
				m_deleteBrickList.pushBack(brick);
				break;
			}
		}
		if (bullet->death)
			continue;
		//�ӵ����ӵ�֮�����ײ
		for (int k = i + 1; k < bulletList.size(); k++)
		{
			auto bullet2 = bulletList.at(k);
			if (bullet->getRect().intersectsRect(bullet2->getRect()))
			{
				audio->setBackgroundMusicVolume(0.1);
				audio->playEffect("vedio/bulletBoom.wav", false);
				// �����ӵ�����
				m_deleteBulletList.pushBack(bullet);
				m_deleteBulletList.pushBack(bullet2);
				bullet->death = true;
				bullet2->death = true;
				break;
			}
		}
		if (bullet->death)
			continue;
		++i;

		//�ӵ���̹��
		for (auto const &g : m_tankList) {
			if (bullet->getRect().intersectsRect(g->getRect()))
			{
				// �ӵ�����
				m_deleteBulletList.pushBack(bullet);
				//̹������
				g->damage();
				if (g->getXp() < 1000)
					m_deleteTankList.pushBack(g);
				break;
			}
		}
		if (bullet->death)
			continue;
	}

	//�����
	for (auto const &bullet : boss->getBulletList2()) {

		//�ӵ���̹��
		for (auto const &g : m_tankList) {
			if (bullet->getRect().intersectsRect(g->getRect()))
			{
				// �ӵ�����
				m_deleteBulletList.pushBack(bullet);
				//̹������
				g->damage();
				if (g->getXp() < 1000)
					m_deleteTankList.pushBack(g);
			}
		}

		// �ӵ���ǽ
		for (int k = 0; k < m_bgList.size(); k++)
		{
			auto brick = m_bgList.at(k);
			if (bullet->getRect().intersectsRect(brick->getRect()))
			{
				// ש������
				m_deleteBrickList.pushBack(brick);
			}
		}
		//�ӵ����ӵ�����
		for (int k = 0; k < m_tankList.size(); k++)
		{
			auto bulletList = m_tankList.at(k)->getBulletList();
			for (int d = 0; d < bulletList.size(); ++d)
				if (bullet->getRect().intersectsRect(bulletList.at(d)->getRect()))
				{
					auto bullet2 = bulletList.at(d);
					bulletList.eraseObject(bullet2);
					bullet2->Blast();
					audio->setBackgroundMusicVolume(0.1);
					audio->playEffect("vedio/bulletBoom.wav", false);
				}
		}
	}

	clearBlock(audio);


}
//����������ײ
void GameClient::forTankCollide(CocosDenshion::SimpleAudioEngine *audio) {
	for (int i = 0; i < m_tankList.size(); i++)
	{
		auto nowTank = m_tankList.at(i);
		for (int j = 0; j < m_bgList.size(); j++)
		{
			auto nowBrick = m_bgList.at(j);

			//���״̬�µ��������ש��
			if (nowTank->getKind() < 3 && player1->isRush || player2->isRush)
				if (nowTank->getRect().intersectsRect(nowBrick->getRect())) {
					m_deleteBrickList.pushBack(nowBrick);
					continue;
				}
			//̹�˻�����&&��ײ&&��ǰ״̬
			if (nowTank->getLife() && (nowTank->getRect().intersectsRect(nowBrick->getRect())) && (nowTank->getDirection() == TANK_UP))
			{
				// ����1���Ĵ�����ת��
				nowTank->setHindered(TANK_UP);
				nowTank->setPositionY(nowTank->getPositionY() - 1); // ������ɹ���̹�˳����ܣ��޷��ж���ɿ�ס

																	// ����2���Ĵ�ֹͣת��
																	// nowTank->Stay(TANK_UP);
			}
			if (nowTank->getLife() && (nowTank->getRect().intersectsRect(nowBrick->getRect())) && (nowTank->getDirection() == TANK_DOWN))
			{
				// ����1���Ĵ�����ת��
				nowTank->setHindered(TANK_DOWN);
				nowTank->setPositionY(nowTank->getPositionY() + 1); // ������ɹ���̹�˳����ܣ��޷��ж���ɿ�ס

																	// ����2���Ĵ�ֹͣת��
																	// nowTank->Stay(TANK_DOWN);
			}
			if (nowTank->getLife() && (nowTank->getRect().intersectsRect(nowBrick->getRect())) && (nowTank->getDirection() == TANK_LEFT))
			{
				// ����1���Ĵ�����ת��
				nowTank->setHindered(TANK_LEFT);
				nowTank->setPositionX(nowTank->getPositionX() + 1); // ������ɹ���̹�˳����ܣ��޷��ж���ɿ�ס

																	// ����2���Ĵ�ֹͣת��
																	// nowTank->Stay(TANK_LEFT);
			}
			if (nowTank->getLife() && (nowTank->getRect().intersectsRect(nowBrick->getRect())) && (nowTank->getDirection() == TANK_RIGHT))
			{
				// ����1���Ĵ�����ת��
				nowTank->setHindered(TANK_RIGHT);
				nowTank->setPositionX(nowTank->getPositionX() - 1); // ������ɹ���̹�˳����ܣ��޷��ж���ɿ�ס

																	// ����2���Ĵ�ֹͣת��
																	// nowTank->Stay(TANK_RIGHT);
			}
		}
		//̹�˺�����
		for (int j = 0; j < m_irList.size(); j++)
		{
			auto now = m_irList.at(j);

			//���״̬�µ������������
			if (nowTank->getKind() < 3 && player1->isRush || player2->isRush)
				if (nowTank->getRect().intersectsRect(now->getRect())) {
					m_deleteIronList.pushBack(now);
					continue;
				}
			if (nowTank->getLife() && (nowTank->getRect().intersectsRect(now->getRect())) && (nowTank->getDirection() == TANK_UP))
			{
				nowTank->setHindered(TANK_UP);
				nowTank->setPositionY(nowTank->getPositionY() - 1);

			}
			if (nowTank->getLife() && (nowTank->getRect().intersectsRect(now->getRect())) && (nowTank->getDirection() == TANK_DOWN))
			{
				nowTank->setHindered(TANK_DOWN);
				nowTank->setPositionY(nowTank->getPositionY() + 1);
			}
			if (nowTank->getLife() && (nowTank->getRect().intersectsRect(now->getRect())) && (nowTank->getDirection() == TANK_LEFT))
			{
				nowTank->setHindered(TANK_LEFT);
				nowTank->setPositionX(nowTank->getPositionX() + 1);
			}
			if (nowTank->getLife() && (nowTank->getRect().intersectsRect(now->getRect())) && (nowTank->getDirection() == TANK_RIGHT))
			{
				nowTank->setHindered(TANK_RIGHT);
				nowTank->setPositionX(nowTank->getPositionX() - 1);
			}


		}
		//̹�˺ͺ�
		for (int j = 0; j < m_seaList.size(); j++)
		{
			auto now = m_seaList.at(j);
			if (nowTank->getLife() && (nowTank->getRect().intersectsRect(now->getRect())) && (nowTank->getDirection() == TANK_UP))
			{
				nowTank->setHindered(TANK_UP);
				nowTank->setPositionY(nowTank->getPositionY() - 1);

			}
			if (nowTank->getLife() && (nowTank->getRect().intersectsRect(now->getRect())) && (nowTank->getDirection() == TANK_DOWN))
			{
				nowTank->setHindered(TANK_DOWN);
				nowTank->setPositionY(nowTank->getPositionY() + 1);
			}
			if (nowTank->getLife() && (nowTank->getRect().intersectsRect(now->getRect())) && (nowTank->getDirection() == TANK_LEFT))
			{
				nowTank->setHindered(TANK_LEFT);
				nowTank->setPositionX(nowTank->getPositionX() + 1);
			}
			if (nowTank->getLife() && (nowTank->getRect().intersectsRect(now->getRect())) && (nowTank->getDirection() == TANK_RIGHT))
			{
				nowTank->setHindered(TANK_RIGHT);
				nowTank->setPositionX(nowTank->getPositionX() - 1);
			}
		}

		// ̹����̹��
		for (int j = 0; j < m_tankList.size(); j++)
		{
			auto anotherTank = m_tankList.at(j);
			if ((nowTank->getLife() && anotherTank->getLife()) && (anotherTank->getID() != nowTank->getID()) && (nowTank->getRect().intersectsRect(anotherTank->getRect())))
			{
				//��ҳ��״̬��ײ������
				if (nowTank->getKind() < 3 && player1->isRush || player2->isRush) {
					anotherTank->damage();
					if (anotherTank->getXp() < 1000)
						m_deleteTankList.pushBack(anotherTank);
					if (anotherTank->getKind() < 3) {
						nowTank->damage();
						if (nowTank->getXp() < 1000)
							m_deleteTankList.pushBack(nowTank);
					}
					else
						nowTank->addXp();
					continue;
				}
				// �����˶���̹�˲��������¶���
				if (nowTank->getDirection() == TANK_UP && nowTank->isMoving())
					nowTank->Stay(TANK_UP);
				if (nowTank->getDirection() == TANK_DOWN && nowTank->isMoving())
					nowTank->Stay(TANK_DOWN);
				if (nowTank->getDirection() == TANK_LEFT && nowTank->isMoving())
					nowTank->Stay(TANK_LEFT);
				if (nowTank->getDirection() == TANK_RIGHT && nowTank->isMoving())
					nowTank->Stay(TANK_RIGHT);


			}
		}

		// ̹�����ӵ�
		for (int j = 0; j < nowTank->getBulletList().size(); j++)
		{
			auto bullet = nowTank->getBulletList().at(j);
			for (int k = 0; k < m_tankList.size(); k++)
			{
				auto tank_another = m_tankList.at(k);
				if (nowTank->getID() != tank_another->getID())
				{
					if (bullet->getRect().intersectsRect(tank_another->getRect()))
					{
						// �ӵ�����
						m_deleteBulletList.pushBack(bullet);

						// ̹������
						tank_another->damage();
						if (tank_another->getXp() < 1000)
							m_deleteTankList.pushBack(tank_another);

						//��һ�ɱ
						if (nowTank->getKind() < 3) {
							if (tank_another->getKind() < 3) {
								nowTank->damage();
								if (nowTank->getXp() < 1000)
									m_deleteTankList.pushBack(nowTank);
							}
							else
								nowTank->addXp();
							break;
						}
					}
				}
			}


			// �ӵ��Զ�boss
			if (boss != nullptr) {
				if (bullet->getRect().intersectsRect(boss->getRect())) {
					boss->damage();
					float p = float(boss->getXp()) / float(boss->maxXp);
					BossHp->setPercent(p * 100);
					m_deleteBulletList.pushBack(bullet);
				}
			}


			// �ӵ���ǽ
			for (int k = 0; k < m_bgList.size(); k++)
			{
				auto brick = m_bgList.at(k);
				if (bullet->getRect().intersectsRect(brick->getRect()))
				{
					// �ӵ�����
					m_deleteBulletList.pushBack(bullet);

					// ש������
					m_deleteBrickList.pushBack(brick);
				}
			}

			//�ӵ�������
			for (int k = 0; k < m_irList.size(); k++)
			{
				auto iron = m_irList.at(k);
				if (bullet->getRect().intersectsRect(iron->getRect()))
					// �ӵ�����
					m_deleteBulletList.pushBack(bullet);
			}


			//�ӵ����ӵ�����
			for (int k = i + 1; k < m_tankList.size(); k++)
			{
				auto bulletList = m_tankList.at(k)->getBulletList();
				for (int d = 0; d < bulletList.size(); ++d)
					if (bullet->getRect().intersectsRect(bulletList.at(d)->getRect()))
					{
						auto bullet2 = bulletList.at(d);
						bulletList.eraseObject(bullet2);
						bullet2->Blast();
						// �ӵ�����
						m_deleteBulletList.pushBack(bullet);
					}

				audio->setBackgroundMusicVolume(0.1);
				audio->playEffect("vedio/bulletBoom.wav", false);
			}

			//�ӵ��ͼ�
			if (home != nullptr)
				if (bullet->getRect().intersectsRect(home->getRect()))
				{
					m_deleteBulletList.pushBack(bullet);
					home->damage();
					if (home->getHp() <= 0) {
						player1->setXp(0);
						player2->setXp(0);
					}
				}
		}
		//̹�˺ͼ�
		if (home != nullptr)
			if (nowTank->getRect().intersectsRect(home->getRect())) {
				// �����˶���̹�˲��������¶���
				if (nowTank->getDirection() == TANK_UP && nowTank->isMoving())
				{
					nowTank->Stay(TANK_UP);
				}
				if (nowTank->getDirection() == TANK_DOWN && nowTank->isMoving())
				{
					nowTank->Stay(TANK_DOWN);
				}
				if (nowTank->getDirection() == TANK_LEFT && nowTank->isMoving())
				{
					nowTank->Stay(TANK_LEFT);
				}
				if (nowTank->getDirection() == TANK_RIGHT && nowTank->isMoving())
				{
					nowTank->Stay(TANK_RIGHT);
				}

				//��ҳ��״̬��ײ��
				if (nowTank->getKind() < 3 && player1->isRush || player2->isRush) {
					home->damage();
					if (home->getHp() <= 0) {
						player1->setXp(0);
						player2->setXp(0);
					}
					nowTank->damage();
				}

			}

		//�����ں�̹��
		if (nowTank != player1)
			for (auto const &g : player1->getXBulletList()) {
				if (nowTank->getRect().intersectsRect(g->getRect())) {
					nowTank->damage();
					if (nowTank->getXp() < 1000)
						m_deleteTankList.pushBack(nowTank);
					if (nowTank->getKind() < 3) {
						auto killer = nowTank->getKind() % 2 + 1;
						switch (killer) {
						case 1:
							player1->damage();
							break;
						case 2:
							player2->damage();
							break;
						}
						if (player1->getXp() < 1000)
							m_deleteTankList.pushBack(player1);

						if (player2->getXp() < 1000)
							m_deleteTankList.pushBack(player2);
					}
					else player1->addXp();
				}
			}

		if (nowTank != player2)
			for (auto const &g : player2->getXBulletList()) {
				if (nowTank->getRect().intersectsRect(g->getRect())) {
					nowTank->damage();
					if (nowTank->getXp() < 1000)
						m_deleteTankList.pushBack(nowTank);
					if (nowTank->getKind() < 3) {
						auto killer = nowTank->getKind() % 2 + 1;
						switch (killer) {
						case 1:
							player1->damage();
							break;
						case 2:
							player2->damage();
							break;
						}
						if (player1->getXp() < 1000)
							m_deleteTankList.pushBack(player1);

						if (player2->getXp() < 1000)
							m_deleteTankList.pushBack(player2);
					}
					else
						player2->addXp();
				}
			}
		// ���ɾ���ӵ��б�
		for (int j = 0; j < m_deleteBulletList.size(); j++)
		{
			auto bullet = m_deleteBulletList.at(j);
			m_deleteBulletList.eraseObject(bullet);
			nowTank->getBulletList().eraseObject(bullet);
			bullet->Blast();
		}
		m_deleteBulletList.clear();

	}
}

//�����ڴ�
void GameClient::clearBlock(CocosDenshion::SimpleAudioEngine *audio) {


	for (int j = 0; j < m_deleteBrickList.size(); j++)
	{
		auto brick = m_deleteBrickList.at(j);
		m_deleteBrickList.eraseObject(brick);
		m_bgList.eraseObject(brick);
		brick->Blast();

		//��Ч
		audio->setBackgroundMusicVolume(0.1);
		audio->playEffect("vedio/otherBoom.mp3", false);
	}

	// ���ɾ��̹���б�
	while (m_deleteTankList.size())
	{
		//��ը��Ч
		audio->setBackgroundMusicVolume(0.1);
		audio->playEffect("vedio/tankBoom.wav", false);


		auto tank = m_deleteTankList.at(0);
		m_deleteTankList.eraseObject(tank);
		m_tankList.eraseObject(tank);
		for (auto const &g : tank->getBulletList())
			bulletList.pushBack(g);
		tank->Blast();
	}

	//��������б�
	for (int j = 0; j < m_deleteIronList.size(); j++)
	{
		auto Iron = m_deleteIronList.at(j);
		m_deleteIronList.eraseObject(Iron);
		m_irList.eraseObject(Iron);
		Iron->Blast();
	}
	// ���ɾ���������б�
	for (int j = 0; j < m_deleteXBulletList.size(); j++)
	{
		auto bullet = m_deleteXBulletList.at(j);
		m_deleteXBulletList.eraseObject(bullet);
		player1->getXBulletList().eraseObject(bullet);
		player2->getXBulletList().eraseObject(bullet);
		bullet->Blast();
	}

	m_deleteXBulletList.clear();
	m_deleteIronList.clear();
	m_deleteBrickList.clear();
	m_deleteTankList.clear();

}


void GameClient::update(float delta)
{

	monsterManager();
	monsterManager1();

	timeLabel->setString(CCString::createWithFormat("%.2f", nowTime)->getCString());

	//��¼̹�����ݣ����㻭ͼ
	CCLOG((std::to_string(player1->getPosition().y / 16) + " " + std::to_string(player1->getPosition().x / 16)).c_str());

	//��ʼ������ϵͳ
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	// �յ������Ŀ�����Ϣ��̹��ִ��Fire

	// ά��̹���б�
	for (int i = 0; i < m_tankList.size(); i++)
	{
		auto nowTank = m_tankList.at(i);
		if (nowTank->getLife() <= 0)
		{
			m_tankList.eraseObject(nowTank);
		}
		bool notDraw = true;
		for (int j = 0; j < m_drawList.size(); j++)
		{
			auto drawTank = m_drawList.at(j);
			if (drawTank->getID() == nowTank->getID())
			{
				notDraw = false;
			}
		}

		// ������δ���Ƶ�̹��-��Ժ��������Ŀͻ���
		if (notDraw)
		{
			this->addChild(nowTank);
			m_drawList.pushBack(nowTank);
		}
	}

	//��������̹�˵��ӵ�
	deadTankBullet(audio);

	// ̹���� ̹�ˣ���Ʒ����ײ���
	//ע�⣡����޸��ӵ���ײ�Ļ�Ҫע�� ����̹�˺ͻ��ߵ�̹��Ҫ�ֿ�����
	forTankCollide(audio);

	//������
	boomerangCollide(player1, audio);
	boomerangCollide(player2, audio);
	if (boss != nullptr)
		bossCollide(audio);

	//boss����ģʽ
	if (boss != nullptr) {
		if (player1->getXp() < 1000) {
			boss->AI(nowTime, player2->getPosition(), player2->getPosition());
		}
		else if (player2->getXp() < 1000)
			boss->AI(nowTime, player1->getPosition(), player1->getPosition());
		else
			boss->AI(nowTime, player1->getPosition(), player2->getPosition());
	}

	//Χǽ
	if (boss != nullptr)
		if (boss->isAttack3) {
			if (player1->getXp() < 1000) {
				drawBossBG(player2->getPosition());
			}
			else if (player2->getXp() < 1000)
				drawBossBG(player1->getPosition());
			else {
				if (int(nowTime) % 2)
					drawBossBG(player1->getPosition());
				else
					drawBossBG(player2->getPosition());
			}
			boss->isAttack3 = false;
		}

	//��Ϸʤ��
	if (boss != nullptr)
		if (!isWin&&boss->getXp() <= 0) {
			isWin = true;

			audio = CocosDenshion::SimpleAudioEngine::getInstance();
			audio->playBackgroundMusic("vedio/win.mp3", true);

			CCDelayTime* delayTime = CCDelayTime::create(3);
			CCCallFunc *callFunND = CCCallFunc::create(this, callfunc_selector(GameClient::win));
			CCSequence *action = CCSequence::create(delayTime, callFunND, NULL);
			this->runAction(action);
		}

	//��Ϸʧ��
	if (!isFail) {

		if (player1->getXp() < 1000 && player2->getXp() < 1000) {
			isFail = true;
			CCDelayTime* delayTime = CCDelayTime::create(3);
			CCCallFunc *callFunND = CCCallFunc::create(this, callfunc_selector(GameClient::fail));
			CCSequence *action = CCSequence::create(delayTime, callFunND, NULL);
			this->runAction(action);
		}
	}

	xpLabel1->setString(std::to_string(player1->getXp()));
	xpLabel2->setString(std::to_string(player2->getXp()));

	clearBlock(audio);
}

//ת��ʤ������
void GameClient::win() {
	//isRestart = true;
	Director::getInstance()->pushScene(RankList::createScene(nowTime, true));
}

//ת��ʧ�ܽ���
void GameClient::fail() {
	//isRestart = true;
	Director::getInstance()->pushScene(Fail::createScene());
}
//boss���μ���
void GameClient::drawBossBG(Vec2 position) {
	for (int i = -3; i < 5; i++)
	{
		for (int j = -3; j < 5; j++)
		{
			if ((i == -1) && (j == -1) || (i == -1) && (j == 0) || (i == -1) && (j == 1) || (i == -1) && (j == 2)
				|| (i == 0) && (j == -1) || (i == 0) && (j == 0) || (i == 0) && (j == 1) || (i == 0) && (j == 2)
				|| (i == 1) && (j == -1) || (i == 1) && (j == -0) || (i == 1) && (j == 1) || (i == 1) && (j == 2)
				|| (i == 2) && (j == -1) || (i == 2) && (j == -0) || (i == 2) && (j == 1) || (i == 2) && (j == 2))
			{
				// �м������γɻ���
				continue;
			}
			auto brick = Brick::create(Vec2(position.x + (0.5 - i) * 16, position.y + (0.5 - j) * 16));
			m_bgList.pushBack(brick);
			this->addChild(brick, 2);
		}
	}
}

//��Ϸ���¿�ʼ
void GameClient::gameRestart() {
	ini.time = nowTime;
	auto myScene = GameClient::createScene(GameData(ini));
	ini.bgmResume = true;
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, myScene, Color3B(0, 255, 255)));
}

//����ͼ
void GameClient::createBackGround()
{
	switch (ini.mapType) {
	case 0:
		drawMap1();
		break;
	case 1:
		drawMap2();
		break;
	case 2:
		drawBoss();
	}
}
void GameClient::drawMap1() {

	//����
	auto map = TMXTiledMap::create("image/map/map.tmx");
	this->addChild(map, 10);
	//����
	auto home = Home::create(Vec2(30 * 16, 1 * 16));
	this->addChild(home, 2);
	this->home = home;
	//����

	for (int i = -8; i <= 7; ++i) {
		for (int j = -1; j < 2; ++j) {
			auto sea = Sea::create(Vec2(29 * 16 + (0.5 - i) * 16, 25 * 16 + (0.5 - j) * 16));
			m_seaList.pushBack(sea);
			this->addChild(sea, 2);
		}
	}

	//������
	for (int i = -4; i <= 3; ++i) {
		for (int j = 0; j < 2; ++j) {
			auto iron = Iron::create(Vec2(29 * 16 + (0.5 - i) * 16, 16 * 16 + (0.5 - j) * 16));
			m_irList.pushBack(iron);
			this->addChild(iron, 2);
		}
	}
	//����
	door = Door::create(Vec2(29 * 16 + 0.5 * 16, 38 * 16 + 0.5 * 16));
	this->addChild(door, 2);

	//��Χǽ
	drawBigBG(Vec2(30 * 16, 1 * 16));
	//���
	//��
	for (int j = -28; j < 0; ++j) {
		auto brick = Brick::create(Vec2(46 * 16 + 0.5 * 16, 4 * 16 + (0.5 - j) * 16));
		m_bgList.pushBack(brick);
		this->addChild(brick, 2);
	}
	//��
	for (int i = -8; i <= 7; ++i) {
		for (int j = 0; j < 2; ++j) {
			auto brick = Brick::create(Vec2(44 * 16 + (0.5 - i) * 16, 31 * 16 + (0.5 - j) * 16));
			m_bgList.pushBack(brick);
			this->addChild(brick, 2);
		}
	}

	//�ұ�
	//��
	for (int j = -28; j < 0; ++j) {
		auto brick = Brick::create(Vec2(13 * 16 + 0.5 * 16, 4 * 16 + (0.5 - j) * 16));
		m_bgList.pushBack(brick);
		this->addChild(brick, 2);
	}
	//��
	for (int i = -8; i <= 7; ++i) {
		for (int j = 0; j < 2; ++j) {
			auto brick = Brick::create(Vec2(15 * 16 + (0.5 - i) * 16, 31 * 16 + (0.5 - j) * 16));
			m_bgList.pushBack(brick);
			this->addChild(brick, 2);
		}
	}
}
void GameClient::drawMap2() {
	//����
	auto map = TMXTiledMap::create("image/map/map1.tmx");
	this->addChild(map, 10);

	//����
	auto home = Home::create(Vec2(30 * 16, 1 * 16));
	this->addChild(home, 2);
	this->home = home;
	//��Χǽ
	drawBigBG(Vec2(30 * 16, 1 * 16));

	//����
	door = Door::create(Vec2(29 * 16 + 0.5 * 16, 38 * 16 + 0.5 * 16));
	this->addChild(door, 2);

	//����
	for (int i = -1; i <= 2; ++i) {
		for (int j = -15; j < 15; ++j) {
			auto sea = Sea::create(Vec2(20 * 16 + (0.5 - i) * 16, 20 * 16 + (0.5 - j) * 16));
			m_seaList.pushBack(sea);
			this->addChild(sea, 2);
		}
	}

	for (int i = -1; i <= 2; ++i) {
		for (int j = -15; j < 15; ++j) {
			auto sea = Sea::create(Vec2(40 * 16 + (0.5 - i) * 16, 20 * 16 + (0.5 - j) * 16));
			m_seaList.pushBack(sea);
			this->addChild(sea, 2);
		}
	}

	//������
	for (int i = -1; i < 2; ++i) {
		for (int j = -1; j < 2; ++j) {
			auto iron = Iron::create(Vec2(12 * 16 + (0.5 - i) * 16, 29 * 16 + (0.5 - j) * 16));
			m_irList.pushBack(iron);
			this->addChild(iron, 2);
		}
	}

	for (int i = -1; i < 2; ++i) {
		for (int j = -1; j < 2; ++j) {
			auto iron = Iron::create(Vec2(48 * 16 + (0.5 - i) * 16, 29 * 16 + (0.5 - j) * 16));
			m_irList.pushBack(iron);
			this->addChild(iron, 2);
		}
	}

	//��ש
	drawBigBG(Vec2(6 * 16, 19 * 16));
	drawBigBG(Vec2(51 * 16, 19 * 16));

}
void GameClient::drawBoss() {
	boss = Boss::create(111, WINDOWWIDTH / 2, WINDOWHEIGHT - 200, 1, 2);
	this->addChild(boss, 2);
}

// ���Ƶ�������ש��
void GameClient::drawBigBG(Vec2 position)
{
	for (int i = -2; i < 4; i++)
	{
		for (int j = -2; j < 4; j++)
		{
			if ((i == 1) && (j == 0) || (i == 0) && (j == 0) || (i == 1) && (j == 1) || (i == 0) && (j == 1))
			{
				// �м������γɻ���
				continue;
			}
			auto brick = Brick::create(Vec2(position.x + (0.5 - i) * 16, position.y + (0.5 - j) * 16));
			m_bgList.pushBack(brick);
			this->addChild(brick, 2);
		}
	}
}

//���̼�������
void GameClient::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		if (player1->isRush)
			return;
		player1->MoveLeft();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		if (player1->isRush)
			return;
		player1->MoveUP();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		if (player1->isRush)
			return;
		player1->MoveDown();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		if (player1->isRush)
			return;
		player1->MoveRight();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		if (player2->isRush)
			return;
		player2->MoveLeft();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		if (player2->isRush)
			return;
		player2->MoveUP();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		if (player2->isRush)
			return;
		player2->MoveDown();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		if (player2->isRush)
			return;
		player2->MoveRight();
		break;
		//��̼���
	case cocos2d::EventKeyboard::KeyCode::KEY_L:
		if (player1->isRush)
		{
			player1->stopRush();
			return;
		}
		if (player1->getXp() >= 5000)
			player1->startRush();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_J:
		if (player1->isRush)
			return;
		if (player1->getXp() >= 5000)
			player1->xFire(nowTime);
		break;
		//���¿�ʼ����
	case cocos2d::EventKeyboard::KeyCode::KEY_R:
		if (CCDirector::sharedDirector()->isPaused()) {
			CCDirector::sharedDirector()->resume();
			pauseLabel->setVisible(false);
			break;
		}
		this->gameRestart();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_Y:
		this->returnToTitle();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_3:
		if (player2->isRush)
		{
			player2->stopRush();
			return;
		}
		if (player2->getXp() >= 5000)
			player2->startRush();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_1:
		if (player2->isRush)
			return;
		if (player2->getXp() >= 5000)
			player2->xFire(nowTime);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_M:
		player1->setXp(5000);
		player2->setXp(5000);
		xpLabel1->setString(std::to_string(player1->getXp()));
		xpLabel2->setString(std::to_string(player2->getXp()));
		break;
	}
}
void GameClient::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		if (player1->isRush)
			return;

		player1->Stay(TANK_LEFT);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		if (player1->isRush)
			return;
		player1->Stay(TANK_UP);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		if (player1->isRush)
			return;
		player1->Stay(TANK_DOWN);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		if (player1->isRush)
			return;
		player1->Stay(TANK_RIGHT);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_N:
		if(ini.mapType<3)
			Director::getInstance()->replaceScene(TransitionSlideInT::create(1, Loading::createScene(GameData(ini.mapType + 1, player1->getXp(), player2->getXp(), nowTime))));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_K:
		if (player1->isRush)
			return;
		//����ҵ��ŵ�ʱ��
		if (door != nullptr)
			if (player1->getRect().intersectsRect(door->getRect())) {
				//��һ��
				if (m_tankList.size() <= 2) {
					Director::getInstance()->replaceScene(TransitionSlideInT::create(1, Loading::createScene(GameData(ini.mapType + 1, player1->getXp(), player2->getXp(), nowTime))));
				}
				//��ˢ��
				else {
					auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
					audio->playEffect("vedio/openDoor.wav", false, 1.0f, 1.0f, 1.0f);
					noMonster = true;
				}
				break;
			}

		if (player1->getLife()) {
			player1->Fire(nowTime);
			for (int i = 2; i < ene1List.size(); ++i) {
				if (ene1List.at(i) != nullptr)
					ene1List.at(i)->Fire(nowTime);
			}
		}
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		if (player2->isRush)
			return;
		player2->Stay(TANK_LEFT);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		if (player2->isRush)
			return;
		player2->Stay(TANK_UP);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		if (player2->isRush)
			return;
		player2->Stay(TANK_DOWN);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		if (player2->isRush)
			return;
		player2->Stay(TANK_RIGHT);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_2:
		if (player2->isRush)
			return;
		//����ҵ��ŵ�ʱ��
		if (door != nullptr)
			if (player2->getRect().intersectsRect(door->getRect())) {
				//��һ��
				if (m_tankList.size() <= 2) {
					Director::getInstance()->replaceScene(TransitionSlideInT::create(1, Loading::createScene(GameData(ini.mapType + 1, player1->getXp(), player2->getXp(), nowTime))));
				}
				//��ˢ��
				else {
					auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
					audio->playEffect("vedio/openDoor.wav", false, 1.0f, 1.0f, 1.0f);
					noMonster = true;
				}
				break;
			}
		if (player2->getLife()) {
			player2->Fire(nowTime);
			for (int i = 2; i < ene1List.size(); ++i) {
				if (ene1List.at(i) != nullptr)
					ene1List.at(i)->Fire(nowTime);
			}
		}
			
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_P:
		if (CCDirector::sharedDirector()->isPaused()) {
			CCDirector::sharedDirector()->resume();
			pauseLabel->setVisible(false);
		}
		else {
			pauseLabel->setVisible(true);
			CCDirector::sharedDirector()->pause();
		}
		break;
	}

}