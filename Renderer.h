#pragma once


#include <corecrt_math_defines.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "FileSystem.h"
#include "Config.h"
#include "SoundManager.h"
#include "TerrainGenerator.h"
#include "HUD.h"
#include "Physics.h"
#include "ProjectileCannon.h"
#include "Explosion.h"
#include "Tank.h"
#include <random>

class Renderer
{
public:
    bool fullscreen = constants::FULLSCREEN;         // convert from config.h to a variable
    bool renderGrid = constants::RENDER_GRID;        // convert from config.h to a variable
    bool showFPS = constants::SHOW_FPS;              // convert from config.h to a variable
    bool betaTerrain = constants::BETA_TERRAIN;      // todo: perhaps not needed?
    TerrainGenerator terrainObj;                     // create terrain object
    sf::RenderStates renderState;                    // Render state for the window
    sf::ContextSettings settings;                    // Settings for the window
    std::string windowTitle = constants::GAME_TITLE; // Window title
    sf::Texture backgroundTexture;                   // Background texture
    sf::Texture terrainTexture;                      // Terrain texture
    sf::Clock clock;                                 // Clock for timing
    sf::Time deltaTime;                              // Time between frames
    sf::RenderWindow window;                         // Render window
    sf::Font font;                                   // Font for rendering text
    sf::Text FPSText;                                // Text for rendering FPS
    std::vector<int> terrain;                        // todo: perhaps not needed?
    std::vector<float> frameTimeBuffer;              // Buffer to store frame times for averaging
    int screenWidth;                                 // Screen width in pixels
    int screenHeight;                                // Screen height in pixels
    int windowWidth;                                 // Window width in pixels
    int windowHeight;                                // Window height in pixels
    // todo: try and clean up all these screen and window variables
    Physics physics;                     // Physics class
    SoundManager SoundsObj;              // Sound manager class
    bool isDragging = false;             // mouse dragging - defaults to off, mouse functions then use this.
    bool enablePhysics = constants::ENABLE_PHYSICS;          // toggle physics simulation
    std::vector<Projectile> projectiles; // Vector of projectiles
    std::vector<Explosion> explosions;   // Vector of explosions
    std::vector<Tank> players;           // Vector of players
    int playerCount = 0;                 // Player count
    sf::VertexArray projectilePath;      // Vertex array for the projectile path
    HUD RenderHud;                       // HUD class

    ///< summary>
    // Constructor for the renderer
    ///</summary>
    ///< param name="screenWidth">int Screen Width in pixels</param>
    ///< param name="screenHeight">int Screen Height in pixels</param>
    Renderer(int screenWidth, int screenHeight);

    ///< summary>
    // Create the Render Window
    ///</summary>
    void createWindow();

    ///< summary>
    // Load the font assets
    ///</summary>
    void LoadFontAssets();

    ///< summary>
    // Define the settings for the render window
    ///</summary>
    void createSettings();

    ///< summary>
    // Load the image assets
    ///</summary>
    // // TODO: Move this to a seperate class
    // TODO: perhaps create a progress system for loading? will the game be that big?
    void LoadImageAssets();

    ///< summary>
    // the background image
    ///</summary>
    ///< param name="image">std::string image path</param>
    // todo: fix this to a more generic function, perhaps pass the variable which texture to load?
    void LoadBackgroundImage(const std::string& image);

    ///< summary>
    // Set the background image
    ///</summary>
    ///< param name="image">std::string image path</param>
    void SetBackgroundImage(const std::string& image);

    ///< summary>
    // Load the terrain image
    ///</summary>
    ///< param name="image">std::string image path</param>
    void LoadTerrainImage(const std::string& image);

    ///< summary>
    // Initialise a game restart, this should reset all necessary values and objects to start a new game
    ///</summary>
    void restartGame();

    ///< summary>
    // Start the Game Loop.
    //
    // This function runs the game loop and handles all events any update code should be included here and rendering is
    // done through the render() function
    ///</summary>
    void start();

    ///< summary>
    // Run the gravity simulation for all objects
    //
    // Currently only implemented for tanks.
    ///</summary>
    void runGravitySimulation();

    ///< summary>
    // Update the window size
    //
    // This function updates the window size variables to match the current window size
    //
    // this->windowWidth and this->windowHeight are used to store the current window size and should be used where
    // possible to ensure the correct size is used
    ///</summary>
    void updateWindowSize();

    ///< summary>
    // Set the frame limits
    //
    // This function sets the frame limits for the window, settings are stored in config.h
    ///</summary>
    void setFrameLimits();

    ///< summary>
    // Set the fullscreen state
    ///</summary>
    void setFullscreen(bool fullscreen);

    ///< summary>
    // Get the fullscreen state
    ///</summary>
    bool getFullscreen();

    ///< summary>
    ///< para>Toggle the fullscreen state</para>
    ///< para>Toggles and sets the fullscreen state, also recreates the window to apply the changes</para>
    ///</summary>
    void toggleFullScreen();

    /// <summary>
    /// <para>Handles user input events</para>
    /// <para>Currently handles:</para>
    /// <para>- Window Resize</para>
    /// <para>- Window Close</para>
    /// <para>- Keyboard Events</para>
    /// <para>- Mouse Events</para>
    /// </summary>
    void handleEvents();

    /// <summary>
    /// Calculate the origin of the projectile on the X axis
    /// </summary>
    // todo: this may need to be moved to the tank class and altered to get the correct position
    /// <param name="tank"> - Tank class</param>
    /// <returns></returns>
    int calculateProjectileOriginX(Tank tank)
    {
        // Calculate the origin of the projectile considering turret rotation, width, and the tip of the turret
        float halfTurretWidth = tank.getTurretWidth() / 2.0f;
        float turretTipOffsetX = halfTurretWidth * std::cos((tank.getAngle() + 90.0f) * M_PI / 180.0f);
        int originX = tank.getX() - static_cast<int>(turretTipOffsetX);
        return originX;
    }

    /// <summary>
    /// Calculate the origin of the projectile on the Y axis
    /// </summary>
    // todo: this may need to be moved to the tank class and altered to get the correct position
    /// <param name="tank"></param>
    /// <param name="projectile"></param>
    /// <returns></returns>
    int calculateProjectileOriginY(Tank tank, Projectile& projectile)
    {
        // Calculate the origin of the projectile at the tip of the turret
        float halfTurretWidth = tank.getTurretWidth() / 2.0f;
        float halfProjectileSize = projectile.getWidth() / 2.0f;
        float turretTipOffsetY = -(tank.getTurretHeight() / 2.0f); // Adjusted here
        int originY = tank.getY() - (tank.getBodyY() - static_cast<int>(turretTipOffsetY)) -
            static_cast<int>(halfTurretWidth + halfProjectileSize);
        return originY;
    }

    /// <summary>
    /// Loops and renders all explosions
    /// </summary>
    void renderExplosions();

    /// <summary>
    /// <para>Fires a projectile from the tank</para>
    ///
    /// <para>Adds projectile to the projectiles vector for rendering</para>
    /// </summary>
    // todo: this may need to be moved to the tank class since there is no specific rendering code here.
    /// <param name="tank"> - Tank Class to set the origin tank</param>
    void fireProjectile(Tank tank)
    {
        // Fire a projectile
        // Create a new projectile and add it to the vector
        Projectile cannon(tank);
        cannon.setExplosionDuration(3.0f);
        SoundsObj.Fire(); // play sound (debug)
        // Determine the initial position (initialX and initialY) and launch angle
        float initialX = calculateProjectileOriginX(tank);         // Set your desired initial X position
        float initialY = calculateProjectileOriginY(tank, cannon); // Set your desired initial Y position
        float launchAngle = tank.getAngle();                       // Set your desired launch angle in degrees

        cannon.launch(initialX, initialY, launchAngle - 90);
        // Add the projectile to the container
        projectiles.push_back(cannon);
    }

    /// <summary>
    /// Renders projectiles and should also render tracer lines
    /// </summary>
    void renderProjectiles();

    /// <summary>
    /// Queue the rendering of an explosion
    /// </summary>
    // todo: perhaps move to the explosion class? or projectile class for expandability, there is no rendering here.
    // other than the terrain update?
    /// <param name="projectile"></param>
    void queueRenderExplosion(Projectile projectile)
    {
        sf::Time duration = sf::seconds(2.0f);
        Explosion explosion(projectile.getX(), projectile.getY(), 40.0f, duration);
        terrainObj.updateTerrainWithExlosion(projectile.getX(), projectile.getY(), explosion.getExplosionSize() * 3,
            explosion.getExplosionSize() * 3);
        explosions.push_back(explosion);
    }

    /// <summary>
    /// Update the FPS counter stats
    /// </summary>
    /// <param name="averageFrameTime"></param>
    void updateFPS(float averageFrameTime);

    /// <summary>
    /// <para>Start the rendering process</para>
    /// <para>Call this function to render all objects to the screen</para>
    /// <para>Call this function last in the game loop</para>
    /// <para>The order of events to be rendered matters, **think HTML z-index**</para>
    /// <para>Currently renders:</para>
    /// <para>- Background</para>
    /// <para>- Terrain</para>
    /// <para>- Tanks</para>
    /// <para>- Projectiles</para>
    /// <para>- Explosions</para>
    /// <para>- HUD</para>
    /// <para>- FPS</para>
    /// <para>- Grid</para>
    /// </summary>
    void render();

    /// <summary>
    /// <para>Generate a texture for the tank</para>
    /// </summary>
    // todo: implement images for this to make more engaging

    /// <param name="tankWidth"> - int width of texture</param>
    /// <param name="tankHeight"> - int height of texture</param>
    /// <returns>sf:Texture</returns>+
    sf::Texture generateTankTexture(int tankWidth, int tankHeight);

    /// <summary>
    /// Generates the turret texture
    /// </summary>
    /// <param name="turretWidth"></param>
    /// <param name="turretHeight"></param>
    // todo: implement images for this to make more engaging
    /// <returns></returns>
    sf::Texture generateTurretTexture(int turretWidth, int turretHeight);

    /// <summary>
    /// Collates the tank and turret textures and renders them to the screen
    /// </summary>
    /// <param name="tank"> - Tank class</param>
    void generateTank(Tank tank)
    {
        // Generate a tank
        sf::Texture tankTexture = generateTankTexture(tank.getBodyX(), tank.getBodyY());
        sf::Sprite tankSprite;
        tankSprite.setTexture(tankTexture);
        tankSprite.setPosition(tank.getX(), tank.getY());
        tankSprite.setOrigin(tank.getOriginX(), tank.getOriginY());
        // Generate a turret
        sf::Texture turretTexture = generateTurretTexture(tank.getTurretWidth(), tank.getTurretHeight());
        sf::Sprite turretSprite;
        turretSprite.setTexture(turretTexture);
        turretSprite.setOrigin(tank.getTurretWidth() / 2, 0);
        turretSprite.setPosition(tank.getX(), tank.getY() - (tank.getBodyY() / 2.0f) + 5);
        turretSprite.setRotation(tank.getAngle());
        // todo: perhaps generate to a texture and then render the texture to the screen?
        //  Draw both the tank and turret
        window.draw(tankSprite);
        window.draw(turretSprite);
    }

    /// <summary>
    /// Sets a player count
    /// </summary>
    // todo: implement this in the game menu
    /// <param name="playerCount"></param>
    void setPlayerCount(int playerCount);

    /// <summary>
    /// <para>Gets the player count</para>
    /// </summary>
    /// <returns>int playercount</returns>
    int getPlayerCount();

    /// <summary>
    /// <para>Loop through player count and create players</para>
    /// </summary>
    void createPlayers();

    /// <summary>
    /// <para>Renders Pixel Grid for debugging</para>
    /// </summary>
    /// <param name="pixels">int - the number of pixels between grid lines</param>
    void renderGridLines(int pixels);

    /// <summary>
    /// <para>Renders the label for frame rate</para>
    /// </summary>
    void renderFPS();
};