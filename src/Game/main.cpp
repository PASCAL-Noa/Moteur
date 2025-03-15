#include "pch.h"
#include "main.h"
#include "GameManager.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
    GameManager gameManager;
    gameManager.GameLoop(hInstance);

    return 0;
}
