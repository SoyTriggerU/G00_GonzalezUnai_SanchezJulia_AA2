#include "Map.h"
#include "Player.h"
#include "Position2D.h"

int main()
{
	Map map;
	std::vector<NPCs> npc;
	map.ReadConfigFile();
	map.InitMap();

	Player player(5, 5);

	map.Draw(player);
	while (true)
	{
		HandleInput(player, map);
	}
}