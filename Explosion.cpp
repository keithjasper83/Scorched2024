#include "Explosion.h"


Explosion::Explosion(float x, float y, float size, sf::Time duration)
    {
        position.x = x;
        position.y = y;
        explosionSize = size;
        explosionDuration = duration;
        active = true;

        // Customize the appearance of the explosion (you can set color, texture, etc.)
        explosionShape.setRadius(explosionSize);
        explosionShape.setFillColor(sf::Color::Blue);
        explosionShape.setOrigin(explosionSize, explosionSize); // Set the origin to the center
        explosionShape.setPosition(position);
    }

    void Explosion::update(sf::Time deltaTime)
    {
        // Update the explosion duration (you can modify other properties here)
        explosionDuration -= deltaTime;

        if (explosionDuration <= sf::Time::Zero)
        {
            active = false; // Deactivate the explosion when it's done
        }
        else
        {
            // printf("Projectile.cpp - Explosion::update - explosionDuration: %f\n", explosionDuration.asSeconds());
        }
    }

    bool Explosion::isActive() const
    {
        return active;
    }

    void Explosion::render(sf::RenderWindow& window) const
    {
        if (active)
        {
            // printf("Projectile.cpp - Explosion::render - active: %d\n", active);
            // printf("Projectile.cpp - Explosion::render - position.x: %f\n", position.x);
            // printf("Projectile.cpp - Explosion::render - position.y: %f\n", position.y);
            window.draw(explosionShape);
        }
        else
        {
            // printf("Projectile.cpp - Explosion::render - active: %d\n", active);
        }
    }

    sf::Vector2f Explosion::getPosition() const
    {
        return this->position;
    }

    float Explosion::getExplosionSize() const
    {
        return this->explosionSize;
    }


