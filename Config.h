#pragma once
#include "ConfigJSON.h"
#include <string>

namespace UNUSEDconstants
{
const int VerboseDebugging = 1;                       // used for debug function to display detailed information
const bool RenderDebugHitboxes = false;               // used for debug function to display hitboxes
const std::string EngineFontFile = "Roboto-Bold.ttf"; // sets the font file
constexpr bool DefaultFullscreen = false;             // sets the game to fullscreen
const std::string GameWindowTitle = "Scorched Earth"; // sets the game title
constexpr float EngineGravity = 9.8f;                 // sets the gravity default
constexpr float EngineSpeed = 1.5f;                   // sets the speed default
constexpr int frame_rate = 0;                         // sets the frame rate default
constexpr bool RenderDebugGrid = false;               // sets the grid to render
constexpr bool RenderFPS = true;                      // sets the fps to show
const int WindowsedResolutionX = 1280;                // sets the screen width (windowed mode)
const int WindowsedResolutionY = 720;                 // sets the screen height (windowed mode)
constexpr int EngineProjectileSpeed = 5.0f;           // sets the projectile speed
constexpr bool EnginePhysicsEnabled = true;           // sets the physics to true
constexpr int RenderBackgroundParticleCount = 100;    // sets the wind to true
constexpr bool RenderParticlesEnabled = true;         // sets the particles to render
constexpr float RenderTankScale = 1.0f;               // sets the tank scale
constexpr bool RenderDebugMenuForce = false; // sets the menu to display - game always starts with menu (DEBUGGING)
const std::string RenderMenuBackgroundFile = "Images/menu_background.png"; // sets the menu background file
const int EngineDefaultStartPlayers = 2;                                   // sets the default starting players
} // namespace UNUSEDconstants
