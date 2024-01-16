#pragma once

#include "0ProjectileDefaults.h"

enum class ProjectileType
{
    Default,
    Cannon,
    // Add more projectile types as needed
};

class ProjectileFactory
{
  public:
    static std::unique_ptr<ProjectileInterface> createProjectile(ProjectileType type, const tank &tank);
};
