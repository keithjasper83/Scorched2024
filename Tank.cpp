#pragma once
#include "Tank.h"

Tank::Tank()
{
    // Constructor logic, if any
}

Tank::Tank(float x, float y, int index, std::string playerName)
{
    location.x = x;
    location.y = y;
    playerInfo.playerName = playerName;
    playerInfo.index = index;
}

void Tank::setOnGround(bool onGround)
{
    this->onGround = onGround;
}

bool Tank::getOnGround()
{
    return this->onGround;
}

void Tank::setAngle(float angle)
{
    printf("Tank.cpp - Player: %s setAngle: %f\n", getPlayerName().c_str(), angle);
    turret.angle = angle;
}

void Tank::incAngle()
{
    turret.angle++;
}

void Tank::decAngle()
{
    turret.angle--;
}

void Tank::incPower()
{
    playerInfo.startPower++;
}

void Tank::decPower()
{
    playerInfo.startPower--;
}

float Tank::getPower()
{
    if (playerInfo.startPower > playerInfo.maxPower)
    {
        playerInfo.startPower = playerInfo.maxPower;
    }
    else if (playerInfo.startPower < playerInfo.minPower)
    {
        playerInfo.startPower = playerInfo.minPower;
    }
    return playerInfo.startPower;
}

float Tank::getAngle()
{
    normalizeAngle();
    return turret.angle;
}

void Tank::normalizeAngle()
{
    if (turret.angle > 360)
    {
        turret.angle = 0;
    }
    if (turret.angle < 0)
    {
        turret.angle = 360;
    }
}

void Tank::setLocation(float x, float y)
{
    // printf("Tank.cpp - Player: %s setLocation: %f, %f\n", getPlayerName().c_str(), x, y);
    location.x = x;
    location.y = y;
}

float Tank::getX()
{
    return location.x;
}

float Tank::getY()
{
    return location.y;
}

void Tank::Fire()
{
    printf("Tank.cpp - FIRE!\n");
    // Create a new projectile and add it to the container

    // Now, you can tell the renderer to draw the projectiles
    // renderer.drawProjectiles(projectiles);
}

float Tank::getOriginX()
{
    return tankSize.bodyX / 2.0f;
}

float Tank::getOriginY()
{
    return tankSize.bodyY / 2.0f;
}

float Tank::getBodyX()
{
    return tankSize.bodyX;
}

float Tank::getBodyY()
{
    return tankSize.bodyY;
}

float Tank::getTurretX()
{
    return tankSize.bodyX / 2.0f;
}

float Tank::getTurretY()
{
    return -tankSize.bodyY;
}

float Tank::getTurretWidth()
{
    return turret.width;
}

float Tank::getTurretHeight()
{
    return turret.height;
}

std::string Tank::getPlayerName()
{
    return playerInfo.playerName;
}

int Tank::getIndex()
{
    return playerInfo.index;
}

int Tank::getHealth()
{
    return playerInfo.health;
}

int Tank::getAmmo()
{
    return playerInfo.ammo;
}

std::string Tank::getWeaponName()
{
    return playerInfo.weapon;
}
