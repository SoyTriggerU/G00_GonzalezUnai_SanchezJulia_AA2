#pragma once
#include "Position2D.h"

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

public:
	Player(int startX, int startY)
	{
		pos.x = startX;
		pos.y = startY;
		currentMoney = 0;
		direction = Direction::DOWN;
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
};

void HandleInput(Player& player, Map& map);

