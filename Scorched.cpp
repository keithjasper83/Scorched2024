
#include "Scorched.h"


Game::Game(int screenWidth, int screenHeight): renderer(screenWidth, screenHeight)
{
    // Initialize your game objects, terrain data, etc.
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}

void Game::run()
{
    srand(static_cast<unsigned>(time(0)));
    renderer.start();
}

void Game::update()
{
    // Update game logic here
}

void Game::render()
{
    // renderer.renderTerrain(); // Call the rendering function from your Renderer
    //  Render other game objects
}

int main()
{
    Game game(constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT);
    game.run();

    return 0;
}
