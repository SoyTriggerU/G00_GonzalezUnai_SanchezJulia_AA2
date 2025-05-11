#pragma once
#include "Position2D.h"
#include "Zone.h"
#include "NPCs.h"
#include <vector>

class Map;

enum class Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Player
{
private:
	Position2D pos;
	int currentMoney;
	Direction direction;
	Zone currentZone;

public:
	Player(int startX, int startY, Zone zone = Zone::LOS_SANTOS)
	{
		pos.x = startX;
		pos.y = startY;
		currentMoney = 0;
		direction = Direction::DOWN;
		currentZone = zone;
	}
	void SetPos(int x, int y)
	{
		pos.x = x;
		pos.y = y;
	}
	void SetDirection(Direction dir)
	{
		direction = dir;
	}
	Direction GetDirection() const
	{
		return direction;
	}

	Position2D GetPos() const
	{
		return pos;
	}

	Zone GetCurrentZone() const  
	{
		return currentZone;
	}

	void SetCurrentZone(Zone zone)
	{
		currentZone = zone;
	}

	void AddMoney(int amount) {
		currentMoney += amount;
	}

	int GetMoney() const {
		return currentMoney;
	}
};

void HandleInput(Player& player, Map& map);