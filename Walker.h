#pragma once
#include "Position2D.h"
#include "Zone.h"

enum class Behaviour
{
	Agresivo,
	Neutral
};

class NPCs
{
private:
	Position2D pos;
	bool isDead;
	bool isMoving;
	Zone zone;
	Behaviour behaviour;
	int health;
	int power;

public:
	NPCs(int spawnX, int spawnY, Zone spawnZone);

	void SetPos(int x, int y);
	Position2D GetPos() const;
	Zone GetZone() const;
	bool GetIsDead() const;
	void Die();
	Behaviour GetBehaviour() const;

	// Combat system
	int GetHealth() const;
	int GetPower() const;
	void TakeDamage(int damage);
	bool IsAggressive() const;
	void AttackPlayer(class Player& player);
	void MoveTowardsPlayer(const Position2D& playerPos, class Map& map);
};