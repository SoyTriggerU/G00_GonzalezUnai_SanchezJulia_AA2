#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include "Player.h"

class Map
{
private:
	enum class CellType
	{
		EMPTY,
		WALL,
		PEDESTRIAN,
		PLAYER,
		MONEY
	};

	const char PLAYER_UP = '^';
	const char PLAYER_DOWN = 'v';
	const char PLAYER_LEFT = '<';
	const char PLAYER_RIGHT = '>';
	const char WALL = 'X';
	const char NPC = 'P';
	const char MONEY = '$';

	int width;
	int height;
	int numNPCs_LosSantos;
	int tax_LosSantos_SanFierro;
	int moneyBeatingPedestrian_LosSantos;
	int numNPCs_SanFierro;
	int tax_SanFierro_LasVenturas;
	int moneyBeatingPedestrian_LasVenturas;

	CellType** map;

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

		while (!file.eof()) 
		{
			file.read(buffer, bufferSize);
			int length = file.gcount();

			for (int i = 0; i < length; ++i) 
			{
				char ch = buffer[i];

				if (ch == ';' || ch == '\n') 
				{
					value[valueIndex] = '\0'; // End current value

					int num = std::atoi(value);

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
					if (valueIndex < 31) 
					{
						value[valueIndex++] = ch;
					}
				}
			}
		}
		file.close();
	}

	Map()
	{

	}

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
		// We multiply the width by 3 because we need 3 maps
		int totalWidth = GetTotalWidth();
		// If we have no height and no width, we don't continue the method
		if (height == 0 && totalWidth == 0)
			return;

		// Reserving memory in heap
		map = new CellType * [height];
		for (int i = 0; i < height; i++)
		{
			map[i] = new CellType[totalWidth];
		}

		int bridge1 = 19;
		int bridge2 = 50;

		// Initializing in each position
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < totalWidth; x++)
			{
				// For the left, right, top and bottom walls that limit of each map
				if (x == 0 || x == totalWidth - 1 || y == 0 || y == height - 1)
					map[y][x] = CellType::WALL;

				// The bridges in the walls that separate the 3 maps
				else if ((x == width && y == height / 2) || (x == 2 * width && y == height / 2))
					map[y][x] = CellType::EMPTY;

				// In between the walls that separate the 3 maps
				else if (x == width || x == 2 * width)
					map[y][x] = CellType::WALL;

				// For the walls that separate the 3 maps 
				else
					map[y][x] = CellType::EMPTY;
			}
		}
	}

	void Draw(const Player& player)
	{
		system("cls");

		int totalWidth = GetTotalWidth();

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < totalWidth; x++)
			{
				bool drawn = false;

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
					drawn = true;
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
				default:
					std::cout << ' ';
					break;
				}
			}
			std::cout << "\n";
		}
	}
};