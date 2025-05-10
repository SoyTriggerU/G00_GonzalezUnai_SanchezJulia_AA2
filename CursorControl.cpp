#include "CursorControl.h"
#include <Windows.h>

// Función para ocultar el cursor
void HideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Oculta el cursor
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// Función para mostrar el cursor
void ShowCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = TRUE;  // Muestra el cursor
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}