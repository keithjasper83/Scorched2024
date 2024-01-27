#pragma once

#include "0ProjectileDefaults.h"
#include "ProjectileInterface.h"
#include "Tank.h"
#include <functional>
#include <map>
#include <memory>

enum class ProjectileType
{
    DefaultProjectile,
    CannonProjectile,
    // Add other projectile types as needed
};

class ProjectileFactory
{
  public:
    using ProjectileCreator = std::function<std::unique_ptr<ProjectileInterface>(const tank &)>;

    static void RegisterProjectileType(const std::string &typeName, ProjectileCreator creator)
    {
        projectileTypeRegistry[typeName] = creator;
    }
    static std::unique_ptr<ProjectileInterface> CreateProjectile(const std::string &typeName, const tank &tank)
    {
        auto it = projectileTypeRegistry.find(typeName);
        if (it != projectileTypeRegistry.end())
        {
            return (it->second)(tank);
        }
        return nullptr; // Return nullptr if the specified type is not registered
    }

    static std::string projectileTypeToString(ProjectileType type)
    {
        switch (type)
        {
        case ProjectileType::DefaultProjectile:
            return "DefaultProjectile";
            break;
        case ProjectileType::CannonProjectile:
            return "CannonProjectile";
            break;
            // Add cases for other projectile types
        default:
            return "DefaultProjectile";
            break;
        }
    }

  private:
    static std::map<std::string, ProjectileCreator> projectileTypeRegistry;
};
