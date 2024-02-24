#pragma once

#include "0ProjectileDefaults.h"
#include "ProjectileInterface.h"
#include "Tank.h"
#include <functional>
#include <map>
#include <memory>

namespace ProjectileTypes
{
enum class ProjectileType
{
    DefaultProjectile,
    CannonProjectile,
    Count = 2,
    // Add other projectile types as needed
};
} // namespace ProjectileTypes

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

    static std::string projectileTypeToString(ProjectileTypes::ProjectileType type)
    {
        switch (type)
        {
        case ProjectileTypes::ProjectileType::DefaultProjectile:
            return "DefaultProjectile";
            break;
        case ProjectileTypes::ProjectileType::CannonProjectile:
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
