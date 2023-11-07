#include "Scorched.h"

game::game(const int screen_width, const int screen_height) : renderer_(screen_width, screen_height)
{
    // Initialize your game objects, terrain data, etc.
    this->screen_width_ = screen_width;
    this->screen_height_ = screen_height;
}

void game::run()
{
    srand(static_cast<unsigned>(time(0)));
    renderer_.start();
}

void game::update()
{
    // Update game logic here
}

void game::render()
{
    // renderer.renderTerrain(); // Call the rendering function from your Renderer
    //  Render other game objects
}

int main()
{
    game game(constants::screen_width, constants::screen_height);
    game.run();

    return 0;
}
