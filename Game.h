#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include "Player.h"
#include "Map.h"

// Game state enumeration
enum class GameState {
    INIT,
    MENU,
    PLAYING,
    GAME_OVER
};

// Game over reason enumeration
enum class GameOverReason {
    NONE,
    PLAYER_DIED,
    BIG_SMOKE_KILLED,
    ARRESTED_AT_TOLL
};

// Menu option enumeration for enhanced menu system
enum class MenuOption {
    PLAY_GAME = 0,
    EXIT = 1,
    MENU_COUNT = 2
};

// Menu entry structure for enhanced menu system
struct MenuEntry {
    std::string text;
    std::string key;
};

class Game {
private:
    // Game state management
    GameState currentState;
    GameOverReason gameOverReason;
    DWORD stateStartTime;

    // Game objects
    Player player;
    Map gameMap;

    // Menu system
    int selectedMenuOption;
    bool previousKeyStates[256]; // To track key states and prevent key repeat

    // Private helper methods
    void ShowInitScreen();
    void ShowMainMenu();
    void PlayGame();
    void ShowGameOver();

    void HandleMenuInput();

    void ChangeState(GameState newState, GameOverReason reason = GameOverReason::NONE);
    void ResetGame();

    bool HasTimeElapsed(int seconds);
    bool IsKeyJustPressed(int vkCode);

public:
    // Constructor
    Game();

    // Main game loop
    void Run();
};