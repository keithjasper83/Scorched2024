#pragma once
#include "debug_output.h"
#include "tank.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>

class ProjectileInterface
{
  public:
    explicit ProjectileInterface(const tank &tank = tank{}); // Declaration with default argument
    virtual ~ProjectileInterface() = default;

    bool is_active() const
    {
        return active_;
    }

    virtual void launch(float initial_x, float initial_y, float launch_angle, float tank_power)
    {
        // Implementation of the launch function
        // ...
        set_start_x(initial_x);
        set_start_y(initial_y);
        // Set initial velocity based on launch angle and tank power
        float power_factor = tank_power * ConfigJSON::getEngineProjectileSpeed();
        float corrected_angle = launch_angle + 90.0f; // Adjust the angle here

        initial_velocity_x_ = std::cos(corrected_angle * (M_PI / 180.0f)) * power_factor;
        initial_velocity_y_ =
            std::sin(corrected_angle * (M_PI / 180.0f)) * power_factor; // Negative for correct y direction
        set_velocity(initial_velocity_x_, initial_velocity_y_);
    }

    virtual void set_active(bool active)
    {
        active_ = active;
    }

    int get_width() const
    {
        return width_;
    }
    int get_height() const
    {
        return height_;
    }
    virtual int get_damage() const
    {
        return damage_;
    }
    float get_velocity_x() const
    {
        return velocity_x_;
    }
    float get_velocity_y() const
    {
        return velocity_y_;
    }
    float get_explosion_duration() const
    {
        return explosion_duration_;
    }
    float get_explosion_timer() const
    {
        return explosion_timer_;
    }
    float get_start_x() const
    {
        return start_x_;
    }
    float get_start_y() const
    {
        return start_y_;
    }

    void set_start_x(const float start_x)
    {
        start_x_ = start_x;
    }

    void set_start_y(const float start_y)
    {
        start_y_ = start_y;
    }
    virtual float get_x() const
    {
        return x_;
    }
    virtual float get_y() const
    {
        return y_;
    }
    bool get_exploded() const
    {
        return exploded_;
    }
    void set_exploded(bool exploded)
    {
        exploded_ = exploded;
    }
    void set_explosion_timer(const float explosion_timer)
    {
        explosion_timer_ = explosion_timer;
    }
    void set_explosion_duration(const float explosion_duration)
    {
        explosion_duration_ = explosion_duration;
    }

    virtual void update_position(const float time_elapsed, const float gravity, const sf::RenderWindow &renderTarget)
    {
        if (x_ == 0 && y_ == 0)
        {
            x_ = start_x_;
            y_ = start_y_;
        }

        if (is_active())
        {
            // Update projectile position based on velocity, time elapsed, and gravity
            // Example code (modify as needed):
            velocity_y_ += gravity * time_elapsed;

            x_ += velocity_x_ * time_elapsed;
            y_ += velocity_y_ * time_elapsed - 0.5f * gravity * time_elapsed * time_elapsed;

            // Check for collision with the ground or window boundaries

            sf::Vector2u windowSize = renderTarget.getSize();

            if (y_ >= windowSize.y || x_ < 0 || x_ > windowSize.x)
            {
                set_active(false);
                KJ::debug_output::print(__FILE__, "ProjectileInterface::update_position() - Projectile out of bounds",
                                        KJ::debug_output::MessageType::ERROR);
            }
        }

        // Update explosion timer if necessary
        if (get_exploded())
        {
            set_explosion_timer(get_explosion_timer() + time_elapsed);
            if (get_explosion_timer() >= get_explosion_duration())
            {
                set_active(false); // Deactivate the projectile after explosion duration
            }
        }
    }

    bool is_visible() const
    {
        return (x_ >= 0 && x_ <= ConfigJSON::getWindowedResolutionX() && y_ >= 0 &&
                y_ <= ConfigJSON::getWindowedResolutionY());
    }

    bool is_visible(int x, int y) const
    {
        return (x_ >= 0 && x_ <= x && y_ >= 0 && y_ <= y);
    }

    virtual const sf::Drawable *get_render_property() const
    {
        // Default rendering properties (can be overridden by derived classes)
        sf::CircleShape *projectile_shape = new sf::CircleShape(get_width());
        projectile_shape->setFillColor(sf::Color::Cyan);
        projectile_shape->setPosition(get_x(), get_y());
        projectile_shape->setOrigin(get_width() / 2.0f, get_height() / 2.0f);
        projectile_shape->setRotation(std::atan2(get_velocity_y(), get_velocity_x()) * (180.0f / M_PI));
        projectile_shape->setScale(1.0f, 1.0f);
        projectile_shape->setOutlineThickness(1.0f);
        projectile_shape->setOutlineColor(sf::Color::Black);
        projectile_shape->setPointCount(360);
        projectile_shape->setFillColor(sf::Color::Yellow);
        return projectile_shape;
    }

  protected:
    int width_ = 5;
    int height_ = 5;
    int damage_ = 1;
    int start_x_ = 200;
    int start_y_ = 200;
    float initial_velocity_x_ = 1.0f;
    float initial_velocity_y_ = 1.0f;
    float velocity_x_ = 1.0f;
    float velocity_y_ = 1.0f;
    float x_ = 0;
    float y_ = 0;
    float explosion_duration_ = 20.0f;
    float explosion_timer_ = 0;
    bool active_ = false;
    bool exploded_ = false;
    tank tank_;

    void set_velocity(float vx, float vy)
    {
        velocity_x_ = vx;
        velocity_y_ = vy;
    }
};

inline ProjectileInterface::ProjectileInterface(const tank &tank) : tank_{tank}
{
    // Initialize other members if needed
}
