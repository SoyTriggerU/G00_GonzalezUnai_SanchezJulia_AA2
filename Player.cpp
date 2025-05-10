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

	if (GetAsyncKeyState(VK_UP) & 0x08000)
	{
		newDir = Direction::UP;
		newPos.y--;
		//map.Draw(player);
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x08000)
	{
		newDir = Direction::DOWN;
		newPos.y++;
		//map.Draw(player);
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x08000)
	{
		newDir = Direction::LEFT;
		newPos.x--;
		//map.Draw(player);
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x08000)
	{
		newDir = Direction::RIGHT;
		newPos.x++;
		//map.Draw(player);
	}

	if (newPos.x > 0 && newPos.x < mapWidth && newPos.y > 0 && newPos.y < mapHeight)
	{
		if (!map.isWall(newPos.x, newPos.y))
		{
			player.SetDirection(newDir);
			player.SetPos(newPos.x, newPos.y);
		}
	}
}
