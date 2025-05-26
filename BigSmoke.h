#pragma once
#include "Position2D.h"
#include "Zone.h"

class BigSmoke
{
private:
    Position2D pos;
    int health;
    int power;
    bool isDead;
    bool isAggressive;

public:
    BigSmoke(int x, int y);

    Position2D GetPos() const;
    void SetPos(int x, int y);
    int GetHealth() const;
    int GetPower() const;
    bool IsDead() const;
    bool IsAggressive() const;
    void TakeDamage(int damage);
    void Die();
    void MoveTowardsPlayer(const Position2D& playerPos, class Map& map);
};