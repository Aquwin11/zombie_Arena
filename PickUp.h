#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class PickUp
{
private:
	const int START_HEALTH_VALUE = 50;
	const int START_AMMO_VALUE = 12;
	const int RIFLE_START_AMMO_VALUE = 20;
	const int SNIPER_START_AMMO_VALUE = 1;
	const int SHOTGUN_START_AMMO_VALUE = 2;
	const int START_WAIT_TIME = 10;
	const int START_SECOND_TO_LIVE = 10;


	Sprite m_sprite;

	IntRect m_Arena;

	int m_Value;

	int m_type;

	bool m_Spawned;
	float m_SecondSinceSpawn;
	float m_SecondSinceDeSpawn;
	float m_SecondToLive;
	float m_SecondToWait;

public:
	PickUp(int type);
	void SetArena(IntRect arena);
	Sprite GetSprite();
	void Spawn();
	void SpawnMeds(FloatRect pos);
	void Update(float elapasedTime);
	FloatRect GetPostion();
	int Gotit();
	void Upgrade();
	bool isSpawned();
};