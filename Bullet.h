#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
class Bullet {
private:
	//Pistol
	const float PISTOL_BULLET_SPEED = 700;
	const bool PISTOL_SPREAD = false;
	const float PISTOL_BULLET_SIZE =3;
	const float PISTOL_DAMAGE = 1;

	//Assualt RIFLE
	const float RIFLE_BULLET_SPEED = 1500;
	const bool  RIFLE_SPREAD = false;
	const float RIFLE_BULLET_SIZE = 2;
	const float RIFLE_DAMAGE = 0.25;

	//SNIPER
	const float SNIPER_BULLET_SPEED = 2500;
	const bool  SNIPER_SPREAD = false;
	const float SNIPER_BULLET_SIZE = 7;
	const float SNIPER_DAMAGE = 6;

	//SHOTGUN
	const float SHOTGUN_BULLET_SPEED = 1000;
	const bool  SHOTGUN_SPREAD = true;
	const float SHOTGUN_BULLET_SIZE = 5;
	const float SHOTGUN_DAMAGE = 2;

	Vector2f b_position;
	RectangleShape b_BulletShape;
	bool b_InFlight=false;
	float b_BulletSpeed;
	bool b_spread;
	float b_BulletSize;
	float b_damge;
	float b_BulletX;
	float b_BulletY;

	float b_minX;
	float b_minY;
	float b_maxX;
	float b_maxY;
public:
	void stop();
	bool IsInFlight();
	void shoot(float startX, float startY, float targetX, float targetY,int type);
	FloatRect GetPosition();
	RectangleShape GetShape();
	void Update(float elapseTime);
	void SwitchGuns(int type);
	float GetDamage();
	float GetBulletSize();
	void SetBulletSize(float size);
};

