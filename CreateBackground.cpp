#include "SFML/Graphics.hpp"
#include "ZombieArena.h"

using namespace sf;

int CreateBackground(VertexArray& rVa, IntRect area)
{
	const int TILESIZE = 50;
	const int TILE_TYPE = 3;
	const int VERTS_IN_QUAD = 4;

	int worldWidth = area.width / TILESIZE;
	int worldHeight = area.height/ TILESIZE;

	rVa.setPrimitiveType(Quads);

	rVa.resize(worldWidth * worldHeight * VERTS_IN_QUAD);

	int currentIndex = 0;

	

	for (int w = 0; w < worldWidth; w++)
	{
		for (int h = 0; h < worldHeight; h++)
		{
			rVa[currentIndex + 0].position = Vector2f(w * TILESIZE, h * TILESIZE);
			rVa[currentIndex + 1].position = Vector2f((w * TILESIZE)+TILESIZE, h * TILESIZE);
			rVa[currentIndex + 2].position = Vector2f((w * TILESIZE)+TILESIZE, (h * TILESIZE)+TILESIZE);
			rVa[currentIndex + 3].position = Vector2f(w * TILESIZE, (h * TILESIZE)+TILESIZE);

			if ((h == 0 || h == worldHeight - 1) ||
				(w == 0 || w == worldWidth - 1))
			{
				rVa[currentIndex + 0].texCoords = Vector2f(0, 0 + TILE_TYPE * TILESIZE);
				rVa[currentIndex + 1].texCoords = Vector2f(TILESIZE, 0 + TILE_TYPE * TILESIZE);
				rVa[currentIndex + 2].texCoords = Vector2f(TILESIZE, TILESIZE + TILE_TYPE * TILESIZE);
				rVa[currentIndex + 3].texCoords = Vector2f(0, TILESIZE + TILE_TYPE * TILESIZE);
			}
			else
			{
				srand((int)time(0) + h * w - h);
				int mOrg = (rand() % TILE_TYPE);
				int verticalOffset = mOrg * TILESIZE;
				rVa[currentIndex + 0].texCoords = Vector2f(0, 0 + verticalOffset);
				rVa[currentIndex + 1].texCoords = Vector2f(TILESIZE, 0 + verticalOffset);
				rVa[currentIndex + 2].texCoords = Vector2f(TILESIZE, TILESIZE + verticalOffset);
				rVa[currentIndex + 3].texCoords = Vector2f(0, TILESIZE + verticalOffset);

			}
			currentIndex = currentIndex + VERTS_IN_QUAD;
		}
	}
	return TILESIZE;
}