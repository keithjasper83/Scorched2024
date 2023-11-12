#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <corecrt_math_defines.h>

#include "Config.h"
#include "Explosion.h"
#include "FileSystem.h"
#include "HUD.h"
#include "Physics.h"
#include "ProjectileCannon.h"
#include "SoundManager.h"
#include "Tank.h"
#include "TerrainGenerator.h"
#include "background_particles.h"
#include <random>

class renderer
{
  public:
    struct
    {
        int x = 0;
        int y = 0;
    } fire_origin;

    bool fullscreen = constants::fullscreen;          // convert from config.h to a variable
    bool render_grid = constants::render_grid;        // convert from config.h to a variable
    bool show_fps = constants::show_fps;              // convert from config.h to a variable
    bool beta_terrain = constants::beta_terrain;      // todo: perhaps not needed?
    terrain_generator terrain_obj;                    // create terrain object
    sf::RenderStates render_state;                    // Render state for the window
    sf::ContextSettings settings;                     // Settings for the window
    std::string window_title = constants::game_title; // Window title
    sf::Texture background_texture;                   // Background texture
    sf::Texture terrain_texture;                      // Terrain texture
    sf::Clock clock;                                  // Clock for timing
    sf::Time delta_time;                              // Time between frames
    sf::RenderWindow window;                          // Render window
    sf::Font font;                                    // Font for rendering text
    sf::Text fps_text;                                // Text for rendering FPS
    std::vector<int> terrain;                         // todo: perhaps not needed?
    std::vector<float> frame_time_buffer;             // Buffer to store frame times for averaging
    int screen_width;                                 // Screen width in pixels
    int screen_height;                                // Screen height in pixels
    int window_width;                                 // Window width in pixels
    int window_height;                                // Window height in pixels
    // todo: try and clean up all these screen and window variables
    physics physics;                                 // Physics class
    sound_manager sounds_obj;                        // Sound manager class
    bool is_dragging = false;                        // mouse dragging - defaults to off, mouse functions then use this.
    bool enable_physics = constants::enable_physics; // toggle physics simulation
    std::vector<projectile> projectiles;             // Vector of projectiles
    std::vector<explosion> explosions;               // Vector of explosions
    std::vector<tank> players;                       // Vector of players
    int player_count = 0;                            // Player count
    sf::VertexArray projectile_path;                 // Vertex array for the projectile path
    hud render_hud;                                  // HUD class
    background_particles particles;                  // Background particles
    bool enable_particles = constants::enable_particles; // toggle background particles

    // HUD class

    ///< summary>
    // Constructor for the renderer
    ///</summary>
    ///< param name="screenWidth">int Screen Width in pixels</param>
    ///< param name="screenHeight">int Screen Height in pixels</param>
    renderer(int screen_width, int screen_height);

    ///< summary>
    // Create the Render Window
    ///</summary>
    void create_window();

    ///< summary>
    // Load the font assets
    ///</summary>
    void load_font_assets();

    ///< summary>
    // Define the settings for the render window
    ///</summary>
    void create_settings();

    ///< summary>
    // Load the image assets
    ///</summary>
    // // TODO: Move this to a seperate class
    // TODO: perhaps create a progress system for loading? will the game be that big?
    void load_image_assets();

    ///< summary>
    // the background image
    ///</summary>
    ///< param name="image">std::string image path</param>
    // todo: fix this to a more generic function, perhaps pass the variable which texture to load?
    void load_background_image(const std::string &image);

    ///< summary>
    // Set the background image
    ///</summary>
    ///< param name="image">std::string image path</param>
    void set_background_image(const std::string &image);

    ///< summary>
    // Load the terrain image
    ///</summary>
    ///< param name="image">std::string image path</param>
    void load_terrain_image(const std::string &image);

    ///< summary>
    // Initialise a game restart, this should reset all necessary values and objects to start a new game
    ///</summary>
    void restart_game();

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
    void run_gravity_simulation();

    ///< summary>
    // Update the window size
    //
    // This function updates the window size variables to match the current window size
    //
    // this->windowWidth and this->windowHeight are used to store the current window size and should be used where
    // possible to ensure the correct size is used
    ///</summary>
    void update_window_size();

    ///< summary>
    // Set the frame limits
    //
    // This function sets the frame limits for the window, settings are stored in config.h
    ///</summary>
    void set_frame_limits();

    ///< summary>
    // Set the fullscreen state
    ///</summary>
    void set_fullscreen(bool fullscreen);

    ///< summary>
    // Get the fullscreen state
    ///</summary>
    bool get_fullscreen() const;

    ///< summary>
    ///< para>Toggle the fullscreen state</para>
    ///< para>Toggles and sets the fullscreen state, also recreates the window to apply the changes</para>
    ///</summary>
    void toggle_full_screen();

    /// <summary>
    /// <para>Handles user input events</para>
    /// <para>Currently handles:</para>
    /// <para>- Window Resize</para>
    /// <para>- Window Close</para>
    /// <para>- Keyboard Events</para>
    /// <para>- Mouse Events</para>
    /// </summary>
    void handle_events();

    /// <summary>
    /// Calculate the origin of the projectile on the X axis
    /// </summary>
    // todo: this may need to be moved to the tank class and altered to get the correct position
    /// <param name="tank"> - Tank class</param>
    /// <returns></returns>
    static int calculate_projectile_origin_x(tank tank);

    /// <summary>
    /// Calculate the origin of the projectile on the Y axis
    /// </summary>
    // todo: this may need to be moved to the tank class and altered to get the correct position
    /// <param name="tank"></param>
    /// <param name="projectile"></param>
    /// <returns></returns>
    static int calculate_projectile_origin_y(tank tank, projectile &projectile);

    /// <summary>
    /// Loops and renders all explosions
    /// </summary>
    void render_explosions();

    /// <summary>
    /// <para>Fires a projectile from the tank</para>
    ///
    /// <para>Adds projectile to the projectiles vector for rendering</para>
    /// </summary>
    // todo: this may need to be moved to the tank class since there is no specific rendering code here.
    /// <param name="tank"> - Tank Class to set the origin tank</param>
    void fire_projectile(tank tank);

    /// <summary>
    /// Renders projectiles and should also render tracer lines
    /// </summary>
    void render_projectiles();

    /// <summary>
    /// Queue the rendering of an explosion
    /// </summary>
    // todo: perhaps move to the explosion class? or projectile class for expandability, there is no rendering here.
    // other than the terrain update?
    /// <param name="projectile"></param>
    void queue_render_explosion(const projectile &projectile);

    /// <summary>
    /// Update the FPS counter stats
    /// </summary>
    /// <param name="average_frame_time"></param>
    void update_fps(float average_frame_time);

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

    /// <param name="tank_width"> - int width of texture</param>
    /// <param name="tank_height"> - int height of texture</param>
    /// <returns>sf:Texture</returns>+
    static sf::Texture generate_tank_texture(int tank_width, int tank_height);

    /// <summary>
    /// Generates the turret texture
    /// </summary>
    /// <param name="turret_width">The turrets width</param>
    /// <param name="turret_height">Turret Height</param>
    // todo: implement images for this to make more engaging
    /// <returns></returns>
    static sf::Texture generate_turret_texture(int turret_width, int turret_height);

    /// <summary>
    /// Collates the tank and turret textures and renders them to the screen
    /// </summary>
    /// <param name="tank"> - Tank class</param>
    void generate_tank(tank tank);

    /// <summary>
    /// Sets a player count
    /// </summary>
    // todo: implement this in the game menu
    /// <param name="player_count"></param>
    void set_player_count(int player_count);

    /// <summary>
    /// <para>Gets the player count</para>
    /// </summary>
    /// <returns>int playercount</returns>
    int get_player_count() const;

    /// <summary>
    /// <para>Loop through player count and create players</para>
    /// </summary>
    void create_players();

    /// <summary>
    /// <para>Renders Pixel Grid for debugging</para>
    /// </summary>
    /// <param name="pixels">int - the number of pixels between grid lines</param>
    void render_grid_lines(int pixels);

    /// <summary>
    /// <para>Renders the label for frame rate</para>
    /// </summary>
    void render_fps();

    static sf::Vector2f get_turret_projectile_origin(tank &tank);
};
