#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

#define M_PI 3.14159265358979323846

class ConfigJSON
{
  public:
    static std::unordered_map<std::string, int> intConfigData;
    static std::unordered_map<std::string, bool> boolConfigData;
    static std::unordered_map<std::string, std::string> stringConfigData;
    static std::unordered_map<std::string, float> floatConfigData;

    ConfigJSON();

    static bool checkConfigLoad()
    {
        std::cout << "Checking config load - " << getConfigLoaded() << std::endl;
        return getConfigLoaded();
    }

    static void dumpMapsToConsole()
    {
        for (const auto &[key, value] : intConfigData)
        {
            std::cout << "Key: " << key << " Value: " << value << std::endl;
        }

        for (const auto &[key, value] : boolConfigData)
        {
            std::cout << "Key: " << key << " Value: " << value << std::endl;
        }

        for (const auto &[key, value] : stringConfigData)
        {
            std::cout << "Key: " << key << " Value: " << value << std::endl;
        }
    }

    static bool saveConfigText(const std::string &filename = "config.txt");
    static bool loadConfigText(const std::string &filename = "config.txt");

    static bool getVerboseMouseDebugging()
    {
        return getConfigValue(VERBOSE_MOUSE_DEBUGGING_KEY, DEFAULT_VERBOSE_MOUSE_DEBUGGING);
    }

    static int getVerboseDebugging()
    {
        return getConfigValue(VERBOSE_DEBUGGING_KEY, DEFAULT_VERBOSE_DEBUGGING);
    }

    static bool getRenderDebugHitboxes()
    {
        return getConfigValue(RENDER_DEBUG_HITBOXES_KEY, DEFAULT_RENDER_DEBUG_HITBOXES);
    }

    static std::string getEngineFontFile()
    {
        return getConfigValue(ENGINE_FONT_FILE_KEY, DEFAULT_ENGINE_FONT_FILE);
    }

    static bool getDefaultFullScreen()
    {
        return getConfigValue(DEFAULT_FULLSCREEN_KEY, DEFAULT_DEFAULT_FULLSCREEN);
    }

    static std::string getGameWindowTitle()
    {
        return getConfigValue(GAME_WINDOW_TITLE_KEY, DEFAULT_GAME_WINDOW_TITLE);
    }

    static float getEngineGravity()
    {
        return getConfigValue(ENGINE_GRAVITY_KEY, DEFAULT_ENGINE_GRAVITY);
    }

    static float getEngineSpeed()
    {
        return getConfigValue(ENGINE_SPEED_KEY, DEFAULT_ENGINE_SPEED);
    }

    static int getMaxFPS()
    {
        return getConfigValue(MAX_FPS_KEY, DEFAULT_MAX_FPS);
    }

    static bool getRenderDebugGrid()
    {
        return getConfigValue(RENDER_DEBUG_GRID_KEY, DEFAULT_RENDER_DEBUG_GRID);
    }

    static bool getRenderFPS()
    {
        return getConfigValue(RENDER_FPS_KEY, DEFAULT_RENDER_FPS);
    }

    static int getWindowedResolutionX()
    {
        return getConfigValue(WINDOWED_RESOLUTION_X_KEY, DEFAULT_WINDOWED_RESOLUTION_X);
    }

    static int getWindowedResolutionY()
    {
        return getConfigValue(WINDOWED_RESOLUTION_Y_KEY, DEFAULT_WINDOWED_RESOLUTION_Y);
    }

    static float getEngineProjectileSpeed()
    {
        return getConfigValue(ENGINE_PROJECTILE_SPEED_KEY, DEFAULT_ENGINE_PROJECTILE_SPEED);
    }

    static bool getEnginePhysicsEnabled()
    {
        return getConfigValue(ENGINE_PHYSICS_ENABLED_KEY, DEFAULT_ENGINE_PHYSICS_ENABLED);
    }

    static int getRenderBackgroundParticleCount()
    {
        return getConfigValue(RENDER_BACKGROUND_PARTICLE_COUNT_KEY, DEFAULT_RENDER_BACKGROUND_PARTICLE_COUNT);
    }

    static bool getRenderParticlesEnabled()
    {
        return getConfigValue(RENDER_PARTICLES_ENABLED_KEY, DEFAULT_RENDER_PARTICLES_ENABLED);
    }

    static float getRenderTankScale()
    {
        return getConfigValue(RENDER_TANK_SCALE_KEY, DEFAULT_RENDER_TANK_SCALE);
    }

    static bool getRenderDebugMenuForce()
    {
        return getConfigValue(RENDER_DEBUG_MENU_FORCE_KEY, DEFAULT_RENDER_DEBUG_MENU_FORCE);
    }

    static std::string getRenderMenuBackgroundFile()
    {
        return getConfigValue(RENDER_MENU_BACKGROUND_FILE_KEY, DEFAULT_RENDER_MENU_BACKGROUND_FILE);
    }

    static int getEngineDefaultStartPlayers()
    {
        return getConfigValue(ENGINE_DEFAULT_START_PLAYERS_KEY, DEFAULT_ENGINE_DEFAULT_START_PLAYERS);
    }

    static bool getConfigLoaded()
    {
        return getConfigValue("configLoaded", false);
    }

    // TODO:: CONTINUE CONVERTING CONFIG TO JSON -- ANY REFERENCE TO CONFIG CONSTANTS WILL NEED TO ME UPDATED ONCE THIS
    // IS COMPLETE!

    // Add other getters for configuration properties...

    // Function to modify configuration data
    // Setters for configuration properties

    static void setVerboseMouseDebugging(bool val)
    {
        setConfigValue(VERBOSE_MOUSE_DEBUGGING_KEY, val);
    }
    static void setVerboseDebugging(int val)
    {
        setConfigValue(VERBOSE_DEBUGGING_KEY, val);
    }

    static void setRenderDebugHitboxes(bool val)
    {
        setConfigValue(RENDER_DEBUG_HITBOXES_KEY, val);
    }

    static void setEngineFontFile(const std::string &val)
    {
        setConfigValue(ENGINE_FONT_FILE_KEY, val);
    }

    static void setDefaultFullScreen(bool val)
    {
        setConfigValue(DEFAULT_FULLSCREEN_KEY, val);
    }

    static void setGameWindowTitle(const std::string &val)
    {
        setConfigValue(GAME_WINDOW_TITLE_KEY, val);
    }

    static void setEngineGravity(float val)
    {
        setConfigValue(ENGINE_GRAVITY_KEY, val);
    }

    static void setEngineSpeed(float val)
    {
        setConfigValue(ENGINE_SPEED_KEY, val);
    }

    static void setMaxFPS(int val)
    {
        setConfigValue(MAX_FPS_KEY, val);
    }

    static void setRenderDebugGrid(bool val)
    {
        setConfigValue(RENDER_DEBUG_GRID_KEY, val);
    }

    static void setRenderFPS(bool val)
    {
        setConfigValue(RENDER_FPS_KEY, val);
    }

    static void setWindowedResolutionX(int val)
    {
        setConfigValue(WINDOWED_RESOLUTION_X_KEY, val);
    }

    static void setWindowedResolutionY(int val)
    {
        setConfigValue(WINDOWED_RESOLUTION_Y_KEY, val);
    }

    static void setEngineProjectileSpeed(float val)
    {
        setConfigValue(ENGINE_PROJECTILE_SPEED_KEY, val);
    }

    static void setEnginePhysicsEnabled(bool val)
    {
        setConfigValue(ENGINE_PHYSICS_ENABLED_KEY, val);
    }

    static void setRenderBackgroundParticleCount(int val)
    {
        setConfigValue(RENDER_BACKGROUND_PARTICLE_COUNT_KEY, val);
    }

    static void setRenderParticlesEnabled(bool val)
    {
        setConfigValue(RENDER_PARTICLES_ENABLED_KEY, val);
    }

    static void setRenderTankScale(float val)
    {
        setConfigValue(RENDER_TANK_SCALE_KEY, val);
    }

    static void setRenderDebugMenuForce(bool val)
    {
        setConfigValue(RENDER_DEBUG_MENU_FORCE_KEY, val);
    }

    static void setRenderMenuBackgroundFile(const std::string &val)
    {
        setConfigValue(RENDER_MENU_BACKGROUND_FILE_KEY, val);
    }

    static void setEngineDefaultStartPlayers(int val)
    {
        setConfigValue(ENGINE_DEFAULT_START_PLAYERS_KEY, val);
    }

  private:
    // Add other data types as needed

    // Getters for configuration properties
    template <typename T> static T getConfigValue(const std::string &key, const T &defaultvalue)
    {
        if constexpr (std::is_same_v<T, float>)
        {
            return getFloatConfigValue(key, defaultvalue);
        }
        else if constexpr (std::is_same_v<T, int>)
        {
            return getIntConfigValue(key, defaultvalue);
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            return getBoolConfigValue(key, defaultvalue);
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            return getStringConfigValue(key, defaultvalue);
        }
        // Add support for other types as needed
        // Default return or error handling for unsupported types
        return T();
    }

    // Setters for configuration properties
    template <typename T> static void setConfigValue(const std::string &key, const T &value)
    {
        if constexpr (std::is_same_v<T, float>)
        {
            setFloatConfigValue(key, value);
        }
        else if constexpr (std::is_same_v<T, int>)
        {
            setIntConfigValue(key, value);
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            setBoolConfigValue(key, value);
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            setStringConfigValue(key, value);
        }
        // Add support for other types as needed
    }
    static float getFloatConfigValue(const std::string &key, const float &defaultValue)
    {
        if (floatConfigData.find(key) != floatConfigData.end())
        {
            return floatConfigData.at(key);
        }
        else
        {
            setFloatConfigValue(key, defaultValue);
            // Handle missing key by returning default Value(
            // std::cout << "Key '" << key << "' not found. Returning default float Value." << std::endl;
            return defaultValue;
        }
    }
    // Functions to get specific types of configuration Value(s
    static int getIntConfigValue(const std::string &key, const int &defaultValue)
    {
        if (intConfigData.find(key) != intConfigData.end())
        {
            // std::cout << "RETURNING KEY: " << key << "Value : " << intConfigData.at(key) << std::endl;
            return intConfigData.at(key);
        }
        else
        {
            setIntConfigValue(key, defaultValue);
            // Handle missing key by returning default Value(
            // std::cout << "Key '" << key << "' not found. Returning default int Value." << std::endl;
            return defaultValue;
        }
    }

    static bool getBoolConfigValue(const std::string &key, const bool &defaultValue)
    {
        if (boolConfigData.find(key) != boolConfigData.end())
        {
            return boolConfigData.at(key);
        }
        else
        {
            setBoolConfigValue(key, defaultValue);
            // Handle missing key by returning default Value(
            // std::cout << "Key '" << key << "' not found. Returning default bool Value." << std::endl;
            return defaultValue;
        }
    }

    static std::string getStringConfigValue(const std::string &key, const std::string &defaultValue)
    {
        if (stringConfigData.find(key) != stringConfigData.end())
        {
            return stringConfigData.at(key);
        }
        else
        {
            setStringConfigValue(key, defaultValue);
            // Handle missing key by returning default Value(
            // std::cout << "Key '" << key << "' not found. Returning default string Value." << std::endl;
            return defaultValue;
        }
    }

    // Functions to set specific types of configuration Value(s
    static void setIntConfigValue(const std::string &key, int value)
    {
        intConfigData[key] = value;
    }
    static void setFloatConfigValue(const std::string &key, float value)
    {
        floatConfigData[key] = value;
    }

    static void setBoolConfigValue(const std::string &key, bool value)
    {
        boolConfigData[key] = value;
    }

    static void setStringConfigValue(const std::string &key, const std::string &value)
    {
        stringConfigData[key] = value;
    }

    // Define your configuration keys here
    static const std::string VERBOSE_MOUSE_DEBUGGING_KEY;
    static const std::string VERBOSE_DEBUGGING_KEY;
    static const std::string RENDER_DEBUG_HITBOXES_KEY;
    static const std::string ENGINE_FONT_FILE_KEY;
    static const std::string DEFAULT_FULLSCREEN_KEY;
    static const std::string GAME_WINDOW_TITLE_KEY;
    static const std::string ENGINE_GRAVITY_KEY;
    static const std::string ENGINE_SPEED_KEY;
    static const std::string MAX_FPS_KEY;
    static const std::string RENDER_DEBUG_GRID_KEY;
    static const std::string RENDER_FPS_KEY;
    static const std::string WINDOWED_RESOLUTION_X_KEY;
    static const std::string WINDOWED_RESOLUTION_Y_KEY;
    static const std::string ENGINE_PROJECTILE_SPEED_KEY;
    static const std::string ENGINE_PHYSICS_ENABLED_KEY;
    static const std::string RENDER_BACKGROUND_PARTICLE_COUNT_KEY;
    static const std::string RENDER_PARTICLES_ENABLED_KEY;
    static const std::string RENDER_TANK_SCALE_KEY;
    static const std::string RENDER_DEBUG_MENU_FORCE_KEY;
    static const std::string RENDER_MENU_BACKGROUND_FILE_KEY;
    static const std::string ENGINE_DEFAULT_START_PLAYERS_KEY;
    // Add other keys as needed

    static const bool DEFAULT_VERBOSE_MOUSE_DEBUGGING;
    static const int DEFAULT_VERBOSE_DEBUGGING;
    static const bool DEFAULT_RENDER_DEBUG_HITBOXES;
    static const std::string DEFAULT_ENGINE_FONT_FILE;
    static const bool DEFAULT_DEFAULT_FULLSCREEN;
    static const std::string DEFAULT_GAME_WINDOW_TITLE;
    static const float DEFAULT_ENGINE_GRAVITY;
    static const float DEFAULT_ENGINE_SPEED;
    static const int DEFAULT_MAX_FPS;
    static const bool DEFAULT_RENDER_DEBUG_GRID;
    static const bool DEFAULT_RENDER_FPS;
    static const int DEFAULT_WINDOWED_RESOLUTION_X;
    static const int DEFAULT_WINDOWED_RESOLUTION_Y;
    static const float DEFAULT_ENGINE_PROJECTILE_SPEED;
    static const bool DEFAULT_ENGINE_PHYSICS_ENABLED;
    static const int DEFAULT_RENDER_BACKGROUND_PARTICLE_COUNT;
    static const bool DEFAULT_RENDER_PARTICLES_ENABLED;
    static const float DEFAULT_RENDER_TANK_SCALE;
    static const bool DEFAULT_RENDER_DEBUG_MENU_FORCE;
    static const std::string DEFAULT_RENDER_MENU_BACKGROUND_FILE;
    static const int DEFAULT_ENGINE_DEFAULT_START_PLAYERS;
};
