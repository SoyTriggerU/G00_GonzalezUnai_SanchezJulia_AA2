#pragma once
#include "Position2D.h"
#include "Zone.h"

enum class Behaviour
{
	Agresivo,
	Neutral
};

class NPCs
{
private:
	Position2D pos;
	bool isDead;
	bool isMoving;
	Zone zone;
	Behaviour behaviour;

public:
	NPCs(int spawnX, int spawnY, Zone spawnZone)
	{
		pos.x = spawnX;
		pos.y = spawnY;
		isDead = false;
		zone = spawnZone;
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

	Zone GetZone() const
	{
		return zone;
	}

	bool GetIsDead()
	{
		return isDead;
	}

	void Die()
	{
		this->isDead = true;
	}

	Behaviour GetBehaviour()
	{
		return behaviour;
	}
};