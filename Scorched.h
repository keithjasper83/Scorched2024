#pragma once
#include "Renderer.h"

class Game
{
public:
    Game(int screenWidth, int screenHeight);

    void run();

private:
    Renderer renderer; // Your Renderer class
    int screenWidth;
    int screenHeight;

    // Add other game-related members here

    void update();

    void render();
};