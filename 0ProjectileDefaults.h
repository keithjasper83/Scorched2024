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

    const sf::Drawable *get_render_property() const override
    {
        // Default rendering properties (can be overridden by derived classes)
        sf::CircleShape *projectile_shape = new sf::CircleShape(10.0f);
        projectile_shape->setFillColor(sf::Color::Cyan);
        projectile_shape->setPosition(get_x(), get_y());
        projectile_shape->setOrigin(get_width() / 2.0f, get_height() / 2.0f);
        projectile_shape->setRotation(std::atan2(get_velocity_y(), get_velocity_x()) * (180.0f / M_PI));
        projectile_shape->setScale(1.0f, 1.0f);
        projectile_shape->setOutlineThickness(1.0f);
        projectile_shape->setOutlineColor(sf::Color::Black);
        projectile_shape->setPointCount(360);
        projectile_shape->setFillColor(sf::Color::Red);
        return projectile_shape;
    }

    // Add more specific functions and members for the default projectile
  private:
    int damage_ = 10;
    // Add members specific to the default projectile
};
