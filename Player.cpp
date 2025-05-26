#include <cstdlib>
#include "Windows.h"
#include "Player.h"
#include "Position2D.h"
#include "Map.h"

Player::Player(int startX, int startY, Zone zone) : inCar(false), carIndex(-1)
{
    pos.x = startX;
    pos.y = startY;
    currentMoney = 0;
    health = 100;
    power = 25;
    direction = Direction::DOWN;
    currentZone = zone;
}

void Player::SetPos(int x, int y)
{
    pos.x = x;
    pos.y = y;
}

void Player::SetDirection(Direction dir)
{
    direction = dir;
}

Direction Player::GetDirection() const
{
    return direction;
}

Position2D Player::GetPos() const
{
    return pos;
}

Zone Player::GetCurrentZone() const
{
    return currentZone;
}

void Player::SetCurrentZone(Zone zone)
{
    currentZone = zone;
}

void Player::AddMoney(int amount)
{
    currentMoney += amount;
}

void Player::SubstractMoney(int amount)
{
    currentMoney -= amount;
    if (currentMoney < 0) currentMoney = 0;
}

int Player::GetMoney() const
{
    return currentMoney;
}

int Player::GetHealth() const
{
    return health;
}

int Player::GetPower() const
{
    return power;
}

void Player::SetHealth(int newHealth)
{
    health = newHealth;
}

void Player::SetPower(int newPower)
{
    power = newPower;
}

void Player::TakeDamage(int damage)
{
    health -= damage;
    if (health < 0) health = 0;
}

bool Player::IsDead() const
{
    return health <= 0;
}

void Player::ResetToStart()
{
    pos.x = 5;
    pos.y = 5;
    currentMoney = 0;
    health = 100;
    power = 25;
    currentZone = Zone::LOS_SANTOS;
    inCar = false;
    carIndex = -1;
}

bool Player::IsInCar() const
{
    return inCar;
}

void Player::EnterCar(int carIdx)
{
    inCar = true;
    carIndex = carIdx;
}

void Player::ExitCar()
{
    inCar = false;
    carIndex = -1;
}

int Player::GetCarIndex() const
{
    return carIndex;
}

void HandleInput(Player& player, Map& map)
{
    int mapWidth = map.GetTotalWidth();
    int mapHeight = map.GetHeight();
    Position2D newPos = player.GetPos();
    Direction newDir = player.GetDirection();
    bool keyPressed = false;

    // Handle car entry/exit
    if (GetAsyncKeyState('E') & 0x8000)
    {
        static bool enterKeyPressed = false;
        if (!enterKeyPressed)
        {
            enterKeyPressed = true;

            if (player.IsInCar())
            {
                // Exit car
                Car* currentCar = nullptr;
                for (Car& car : map.cars)
                {
                    if (car.GetPos().x == player.GetPos().x && car.GetPos().y == player.GetPos().y)
                    {
                        currentCar = &car;
                        break;
                    }
                }

                if (currentCar)
                {
                    currentCar->SetOccupied(false);
                    currentCar->SetActive(false); // Car becomes inactive after use
                    map.setCell(player.GetPos().x, player.GetPos().y, Map::CellType::EMPTY);
                }

                player.ExitCar();
            }
            else if (map.CanPlayerEnterCar(player))
            {
                // Enter car
                Position2D playerPos = player.GetPos();

                for (int dx = -1; dx <= 1; dx++)
                {
                    for (int dy = -1; dy <= 1; dy++)
                    {
                        int x = playerPos.x + dx;
                        int y = playerPos.y + dy;

                        if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight)
                        {
                            if (map.getCell(x, y) == Map::CellType::CAR)
                            {
                                Car* car = map.GetCarAt(x, y);
                                if (car && !car->IsOccupied())
                                {
                                    car->SetOccupied(true);
                                    map.RemoveCar(x, y);
                                    player.SetPos(x, y);
                                    player.EnterCar(0);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        static bool enterKeyPressed = false;
        enterKeyPressed = false;
    }

    // Movement input
    if (GetAsyncKeyState(VK_UP) & 0x8000)
    {
        newDir = Direction::UP;
        newPos.y--;
        keyPressed = true;
    }
    else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        newDir = Direction::DOWN;
        newPos.y++;
        keyPressed = true;
    }
    else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        newDir = Direction::LEFT;
        newPos.x--;
        keyPressed = true;
    }
    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        newDir = Direction::RIGHT;
        newPos.x++;
        keyPressed = true;
    }

    if (keyPressed)
    {
        player.SetDirection(newDir);

        if (newPos.x > 0 && newPos.x < mapWidth &&
            newPos.y > 0 && newPos.y < mapHeight &&
            !map.isWall(newPos.x, newPos.y))
        {
            bool canMove = true;

            // Check for Big Smoke collision (can't run over with car)
            if (map.getCell(newPos.x, newPos.y) == Map::CellType::BIG_SMOKE && player.IsInCar())
            {
                canMove = false; // Can't run over Big Smoke
            }
            // Check for NPC collision when in car
            else if (map.getCell(newPos.x, newPos.y) == Map::CellType::NPC && player.IsInCar())
            {
                map.RunOverNPC(newPos.x, newPos.y, player);
                canMove = true;
            }
            // Check for car collision when not in car
            else if (map.getCell(newPos.x, newPos.y) == Map::CellType::CAR && !player.IsInCar())
            {
                canMove = false; // Can't walk through cars
            }

            if (canMove)
            {
                player.SetPos(newPos.x, newPos.y);

                // Update zone based on position
                int totalWidth = map.GetTotalWidth();
                if (newPos.x < totalWidth / 3)
                {
                    player.SetCurrentZone(Zone::LOS_SANTOS);
                }
                else if (newPos.x < 2 * totalWidth / 3)
                {
                    player.SetCurrentZone(Zone::SAN_FIERRO);
                }
                else
                {
                    player.SetCurrentZone(Zone::LAS_VENTURAS);
                }

                // Handle money pickup (only if not in car)
                if (map.getCell(newPos.x, newPos.y) == Map::CellType::MONEY && !player.IsInCar())
                {
                    Zone zone = player.GetCurrentZone();
                    int maxAmount = 0;

                    switch (zone)
                    {
                    case Zone::LOS_SANTOS:
                        maxAmount = map.moneyBeatingNPCs_LosSantos;
                        break;
                    case Zone::SAN_FIERRO:
                        maxAmount = map.moneyBeatingNPCs_SanFierro;
                        break;
                    case Zone::LAS_VENTURAS:
                        maxAmount = map.moneyBeatingNPCs_LasVenturas;
                        break;
                    }

                    if (maxAmount > 0)
                    {
                        int randomAmount = 1 + rand() % maxAmount;
                        player.AddMoney(randomAmount);
                    }
                    else
                    {
                        player.AddMoney(1);
                    }

                    map.setCell(newPos.x, newPos.y, Map::CellType::EMPTY);
                }

                // Handle toll crossing
                if (map.getCell(newPos.x, newPos.y) == Map::CellType::TOLL)
                {
                    if (!map.HandleTollCrossing(player))
                    {
                        // Player arrested - trigger game over
                        // This should be handled by the Game class
                    }
                }
            }
        }

        map.Draw(player);
    }
}