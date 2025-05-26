#pragma once
#include "Position2D.h"
#include "Zone.h"

class Car
{
private:
    Position2D pos;
    Zone zone;
    bool occupied;
    bool active;

public:
    Car(int x, int y, Zone spawnZone);

    Position2D GetPos() const;
    void SetPos(int x, int y);
    Zone GetZone() const;
    bool IsOccupied() const;
    bool IsActive() const;
    void SetOccupied(bool occupied);
    void SetActive(bool active);
};