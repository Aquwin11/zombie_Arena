#include<SFML/Graphics.hpp>
#include "Zombie.h"
#include "ZombieArena.h"

Zombie* CreateZombieHorde(int numZombie, IntRect area) {

	Zombie* zombie = new Zombie[numZombie];

	int maxY = area.height - 20;
	int minY = area.top + 20;
	int maxX = area.width - 20;
	int minX = area.left + 20;

	for (int i = 0; i < numZombie; i++)
	{
		srand((int)time(0) * i);
		int side = (rand() % 4);
		float x, y;
		switch (side)
		{
			//Left
		case 0:
			x = minX;
			y = (rand() % maxY) + minY;
			break;
			//right
		case 1:
			x = maxX;
			y = (rand() % maxY) + minY;
			break;
			//top
		case 2:
			x = (rand() % maxX) + minX;
			y = minY;
			break;
			//bottom
		case 3:
			x = (rand() % maxX) + minX;
			y = maxY;
			break;
		}
		srand((int)time(0) * i * 2);
		int z_type = (rand() % 3);
		zombie[i].Spawn(x, y, z_type, i);
		zombie[i].setSpriteAlpha(255);
	}
	return zombie;
}