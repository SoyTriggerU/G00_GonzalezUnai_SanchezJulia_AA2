#include "Map.h"
#include "Player.h"
#include "Position2D.h"
#include "Windows.h"

int main()
{
	Map map;
	map.ReadConfigFile();
	map.InitMap();

	Player player(5, 5);

	map.Draw(player);
	while (true)
	{
		Sleep(10);
		HandleInput(player, map);
	}
}