#include<SFML/Graphics.hpp>
#include"Bullet.h"


sf::FloatRect Bullet::GetPosition()
{
	return b_BulletShape.getGlobalBounds();
}

bool Bullet::IsInFlight()
{
	return b_InFlight;

}
void Bullet::stop()
{
	b_InFlight = false;
}
sf::RectangleShape Bullet::GetShape() {
	return b_BulletShape;
}
float Bullet::GetDamage()
{
	return b_damge;
}
void Bullet::SetBulletSize(float size)
{
	b_BulletShape.setSize(Vector2f(size, size));
}
float Bullet::GetBulletSize()
{
	return b_BulletSize;
}
void Bullet::SwitchGuns(int type)
{
	switch (type)
	{
	case 1:
		//pistol
		b_BulletSize = PISTOL_BULLET_SIZE;
		b_spread = PISTOL_SPREAD;
		b_damge = PISTOL_DAMAGE;
		b_BulletSpeed = PISTOL_BULLET_SPEED;
		break;
	case 2:
		//Rifle
		b_BulletSize = RIFLE_BULLET_SIZE;
		b_spread = RIFLE_SPREAD;
		b_damge = RIFLE_DAMAGE;
		b_BulletSpeed = RIFLE_BULLET_SPEED;
		break;
	case 3:
		//Sniper
		b_BulletSize = SNIPER_BULLET_SIZE;
		b_spread = SNIPER_SPREAD;
		b_BulletSpeed = SNIPER_BULLET_SPEED;
		b_damge = SNIPER_DAMAGE;
		break;
	case 4:
		//
		b_BulletSize = SHOTGUN_BULLET_SIZE;
		b_spread = SHOTGUN_SPREAD;
		b_BulletSpeed = SHOTGUN_BULLET_SPEED;
		b_damge = SHOTGUN_DAMAGE;
		break;
	}
}
void Bullet::shoot(float startX, float startY, float targetX, float targetY,int type)
{
	SwitchGuns(type);
	b_InFlight = true;
	b_position = Vector2f(startX, startY);

	float gradient = (startX - targetX) / (startY - targetY);
	if (gradient < 0)
	{
		gradient *= -1;
	}
	float ratioXY = b_BulletSpeed / (1 + gradient);
	b_BulletY = ratioXY;
	b_BulletX = ratioXY*gradient;
	if (targetX < startX)
	{
		b_BulletX *= -1;
	}
	if (targetY < startY)
	{
		b_BulletY *= -1;
	}
	float range = 1000;
	b_minX = startX - range;
	b_minY = startY - range;
	b_maxX = targetX + range;
	b_maxY = targetY + range;

	b_BulletShape.setPosition(b_position);
}

void Bullet::Update(float elapseTime)
{
	b_position.x += b_BulletX * elapseTime;
	b_position.y += b_BulletY * elapseTime;
	b_BulletShape.setPosition(b_position);
	if (b_position.x<b_minX || b_position.y < b_minY || b_position.x > b_maxX || b_position.y > b_maxY)
	{
		b_InFlight = false;
	}
}
