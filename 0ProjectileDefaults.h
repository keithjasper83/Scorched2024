#pragma once

#include "ProjectileFactory.h"
#include "ProjectileInterface.h"
#include "Tank.h"

class DefaultProjectile : public ProjectileInterface
{
  public:
    DefaultProjectile(const tank &tank) : ProjectileInterface(tank){};
    // Implement virtual functions from ProjectileInterface
    int get_damage() const override;

    // Add more specific functions and members for the default projectile
  private:
    int damage_ = 1;
    // Add members specific to the default projectile
};

class CannonProjectile : public ProjectileInterface
{
  public:
    CannonProjectile(const tank &tank) : ProjectileInterface(tank){};

    // Implement virtual functions from ProjectileInterface
    int get_damage() const override;

    // Add more specific functions and members for the default projectile
  private:
    int damage_ = 2;
    // Add members specific to the default projectile
};
