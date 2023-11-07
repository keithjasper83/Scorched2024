#include "Explosion.h"

explosion::explosion(const float x, const float y, const float size, const sf::Time duration)
{
    position_.x = x;
    position_.y = y;
    explosion_size_ = size;
    explosion_duration_ = duration;
    active_ = true;

    // Customize the appearance of the explosion (you can set color, texture, etc.)
    explosion_shape_.setRadius(explosion_size_);
    explosion_shape_.setFillColor(sf::Color::Blue);
    explosion_shape_.setOrigin(explosion_size_, explosion_size_); // Set the origin to the center
    explosion_shape_.setPosition(position_);
}

void explosion::update(const sf::Time delta_time)
{
    // Update the explosion duration (you can modify other properties here)
    explosion_duration_ -= delta_time;

    if (explosion_duration_ <= sf::Time::Zero)
    {
        active_ = false; // Deactivate the explosion when it's done
    }
    else
    {
        // printf("Projectile.cpp - Explosion::update - explosionDuration: %f\n", explosionDuration.asSeconds());
    }
}

bool explosion::is_active() const
{
    return active_;
}

void explosion::render(sf::RenderWindow &window) const
{
    if (active_)
    {
        // printf("Projectile.cpp - Explosion::render - active: %d\n", active);
        // printf("Projectile.cpp - Explosion::render - position.x: %f\n", position.x);
        // printf("Projectile.cpp - Explosion::render - position.y: %f\n", position.y);
        window.draw(explosion_shape_);
    }
    else
    {
        // printf("Projectile.cpp - Explosion::render - active: %d\n", active);
    }
}

sf::Vector2f explosion::get_position() const
{
    return this->position_;
}

float explosion::get_explosion_size() const
{
    return this->explosion_size_;
}
