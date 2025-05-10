#pragma once
#include "Position2D.h"
class NPCs
{
private:
	Position2D pos;
	bool isDead;
	bool isMoving;
public:
	NPCs(int spawnX, int spawnY)
	{
		pos.x = spawnX;
		pos.y = spawnY;
		isDead = false;
	}

	void SetPos(int x, int y)
	{
		pos.x = x;
		pos.y = y;
	}

	Position2D GetPos()
	{
		return pos;
	}

};

