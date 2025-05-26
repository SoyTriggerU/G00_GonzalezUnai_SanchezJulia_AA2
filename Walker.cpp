#include "Walker.h"
#include "Player.h"
#include "Map.h"
#include <cmath>

NPCs::NPCs(int spawnX, int spawnY, Zone spawnZone) : isDead(false), isMoving(false), zone(spawnZone)
{
    pos.x = spawnX;
    pos.y = spawnY;

    // Set stats based on zone
    switch (spawnZone)
    {
    case Zone::LOS_SANTOS:
        health = 50;  // These should come from map config
        power = 15;
        break;
    case Zone::SAN_FIERRO:
        health = 75;
        power = 20;
        break;
    case Zone::LAS_VENTURAS:
        health = 100;
        power = 25;
        break;
    }

    // Random behavior (70% neutral, 30% aggressive)
    behaviour = (rand() % 100 < 70) ? Behaviour::Neutral : Behaviour::Agresivo;
}

void NPCs::SetPos(int x, int y)
{
    pos.x = x;
    pos.y = y;
}

Position2D NPCs::GetPos() const
{
    return pos;
}

Zone NPCs::GetZone() const
{
    return zone;
}

bool NPCs::GetIsDead() const
{
    return isDead;
}

void NPCs::Die()
{
    isDead = true;
}

Behaviour NPCs::GetBehaviour() const
{
    return behaviour;
}

int NPCs::GetHealth() const
{
    return health;
}

int NPCs::GetPower() const
{
    return power;
}

void NPCs::TakeDamage(int damage)
{
    health -= damage;
    if (health <= 0)
    {
        health = 0;
        Die();
    }
}

bool NPCs::IsAggressive() const
{
    return behaviour == Behaviour::Agresivo;
}

void NPCs::AttackPlayer(Player& player)
{
    if (!isDead && IsAggressive())
    {
        player.TakeDamage(power);
    }
}

void NPCs::MoveTowardsPlayer(const Position2D& playerPos, Map& map)
{
    if (isDead) return;

    int dx = playerPos.x - pos.x;
    int dy = playerPos.y - pos.y;

    int newX = pos.x;
    int newY = pos.y;

    // Move towards player
    if (abs(dx) > abs(dy))
    {
        newX += (dx > 0) ? 1 : -1;
    }
    else
    {
        newY += (dy > 0) ? 1 : -1;
    }

    // Check if movement is valid
    if (!map.isWall(newX, newY) && map.getCell(newX, newY) == Map::CellType::EMPTY)
    {
        map.RemoveNPC(pos.x, pos.y);
        map.PlaceNPC(newX, newY);
        pos.x = newX;
        pos.y = newY;
    }
}