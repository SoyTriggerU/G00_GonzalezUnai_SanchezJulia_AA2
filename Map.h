#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <Windows.h>
#include <algorithm>
#include "Player.h"
#include "CursorControl.h"
#include "NPCs.h"


class Map
{
private:
	enum class CellType
	{
		EMPTY,
		WALL,
		NPC,
		PLAYER,
		MONEY
	};

	CellType** map;

	const char PLAYER_UP = '^';
	const char PLAYER_DOWN = 'v';
	const char PLAYER_LEFT = '<';
	const char PLAYER_RIGHT = '>';
	const char WALL = 'X';
	const char NPC = 'P';
	const char MONEY = '$';

	const int widthCenter = 41;
	const int heightCenter = 21;

	int width;
	int height;
	int numNPCs_LosSantos;
	int tax_LosSantos_SanFierro;
	int moneyBeatingPedestrian_LosSantos;
	int numNPCs_SanFierro;
	int tax_SanFierro_LasVenturas;
	int moneyBeatingPedestrian_LasVenturas;

	// Odd numbers so the player is in the middle
	const int cameraWidth = widthCenter;
	const int cameraHeight = heightCenter;

	std::vector<NPCs> npcs;

public:
	void ReadConfigFile()
	{
		std::ifstream file("config.txt", std::ios::in);
		if (!file.is_open())
		{
			std::cout << "Error opening file.\n";
			return;
		}

		const int bufferSize = 256;
		char buffer[bufferSize];
		char value[32];
		int valueIndex = 0;
		int valueCount = 0;

		// Read line by line
		while (file.getline(buffer, bufferSize)) // We use getline to read a complete line
		{
			int i = 0;

			// We go through the line
			while (buffer[i] != '\0') // All the way to the end
			{
				char ch = buffer[i];

				if (ch == ';' || ch == '\n' || ch == '\0') // If we find a ; or a jump line
				{
					value[valueIndex] = '\0'; // End value

					int num = std::atoi(value); // Converts the value to an integer

					// We assign the value to the appropriate field according to valueCount
					switch (valueCount)
					{
					case 0:
						height = num;
						break;
					case 1:
						width = num;
						break;
					case 2:
						numNPCs_LosSantos = num;
						break;
					case 3:
						tax_LosSantos_SanFierro = num;
						break;
					case 4:
						moneyBeatingPedestrian_LosSantos = num;
						break;
					case 5:
						numNPCs_SanFierro = num;
						break;
					case 6:
						tax_SanFierro_LasVenturas = num;
						break;
					case 7:
						moneyBeatingPedestrian_LasVenturas = num;
						break;
					default:
						break;
					}

					valueCount++; 
					valueIndex = 0; 
				}
				else
				{
					// We add the character to the current value if it is not a delimiter
					if (valueIndex < 31)
					{
						value[valueIndex++] = ch;
					}
				}

				i++;
			}
		}

		file.close();
	}

	Map() {}

	int GetTotalWidth() const
	{
		return 3 * width;
	}

	int GetHeight() const
	{
		return height;
	}

	void InitMap()
	{
		int totalWidth = GetTotalWidth();
		// If we have no height and no width, we don't continue the method
		if (height == 0 && totalWidth == 0)
			return;

		// Reserving memory in heap
		// We start by creating height because the number of height is equivalent to the number of rows
		map = new CellType * [height];
		for (int i = 0; i < height; i++)
		{
			map[i] = new CellType[totalWidth];
		}

		// Initializing in each position
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < totalWidth; x++)
			{
				if (x == 0 || x == totalWidth - 1 || y == 0 || y == height - 1 || x == width || x == 2 * width)
				{
					// For the bridges in the walls
					if (y == height / 2)
						map[y][x] = CellType::EMPTY;

					// For the walls
					else
						map[y][x] = CellType::WALL;
				} 
				// For everything else
				else
					map[y][x] = CellType::EMPTY;
			}
		}
	}

	bool isWall(int x, int y) const
	{
		if (x < 0 || x >= GetTotalWidth() || y < 0 || y >= height)
			return true;
		return map[y][x] == CellType::WALL;
	}

	void ClearScreen() {
		COORD coord = { 0, 0 }; // Position top-left
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}

	void InitNPCs(Player& player) {
		Zone currentZone = player.GetCurrentZone();  // Obtener la zona del jugador

		// Colocar NPCs dependiendo de la zona del jugador
		switch (currentZone)
		{
        case Zone::LOS_SANTOS:
            SetNPCsOnMap(npcs, numNPCs_LosSantos, Zone::LOS_SANTOS);  // Crear NPCs para Los Santos
            break;
        case Zone::SAN_FIERRO:
            SetNPCsOnMap(npcs, numNPCs_SanFierro, Zone::SAN_FIERRO);  // Crear NPCs para San Fierro
            break;
        default:
            break;
		}
	}

	void SetNPCsOnMap(std::vector<NPCs>& npc, int numNPCs, Zone zone)
	{
		int totalWidth = GetTotalWidth();
		int counter = 0;
		while (counter < numNPCs)
		{
			int posX = 1 + rand() % (totalWidth - 2);
			int posY = 1 + rand() % (height - 2);

			if (map[posY][posX] == CellType::EMPTY)
			{
				NPCs newNPC(posX, posY, zone);
				npc.push_back(newNPC);

				map[posY][posX] = CellType::NPC;
				counter++;
			}
		}
	}

	void KillingNPCs(Player& player, std::vector<NPCs>& npcs)
	{

	}

	//void MovementNPCs(Player& player, std::vector<NPCs>& npcs)
	//{
	//	int totalWidth = GetTotalWidth();

	//	for (NPCs& npc : npcs)
	//	{
	//		if (npc.IsDead() == false)
	//		{
	//			Zone npcZone = npc.GetZone();
	//			int startX;
	//			int endX;

	//			if (npcZone == Zone::LOS_SANTOS)
	//			{
	//				startX = 0;
	//				endX = totalWidth / 3;
	//			}
	//			else if (npcZone == Zone::SAN_FIERRO)
	//			{
	//				startX = totalWidth / 3;
	//				endX = 2 * (totalWidth / 3);
	//			}

	//			int npcX = npc.GetPos().x;
	//			int npcY = npc.GetPos().y;

	//			int playerX = player.GetPos().x;
	//			int playerY = player.GetPos().y;

	//			if (map[npcY--][npcX] != map[playerY][playerX] ||
	//				map[npcY++][npcX] != map[playerY][playerX] || map[npcY][npcX--] != map[playerY][playerX]
	//				|| map[npcY][npcX++] != map[playerY][playerX])
	//			{
	//				int NPCdir = rand() % 2; // 0 = horizontal, 1 = vertical
	//				int NPCmove = (rand() % 2 == 0) ? -1 : 1;

	//				if (NPCdir == 0)
	//				{
	//					npcX += NPCmove;
	//					if (npcX < startX || npcX >= endX) npcX -= NPCmove;
	//				}
	//				else
	//				{
	//					npcY += NPCmove;
	//					if (npcY < 0 || npcY >= height) npcY -= NPCmove;
	//				}
	//				npc.SetPos(npcX, npcY);
	//				map[npcY][npcX] = CellType::NPC;
	//			}
	//		}								
	//	}
	//}

	void Draw(const Player& player)
	{
		HideCursor();
		ClearScreen();

		int totalWidth = GetTotalWidth();

		// Defining camera view
		int playerPosX = player.GetPos().x;
		int playerPosY = player.GetPos().y;

		// Left and top sides of the camera
		int cameraLeft = playerPosX - (cameraWidth / 2);
		int cameraTop = playerPosY - (cameraHeight / 2);
		if (cameraLeft <= 0) cameraLeft = 0;
		if (cameraTop <= 0) cameraTop = 0;

		// Right and bottom sides of the camera
		int cameraRight = cameraLeft + (cameraWidth - 1);
		int cameraBottom = cameraTop + (cameraHeight - 1);
		if (cameraRight >= totalWidth) cameraRight = totalWidth;
		if (cameraBottom >= height) cameraBottom = height;

		if (cameraBottom >= height) {
			cameraBottom = height - 1;
			cameraTop = (std::max)(0, cameraBottom - cameraHeight + 1); // Para mantener altura
		}

		// Get current player's zone
		Zone currentZone = player.GetCurrentZone();

		for (int y = cameraTop; y <= cameraBottom; y++)
		{
			for (int x = cameraLeft; x <= cameraRight; x++)
			{

				if (player.GetPos().x == x && player.GetPos().y == y)
				{
					switch (player.GetDirection())
					{
					case Direction::UP:
						std::cout << '^';
						break;
					case Direction::DOWN:
						std::cout << 'v';
						break;
					case Direction::LEFT:
						std::cout << '<';
						break;
					case Direction::RIGHT:
						std::cout << '>';
						break;
					}
					continue; // To skip impression of the original cell
				}

				switch (map[y][x])
				{
				case CellType::WALL:
					std::cout << 'X';
					break;
				case CellType::EMPTY:
					std::cout << ' ';
					break;
				case CellType::MONEY:
					std::cout << '$';
					break;
				case CellType::NPC:
					std::cout << "P";
					break;
				default:
					std::cout << ' ';
					break;
				}
			}
			std::cout << std::endl;
		}
		ShowCursor();
		Sleep(150);
	}

	~Map() {
		for (int i = 0; i < height; i++)
			delete[] map[i];
		delete[] map;
	}
};