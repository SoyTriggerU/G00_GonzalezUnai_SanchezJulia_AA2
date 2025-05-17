#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <Windows.h>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include "Player.h"
#include "CursorControl.h"
#include "NPCs.h"


class Map
{
public:
	enum class CellType
	{
		EMPTY,
		WALL,
		NPC,
		PLAYER,
		MONEY,
		TOLL
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
	int moneyBeatingNPCs_LosSantos;
	int numNPCs_SanFierro;
	int tax_SanFierro_LasVenturas;
	int moneyBeatingNPCs_SanFierro;

	// Odd numbers so the player is in the middle
	const int cameraWidth = widthCenter;
	const int cameraHeight = heightCenter;
	
	int randomY;
	std::vector<NPCs> npcs;

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
						moneyBeatingNPCs_LosSantos = num;
						break;
					case 5:
						numNPCs_SanFierro = num;
						break;
					case 6:
						tax_SanFierro_LasVenturas = num;
						break;
					case 7:
						moneyBeatingNPCs_SanFierro = num;
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

		if (height > 0)
		{
			randomY = rand() % height;
		}
		else {}

		// Initializing in each position
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < totalWidth; x++)
			{
				if (x == 0 || x == totalWidth - 1 || y == 0 || y == height - 1)
				{
					map[y][x] = CellType::WALL;
				}
				else if (x == totalWidth / 3 || x == 2 * totalWidth / 3)
				{
					if (y == randomY)
						map[y][x] = CellType::TOLL;
					else
						map[y][x] = CellType::WALL;
				}
				else
					map[y][x] = CellType::EMPTY;
			}
		}
	}

	CellType getCell(int x, int y) const 
	{
		return map[y][x];
	}

	void setCell(int x, int y, CellType cellType) 
	{
		map[y][x] = cellType;
	}

	bool isWall(int x, int y) const
	{
		if (x < 0 || x >= GetTotalWidth() || y < 0 || y >= height)
			return true;
		return map[y][x] == CellType::WALL;
	}

	void ClearScreen() 
	{
		COORD coord = { 0, 0 }; // Position top-left
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}

	void RemoveNPC(int x, int y)
	{
		map[y][x] = CellType::EMPTY;
	}

	void PlaceNPC(int x, int y)
	{
		map[y][x] = CellType::NPC;
	}

	void InitNPCs(Player& player) 
	{
		SetNPCsOnMap(npcs, numNPCs_LosSantos, Zone::LOS_SANTOS);  
        SetNPCsOnMap(npcs, numNPCs_SanFierro, Zone::SAN_FIERRO);
	}

	void SetNPCsOnMap(std::vector<NPCs>& npc, int numNPCs, Zone zone)
	{
		int totalWidth = GetTotalWidth();
		int startX;
		int endX;

		if (zone == Zone::LOS_SANTOS)
		{
			startX = 0;
			endX = totalWidth / 3;
		}
		else if (zone == Zone::SAN_FIERRO)
		{
			startX = totalWidth / 3;
			endX = (2 * totalWidth) / 3;
		}

		int counter = 0;
		while (counter < numNPCs)
		{
			int posX = startX + rand() % (endX - startX);
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

	void KillingNPCs(Player& player)
	{
		for (int i = 0; i < npcs.size(); i++)
		{
			if (abs(player.GetPos().x - npcs[i].GetPos().x) <= 1 && abs(player.GetPos().y - npcs[i].GetPos().y) <= 1)
			{
				if (GetAsyncKeyState(VK_SPACE) & 0x8000)
				{
					int npcX = npcs[i].GetPos().x;
					int npcY = npcs[i].GetPos().y;
					Zone zone = npcs[i].GetZone();

					npcs[i].Die();
					map[npcY][npcX] = CellType::MONEY;
					npcs.erase(npcs.begin() + i);
				}
			}
		}
	}

	void MovementNPCs(Player& player)
	{
		int totalWidth = GetTotalWidth();

		for (NPCs& npc : npcs)
		{
			if (!npc.GetIsDead())
			{				
				Zone zone = npc.GetZone();

				int npcX = npc.GetPos().x;
				int npcY = npc.GetPos().y;
				int playerX = player.GetPos().x;
				int playerY = player.GetPos().y;

				if (abs(playerX - npcX) > 1 || abs(playerY - npcY) > 1)
				{
					int newX = npcX;
					int newY = npcY;

					int dir = rand() % 4;
					switch (dir)
					{
					case 0: newX--; break; // Moving left
					case 1: newX++; break; // Moving right
					case 2: newY--; break; // Moving up
					case 3: newY++; break; // Moving down
					default: break;
					}

					if ((newX >= 0 && newX < totalWidth && newY >= 0 && newY < height) && map[newY][newX] == CellType::EMPTY)
					{
						RemoveNPC(npcX, npcY);
						PlaceNPC(newX, newY);
						npc.SetPos(newX, newY);
					}
				}
			
			}								
		}
	}

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
				case CellType::TOLL:
					std::cout << "T";
					break;
				default:
					std::cout << ' ';
					break;
				}
			}
			std::cout << std::endl;
		}
		std::cout << "\nCJ's money: $" << player.GetMoney() << std::endl;

		ShowCursor();
	}

	~Map() {
		for (int i = 0; i < height; i++)
			delete[] map[i];
		delete[] map;
	}
};