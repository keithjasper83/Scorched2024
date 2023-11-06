#pragma once
#include "Tank.h"
#include <cmath>
#include <corecrt_math_defines.h>
#include "Config.h"

class Projectile
{
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
    float startX = 0.0f;
    float startY = 0.0f;

    Projectile(Tank& tank);
    bool isActive();
    float calculateLaunchAngle(float turretAngle);
    void setActive(bool active);
    void launch(float initialX, float initialY, float launchAngle);
    void updatePosition(float timeElapsed, float gravity);
    float getInitialVelocity();
    float getX();
    float getY();
    float getHeight();
    float getWidth();
    void setExplosionDuration(float duration);
    float getExplosionDuration() const;
    float getExplosionTimer() const;
    void updateExplosionTimer(float deltaTime);
    float getStartY() const;
    float getStartX() const;
    void setStartY(float startY);
    void setStartX(float startX);

private:
    // ...
    bool exploded;
    // Add these two members
    float explosionDuration;
    float explosionTimer;
};