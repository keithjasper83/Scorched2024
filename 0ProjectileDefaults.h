#pragma once

#include "Tank.h"

class ProjectileInterface
{
  public:
    virtual ~ProjectileInterface() = default;

    virtual bool is_active() const = 0;
    virtual void launch(const float initial_x, const float initial_y, float launch_angle) = 0;
    virtual void update_position(const float time_elapsed, const float gravity) = 0;
    virtual float get_x() const = 0;
    virtual float get_y() const = 0;
    virtual void set_active(bool active) = 0;
    virtual int get_damage() const = 0;

    int get_width() const
    {
        return width_;
    }
    int get_height() const
    {
        return height_;
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

  private:
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
    // Add more pure virtual functions for common projectile features
};

class DefaultProjectile : public ProjectileInterface
{
  public:
    DefaultProjectile(const tank &tank);

    // Implement virtual functions from ProjectileInterface
    bool is_active() const override;
    void launch(const float initial_x, const float initial_y, float launch_angle) override;
    void update_position(const float time_elapsed, const float gravity) override;
    float get_x() const override;
    float get_y() const override;
    void set_active(bool active) override;
    int get_damage() const override;

    // Add more specific functions and members for the default projectile
  private:
    // Add members specific to the default projectile
};

class CannonProjectile : public ProjectileInterface
{
  public:
    CannonProjectile(const tank &tank);

    // Implement virtual functions from ProjectileInterface
    bool is_active() const override;
    void launch(const float initial_x, const float initial_y, float launch_angle) override;
    void update_position(const float time_elapsed, const float gravity) override;
    float get_x() const override;
    float get_y() const override;
    void set_active(bool active) override;
    int get_damage() const override;

    // Add more specific functions and members for the default projectile
  private:
    // Add members specific to the default projectile
};

// class Projectile
//{
//   private:
//     float x, y;                   // Current position
//     float velocity_x, velocity_y; // Velocity components
//     float height, width;          // Height and width of the projectile
//     float explosion_duration_;
//     float explosion_timer_;
//     float start_x, start_y;
//     int damage;
//     bool active = false;
//     bool exploded_ = false;
//     tank player;
//
//     float calculate_launch_angle(const float turret_angle);
//
//   public:
//     Projectile(const tank &tank);
//
//     bool is_active() const;
//
//     void set_active(bool active);
//
//     void launch(const float initial_x, const float initial_y, float launch_angle);
//
//     void update_position(const float time_elapsed, const float gravity);
//
//     float get_initial_velocity();
//
//     float get_x() const;
//
//     float get_y() const;
//
//     float get_height() const;
//
//     float get_width() const;
//
//     void set_explosion_duration(const float duration);
//
//     float get_explosion_duration() const;
//
//     float get_explosion_timer() const;
//
//     void update_explosion_timer(const float delta_time);
//
//     float get_start_y() const;
//
//     float get_start_x() const;
//
//     void set_start_y(const float start_y);
//
//     void set_start_x(const float start_x);
//
//     int get_damage() const;
// };
