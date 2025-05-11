#include "Map.h"
#include "Player.h"
#include "Position2D.h"
#include "NPCs.h"

int main()
{
	Map map;
	Player player(5, 5);
	map.ReadConfigFile();
	map.InitMap();
	map.InitNPCs(player);

	

	//map.Draw(player);
	while (true)
	{
		//HandleInput(player, map);
	}
}