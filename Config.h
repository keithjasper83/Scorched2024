#pragma once
#include <string>

namespace constants
{
const int verbose = 1;                           // used for debug function to display detailed information
const bool debug_hitboxes = false;               // used for debug function to display hitboxes
const std::string font_file = "Roboto-Bold.ttf"; // sets the font file
constexpr bool fullscreen = true;                // sets the game to fullscreen
const std::string game_title = "Scorched Earth"; // sets the game title
constexpr float gravity = 9.8f;                  // sets the gravity default
constexpr float speed = 1.5f;                    // sets the speed default
constexpr int frame_rate = 0;                    // sets the frame rate default
constexpr bool render_grid = false;              // sets the grid to render
constexpr bool show_fps = true;                  // sets the fps to show
constexpr bool beta_terrain = false;             // sets the beta terrain to false
const int window_screen_width = 1280;            // sets the screen width (windowed mode)
const int window_screen_height = 720;            // sets the screen height (windowed mode)
constexpr int projectile_speed = 5.0f;           // sets the projectile speed
constexpr bool enable_physics = true;            // sets the physics to true
constexpr int particle_count = 100;              // sets the wind to true
constexpr bool enable_particles = true;          // sets the particles to render
constexpr float tank_scale = 1.0f;               // sets the tank scale
constexpr bool display_menu = false;             // sets the menu to display - game always starts with menu (DEBUGGING)
const std::string menu_background_file = "Images/menu_background.png"; // sets the menu background file
const int default_starting_players = 2;                                // sets the default starting players
} // namespace constants
