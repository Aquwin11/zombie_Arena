#pragma once
#include "Zombie.h"

using namespace sf;

int CreateBackground(VertexArray &rVa,IntRect area);
Zombie* CreateZombieHorde(int numZombies, IntRect area);
