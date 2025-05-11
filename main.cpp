#include "Map.h"
#include "Player.h"
#include "Position2D.h"
#include "NPCs.h"

int main()
{
	srand(time(NULL));
	Map map;
	Player player(5, 5);
	std::vector<NPCs> npc;
	map.ReadConfigFile();
	map.InitMap();
	map.InitNPCs(player);

	map.Draw(player);
	while (true)
	{
		HandleInput(player, map);
		map.KillingNPCs(player, npc);
		map.MovementNPCs(player, npc);
		Sleep(150);
	}
}