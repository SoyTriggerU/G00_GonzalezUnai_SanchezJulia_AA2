#include "Windows.h"
#include "Player.h"
#include "Position2D.h"
#include "Map.h"

void HandleInput(Player& player, Map& map)
{
	int mapWidth = map.GetTotalWidth();
	int mapHeight = map.GetHeight();

	Position2D newPos = player.GetPos();
	Direction newDir = player.GetDirection();

	bool keyPressed = false;

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		newDir = Direction::UP;
		newPos.y--;
		keyPressed = true;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		newDir = Direction::DOWN;
		newPos.y++;
		keyPressed = true;
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		newDir = Direction::LEFT;
		newPos.x--;
		keyPressed = true;
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		newDir = Direction::RIGHT;
		newPos.x++;
		keyPressed = true;
	}

	if (keyPressed)
	{
		player.SetDirection(newDir);

		if (newPos.x > 0 && newPos.x < mapWidth &&
			newPos.y > 0 && newPos.y < mapHeight &&
			!map.isWall(newPos.x, newPos.y))
		{
			player.SetPos(newPos.x, newPos.y);
		}

		map.Draw(player);
	}
}
