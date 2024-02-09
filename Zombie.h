#pragma once
#include<SFML/Graphics.hpp>

using namespace sf;

class Zombie
{
private:
	const float BLOATER_SPEED = 1;
	const float CHASER_SPEED = 1;
	const float CRAWLER_SPEED = 1;

	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	const int MAX_VARIENCE = 30;
	const int VARIENCE_OFFSET = 101 - MAX_VARIENCE;

	Vector2f z_Position;

	float z_Health;

	float z_Speed;

	Sprite z_Sprite;

	bool z_IsAlive;

	bool z_isFading;
	float z_splatOpacity;
	const float FADE_SPEED=25;
	Color z_zombieSpriteColor;
public:
	bool Hit(float damage);

	bool IsAlive();
	bool IsFading();

	void Spawn(float startX, float satrtY, int zomType, int seed);

	FloatRect GetPosition();

	Sprite GetSprite();

	void Update(float elaspedTime, Vector2f pos);
	void FadeSplat(float elaspsedTime);
	void setSpriteAlpha(float alphaValue);
};
