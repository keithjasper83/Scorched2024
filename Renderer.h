
#pragma once
#include "debug_output.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <corecrt_math_defines.h>

#include "0ProjectileDefaults.h"
#include "ProjectileFactory.h"

#include "Config.h"
#include "Explosion.h"
#include "FileSystem.h"
#include "HUD.h"
#include "MenuManager.h"
#include "MenuModalWindow.h"
#include "Physics.h"

#include "SoundManager.h"
#include "Tank.h"
#include "TerrainGenerator.h"
#include "background_particles.h"
#include "game_state_controller.h"
#include "window_manager.h"
#include <random>

class renderer
{
  public:
    struct
    {
        int x = 0;
        int y = 0;
    } fire_origin;

    enum class MenuOption
    {
        None,
        Resume,
        Settings,
        Quit
    };

    window_manager::canvas_resolution canvas_resolution;
    window_manager::display_resolution display_resolution;

    game_state_controller game_state;
    sf::Image iconImage_;

    renderer(int screen_width, int screen_height);
    /*
    RENDER FUNCTIONS
    */
    void render(sf::RenderWindow &renderWindow);
    void renderBackground(sf::RenderWindow &renderWindow);
    void renderTerrain(sf::RenderWindow &renderWindow);
    void renderPlayers(sf::RenderWindow &renderWindow);
    void renderPlayerHud(sf::RenderWindow &renderWindow);
    void renderExplosions(sf::RenderWindow &renderWindow);
    void renderProjectiles(sf::RenderWindow &renderWindow);
    void renderParticles(sf::RenderWindow &renderWindow);
    void renderMenuScreens(sf::RenderWindow &renderWindow);
    void renderPauseMenu(sf::RenderWindow &renderWindow);
    void renderDebugData(sf::RenderWindow &renderWindow);
    void renderDebugGrid(sf::RenderWindow &renderWindow, int pixels = 100);
    void renderDebugFPS(sf::RenderWindow &renderWindow);
    void renderDebugCollisionDetectionShowHitboxes(sf::RenderWindow &renderWindow);
    void renderWinScreen(sf::RenderWindow &renderWindow, std::string winMessage);
    void renderPlayer(sf::RenderWindow &renderWindow, tank tank);

    /*
    GameState Functions
    */
    void checkWinState(sf::RenderWindow &renderWindow);
    void rotatePlayerTurn();
    void restart_game();

    /*
    Engine Functions
    */
    void engineSetup(sf::RenderWindow &renderWindow);
    void engineHandleInputEvents(sf::RenderWindow &renderWindow);
    void engineFireProjectile(tank tank);
    void create_window();
    void initializeRenderer(const int screen_width, const int screen_height);
    void setIcon(sf::RenderWindow &window);
    void createSettings();
    void frameUpdate();
    void engineUpdate(sf::RenderWindow &renderWindow);
    void loadImageAssets();
    void loadImage(const std::string &image, sf::Texture &texture);
    void load_terrain_image(const std::string &image);
    void start();
    void physicsRun();
    void engineSetFrameLimits(sf::RenderWindow &renderWindow);
    void update_fps(float average_frame_time);

    void engineHandleMenuSelection(MenuOption selectedMenuOption);

    void engineResumeGame();
    void engineQuitGame();
    void engineOpenSettingsMenu();

    /*
    Validation Functions
    */
    void validateResolutions(const int screen_width, const int screen_height);
    bool loadIconImage();
    void loadFontAsset();

    /*
    Fullscreen Functions
    */
    void set_fullscreen(bool fullscreen);
    bool get_fullscreen() const;
    void toggle_full_screen(bool startFull = false);

    /*
    Fire Projectile Functions
    */
    static int calculate_projectile_origin_x(tank tank);
    static int calculate_projectile_origin_y(tank tank, ProjectileInterface &projectile);
    // void fire_projectile(tank tank);
    void queue_render_explosion(const ProjectileInterface &projectile);
    sf::Vector2f get_turret_projectile_origin(tank &tank);

    /*
    Generate Players
    */
    static sf::Texture generate_tank_texture(int tank_width, int tank_height);
    static sf::Texture generate_turret_texture(int turret_width, int turret_height);
    void engineCreatePlayers();

    /*
    Collision Detection
    */
    void collide_with_terrain(ProjectileInterface &projectile);
    void detect_tank(ProjectileInterface &projectile);
    void collide_with_tank(int player_index, ProjectileInterface &projectile);
    sf::FloatRect collisonDetectionGenerateHitbox(sf::RenderWindow &renderWindow, tank &tank);

    /*
    Getters and Setters
    */
    void set_player_count(int player_count);
    int get_player_count() const;

  private:
    size_t fpsCount = 0;          // Index of the current frame time
    float fpsFrameTimeSum = 0.0f; // Sum of the frame times
    sf::RenderWindow window_;     // Render window

    window_manager _window_manager;
    int currentPlayer = 0;
    bool fullscreen = ConfigJSON::getDefaultFullScreen();        // convert from config.h to a variable
    bool render_grid = ConfigJSON::getRenderDebugGrid();         // convert from config.h to a variable
    bool show_fps = ConfigJSON::getRenderFPS();                  // convert from config.h to a variable
    terrain_generator terrain_obj;                               // create terrain object
    sf::RenderStates render_state;                               // Render state for the window
    sf::ContextSettings settings;                                // Settings for the window
    std::string window_title = ConfigJSON::getGameWindowTitle(); // Window title
    sf::Texture background_texture;                              // Background texture
    sf::Texture terrain_texture;                                 // Terrain texture
    sf::Clock clock;                                             // Clock for timing
    sf::Time delta_time;                                         // Time between frames
    sf::Font font;                                               // Font for rendering text
    sf::Text fps_text;                                           // Text for rendering FPS
    std::vector<int> terrain;                                    // todo: perhaps not needed?
    std::vector<float> frame_time_buffer;                        // Buffer to store frame times for averaging

    sf::Vector2i mousePosition;
    // todo: try and clean up all these screen and window variables
    physics physics;          // Physics class
    sound_manager sounds_obj; // Sound manager class
    bool is_dragging = false; // mouse dragging - defaults to off, mouse functions then use this.
    bool enable_physics = ConfigJSON::getEnginePhysicsEnabled();     // toggle physics simulation
    std::vector<ProjectileInterface> projectiles;                    // Vector of projectiles
    std::vector<explosion> explosions;                               // Vector of explosions
    std::vector<tank> players;                                       // Vector of players
    int player_count = 0;                                            // Player count
    sf::VertexArray projectile_path;                                 // Vertex array for the projectile path
    hud render_hud;                                                  // HUD class
    background_particles particles;                                  // Background particles
    bool enable_particles = ConfigJSON::getRenderParticlesEnabled(); // toggle background particles
    sf::Texture tank_texture = tank::get_tank_texture();             // generate tank texture
    sf::Texture turret_texture = tank::get_turret_texture();         // generate turret texture
    bool display_menu = ConfigJSON::getRenderDebugMenuForce();       // Display the menu
    bool display_settings_menu = false;

    std::string selectedProjectileType = "DefaultProjectile"; // or "Missile" based on user selection or logic
};
