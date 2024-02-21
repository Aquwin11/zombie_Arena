#pragma once
#include<SFML/Graphics.hpp>

using namespace sf;

class Zombie
{
private:
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;
	

	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	const float BLOATER_DAMAGE = 20;
	const float CHASER_DAMAGE = 15;
	const float CRAWLER_DAMAGE = 10;

	const int MAX_VARIENCE = 35;
	const int VARIENCE_OFFSET = 101 - MAX_VARIENCE;

	Vector2f z_Position;

	float z_Health;

	float z_Speed;

	Sprite z_Sprite;
	float z_damage;
	bool z_IsAlive;
	float z_type;
	bool z_isFading;
	float z_splatOpacity;
	const float FADE_SPEED=25;
	Color z_zombieSpriteColor;
	float z_ShootInterval = 15.0f; // Seconds between shots
	float z_LastShootTime = 0; // Track the last time shot was fired
	float z_ShootDistance = 200.0f; // Max distance to shoot

public:
	bool Hit(float damage);

	bool IsAlive();
	bool IsFading();

	void Spawn(float startX, float satrtY, int zomType, int seed);

	FloatRect GetPosition();

	Sprite GetSprite();
	float GetZ_Type();
	void Update(float elaspedTime, Vector2f pos);
	void FadeSplat(float elaspsedTime);
	void setSpriteAlpha(float alphaValue);
	float GetZomDamage();
	bool CanShoot(float elapsedTime, Vector2f playerPosition);
};
