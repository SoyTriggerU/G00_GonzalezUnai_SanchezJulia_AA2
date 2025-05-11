#include <cstdlib>
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

			// Si hay dinero en la celda, el jugador lo recoge
			if (map.getCell(newPos.x, newPos.y) == Map::CellType::MONEY)
			{
				Zone zone = player.GetCurrentZone();
				int maxAmount = 0;

				switch (zone) {
				case Zone::LOS_SANTOS:
					maxAmount = map.moneyBeatingNPCs_LosSantos;
					break;
				case Zone::SAN_FIERRO:
					maxAmount = map.moneyBeatingNPCs_SanFierro;
					break;
				default:
					maxAmount = 0;
					break;
				}

				if (maxAmount > 0) {
					int randomAmount = 1 + rand() % maxAmount;
					player.AddMoney(randomAmount);
				}
				else {
					player.AddMoney(1);
				}

				map.setCell(newPos.x, newPos.y, Map::CellType::EMPTY); // Eliminar el dinero de la celda
			}
		}
		map.Draw(player);
	}
}