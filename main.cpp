#include "Map.h"
#include "Player.h"
#include "Position2D.h"

int main()
{
	Map map;
	map.ReadConfigFile();
	map.InitMap();

	Player player(5, 5);

	map.Draw(player);
	while (true)
	{
		HandleInput(player, map);
	}
}