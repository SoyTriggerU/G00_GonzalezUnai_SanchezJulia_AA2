#pragma once

struct Position2D
{
	int x;
	int y;

	// Including operators to compare positions
	bool operator==(Position2D& otherPos)
	{
		return x == otherPos.x && y == otherPos.y;
	}
};
