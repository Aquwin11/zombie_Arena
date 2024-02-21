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
	else if(m_type==2)
	{
		m_sprite = Sprite(TextureHolder::GetTexture("graphics/ammo-pistol.png"));
		m_Value = START_AMMO_VALUE;
	}
	else if(m_type ==3)
	{
		m_sprite = Sprite(TextureHolder::GetTexture("graphics/ammo-rifle.png"));
		m_Value = RIFLE_START_AMMO_VALUE;
	}
	else if(m_type==4)
	{
		m_sprite = Sprite(TextureHolder::GetTexture("graphics/sniperammo_icon.png"));
		m_Value = SNIPER_START_AMMO_VALUE;
	}
	else
	{
		m_sprite = Sprite(TextureHolder::GetTexture("graphics/ammo-shotgun.png"));
		m_Value = SHOTGUN_START_AMMO_VALUE;
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
	// srand should be called once at the start of the program.
	// Remove the reseeding from here to avoid resetting the random sequence.

	int x = (rand() % m_Arena.width);
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
	if (m_SecondSinceDeSpawn > m_SecondToWait && !m_Spawned && m_type!=1)
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
	else if(m_type==2)
	{
		m_Value += (START_AMMO_VALUE * .5);
	}
	else if(m_type==3)
	{
		m_Value += (RIFLE_START_AMMO_VALUE *5);
	}
	else if (m_type == 4)
	{
		m_Value += (SNIPER_START_AMMO_VALUE + 1);
	}
	else
	{
		m_Value += (SHOTGUN_START_AMMO_VALUE * 2);
	}

	//spawn often and last longer
	m_SecondToLive += (START_SECOND_TO_LIVE / 10);
	m_SecondToWait -= (START_WAIT_TIME / 10);
}

void PickUp::SpawnMeds(FloatRect pos)
{
	m_SecondSinceSpawn = 0;
	m_Spawned = true;

	m_sprite.setPosition(pos.getPosition().x, pos.getPosition().y);
}