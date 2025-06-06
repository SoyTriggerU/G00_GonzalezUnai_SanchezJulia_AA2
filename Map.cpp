#include "Map.h"
#include "Player.h"

Map::Map() : bigSmoke(nullptr) {}

Map::~Map()
{
    if (map != nullptr)
    {
        for (int i = 0; i < height; i++)
            delete[] map[i];
        delete[] map;
    }

    if (bigSmoke != nullptr)
    {
        delete bigSmoke;
    }
}

void Map::ReadConfigFile()
{
    std::ifstream file("config.txt", std::ios::in);
    if (!file.is_open())
    {
        std::cout << "Error opening file.\n";
        return;
    }

    const int bufferSize = 256;
    char buffer[bufferSize];
    char value[32];
    int valueIndex = 0;
    int valueCount = 0;

    while (file.getline(buffer, bufferSize))
    {
        int i = 0;
        while (buffer[i] != '\0')
        {
            char ch = buffer[i];

            if (ch == ';' || ch == '\n' || ch == '\0')
            {
                value[valueIndex] = '\0';
                int num = std::atoi(value);

                switch (valueCount)
                {
                case 0: height = num; break;
                case 1: width = num; break;
                case 2: CJsHealth = num; break;
                case 3: CJsPower = num; break;
                case 4: tax_LosSantos_SanFierro = num; break;
                case 5: tax_SanFierro_LasVenturas = num; break;
                case 6: numNPCs_LosSantos = num; break;
                case 7: moneyBeatingNPCs_LosSantos = num; break;
                case 8: healthNPCs_LosSantos = num; break;
                case 9: powerNPCs_LosSantos = num; break;
                case 10: numNPCs_SanFierro = num; break;
                case 11: moneyBeatingNPCs_SanFierro = num; break;
                case 12: healthNPCs_SanFierro = num; break;
                case 13: powerNPCs_SanFierro = num; break;
                case 14: numNPCs_LasVenturas = num; break;
                case 15: moneyBeatingNPCs_LasVenturas = num; break;
                case 16: healthNPCs_LasVenturas = num; break;
                case 17: powerNPCs_LasVenturas = num; break;
                }

                valueCount++;
                valueIndex = 0;
            }
            else
            {
                if (valueIndex < 31)
                {
                    value[valueIndex++] = ch;
                }
            }
            i++;
        }
    }

    file.close();
}

int Map::GetTotalWidth() const
{
    return 3 * width;
}

int Map::GetHeight() const
{
    return height;
}

void Map::InitMap()
{
    int totalWidth = GetTotalWidth();
    if (height == 0 && totalWidth == 0)
        return;

    map = new CellType * [height];
    for (int i = 0; i < height; i++)
    {
        map[i] = new CellType[totalWidth];
    }

    if (height > 0)
    {
        randomY = rand() % height;
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < totalWidth; x++)
        {
            if (x == 0 || x == totalWidth - 1 || y == 0 || y == height - 1)
            {
                map[y][x] = CellType::WALL;
            }
            else if (x == totalWidth / 3 || x == 2 * totalWidth / 3)
            {
                if (y == randomY)
                    map[y][x] = CellType::TOLL;
                else
                    map[y][x] = CellType::WALL;
            }
            else
                map[y][x] = CellType::EMPTY;
        }
    }
}

Map::CellType Map::getCell(int x, int y) const
{
    return map[y][x];
}

void Map::setCell(int x, int y, CellType cellType)
{
    map[y][x] = cellType;
}

bool Map::isWall(int x, int y) const
{
    if (x < 0 || x >= GetTotalWidth() || y < 0 || y >= height)
        return true;
    return map[y][x] == CellType::WALL;
}

Map& Map::operator=(Map&& other) noexcept
{
    if (this != &other)
    {
        // Clean up current resources
        if (map != nullptr)
        {
            for (int i = 0; i < height; i++)
                delete[] map[i];
            delete[] map;
        }

        if (bigSmoke != nullptr)
        {
            delete bigSmoke;
        }

        // Move primitive data members
        width = other.width;
        height = other.height;
        CJsHealth = other.CJsHealth;
        CJsPower = other.CJsPower;
        tax_LosSantos_SanFierro = other.tax_LosSantos_SanFierro;
        tax_SanFierro_LasVenturas = other.tax_SanFierro_LasVenturas;
        numNPCs_LosSantos = other.numNPCs_LosSantos;
        moneyBeatingNPCs_LosSantos = other.moneyBeatingNPCs_LosSantos;
        healthNPCs_LosSantos = other.healthNPCs_LosSantos;
        powerNPCs_LosSantos = other.powerNPCs_LosSantos;
        numNPCs_SanFierro = other.numNPCs_SanFierro;
        moneyBeatingNPCs_SanFierro = other.moneyBeatingNPCs_SanFierro;
        healthNPCs_SanFierro = other.healthNPCs_SanFierro;
        powerNPCs_SanFierro = other.powerNPCs_SanFierro;
        numNPCs_LasVenturas = other.numNPCs_LasVenturas;
        moneyBeatingNPCs_LasVenturas = other.moneyBeatingNPCs_LasVenturas;
        healthNPCs_LasVenturas = other.healthNPCs_LasVenturas;
        powerNPCs_LasVenturas = other.powerNPCs_LasVenturas;
        randomY = other.randomY;

        // Move the 2D array
        map = other.map;
        other.map = nullptr;

        // Move vectors (they have move semantics built-in)
        npcs = std::move(other.npcs);
        cars = std::move(other.cars);

        // Move the BigSmoke pointer
        bigSmoke = other.bigSmoke;
        other.bigSmoke = nullptr;

        // Reset other's primitive members to safe values
        other.width = 0;
        other.height = 0;
        other.randomY = 0;
        // Reset other config values if needed
        other.CJsHealth = 0;
        other.CJsPower = 0;
        other.tax_LosSantos_SanFierro = 0;
        other.tax_SanFierro_LasVenturas = 0;
        other.numNPCs_LosSantos = 0;
        other.moneyBeatingNPCs_LosSantos = 0;
        other.healthNPCs_LosSantos = 0;
        other.powerNPCs_LosSantos = 0;
        other.numNPCs_SanFierro = 0;
        other.moneyBeatingNPCs_SanFierro = 0;
        other.healthNPCs_SanFierro = 0;
        other.powerNPCs_SanFierro = 0;
        other.numNPCs_LasVenturas = 0;
        other.moneyBeatingNPCs_LasVenturas = 0;
        other.healthNPCs_LasVenturas = 0;
        other.powerNPCs_LasVenturas = 0;
    }
    return *this;
}

void Map::ClearScreen()
{
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Map::RemoveNPC(int x, int y)
{
    map[y][x] = CellType::EMPTY;
}

void Map::PlaceNPC(int x, int y)
{
    map[y][x] = CellType::NPC;
}

void Map::InitNPCs(Player& player)
{
    SetNPCsOnMap(npcs, numNPCs_LosSantos, Zone::LOS_SANTOS);
    SetNPCsOnMap(npcs, numNPCs_SanFierro, Zone::SAN_FIERRO);
    SetNPCsOnMap(npcs, numNPCs_LasVenturas, Zone::LAS_VENTURAS);
}

void Map::SetNPCsOnMap(std::vector<NPCs>& npc, int numNPCs, Zone zone)
{
    int totalWidth = GetTotalWidth();
    int startX, endX;

    if (zone == Zone::LOS_SANTOS)
    {
        startX = 1;
        endX = totalWidth / 3;
    }
    else if (zone == Zone::SAN_FIERRO)
    {
        startX = totalWidth / 3 + 1;
        endX = (2 * totalWidth) / 3;
    }
    else // LAS_VENTURAS
    {
        startX = (2 * totalWidth) / 3 + 1;
        endX = totalWidth - 1;
    }

    int counter = 0;
    while (counter < numNPCs)
    {
        int posX = startX + rand() % (endX - startX);
        int posY = 1 + rand() % (height - 2);

        if (map[posY][posX] == CellType::EMPTY)
        {
            NPCs newNPC(posX, posY, zone);
            npc.push_back(newNPC);
            map[posY][posX] = CellType::NPC;
            counter++;
        }
    }
}

void Map::KillingNPCs(Player& player)
{
        for (int i = 0; i < npcs.size(); i++)
        {
            if (IsPlayerAdjacentToNPC(player, npcs[i]))
            {
                if (GetAsyncKeyState(VK_SPACE) & 0x8000)
                {
                    int npcX = npcs[i].GetPos().x;
                    int npcY = npcs[i].GetPos().y;
                    Zone zone = npcs[i].GetZone();

                    // Player attacks NPC
                    npcs[i].TakeDamage(player.GetPower());

                    if (npcs[i].GetIsDead())
                    {
                        map[npcY][npcX] = CellType::MONEY;
                        npcs.erase(npcs.begin() + i);
                        RepositionNPCInZone(zone);
                    }
                    else if (npcs[i].IsAggressive())
                    {
                        // NPC fights back
                        npcs[i].AttackPlayer(player);
                    }
                    break;
                }
            }
        }
}

void Map::RepositionNPCInZone(Zone zone)
{
    int totalWidth = GetTotalWidth();
    int startX, endX;

    if (zone == Zone::LOS_SANTOS)
    {
        startX = 1;
        endX = totalWidth / 3;
    }
    else if (zone == Zone::SAN_FIERRO)
    {
        startX = totalWidth / 3 + 1;
        endX = 2 * totalWidth / 3;
    }
    else // LAS_VENTURAS
    {
        startX = 2 * totalWidth / 3 + 1;
        endX = totalWidth - 1;
    }

    for (int attempts = 0; attempts < 100; ++attempts)
    {
        int x = startX + rand() % (endX - startX);
        int y = 1 + rand() % (height - 2);

        if (map[y][x] == CellType::EMPTY)
        {
            NPCs newNPC(x, y, zone);
            npcs.push_back(newNPC);
            map[y][x] = CellType::NPC;
            break;
        }
    }
}

void Map::MovementNPCs(Player& player)
{
    int totalWidth = GetTotalWidth();

    for (NPCs& npc : npcs)
    {
        if (!npc.GetIsDead())
        {
            int npcX = npc.GetPos().x;
            int npcY = npc.GetPos().y;
            int playerX = player.GetPos().x;
            int playerY = player.GetPos().y;

            if (npc.IsAggressive() && (abs(playerX - npcX) <= 3 && abs(playerY - npcY) <= 3))
            {
                // Aggressive NPCs move towards player
                npc.MoveTowardsPlayer(player.GetPos(), *this);
            }
            else if (abs(playerX - npcX) > 1 || abs(playerY - npcY) > 1)
            {
                // Random movement for neutral NPCs or distant aggressive ones
                int newX = npcX;
                int newY = npcY;

                int dir = rand() % 4;
                switch (dir)
                {
                case 0: newX--; break;
                case 1: newX++; break;
                case 2: newY--; break;
                case 3: newY++; break;
                }

                if ((newX >= 0 && newX < totalWidth && newY >= 0 && newY < height) &&
                    map[newY][newX] == CellType::EMPTY)
                {
                    RemoveNPC(npcX, npcY);
                    PlaceNPC(newX, newY);
                    npc.SetPos(newX, newY);
                }
            }
        }
    }
}

// Car methods
void Map::InitCars()
{
    int totalWidth = GetTotalWidth();
    int numCars = 5; // Spawn 5 random cars (HAS TO BE READ BY COFIG FILE BUT THERE'S NO SPECIFICATION ON WHERE)

    for (int i = 0; i < numCars; i++)
    {
        SpawnRandomCar();
    }
}

// THIS HAS TO BE ADAPTED BUT FOR NOW, TO TRY IT, WORKS OK
void Map::SpawnRandomCar()
{
    int totalWidth = GetTotalWidth();
    Zone randomZone = static_cast<Zone>(rand() % 3);

    int startX, endX;
    if (randomZone == Zone::LOS_SANTOS)
    {
        startX = 1;
        endX = totalWidth / 3;
    }
    else if (randomZone == Zone::SAN_FIERRO)
    {
        startX = totalWidth / 3 + 1;
        endX = 2 * totalWidth / 3;
    }
    else // LAS_VENTURAS
    {
        startX = 2 * totalWidth / 3 + 1;
        endX = totalWidth - 1;
    }

    for (int attempts = 0; attempts < 50; attempts++)
    {
        int x = startX + rand() % (endX - startX);
        int y = 1 + rand() % (height - 2);

        if (map[y][x] == CellType::EMPTY)
        {
            Car newCar(x, y, randomZone);
            cars.push_back(newCar);
            map[y][x] = CellType::CAR;
            break;
        }
    }
}

Car* Map::GetCarAt(int x, int y)
{
    for (Car& car : cars)
    {
        if (car.GetPos().x == x && car.GetPos().y == y && car.IsActive())
        {
            return &car;
        }
    }
    return nullptr;
}

void Map::RemoveCar(int x, int y)
{
    map[y][x] = CellType::EMPTY;
}

void Map::PlaceCar(int x, int y)
{
    map[y][x] = CellType::CAR;
}

bool Map::CanPlayerEnterCar(const Player& player)
{
    Position2D playerPos = player.GetPos();

    // Check adjacent cells for cars
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            int x = playerPos.x + dx;
            int y = playerPos.y + dy;

            if (x >= 0 && x < GetTotalWidth() && y >= 0 && y < height)
            {
                if (map[y][x] == CellType::CAR)
                {
                    Car* car = GetCarAt(x, y);
                    if (car && !car->IsOccupied())
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void Map::RunOverNPC(int x, int y, Player& player)
{
        for (int i = 0; i < npcs.size(); i++)
        {
            if (npcs[i].GetPos().x == x && npcs[i].GetPos().y == y)
            {
                // Remove NPC (run over)
                Zone zone = npcs[i].GetZone();
                npcs.erase(npcs.begin() + i);
                map[y][x] = CellType::MONEY;

                // Spawn new NPC in same zone
                RepositionNPCInZone(zone);
                break;
            }
        }
}

// Big Smoke methods
void Map::InitBigSmoke()
{
    int totalWidth = GetTotalWidth();
    int startX = 2 * totalWidth / 3 + 1; // Las Venturas
    int endX = totalWidth - 1;

    for (int attempts = 0; attempts < 50; attempts++)
    {
        int x = startX + rand() % (endX - startX);
        int y = 1 + rand() % (height - 2);

        if (map[y][x] == CellType::EMPTY)
        {
            bigSmoke = new BigSmoke(x, y);
            map[y][x] = CellType::BIG_SMOKE;
            break;
        }
    }
}

void Map::MoveBigSmoke(const Player& player)
{
    if (bigSmoke && !bigSmoke->IsDead())
    {
        bigSmoke->MoveTowardsPlayer(player.GetPos(), *this);
    }
}

bool Map::AttackPlayer(Player& player)
{
    if (bigSmoke && !bigSmoke->IsDead() && IsPlayerAdjacentToBigSmoke(player))
    {
        player.TakeDamage(bigSmoke->GetPower());
        return true;
    }
    return false;
}

// Combat system
void Map::HandleCombat(Player& player)
{
        if (GetAsyncKeyState(VK_SPACE) & 0x8000)
        {
            // Check if attacking Big Smoke
            if (bigSmoke && !bigSmoke->IsDead() && IsPlayerAdjacentToBigSmoke(player))
            {
                if (!player.IsInCar()) // Can't attack from car
                {
                    bigSmoke->TakeDamage(player.GetPower());
                    if (!bigSmoke->IsDead())
                    {
                        // Big Smoke attacks back
                        player.TakeDamage(bigSmoke->GetPower());
                    }
                }
            }
        }

        // Big Smoke attacks player if adjacent
        if (bigSmoke && !bigSmoke->IsDead() && IsPlayerAdjacentToBigSmoke(player))
        {
            static int attackCooldown = 0;
            if (attackCooldown <= 0)
            {
                player.TakeDamage(bigSmoke->GetPower());
                attackCooldown = 10; // Attack every 10 frames
            }
            else
            {
                attackCooldown--;
            }
        }
}

bool Map::IsPlayerAdjacentToNPC(const Player& player, const NPCs& npc)
{
    return abs(player.GetPos().x - npc.GetPos().x) <= 1 &&
        abs(player.GetPos().y - npc.GetPos().y) <= 1;
}

bool Map::IsPlayerAdjacentToBigSmoke(const Player& player)
{
    if (!bigSmoke || bigSmoke->IsDead()) return false;

    return abs(player.GetPos().x - bigSmoke->GetPos().x) <= 1 &&
        abs(player.GetPos().y - bigSmoke->GetPos().y) <= 1;
}

bool Map::HandleTollCrossing(Player& player)
{
    Position2D pos = player.GetPos();
    Zone currentZone = player.GetCurrentZone();
    int tollFee = 0;
    Zone nextZone = currentZone;

    if (map[pos.y][pos.x] == CellType::TOLL && currentZone == Zone::LOS_SANTOS)
    {
        tollFee = tax_LosSantos_SanFierro;
        nextZone = Zone::SAN_FIERRO;
        if (player.GetMoney() >= tollFee)
        {
            player.SubstractMoney(tollFee);
            player.SetCurrentZone(nextZone);
            setCell(pos.x, pos.y, CellType::EMPTY);
            //map[pos.y][pos.x] = CellType::EMPTY;
            return true;
        }
        else return false;
    }
    else if (map[pos.y][pos.x] == CellType::TOLL && currentZone == Zone::SAN_FIERRO)
    {
        tollFee = tax_SanFierro_LasVenturas;
        nextZone = Zone::LAS_VENTURAS;

        if (player.GetMoney() >= tollFee)
        {
            player.SubstractMoney(tollFee);
            player.SetCurrentZone(nextZone);
            setCell(pos.x, pos.y, CellType::EMPTY);
            //map[pos.y][pos.x] = CellType::EMPTY;
            return true;
        }
        else return false;
    }
    return true;
}
void Map::Draw(const Player& player)
{
    system("cls");
    HideCursor();
    ClearScreen();

    int totalWidth = GetTotalWidth();
    int playerPosX = player.GetPos().x;
    int playerPosY = player.GetPos().y;

    int cameraLeft = playerPosX - (cameraWidth / 2);
    int cameraTop = playerPosY - (cameraHeight / 2);
    if (cameraLeft <= 0) cameraLeft = 0;
    if (cameraTop <= 0) cameraTop = 0;

    int cameraRight = cameraLeft + (cameraWidth - 1);
    int cameraBottom = cameraTop + (cameraHeight - 1);
    if (cameraRight >= totalWidth) cameraRight = totalWidth - 1;
    if (cameraBottom >= height) cameraBottom = height - 1;

    if (cameraBottom >= height) {
        cameraBottom = height - 1;
        cameraTop = (std::max)(0, cameraBottom - cameraHeight + 1);
    }

    for (int y = cameraTop; y <= cameraBottom; y++)
    {
        for (int x = cameraLeft; x <= cameraRight; x++)
        {
            if (player.GetPos().x == x && player.GetPos().y == y)
            {
                if (player.IsInCar())
                {
                    std::cout << '@'; // Player in car
                }
                else
                {
                    switch (player.GetDirection())
                    {
                    case Direction::UP: std::cout << '^'; break;
                    case Direction::DOWN: std::cout << 'v'; break;
                    case Direction::LEFT: std::cout << '<'; break;
                    case Direction::RIGHT: std::cout << '>'; break;
                    }
                }
                continue;
            }

            switch (map[y][x])
            {
            case CellType::WALL: std::cout << 'X'; break;
            case CellType::EMPTY: std::cout << ' '; break;
            case CellType::MONEY: std::cout << '$'; break;
            case CellType::NPC: std::cout << "P"; break;
            case CellType::TOLL: std::cout << "T"; break;
            case CellType::CAR: std::cout << "C"; break;
            case CellType::BIG_SMOKE: std::cout << "B"; break;
            default: std::cout << ' '; break;
            }
        }
        std::cout << std::endl;
    }

    // Status display
    std::cout << "\nCJ's Stats:\n";
    std::cout << "Money: $" << player.GetMoney() << " | ";
    std::cout << "Health: " << player.GetHealth() << " | ";
    std::cout << "Power: " << player.GetPower() << "\n";
    std::cout << "Zone: ";

    switch (player.GetCurrentZone())
    {
    case Zone::LOS_SANTOS: std::cout << "Los Santos"; break;
    case Zone::SAN_FIERRO: std::cout << "San Fierro"; break;
    case Zone::LAS_VENTURAS: std::cout << "Las Venturas"; break;
    }

    if (player.IsInCar())
    {
        std::cout << " | IN CAR";
    }
    else if (!player.IsInCar())
    {
        std::cout << " | NOT IN CAR";
    }

    std::cout << "\n\nControls: Arrow keys to move, SPACE to attack, E to enter/exit car\n";

    ShowCursor();
}