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
#include "Walker.h"
#include "Car.h"
#include "BigSmoke.h"

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
		TOLL,
		CAR,
		BIG_SMOKE
	};

	CellType** map;

	const char PLAYER_UP = '^';
	const char PLAYER_DOWN = 'v';
	const char PLAYER_LEFT = '<';
	const char PLAYER_RIGHT = '>';
	const char WALL = 'X';
	const char NPC = 'P';
	const char MONEY = '$';
	const char CAR = 'C';
	const char BIG_SMOKE_CHAR = 'B';

	const int widthCenter = 41;
	const int heightCenter = 21;

	int width;
	int height;
	int CJsHealth;
	int CJsPower;
	int tax_LosSantos_SanFierro;
	int tax_SanFierro_LasVenturas;
	int numNPCs_LosSantos;
	int moneyBeatingNPCs_LosSantos;
	int healthNPCs_LosSantos;
	int powerNPCs_LosSantos;
	int numNPCs_SanFierro;
	int moneyBeatingNPCs_SanFierro;
	int healthNPCs_SanFierro;
	int powerNPCs_SanFierro;
	int numNPCs_LasVenturas;
	int moneyBeatingNPCs_LasVenturas;
	int healthNPCs_LasVenturas;
	int powerNPCs_LasVenturas;

	const int cameraWidth = widthCenter;
	const int cameraHeight = heightCenter;

	int randomY;
	std::vector<NPCs> npcs;
	std::vector<Car> cars;
	BigSmoke* bigSmoke;

	void ReadConfigFile();
	Map();
	~Map();

	// Move assignment operator
	Map& operator=(Map&& other) noexcept;	//ASK WHAT'S THIS CAUSE WITHOUT THIS NOTHING WORKED

	int GetTotalWidth() const;
	int GetHeight() const;
	void InitMap();
	CellType getCell(int x, int y) const;
	void setCell(int x, int y, CellType cellType);
	bool isWall(int x, int y) const;
	static void ClearScreen();
	void RemoveNPC(int x, int y);
	void PlaceNPC(int x, int y);
	void InitNPCs(Player& player);
	void SetNPCsOnMap(std::vector<NPCs>& npc, int numNPCs, Zone zone);
	void KillingNPCs(Player& player);
	void RepositionNPCInZone(Zone zone);
	void MovementNPCs(Player& player);
	void Draw(const Player& player);

	// Car methods
	void InitCars();
	void SpawnRandomCar();
	Car* GetCarAt(int x, int y);
	void RemoveCar(int x, int y);
	void PlaceCar(int x, int y);
	bool CanPlayerEnterCar(const Player& player);
	void RunOverNPC(int x, int y, Player& player);

	// Big Smoke methods
	void InitBigSmoke();
	void MoveBigSmoke(const Player& player);
	bool AttackPlayer(Player& player);

	// Combat system
	void HandleCombat(Player& player);
	bool IsPlayerAdjacentToNPC(const Player& player, const NPCs& npc);
	bool IsPlayerAdjacentToBigSmoke(const Player& player);

	// Toll system updates
	bool HandleTollCrossing(Player& player);
};