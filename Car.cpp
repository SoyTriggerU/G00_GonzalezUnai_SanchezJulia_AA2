#include "Car.h"

Car::Car(int x, int y, Zone spawnZone) : occupied(false), active(true), zone(spawnZone)
{
    pos.x = x;
    pos.y = y;
}

Position2D Car::GetPos() const
{
    return pos;
}

void Car::SetPos(int x, int y)
{
    pos.x = x;
    pos.y = y;
}

Zone Car::GetZone() const
{
    return zone;
}

bool Car::IsOccupied() const
{
    return occupied;
}

bool Car::IsActive() const
{
    return active;
}

void Car::SetOccupied(bool occupied)
{
    this->occupied = occupied;
}

void Car::SetActive(bool active)
{
    this->active = active;
}