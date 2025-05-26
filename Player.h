#pragma once
#include "Position2D.h"
#include "Zone.h"
#include "Walker.h"
#include <vector>
class Map;

enum class Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Player
{
private:
	Position2D pos;
	int currentMoney;
	int health;
	int power;
	Direction direction;
	Zone currentZone;
	bool inCar;
	int carIndex; // Index of the car the player is in

public:
	Player(int startX, int startY, Zone zone = Zone::LOS_SANTOS);

	void SetPos(int x, int y);
	void SetDirection(Direction dir);
	Direction GetDirection() const;
	Position2D GetPos() const;
	Zone GetCurrentZone() const;
	void SetCurrentZone(Zone zone);
	void AddMoney(int amount);
	void SubstractMoney(int amount);
	int GetMoney() const;

	// Health and combat
	int GetHealth() const;
	int GetPower() const;
	void SetHealth(int newHealth);
	void SetPower(int newPower);
	void TakeDamage(int damage);
	bool IsDead() const;
	void ResetToStart(); // Reset player to Los Santos with nothing

	// Car system
	bool IsInCar() const;
	void EnterCar(int carIdx);
	void ExitCar();
	int GetCarIndex() const;
};

void HandleInput(Player& player, Map& map);