#include "ProjectileFactory.h"
// Include headers for other projectile types as needed

std::unique_ptr<ProjectileInterface> ProjectileFactory::createProjectile(ProjectileType type, const tank &tank)
{
    switch (type)
    {
    case ProjectileType::Default:
        return std::make_unique<DefaultProjectile>(tank);
    // Add cases for other projectile types
    case ProjectileType::Cannon:
        return std::make_unique<CannonProjectile>(tank);
    default:
        return nullptr; // Handle unknown projectile types
    }
}
