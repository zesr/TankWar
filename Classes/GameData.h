#pragma once
struct GameData {
	GameData(int map, int xp1, int xp2, float t,bool bgm=false) :mapType(map), player1Xp(xp1), player2Xp(xp2), time(t), bgmResume(bgm) {}
	GameData() {}
	bool bgmResume;
	int mapType;
	int player1Xp;
	int player2Xp;
	float time;
};