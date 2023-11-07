#pragma once

#include "ProjectileCannon.h"

projectile::projectile(const tank &tank)
{
    this->player = tank;
    set_explosion_duration(1.0f);
}

bool projectile::is_active() const
{
    return active;
}

float projectile::calculate_launch_angle(const float turret_angle)
{
    const float flipped_angle = 180.0f - turret_angle;
    return flipped_angle;
}

void projectile::set_active(bool active)
{
    this->active = active;
}

void projectile::launch(const float initial_x, const float initial_y, float launch_angle)
{
    this->x = initial_x;
    this->y = initial_y;
    launch_angle = calculate_launch_angle(launch_angle);
    this->active = true;

    // Calculate initial velocity components based on the turret's angle
    const float angle_in_radians = (-launch_angle) * (M_PI / 180.0f);
    const float initial_velocity = get_initial_velocity();
    velocity_x = initial_velocity * std::cos(angle_in_radians) * constants::speed;
    velocity_y = initial_velocity * std::sin(angle_in_radians) * constants::speed;
}

void projectile::update_position(const float time_elapsed, const float gravity)
{
    if (active)
    {
        // Update the projectile's position based on its current state
        const float new_x = x + velocity_x * time_elapsed;
        const float new_y = y + velocity_y * time_elapsed;
        // Update velocity with gravity
        velocity_y += gravity * time_elapsed;

        // Set the new position
        x = new_x;
        y = new_y;
    }
}

float projectile::get_initial_velocity()
{
    printf("Projectile.cpp - Player: %s getInitialVelocity: %f\n", player.get_player_name().c_str(),
           player.get_power());
    // exit(0);
    return player.get_power();
}

float projectile::get_x() const
{
    return x;
}

float projectile::get_y() const
{
    return y;
}

float projectile::get_height() const
{
    return this->height;
}

float projectile::get_width() const
{
    return this->width;
}

// Set the explosion duration
void projectile::set_explosion_duration(const float duration)
{
    explosion_duration_ = duration;
}

// Get the explosion duration
float projectile::get_explosion_duration() const
{
    return explosion_duration_;
}

float projectile::get_explosion_timer() const
{
    return explosion_timer_;
}

// Update the explosion timer
void projectile::update_explosion_timer(const float delta_time)
{
    if (exploded_)
    {
        explosion_timer_ += delta_time;

        // If the explosion timer exceeds the explosion duration, mark the projectile as inactive
        if (explosion_timer_ >= explosion_duration_)
        {
            set_active(false);
        }
    }
}

float projectile::get_start_y() const
{
    return this->start_y;
}

float projectile::get_start_x() const
{
    return this->start_x;
}

void projectile::set_start_y(const float start_y)
{
    this->start_y = start_y;
}

void projectile::set_start_x(const float start_x)
{
    this->start_x = start_x;
}
