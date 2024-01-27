#include "ConfigJSON.h"
#include "debug_output.h"
#include <Windows.h>
#include <fstream>
#include <iomanip>
#include <iostream>
ConfigJSON::ConfigJSON()
{
    std::cout << "CONFIG CONSTRUCTOR" << std::endl;
    std::cout << "CONFIG CONSTRUCTOR" << std::endl;
    std::cout << "CONFIG CONSTRUCTOR" << std::endl;
    KJ::debug_output::print(__FILE__, "ConfigJSON::ConfigJSON()", KJ::debug_output::MessageType::GOOD);
    loadConfigText();
    Sleep(100);
    dumpMapsToConsole();
    std::cout << "CONFIG CONSTRUCTOR" << std::endl;
    std::cout << "CONFIG CONSTRUCTOR" << std::endl;
}

bool ConfigJSON::saveConfigText(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Couldn't open file for writing: " << filename << std::endl;
        return false;
    }
    for (const auto &pair : intConfigData)
    {
        file << pair.first << " = " << pair.second << "\n";
        // std::cout << "AAAAA - " << pair.first << " = " << pair.second << "\n";
    }
    for (const auto &pair : boolConfigData)
    {
        file << pair.first << " = " << (pair.second ? "true" : "false") << "\n";
        // std::cout << "AAAAA - " << pair.first << " = " << pair.second << "\n";
    }
    for (const auto &pair : stringConfigData)
    {
        file << pair.first << " = " << pair.second << "\n";
        // std::cout << "AAAAA - " << pair.first << " = " << pair.second << "\n";
    }
    file << "configLoaded"
         << " = "
         << "true"
         << "\n";

    file.close();
    return true;
}

bool ConfigJSON::loadConfigText(const std::string &filename)
{
    std::cout << "Loading config file: " << filename << std::endl;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Couldn't open file for reading: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos)
        {
            // std::cout << "Delimiter found in line: " << line << std::endl;
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);
            // Trim spaces
            key.erase(0, key.find_first_not_of(" \t\r\n"));
            key.erase(key.find_last_not_of(" \t\r\n") + 1);
            value.erase(0, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);
            //  Assuming keys are unique
            if (intConfigData.find(key) != intConfigData.end())
            {
                intConfigData[key] = std::stoi(value);
                // std::cout << "AAAAA - " << key << " = " << intConfigData[key] << "\n";
            }
            else if (boolConfigData.find(key) != boolConfigData.end())
            {
                boolConfigData[key] = (value == "true");
                // std::cout << "AAAAA - " << key << " = " << boolConfigData[key] << "\n";
            }
            else if (stringConfigData.find(key) != stringConfigData.end())
            {
                stringConfigData[key] = value;
                // std::cout << "AAAAA - " << key << " = " << stringConfigData[key] << "\n";
            }
        }
        else
        {
            std::cout << "No delimiter found in line: " << line << std::endl;
        }
    }
    file.close();
    return true;
};

std::unordered_map<std::string, int> ConfigJSON::intConfigData;
std::unordered_map<std::string, bool> ConfigJSON::boolConfigData;
std::unordered_map<std::string, std::string> ConfigJSON::stringConfigData;
std::unordered_map<std::string, float> ConfigJSON::floatConfigData;

// Define your configuration keys here

const std::string ConfigJSON::VERBOSE_MOUSE_DEBUGGING_KEY = "verboseMouseDebugging";
const std::string ConfigJSON::VERBOSE_DEBUGGING_KEY = "verboseDebugging";
const std::string ConfigJSON::RENDER_DEBUG_HITBOXES_KEY = "renderDebugHitboxes";
const std::string ConfigJSON::ENGINE_FONT_FILE_KEY = "engineFontFile";
const std::string ConfigJSON::DEFAULT_FULLSCREEN_KEY = "defaultFullscreen";
const std::string ConfigJSON::GAME_WINDOW_TITLE_KEY = "gameWindowTitle";
const std::string ConfigJSON::ENGINE_GRAVITY_KEY = "engineGravity";
const std::string ConfigJSON::ENGINE_SPEED_KEY = "engineSpeed";
const std::string ConfigJSON::MAX_FPS_KEY = "maxFPS";
const std::string ConfigJSON::RENDER_DEBUG_GRID_KEY = "renderDebugGrid";
const std::string ConfigJSON::RENDER_FPS_KEY = "renderFPS";
const std::string ConfigJSON::WINDOWED_RESOLUTION_X_KEY = "windowedResolutionX";
const std::string ConfigJSON::WINDOWED_RESOLUTION_Y_KEY = "windowedResolutionY";
const std::string ConfigJSON::ENGINE_PROJECTILE_SPEED_KEY = "engineProjectileSpeed";
const std::string ConfigJSON::ENGINE_PHYSICS_ENABLED_KEY = "enginePhysicsEnabled";
const std::string ConfigJSON::RENDER_BACKGROUND_PARTICLE_COUNT_KEY = "renderBackgroundParticleCount";
const std::string ConfigJSON::RENDER_PARTICLES_ENABLED_KEY = "renderParticlesEnabled";
const std::string ConfigJSON::RENDER_TANK_SCALE_KEY = "renderTankScale";
const std::string ConfigJSON::RENDER_DEBUG_MENU_FORCE_KEY = "renderDebugMenuForce";
const std::string ConfigJSON::RENDER_MENU_BACKGROUND_FILE_KEY = "renderMenuBackgroundFile";
const std::string ConfigJSON::ENGINE_DEFAULT_START_PLAYERS_KEY = "engineDefaultStartPlayers";

const bool ConfigJSON::DEFAULT_VERBOSE_MOUSE_DEBUGGING = false;
const int ConfigJSON::DEFAULT_VERBOSE_DEBUGGING = 1;
const bool ConfigJSON::DEFAULT_RENDER_DEBUG_HITBOXES = false;
const std::string ConfigJSON::DEFAULT_ENGINE_FONT_FILE = "Roboto-Bold.ttf";
const bool ConfigJSON::DEFAULT_DEFAULT_FULLSCREEN = false;
const std::string ConfigJSON::DEFAULT_GAME_WINDOW_TITLE = "Scorched Earth";
const float ConfigJSON::DEFAULT_ENGINE_GRAVITY = 9.81f;
const float ConfigJSON::DEFAULT_ENGINE_SPEED = 1.5f;
const int ConfigJSON::DEFAULT_MAX_FPS = 0;
const bool ConfigJSON::DEFAULT_RENDER_DEBUG_GRID = false;
const bool ConfigJSON::DEFAULT_RENDER_FPS = true;
const int ConfigJSON::DEFAULT_WINDOWED_RESOLUTION_X = 1280;
const int ConfigJSON::DEFAULT_WINDOWED_RESOLUTION_Y = 720;
const float ConfigJSON::DEFAULT_ENGINE_PROJECTILE_SPEED = 2.0f;
const bool ConfigJSON::DEFAULT_ENGINE_PHYSICS_ENABLED = true;
const int ConfigJSON::DEFAULT_RENDER_BACKGROUND_PARTICLE_COUNT = 100;
const bool ConfigJSON::DEFAULT_RENDER_PARTICLES_ENABLED = true;
const float ConfigJSON::DEFAULT_RENDER_TANK_SCALE = 1.0f;
const bool ConfigJSON::DEFAULT_RENDER_DEBUG_MENU_FORCE = false;
const std::string ConfigJSON::DEFAULT_RENDER_MENU_BACKGROUND_FILE = "Images/menu_background.png";
const int ConfigJSON::DEFAULT_ENGINE_DEFAULT_START_PLAYERS = 2;
