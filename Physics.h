#pragma once
#include "SFML/Graphics.hpp"
#include "Tank.h"
#include "TerrainGenerator.h"
#include <vector>

class Physics {
public:
    Physics();
    Physics(float gravity);

    float getGravity();

    void applyGravity(std::vector<Tank>& tanks, TerrainGenerator& terrain, sf::Time deltaTime);
private:
    float gravity;
};