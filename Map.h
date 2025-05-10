#pragma once
#include <fstream>
#include <iostream>
#include <string>

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
	const char PEDESTRIAN = 'P';
	const char MONEY = '$';

	int width;
	int height;
	int numPedestrians_LosSantos;
	int tax_LosSantos_SanFierro;
	int moneyBeatingPedestrian_LosSantos;
	int numPedestrians_SanFierro;
	int tax_SanFierro_LasVenturas;
	int moneyBeatingPedestrian_LasVenturas;

	CellType** map;

public:
	void ReadConfigFile()
	{
		std::ifstream file("config.txt");
		if (!file.is_open())
		{
			std::cout << "Error openeing the file, try again" << std::endl;
			return;
		}
		std::cout << "File opened successfully!" << std::endl;
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

	}

	void Draw()
	{

	}
};