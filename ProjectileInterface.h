#pragma once
#include "Tank.h"

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
        velocity_x_ = std::cos(launch_angle * (M_PI / 180.0f)) * power_factor;
        velocity_y_ = std::sin(launch_angle * (M_PI / 180.0f)) * power_factor; // Negative for correct y direction
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

    // Assuming this is part of the ProjectileInterface class declaration
    virtual void update_position(const float time_elapsed, const float gravity)
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
            y_ -= velocity_y_ * time_elapsed - 0.5f * gravity;

            if (x_ < 0 || x_ > ConfigJSON::getWindowedResolutionX() || y_ < 0 ||
                y_ > ConfigJSON::getWindowedResolutionY())
            {
                set_active(false);
                KJ::debug_output::print(__FILE__, "ProjectileInterface::update_position() - Projectile out of bounds",
                                        KJ::debug_output::MessageType::ERROR);
                x_ = 0;
                y_ = 0;
            }
            std::cout << "New Projectile position: " << x_ << ", " << y_ << "\n";
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
    }

  protected:
    int width_ = 5;
    int height_ = 5;
    int damage_ = 1;
    int start_x_ = 200;
    int start_y_ = 200;
    float velocity_x_ = 1.0f;
    float velocity_y_ = 1.0f;
    float x_ = 0;
    float y_ = 0;
    float explosion_duration_ = 20.0f;
    float explosion_timer_ = 0;
    bool active_ = false;
    bool exploded_ = false;
    tank tank_;
    // Add more pure virtual functions for common projectile features
};

inline ProjectileInterface::ProjectileInterface(const tank &tank) : tank_{tank}
{
    // Initialize other members if needed
}
