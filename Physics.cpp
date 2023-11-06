#pragma once

#include "Physics.h"


Physics::Physics()
{
    this->gravity = constants::GRAVITY;
}
Physics::Physics(float gravity) : gravity(gravity)
{
    // Constructor logic, if any
    this->gravity = gravity;
}

float Physics::getGravity()
{
    return this->gravity;
}

void Physics::applyGravity(std::vector<Tank>& tanks, TerrainGenerator& terrain, sf::Time deltaTime)
{
    for (Tank& tank : tanks)
    {
        if (!tank.getOnGround())
        {
            float pixelsPerSecond = 10.0f; // Adjust this value to control the falling
            terrain.updateScale();
            float scale = terrain.getScale().y;
            float xScale = terrain.getScale().x;
            float groundHeight = terrain.getFirstNonTransparentPixelinX(static_cast<int>(tank.getX() / xScale));
            float scaledGroundHeight = groundHeight * scale;
            float oldLocation = tank.getY();
            float newLocation = oldLocation + pixelsPerSecond;
            if (scaledGroundHeight <= oldLocation)
            {
                tank.setOnGround(true);
            }
            else
            {
                tank.setLocation(tank.getX(), newLocation);
                tank.setOnGround(false);
            }
        }
    }
}