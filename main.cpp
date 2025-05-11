#include <ctime>
#include "Map.h"
#include "Player.h"
#include "Position2D.h"
#include "NPCs.h"

int main()
{
	srand(time(NULL));
	Map map;
	Player player(5, 5);
	map.ReadConfigFile();
	map.InitMap();
	map.InitNPCs(player);

	map.Draw(player);
	while (true)
	{
		HandleInput(player, map);
		map.KillingNPCs(player);
		map.MovementNPCs(player);
		Sleep(150);
	}
}