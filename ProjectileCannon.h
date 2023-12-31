#pragma once

#include "Config.h"
#include "Tank.h"
#include "debug_output.h"
#include <cmath>
#include <corecrt_math_defines.h>

class projectile
{
  public:
    tank player;
    bool active = false;
    float x = 0.0f;
    float y = 0.0f;
    float angle = 0.0f;
    float velocity_x = 0.0f;
    float velocity_y = 0.0f;
    float width = 5.0f;
    float height = 5.0f;
    float start_x = 0.0f;
    float start_y = 0.0f;

    explicit projectile(const tank &tank);
    bool is_active() const;
    static float calculate_launch_angle(float turret_angle);
    void set_active(bool active);
    void launch(float initial_x, float initial_y, float launch_angle);
    void update_position(float time_elapsed, float gravity);
    float get_initial_velocity();
    float get_x() const;
    float get_y() const;
    float get_height() const;
    float get_width() const;
    void set_explosion_duration(float duration);
    float get_explosion_duration() const;
    float get_explosion_timer() const;
    void update_explosion_timer(float delta_time);
    float get_start_y() const;
    float get_start_x() const;
    void set_start_y(float start_y);
    void set_start_x(float start_x);
    int get_damage() const;

  private:
    // ...
    bool exploded_;
    // Add these two members
    float explosion_duration_;
    float explosion_timer_;

    int damage = 110;
};
