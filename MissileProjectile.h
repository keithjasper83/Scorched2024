#pragma once
#include "Projectile.h"
#include "ProjectileFactory.h"
#include "Tank.h"

class MissileProjectile : public Projectile
{
  public:
    void launch(float initial_x, float initial_y, float launch_angle) override
    {
        // Implement the launch method for the missile projectile
        // ...
    }
    // Other methods and members specific to MissileProjectile
};

class MissileProjectileFactory : public ProjectileFactory
{
  public:
    std::unique_ptr<Projectile> createProjectile(const tank &tank) override
    {
        return std::make_unique<MissileProjectile>(tank);
    }
};
