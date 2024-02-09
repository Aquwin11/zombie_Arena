#include<SFML/Graphics.hpp>
#include"PickUp.h"
#include "TextureHolder.h"

using namespace sf;

PickUp::PickUp(int type)
{
	m_type = type;
	if (m_type == 1)
	{
		m_sprite = Sprite(TextureHolder::GetTexture("graphics/health_pickup.png"));
		m_Value = START_HEALTH_VALUE;
	}
	else
	{
		m_sprite = Sprite(TextureHolder::GetTexture("graphics/ammo_pickup.png"));
		m_Value = START_AMMO_VALUE;
	}
	m_sprite.setOrigin(25, 25);
	m_SecondToLive = START_SECOND_TO_LIVE;
	m_SecondToWait = START_WAIT_TIME;

}

void PickUp::SetArena(IntRect arena)
{
	m_Arena.left = arena.left + 50;
	m_Arena.width = arena.width - 50;
	m_Arena.top = arena.top + 50;
	m_Arena.height = arena.height - 50;

	Spawn();
}

void PickUp::Spawn()
{
	srand((int)time(0) / m_type);
	int x = (rand() % m_Arena.width);
	srand((int)time(0) / m_type);
	int y = (rand() % m_Arena.height);

	m_SecondSinceSpawn = 0;
	m_Spawned = true;

	m_sprite.setPosition(x, y);

}

bool PickUp::isSpawned()
{
	return m_Spawned;
}

Sprite PickUp::GetSprite()
{
	return m_sprite;
}

FloatRect PickUp::GetPostion()
{
	return m_sprite.getGlobalBounds();
}

int PickUp::Gotit()
{
	m_Spawned = false;
	m_SecondSinceDeSpawn = 0;
	return m_Value;
}

void PickUp::Update(float elapsedTime)
{
	if (m_Spawned)
	{
		m_SecondSinceSpawn += elapsedTime;
	}
	else
	{
		m_SecondSinceDeSpawn += elapsedTime;
	}
	if (m_SecondSinceSpawn > m_SecondToLive && m_Spawned)
	{
		m_Spawned = false;
		m_SecondSinceDeSpawn = 0;
	}
	if (m_SecondSinceDeSpawn > m_SecondToWait && !m_Spawned)
	{
		Spawn();
	}
}

void PickUp::Upgrade()
{
	if (m_type == 1)
	{
		m_Value += (START_HEALTH_VALUE * .5);
	}
	else
	{
		m_Value += (START_AMMO_VALUE * .5);
	}

	//spawn often and last longer
	m_SecondToLive += (START_SECOND_TO_LIVE / 10);
	m_SecondToWait -= (START_WAIT_TIME / 10);
}