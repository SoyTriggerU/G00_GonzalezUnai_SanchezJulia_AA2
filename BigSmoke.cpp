#include "BigSmoke.h"
#include "Map.h"
#include <cmath>

BigSmoke::BigSmoke(int x, int y) : isDead(false), isAggressive(true), health(200), power(50)
{
    pos.x = x;
    pos.y = y;
}

Position2D BigSmoke::GetPos() const
{
    return pos;
}

void BigSmoke::SetPos(int x, int y)
{
    pos.x = x;
    pos.y = y;
}

int BigSmoke::GetHealth() const
{
    return health;
}

int BigSmoke::GetPower() const
{
    return power;
}

bool BigSmoke::IsDead() const
{
    return isDead;
}

bool BigSmoke::IsAggressive() const
{
    return isAggressive;
}

void BigSmoke::TakeDamage(int damage)
{
    health -= damage;
    if (health <= 0)
    {
        health = 0;
        Die();
    }
}

void BigSmoke::Die()
{
    isDead = true;
}

void BigSmoke::MoveTowardsPlayer(const Position2D& playerPos, Map& map)
{
    if (isDead) return;

    // Calculate distance to player
    int distanceX = playerPos.x - pos.x;
    int distanceY = playerPos.y - pos.y;

    // If player is close, be more aggressive
    if (abs(distanceX) + abs(distanceY) <= 3)
    {
        int newX = pos.x;
        int newY = pos.y;

        // Move towards player
        if (abs(distanceX) > abs(distanceY))
        {
            newX += (distanceX > 0) ? 1 : -1;
        }
        else
        {
            newY += (distanceY > 0) ? 1 : -1;
        }

        // Check if movement is valid
        if (!map.isWall(newX, newY) && map.getCell(newX, newY) != Map::CellType::NPC)
        {
            map.setCell(pos.x, pos.y, Map::CellType::EMPTY);
            pos.x = newX;
            pos.y = newY;
            map.setCell(pos.x, pos.y, Map::CellType::BIG_SMOKE);
        }
    }
    else
    {
        // Random movement when player is far
        int dir = rand() % 4;
        int newX = pos.x;
        int newY = pos.y;

        switch (dir)
        {
        case 0: newX--; break;
        case 1: newX++; break;
        case 2: newY--; break;
        case 3: newY++; break;
        }

        if (!map.isWall(newX, newY) && map.getCell(newX, newY) == Map::CellType::EMPTY)
        {
            map.setCell(pos.x, pos.y, Map::CellType::EMPTY);
            pos.x = newX;
            pos.y = newY;
            map.setCell(pos.x, pos.y, Map::CellType::BIG_SMOKE);
        }
    }
}