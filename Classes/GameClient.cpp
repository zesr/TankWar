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
	// 背景
	createBackGround();

	// 玩家
	player2 = Player::create(111, WINDOWWIDTH / 2 + 100, 100, 1, 2);
	player1 = Player::create(110, WINDOWWIDTH / 2 - 100, 100, 1, 1);
	player1->setXp(ini.player1Xp);
	player2->setXp(ini.player2Xp);
	m_tankList.pushBack(player1);
	m_tankList.pushBack(player2);

	// 碰撞检测
	this->scheduleUpdate();

	// 键盘事件
	auto key_listener = EventListenerKeyboard::create();
	key_listener->onKeyPressed = CC_CALLBACK_2(GameClient::onKeyPressed, this);
	key_listener->onKeyReleased = CC_CALLBACK_2(GameClient::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(key_listener, this);

	this->addChild(player1, 2);
	this->addChild(player2, 2);
	m_drawList.pushBack(player1); // 联网后再加入，因为ID由服务器分配
	m_drawList.pushBack(player2);

	//经验栏
	auto bar = Sprite::create("image/UI/timg.png");
	bar->setPosition(30 * 16, 38 * 16);
	this->addChild(bar, 0);

	//经验值1
	xpLabel1 = Label::createWithSystemFont(std::to_string(player1->getXp()), "Arial", 25);
	xpLabel1->setPosition(15 * 16, 38 * 16);
	this->addChild(xpLabel1);
	////经验值2
	xpLabel2 = Label::createWithSystemFont(std::to_string(player2->getXp()), "Arial", 25);
	xpLabel2->setPosition(41 * 16, 38 * 16);
	this->addChild(xpLabel2);

	//时间
	this->schedule(schedule_selector(GameClient::mTimer), 0.01f);//开启一个每1s执行一次的计时器  
	timeLabel = Label::createWithSystemFont(std::to_string(nowTime), "Arial", 25);
	timeLabel->setPosition(25 * 16, 38 * 16);
	this->addChild(timeLabel);

	if (!ini.bgmResume) {
		//设置bgm
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


	//进度条
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

//控制画那张地图

//控制怪兽
//偷家
void GameClient::monsterManager() {
	
	//是否需要刷怪
	if (ini.mapType==2||noMonster || m_tankList.size() >= 6)
		return;

	if (nowTime == ini.time) {
		lastTime = ini.time;
	}
	//一秒最多一个，3S刷一次
	if (nowTime - lastTime > 1 && int(nowTime) % 4 == 3) {
		lastTime = nowTime;
		Enemy1 *enemy;
		//不同关卡刷怪点不同
		if (ini.mapType == 0)
			enemy = Enemy1::create(112 + enemyCount++, 4 * 16, 34 * 16, 2, ENEMY1, 4);
		else
			enemy = Enemy1::create(enemyCount++ + 112, 1 * 16, 34 * 16, 2, ENEMY1, 4);
		ene1List.pushBack(enemy);
		m_tankList.pushBack(enemy);
	}
}
//玩家杀手
void GameClient::monsterManager1() {
	//boss关不需要
	if (ini.mapType == 2)
		return;
	//Ai行动，靠近玩家并且攻击玩家
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
			//躲过障碍物
		else if (g->getPosition().y - targetPlayer->getPosition().y > 0 && g->getHindered() != TANK_DOWN)
			g->MoveDown();
		else if (g->getPosition().y - targetPlayer->getPosition().y < 0 && g->getHindered() != TANK_UP)
			g->MoveUP();
		else if (g->getPosition().x - targetPlayer->getPosition().x > 0 && g->getHindered() != TANK_LEFT)
			g->MoveLeft();
		else  if (g->getPosition().x - targetPlayer->getPosition().x < 0 && g->getHindered() != TANK_RIGHT)
			g->MoveRight();
	}

	//不需要怪物/怪物太多了
	if (noMonster || m_tankList.size() > 7)
		return;

	//每四秒一次
	if (nowTime - lastTime > 1 && int(nowTime) % 4 == 1) {
		lastTime = nowTime;
		Enemy2 *enemy;

		//刷怪点
		if (ini.mapType == 0)
			enemy = Enemy2::create(112 + enemyCount++, 55 * 16, 34 * 16, 2, ENEMY2);
		else
			enemy = Enemy2::create(enemyCount++ + 112, 58 * 16, 34 * 16, 2, ENEMY2);

		//杀手选择一个敌人
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

//返回主界面
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

//死亡坦克的子弹碰撞检测
void GameClient::deadTankBullet(CocosDenshion::SimpleAudioEngine *audio) {
	int i = 0;
	for (auto const &bullet : bulletList)
	{
		if (bullet->death)
			continue;
		// 子弹和墙
		for (int k = 0; k < m_bgList.size(); k++)
		{
			auto brick = m_bgList.at(k);
			if (bullet->getRect().intersectsRect(brick->getRect()))
			{
				// 子弹消除
				m_deleteBulletList.pushBack(bullet);

				// 砖块消除
				m_deleteBrickList.pushBack(brick);
				bullet->death = true;
				break;
			}
		}
		if (bullet->death)
			continue;

		// 子弹和铁块
		for (int k = 0; k < m_irList.size(); k++)
		{
			auto iron = m_irList.at(k);
			if (bullet->getRect().intersectsRect(iron->getRect())) {
				// 子弹消除
				m_deleteBulletList.pushBack(bullet);
				bullet->death = true;
				break;
			}
		}
		if (bullet->death)
			continue;
		//子弹和子弹之间的碰撞
		for (int k = i + 1; k < bulletList.size(); k++)
		{
			auto bullet2 = bulletList.at(k);
			if (bullet->getRect().intersectsRect(bullet2->getRect()))
			{
				audio->setBackgroundMusicVolume(0.1);
				audio->playEffect("vedio/bulletBoom.wav", false);
				// 两个子弹消除
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

		//子弹和坦克
		for (auto const &g : m_tankList) {
			if (bullet->getRect().intersectsRect(g->getRect()))
			{
				// 子弹消除
				m_deleteBulletList.pushBack(bullet);
				//坦克受伤
				g->damage();
				if (g->getXp() < 1000)
					m_deleteTankList.pushBack(g);
				bullet->death = true;
				break;
			}
		}
		if (bullet->death)
			continue;
		//子弹和家
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
		// 死亡子弹对boss
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
//回旋镖碰撞就检测
void GameClient::boomerangCollide(Player *input, CocosDenshion::SimpleAudioEngine *audio) {
	//子弹和砖块
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
		// 子弹和墙
		for (int k = 0; k < m_bgList.size(); k++)
		{
			auto brick = m_bgList.at(k);
			if (bullet->getRect().intersectsRect(brick->getRect()))
			{
				// 砖块消除
				if (bullet->hp-- > 0)
					m_deleteBrickList.pushBack(brick);
			}

		}

		//子弹和铁块
		for (int k = 0; k < m_irList.size(); k++)
		{
			auto iron = m_irList.at(k);
			if (bullet->getRect().intersectsRect(iron->getRect())) {
				// 子弹消除

				if (bullet->hp-- > 0)
					m_deleteIronList.pushBack(iron);
			}
		}


		//子弹和子弹消除
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

		//子弹和家
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

		//回旋镖对boss
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
//boss攻击碰撞
void GameClient::bossCollide(CocosDenshion::SimpleAudioEngine *audio) {
	int i = 0;
	for (auto const &bullet : boss->getBulletList1())
	{
		if (bullet->death)
			continue;
		// 扇形
		for (int k = 0; k < m_bgList.size(); k++)
		{
			auto brick = m_bgList.at(k);
			if (bullet->getRect().intersectsRect(brick->getRect()))
			{
				// 子弹消除
				m_deleteBulletList.pushBack(bullet);

				// 砖块消除
				m_deleteBrickList.pushBack(brick);
				break;
			}
		}
		if (bullet->death)
			continue;
		//子弹和子弹之间的碰撞
		for (int k = i + 1; k < bulletList.size(); k++)
		{
			auto bullet2 = bulletList.at(k);
			if (bullet->getRect().intersectsRect(bullet2->getRect()))
			{
				audio->setBackgroundMusicVolume(0.1);
				audio->playEffect("vedio/bulletBoom.wav", false);
				// 两个子弹消除
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

		//子弹和坦克
		for (auto const &g : m_tankList) {
			if (bullet->getRect().intersectsRect(g->getRect()))
			{
				// 子弹消除
				m_deleteBulletList.pushBack(bullet);
				//坦克受伤
				g->damage();
				if (g->getXp() < 1000)
					m_deleteTankList.pushBack(g);
				break;
			}
		}
		if (bullet->death)
			continue;
	}

	//冲击波
	for (auto const &bullet : boss->getBulletList2()) {

		//子弹和坦克
		for (auto const &g : m_tankList) {
			if (bullet->getRect().intersectsRect(g->getRect()))
			{
				// 子弹消除
				m_deleteBulletList.pushBack(bullet);
				//坦克受伤
				g->damage();
				if (g->getXp() < 1000)
					m_deleteTankList.pushBack(g);
			}
		}

		// 子弹和墙
		for (int k = 0; k < m_bgList.size(); k++)
		{
			auto brick = m_bgList.at(k);
			if (bullet->getRect().intersectsRect(brick->getRect()))
			{
				// 砖块消除
				m_deleteBrickList.pushBack(brick);
			}
		}
		//子弹和子弹消除
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
//处理正常碰撞
void GameClient::forTankCollide(CocosDenshion::SimpleAudioEngine *audio) {
	for (int i = 0; i < m_tankList.size(); i++)
	{
		auto nowTank = m_tankList.at(i);
		for (int j = 0; j < m_bgList.size(); j++)
		{
			auto nowBrick = m_bgList.at(j);

			//冲刺状态下的玩家消除砖块
			if (nowTank->getKind() < 3 && player1->isRush || player2->isRush)
				if (nowTank->getRect().intersectsRect(nowBrick->getRect())) {
					m_deleteBrickList.pushBack(nowBrick);
					continue;
				}
			//坦克还活着&&碰撞&&当前状态
			if (nowTank->getLife() && (nowTank->getRect().intersectsRect(nowBrick->getRect())) && (nowTank->getDirection() == TANK_UP))
			{
				// 方法1：履带持续转动
				nowTank->setHindered(TANK_UP);
				nowTank->setPositionY(nowTank->getPositionY() - 1); // 避免检测成功后坦克持续受，无法行动造成卡住

																	// 方法2：履带停止转动
																	// nowTank->Stay(TANK_UP);
			}
			if (nowTank->getLife() && (nowTank->getRect().intersectsRect(nowBrick->getRect())) && (nowTank->getDirection() == TANK_DOWN))
			{
				// 方法1：履带持续转动
				nowTank->setHindered(TANK_DOWN);
				nowTank->setPositionY(nowTank->getPositionY() + 1); // 避免检测成功后坦克持续受，无法行动造成卡住

																	// 方法2：履带停止转动
																	// nowTank->Stay(TANK_DOWN);
			}
			if (nowTank->getLife() && (nowTank->getRect().intersectsRect(nowBrick->getRect())) && (nowTank->getDirection() == TANK_LEFT))
			{
				// 方法1：履带持续转动
				nowTank->setHindered(TANK_LEFT);
				nowTank->setPositionX(nowTank->getPositionX() + 1); // 避免检测成功后坦克持续受，无法行动造成卡住

																	// 方法2：履带停止转动
																	// nowTank->Stay(TANK_LEFT);
			}
			if (nowTank->getLife() && (nowTank->getRect().intersectsRect(nowBrick->getRect())) && (nowTank->getDirection() == TANK_RIGHT))
			{
				// 方法1：履带持续转动
				nowTank->setHindered(TANK_RIGHT);
				nowTank->setPositionX(nowTank->getPositionX() - 1); // 避免检测成功后坦克持续受，无法行动造成卡住

																	// 方法2：履带停止转动
																	// nowTank->Stay(TANK_RIGHT);
			}
		}
		//坦克和铁块
		for (int j = 0; j < m_irList.size(); j++)
		{
			auto now = m_irList.at(j);

			//冲刺状态下的玩家消除铁块
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
		//坦克和海
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

		// 坦克与坦克
		for (int j = 0; j < m_tankList.size(); j++)
		{
			auto anotherTank = m_tankList.at(j);
			if ((nowTank->getLife() && anotherTank->getLife()) && (anotherTank->getID() != nowTank->getID()) && (nowTank->getRect().intersectsRect(anotherTank->getRect())))
			{
				//玩家冲刺状态下撞击敌人
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
				// 正在运动的坦克才作出如下动作
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

		// 坦克与子弹
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
						// 子弹消除
						m_deleteBulletList.pushBack(bullet);

						// 坦克消除
						tank_another->damage();
						if (tank_another->getXp() < 1000)
							m_deleteTankList.pushBack(tank_another);

						//玩家击杀
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


			// 子弹对对boss
			if (boss != nullptr) {
				if (bullet->getRect().intersectsRect(boss->getRect())) {
					boss->damage();
					float p = float(boss->getXp()) / float(boss->maxXp);
					BossHp->setPercent(p * 100);
					m_deleteBulletList.pushBack(bullet);
				}
			}


			// 子弹和墙
			for (int k = 0; k < m_bgList.size(); k++)
			{
				auto brick = m_bgList.at(k);
				if (bullet->getRect().intersectsRect(brick->getRect()))
				{
					// 子弹消除
					m_deleteBulletList.pushBack(bullet);

					// 砖块消除
					m_deleteBrickList.pushBack(brick);
				}
			}

			//子弹和铁块
			for (int k = 0; k < m_irList.size(); k++)
			{
				auto iron = m_irList.at(k);
				if (bullet->getRect().intersectsRect(iron->getRect()))
					// 子弹消除
					m_deleteBulletList.pushBack(bullet);
			}


			//子弹和子弹消除
			for (int k = i + 1; k < m_tankList.size(); k++)
			{
				auto bulletList = m_tankList.at(k)->getBulletList();
				for (int d = 0; d < bulletList.size(); ++d)
					if (bullet->getRect().intersectsRect(bulletList.at(d)->getRect()))
					{
						auto bullet2 = bulletList.at(d);
						bulletList.eraseObject(bullet2);
						bullet2->Blast();
						// 子弹消除
						m_deleteBulletList.pushBack(bullet);
					}

				audio->setBackgroundMusicVolume(0.1);
				audio->playEffect("vedio/bulletBoom.wav", false);
			}

			//子弹和家
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
		//坦克和家
		if (home != nullptr)
			if (nowTank->getRect().intersectsRect(home->getRect())) {
				// 正在运动的坦克才作出如下动作
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

				//玩家冲刺状态下撞击
				if (nowTank->getKind() < 3 && player1->isRush || player2->isRush) {
					home->damage();
					if (home->getHp() <= 0) {
						player1->setXp(0);
						player2->setXp(0);
					}
					nowTank->damage();
				}

			}

		//回旋镖和坦克
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
		// 清除删除子弹列表
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

//清理内存
void GameClient::clearBlock(CocosDenshion::SimpleAudioEngine *audio) {


	for (int j = 0; j < m_deleteBrickList.size(); j++)
	{
		auto brick = m_deleteBrickList.at(j);
		m_deleteBrickList.eraseObject(brick);
		m_bgList.eraseObject(brick);
		brick->Blast();

		//音效
		audio->setBackgroundMusicVolume(0.1);
		audio->playEffect("vedio/otherBoom.mp3", false);
	}

	// 清除删除坦克列表
	while (m_deleteTankList.size())
	{
		//爆炸音效
		audio->setBackgroundMusicVolume(0.1);
		audio->playEffect("vedio/tankBoom.wav", false);


		auto tank = m_deleteTankList.at(0);
		m_deleteTankList.eraseObject(tank);
		m_tankList.eraseObject(tank);
		for (auto const &g : tank->getBulletList())
			bulletList.pushBack(g);
		tank->Blast();
	}

	//清除铁块列表
	for (int j = 0; j < m_deleteIronList.size(); j++)
	{
		auto Iron = m_deleteIronList.at(j);
		m_deleteIronList.eraseObject(Iron);
		m_irList.eraseObject(Iron);
		Iron->Blast();
	}
	// 清除删除回旋镖列表
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

	//记录坦克数据，方便画图
	CCLOG((std::to_string(player1->getPosition().y / 16) + " " + std::to_string(player1->getPosition().x / 16)).c_str());

	//初始化声音系统
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	// 收到传来的开火消息的坦克执行Fire

	// 维护坦克列表
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

		// 绘制尚未绘制的坦克-针对后连进来的客户端
		if (notDraw)
		{
			this->addChild(nowTank);
			m_drawList.pushBack(nowTank);
		}
	}

	//处理死亡坦克的子弹
	deadTankBullet(audio);

	// 坦克与 坦克，物品的碰撞检测
	//注意！如果修改子弹碰撞的话要注意 死亡坦克和或者的坦克要分开处理！
	forTankCollide(audio);

	//回旋镖
	boomerangCollide(player1, audio);
	boomerangCollide(player2, audio);
	if (boss != nullptr)
		bossCollide(audio);

	//boss攻击模式
	if (boss != nullptr) {
		if (player1->getXp() < 1000) {
			boss->AI(nowTime, player2->getPosition(), player2->getPosition());
		}
		else if (player2->getXp() < 1000)
			boss->AI(nowTime, player1->getPosition(), player1->getPosition());
		else
			boss->AI(nowTime, player1->getPosition(), player2->getPosition());
	}

	//围墙
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

	//游戏胜利
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

	//游戏失败
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

//转到胜利界面
void GameClient::win() {
	//isRestart = true;
	Director::getInstance()->pushScene(RankList::createScene(nowTime, true));
}

//转到失败界面
void GameClient::fail() {
	//isRestart = true;
	Director::getInstance()->pushScene(Fail::createScene());
}
//boss地形技能
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
				// 中间留空形成回字
				continue;
			}
			auto brick = Brick::create(Vec2(position.x + (0.5 - i) * 16, position.y + (0.5 - j) * 16));
			m_bgList.pushBack(brick);
			this->addChild(brick, 2);
		}
	}
}

//游戏重新开始
void GameClient::gameRestart() {
	ini.time = nowTime;
	auto myScene = GameClient::createScene(GameData(ini));
	ini.bgmResume = true;
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, myScene, Color3B(0, 255, 255)));
}

//画地图
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

	//画草
	auto map = TMXTiledMap::create("image/map/map.tmx");
	this->addChild(map, 10);
	//画家
	auto home = Home::create(Vec2(30 * 16, 1 * 16));
	this->addChild(home, 2);
	this->home = home;
	//画海

	for (int i = -8; i <= 7; ++i) {
		for (int j = -1; j < 2; ++j) {
			auto sea = Sea::create(Vec2(29 * 16 + (0.5 - i) * 16, 25 * 16 + (0.5 - j) * 16));
			m_seaList.pushBack(sea);
			this->addChild(sea, 2);
		}
	}

	//画铁块
	for (int i = -4; i <= 3; ++i) {
		for (int j = 0; j < 2; ++j) {
			auto iron = Iron::create(Vec2(29 * 16 + (0.5 - i) * 16, 16 * 16 + (0.5 - j) * 16));
			m_irList.pushBack(iron);
			this->addChild(iron, 2);
		}
	}
	//画门
	door = Door::create(Vec2(29 * 16 + 0.5 * 16, 38 * 16 + 0.5 * 16));
	this->addChild(door, 2);

	//画围墙
	drawBigBG(Vec2(30 * 16, 1 * 16));
	//左边
	//竖
	for (int j = -28; j < 0; ++j) {
		auto brick = Brick::create(Vec2(46 * 16 + 0.5 * 16, 4 * 16 + (0.5 - j) * 16));
		m_bgList.pushBack(brick);
		this->addChild(brick, 2);
	}
	//横
	for (int i = -8; i <= 7; ++i) {
		for (int j = 0; j < 2; ++j) {
			auto brick = Brick::create(Vec2(44 * 16 + (0.5 - i) * 16, 31 * 16 + (0.5 - j) * 16));
			m_bgList.pushBack(brick);
			this->addChild(brick, 2);
		}
	}

	//右边
	//竖
	for (int j = -28; j < 0; ++j) {
		auto brick = Brick::create(Vec2(13 * 16 + 0.5 * 16, 4 * 16 + (0.5 - j) * 16));
		m_bgList.pushBack(brick);
		this->addChild(brick, 2);
	}
	//横
	for (int i = -8; i <= 7; ++i) {
		for (int j = 0; j < 2; ++j) {
			auto brick = Brick::create(Vec2(15 * 16 + (0.5 - i) * 16, 31 * 16 + (0.5 - j) * 16));
			m_bgList.pushBack(brick);
			this->addChild(brick, 2);
		}
	}
}
void GameClient::drawMap2() {
	//画草
	auto map = TMXTiledMap::create("image/map/map1.tmx");
	this->addChild(map, 10);

	//画家
	auto home = Home::create(Vec2(30 * 16, 1 * 16));
	this->addChild(home, 2);
	this->home = home;
	//画围墙
	drawBigBG(Vec2(30 * 16, 1 * 16));

	//画门
	door = Door::create(Vec2(29 * 16 + 0.5 * 16, 38 * 16 + 0.5 * 16));
	this->addChild(door, 2);

	//画海
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

	//画铁块
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

	//画砖
	drawBigBG(Vec2(6 * 16, 19 * 16));
	drawBigBG(Vec2(51 * 16, 19 * 16));

}
void GameClient::drawBoss() {
	boss = Boss::create(111, WINDOWWIDTH / 2, WINDOWHEIGHT - 200, 1, 2);
	this->addChild(boss, 2);
}

// 绘制单个回字砖块
void GameClient::drawBigBG(Vec2 position)
{
	for (int i = -2; i < 4; i++)
	{
		for (int j = -2; j < 4; j++)
		{
			if ((i == 1) && (j == 0) || (i == 0) && (j == 0) || (i == 1) && (j == 1) || (i == 0) && (j == 1))
			{
				// 中间留空形成回字
				continue;
			}
			auto brick = Brick::create(Vec2(position.x + (0.5 - i) * 16, position.y + (0.5 - j) * 16));
			m_bgList.pushBack(brick);
			this->addChild(brick, 2);
		}
	}
}

//键盘监听函数
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
		//冲刺技能
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
		//重新开始场景
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
		//当玩家到门的时候
		if (door != nullptr)
			if (player1->getRect().intersectsRect(door->getRect())) {
				//下一关
				if (m_tankList.size() <= 2) {
					Director::getInstance()->replaceScene(TransitionSlideInT::create(1, Loading::createScene(GameData(ini.mapType + 1, player1->getXp(), player2->getXp(), nowTime))));
				}
				//不刷怪
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
		//当玩家到门的时候
		if (door != nullptr)
			if (player2->getRect().intersectsRect(door->getRect())) {
				//下一关
				if (m_tankList.size() <= 2) {
					Director::getInstance()->replaceScene(TransitionSlideInT::create(1, Loading::createScene(GameData(ini.mapType + 1, player1->getXp(), player2->getXp(), nowTime))));
				}
				//不刷怪
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