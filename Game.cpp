#include "Game.h"
#include <ctime>

Game::Game() : currentState(GameState::INIT), gameOverReason(GameOverReason::NONE),
player(5, 5), selectedMenuOption(0)
{
    srand(time(NULL));
    stateStartTime = GetTickCount();

    // Initialize key states
    for (int i = 0; i < 256; i++) {
        previousKeyStates[i] = false;
    }
}

void Game::Run()
{
    while (true)
    {
        switch (currentState)
        {
        case GameState::INIT:
            ShowInitScreen();
            break;
        case GameState::MENU:
            ShowMainMenu();
            break;
        case GameState::PLAYING:
            PlayGame();
            break;
        case GameState::GAME_OVER:
            ShowGameOver();
            break;
        }
    }
}

void Game::ShowInitScreen()
{
    Map::ClearScreen();
    std::cout << "\n\n\n";
    std::cout << "  ╔═════════════════════════════════════╗\n";
    std::cout << "  ║                                     ║\n";
    std::cout << "  ║        GTA SAN ANDREAS MINI         ║\n";
    std::cout << "  ║                                     ║\n";
    std::cout << "  ║           Welcome to CJ's           ║\n";
    std::cout << "  ║              Adventure              ║\n";
    std::cout << "  ║                                     ║\n";
    std::cout << "  ╚═════════════════════════════════════╝\n";

    if (HasTimeElapsed(3))
    {
        ChangeState(GameState::MENU);
    }
    Sleep(100);
}

void Game::ShowMainMenu()
{
    Map::ClearScreen();
    std::cout << "\n\n";
    std::cout << "  ╔═════════════════════════════════════╗\n";
    std::cout << "  ║            MAIN MENU                ║\n";
    std::cout << "  ║                                     ║\n";

    // Display menu options with selection indicator
    if (selectedMenuOption == 0) {
        std::cout << "  ║  > Play Game                     <  ║\n";
        std::cout << "  ║    Exit                             ║\n";
    }
    else {
        std::cout << "  ║    Play Game                        ║\n";
        std::cout << "  ║  > Exit                          <  ║\n";
    }

    std::cout << "  ║                                     ║\n";
    std::cout << "  ║  Use UP/DOWN arrow keys to navigate ║\n";
    std::cout << "  ║  SPACE to select option             ║\n";
    std::cout << "  ║                                     ║\n";
    std::cout << "  ║  In Game Controls:                  ║\n";
    std::cout << "  ║  Arrow keys to move                 ║\n";
    std::cout << "  ║  SPACE to attack/interact           ║\n";
    std::cout << "  ║  E to enter/exit car                ║\n";
    std::cout << "  ║                                     ║\n";
    std::cout << "  ╚═════════════════════════════════════╝\n";

    HandleMenuInput();
    Sleep(100);
}

void Game::PlayGame()
{
    HandleInput(player, gameMap);
    gameMap.KillingNPCs(player);
    gameMap.MovementNPCs(player);
    gameMap.MoveBigSmoke(player);
    gameMap.HandleCombat(player);

    // Check game over conditions
    if (player.IsDead())
    {
        ChangeState(GameState::GAME_OVER, GameOverReason::PLAYER_DIED);
        return;
    }

    if (gameMap.bigSmoke && gameMap.bigSmoke->IsDead())
    {
        ChangeState(GameState::GAME_OVER, GameOverReason::BIG_SMOKE_KILLED);
        return;
    }

    Sleep(150);
}

void Game::ShowGameOver()
{
    system("cls");
    std::cout << "\n\n\n";
    std::cout << "  ╔══════════════════════════════════════╗\n";
    std::cout << "  ║             GAME OVER                ║\n";
    std::cout << "  ║                                      ║\n";

    switch (gameOverReason)
    {
    case GameOverReason::PLAYER_DIED:
        std::cout << "  ║     CJ died! Respawning...           ║\n";
        std::cout << "  ║   All progress lost!                 ║\n";
        break;
    case GameOverReason::BIG_SMOKE_KILLED:
        std::cout << "  ║   Congratulations!                   ║\n";
        std::cout << "  ║   Big Smoke has been defeated!       ║\n";
        std::cout << "  ║   You completed the game!            ║\n";
        break;
    case GameOverReason::ARRESTED_AT_TOLL:
        std::cout << "  ║   CJ was arrested at the toll!       ║\n";
        std::cout << "  ║   Game Over!                         ║\n";
        break;
    }

    std::cout << "  ║                                      ║\n";
    std::cout << "  ╚══════════════════════════════════════╝\n";

    if (HasTimeElapsed(5))
    {
        if (gameOverReason == GameOverReason::PLAYER_DIED)
        {
            ResetGame();
            ChangeState(GameState::PLAYING);
        }
        else
        {
            exit(0); // Close game
        }
    }
    Sleep(100);
}

void Game::HandleMenuInput()
{
    // Handle menu navigation
    if (IsKeyJustPressed(VK_UP)) {
        selectedMenuOption--;
        if (selectedMenuOption < 0) {
            selectedMenuOption = 1; // Wrap to last option (Exit)
        }
    }
    else if (IsKeyJustPressed(VK_DOWN)) {
        selectedMenuOption++;
        if (selectedMenuOption > 1) {
            selectedMenuOption = 0; // Wrap to first option (Play)
        }
    }
    else if (IsKeyJustPressed(VK_SPACE)) {
        // Execute selected option
        switch (selectedMenuOption) {
        case 0: // Play Game
            ResetGame();
            ChangeState(GameState::PLAYING);
            break;
        case 1: // Exit
            exit(0);
            break;
        }
    }
}

void Game::ChangeState(GameState newState, GameOverReason reason)
{
    currentState = newState;
    gameOverReason = reason;
    stateStartTime = GetTickCount();
}

void Game::ResetGame()
{
    player = Player(5, 5);
    gameMap = Map();
    gameMap.ReadConfigFile();
    gameMap.InitMap();
    gameMap.InitNPCs(player);
    gameMap.InitCars();
    gameMap.InitBigSmoke();

    // Set player stats from config
    player.SetHealth(gameMap.CJsHealth);
    player.SetPower(gameMap.CJsPower);

    gameMap.Draw(player);
}

bool Game::HasTimeElapsed(int seconds)
{
    DWORD currentTime = GetTickCount();
    DWORD elapsedTime = currentTime - stateStartTime;
    return elapsedTime >= (seconds * 1000); // Convert seconds to milliseconds
}

bool Game::IsKeyJustPressed(int vkCode)
{
    bool currentState = (GetAsyncKeyState(vkCode) & 0x8000) != 0;
    bool justPressed = currentState && !previousKeyStates[vkCode];
    previousKeyStates[vkCode] = currentState;
    return justPressed;
}