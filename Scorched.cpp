#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include <cmath>
#include <ctime>
#include <vector>
#include <string>


#include "Renderer.cpp"
#include "TerrainGenerator.cpp"

class Game {
public:

    Game(int screenWidth, int screenHeight, const std::string& windowTitle)
        : renderer(screenWidth, screenHeight, windowTitle) {
        // Initialize your game objects, terrain data, etc.
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
    }

    void run() {
        srand(static_cast<unsigned>(time(0)));
        renderer.start();
    }


private:
    Renderer renderer; // Your Renderer class
    int screenWidth;
    int screenHeight;

    // Add other game-related members here

    void update() {
        // Update game logic here
    }

    void render() {
        //renderer.renderTerrain(); // Call the rendering function from your Renderer
        // Render other game objects
    }
};

int main() {

    Game game(1280, 720, constants::GAME_TITLE);
    game.run();

    return 0;
}
