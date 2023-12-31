#pragma once
#include "debug_output.h" 
#include "SFML/Graphics.hpp"
#include "Tank.h"
#include "TerrainGenerator.h"
#include <vector>

class physics {
public:
    physics();
    explicit physics(float gravity);

    float get_gravity() const;

    static void apply_gravity(std::vector<tank>& tanks, terrain_generator& terrain);
private:
    float gravity_;
};