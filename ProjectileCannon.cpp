#include "Tank.cpp"
#include <corecrt_math_defines.h>
#include <cmath>
#include "SFML/Graphics.hpp"
#include "Config.h"

class Projectile {
public:
    Tank player;
    bool active = false;
    float x = 0.0f;
    float y = 0.0f;
    float angle = 0.0f;
    float velocityX = 0.0f;
    float velocityY = 0.0f;
    float width = 5.0f;
    float height = 5.0f;

    Projectile(Tank& tank) {
        this->player = tank;
        setExplosionDuration(1.0f);
    }

    bool isActive() {
		return active;
	}

    float calculateLaunchAngle(float turretAngle) {
        float flippedAngle = 180.0f - turretAngle;
        return flippedAngle;
    }

    void setActive(bool active) {
		this->active = active;
	}

    void launch(float initialX, float initialY, float launchAngle) {
        this->x = initialX;
        this->y = initialY;
        launchAngle = calculateLaunchAngle(launchAngle);
        this->active = true;

        // Calculate initial velocity components based on the turret's angle
        float angleInRadians = (-launchAngle) * (M_PI / 180.0f);
        float initialVelocity = getInitialVelocity();
        velocityX = initialVelocity * std::cos(angleInRadians) * constants::SPEED;
        velocityY = initialVelocity * std::sin(angleInRadians) * constants::SPEED;
    }


    void updatePosition(float timeElapsed, float gravity) {
        if (active) {
            // Update the projectile's position based on its current state
            float newX = x + velocityX * timeElapsed;
            float newY = y + velocityY * timeElapsed;
            // Update velocity with gravity
            velocityY += gravity * timeElapsed;

            // Set the new position
            x = newX;
            y = newY;
        }
    }

    float getInitialVelocity() {
        printf("Projectile.cpp - Player: %s getInitialVelocity: %f\n", player.getPlayerName().c_str(), player.getPower());
        //exit(0);
        return player.getPower();
    }

    float getX() {
        return x;
	}
    float getY() {
		return y;
    }
    float getHeight() {
        return this->height;
    }
    float getWidth() {
		return this->width;
	}


    // Other member functions...


    // Set the explosion duration
    void setExplosionDuration(float duration) {
        explosionDuration = duration;
    }

    // Get the explosion duration
    float getExplosionDuration() const {
        return explosionDuration;
    }

    float getExplosionTimer() const {
		return explosionTimer;
	}

    // Update the explosion timer
    void updateExplosionTimer(float deltaTime) {
        if (exploded) {
            explosionTimer += deltaTime;

            // If the explosion timer exceeds the explosion duration, mark the projectile as inactive
            if (explosionTimer >= explosionDuration) {
                setActive(false);
            }
        }
    }



private:
    // ...
    bool exploded;
    // Add these two members
    float explosionDuration;
    float explosionTimer;


};

#include <SFML/Graphics.hpp>

class Explosion {
public:
    Explosion(float x, float y, float size, sf::Time duration) {
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

    void update(sf::Time deltaTime) {
        // Update the explosion duration (you can modify other properties here)
        explosionDuration -= deltaTime;

        if (explosionDuration <= sf::Time::Zero) {
            active = false; // Deactivate the explosion when it's done
        }
        else {
            //printf("Projectile.cpp - Explosion::update - explosionDuration: %f\n", explosionDuration.asSeconds());
        }
    }


    bool isActive() const {
        return active;
    }

    void render(sf::RenderWindow& window) const {
        if (active) {
            //printf("Projectile.cpp - Explosion::render - active: %d\n", active);
            //printf("Projectile.cpp - Explosion::render - position.x: %f\n", position.x);
            //printf("Projectile.cpp - Explosion::render - position.y: %f\n", position.y);
            window.draw(explosionShape);
        }
        else {
            //printf("Projectile.cpp - Explosion::render - active: %d\n", active);
        }
    }

    sf::Vector2f getPosition() const {
		return this->position;
	}

    float getExplosionSize() const {
        return this->explosionSize;
    }

private:
    sf::Vector2f position;
    float explosionSize;
    sf::Time explosionDuration;
    bool active;
    sf::CircleShape explosionShape; // Used for rendering
};
