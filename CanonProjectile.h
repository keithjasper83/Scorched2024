#pragma once
#include "Projectile.h"
#include "ProjectileFactory.h"
#include "Tank.h"

// Concrete projectile types
class CannonProjectile : public Projectile
{
  public:
    void launch(float initial_x, float initial_y, float launch_angle) override
    {
        // Implement the launch method for the cannon projectile
        // ...
    }
    // Other methods and members specific to CannonProjectile
};

class CannonProjectileFactory : public ProjectileFactory
{
  public:
    std::unique_ptr<Projectile> createProjectile(const tank &tank) override
    {
        return std::make_unique<CannonProjectile>(tank);
    }
};
