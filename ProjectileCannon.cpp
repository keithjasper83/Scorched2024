#pragma once

#include "ProjectileCannon.h"


Projectile::Projectile(Tank& tank)
{
    this->player = tank;
    setExplosionDuration(1.0f);
}

bool Projectile::isActive()
{
    return active;
}

float Projectile::calculateLaunchAngle(float turretAngle)
{
    float flippedAngle = 180.0f - turretAngle;
    return flippedAngle;
}

void Projectile::setActive(bool active)
{
    this->active = active;
}

void Projectile::launch(float initialX, float initialY, float launchAngle)
{
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

void Projectile::updatePosition(float timeElapsed, float gravity)
{
    if (active)
    {
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

float Projectile::getInitialVelocity()
{
    printf("Projectile.cpp - Player: %s getInitialVelocity: %f\n", player.getPlayerName().c_str(),
        player.getPower());
    // exit(0);
    return player.getPower();
}

float Projectile::getX()
{
    return x;
}

float Projectile::getY()
{
    return y;
}

float Projectile::getHeight()
{
    return this->height;
}

float Projectile::getWidth()
{
    return this->width;
}

    // Set the explosion duration
void Projectile::setExplosionDuration(float duration)
{
    explosionDuration = duration;
}

    // Get the explosion duration
float Projectile::getExplosionDuration() const
{
    return explosionDuration;
}

float Projectile::getExplosionTimer() const
{
    return explosionTimer;
}

    // Update the explosion timer
void Projectile::updateExplosionTimer(float deltaTime)
{
    if (exploded)
    {
        explosionTimer += deltaTime;

        // If the explosion timer exceeds the explosion duration, mark the projectile as inactive
        if (explosionTimer >= explosionDuration)
        {
            setActive(false);
        }
    }
}

float Projectile::getStartY() const
{
    return this->startY;
}

float Projectile::getStartX() const
{
    return this->startX;
}

void Projectile::setStartY(float startY)
{
    this->startY = startY;
}

void Projectile::setStartX(float startX)
{
    this->startX = startX;
}