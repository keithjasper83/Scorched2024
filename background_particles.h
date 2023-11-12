#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>

#include "Config.h"

class background_particles
{
  public:
    sf::RenderTexture particle_texture;
    std::vector<sf::Vector2f> particle_vector;

    bool update_left = false;
    int screen_width = constants::screen_width;
    int screen_height = constants::screen_height;

    background_particles();
    sf::Sprite render();
    void set_screen_dimensions(const int width, const int height);
    // todo implemnt below functions to update particle locations. Perhaps move the vector to the renderer class and
    // update locations by reference.
    void update();
    void create(const int quantity = 100);
    void destroy();
    void remove_out_of_bounds_particles();
};
