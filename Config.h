#pragma once
#include <string>

namespace constants
{
const std::string font_file = "Roboto-Bold.ttf"; // sets the font file
constexpr bool fullscreen = false;               // sets the game to fullscreen
const std::string game_title = "Scorched Earth"; // sets the game title
constexpr float gravity = 9.8f;                  // sets the gravity default
constexpr float speed = 1.5f;                    // sets the speed default
constexpr int frame_rate = 0;                    // sets the frame rate default
constexpr bool render_grid = false;              // sets the grid to render
constexpr bool show_fps = true;                  // sets the fps to show
constexpr bool beta_terrain = false;             // sets the beta terrain to false
constexpr int screen_width = 1280;               // sets the screen width (windowed mode)
constexpr int screen_height = 720;               // sets the screen height (windowed mode)
constexpr int projectile_speed = 5.0f;           // sets the projectile speed
constexpr bool enable_physics = true;            // sets the physics to true
} // namespace constants
