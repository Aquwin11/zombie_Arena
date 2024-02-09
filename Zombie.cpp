#include <SFML/Graphics.hpp>
#include "Zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>

using namespace std;

void Zombie::Spawn(float startX, float startY, int zomType, int seed)
{
	switch (zomType)
	{
		//Bloater
	case 0:
		z_Speed = BLOATER_SPEED;
		z_Health = BLOATER_HEALTH;
		z_Sprite = Sprite(TextureHolder::GetTexture("graphics/bloater.png"));
		break;
		//Chaser
	case 1:
		z_Speed = CHASER_SPEED;
		z_Health = CHASER_HEALTH;
		z_Sprite = Sprite(TextureHolder::GetTexture("graphics/chaser.png"));
		break;
		//Crawler
	case 2:
		z_Speed = CRAWLER_SPEED;
		z_Health = CRAWLER_HEALTH;
		z_Sprite = Sprite(TextureHolder::GetTexture("graphics/crawler.png"));
		break;
	}

	srand((int)time(0) * seed);
	float modifier = (rand() % MAX_VARIENCE) + VARIENCE_OFFSET;

	modifier /= 100;
	z_Speed *= modifier;
	z_Position = Vector2f(startX, startY);

	z_Sprite.setOrigin(25, 25);

	z_Sprite.setPosition(z_Position);
}

bool Zombie::Hit(float damge)
{
	z_Health-=damge;

	if (z_Health < 0)
	{
		// dead
		z_IsAlive = false;
		z_Sprite.setTexture(TextureHolder::GetTexture(
			"graphics/blood.png"));
		z_zombieSpriteColor = z_Sprite.getColor();
		z_isFading = true;
		return true;
	}
	return false;

}

FloatRect Zombie::GetPosition()
{
	return z_Sprite.getGlobalBounds();
}

Sprite Zombie::GetSprite()
{
	return z_Sprite;
}

bool Zombie::IsAlive()
{
	return z_IsAlive;
}
bool Zombie::IsFading()
{
	return z_isFading;
}

void Zombie::setSpriteAlpha(float value)
{
	z_splatOpacity = value;
}
void Zombie::Update(float elaspedTime, Vector2f playerpos)
{
	if (playerpos.x > z_Position.x)
	{
		z_Position.x = z_Position.x + (z_Speed * elaspedTime);
	}
	if (playerpos.y > z_Position.y)
	{
		z_Position.y = z_Position.y + (z_Speed * elaspedTime);
	}
	if (playerpos.x < z_Position.x)
	{
		z_Position.x = z_Position.x - (z_Speed * elaspedTime);
	}
	if (playerpos.y < z_Position.y)
	{
		z_Position.y = z_Position.y - (z_Speed * elaspedTime);
	}
	z_Sprite.setPosition(z_Position);

	float angle = (atan2(playerpos.y - z_Position.y, playerpos.x - z_Position.x) * 180) / 3.141;
	z_Sprite.setRotation(angle);
}

void Zombie::FadeSplat(float elaspedTime)
{
	if (z_isFading)
	{
		z_splatOpacity -= FADE_SPEED * elaspedTime;
		if (z_splatOpacity <= 0)
		{
			z_isFading = false;
			z_splatOpacity = 0;
		}
	}
	z_Sprite.setColor(Color(z_zombieSpriteColor.r, z_zombieSpriteColor.g, z_zombieSpriteColor.b, z_splatOpacity));
}