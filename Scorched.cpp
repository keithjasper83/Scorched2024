#include "Scorched.h"

// todo: modify classes to load assets from this class (individual items into buffers) to allow for a progress bar to be
// sent to renderer for "splash screen" todo: create a settings class to switch away from using config.h for settings
// this will allow for settings interface to be created
// todo: reset all game scaling of screen.
// todo: create rendering contextt independant of screen size so a canvas is drawn which can the be drawn to the screen,
// allowing for a "zoom" feature todo: create type casting functions to manually cast between types (i.e. float to int,
// int to float, etc.) todo: any math functions need to be protected from divide by zero errors

// todo: create user interface elements for confdiguration screen
// todo: add volume controls and notification of volume and mute status - perhaps a time dependant display which fades
// out after a few seconds todo: create player naming screen todo: create play rotation system
// todo: create "points"/currency system todo: create a system to choose resolution and windowed/fullscreen mode

// todo: find unused variables and remove them
// todo: find unused functions and remove them
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
    // TODO - add a splash screen
    // TODO - add a loading screen
    ConfigJSON config = ConfigJSON();
    ConfigJSON::loadConfigText();
    ConfigJSON::dumpMapsToConsole();
    game game(ConfigJSON::getWindowedResolutionX(), ConfigJSON::getWindowedResolutionY());
    game.run();

    return 0;
}
