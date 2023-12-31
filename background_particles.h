#pragma once
#include "Config.h"
#include "animated_sprite.h"
#include "window_manager.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>

class background_particles
{
  public:
    sf::RenderTexture particle_texture;
    std::vector<sf::Vector3f> particle_vector;

    sf::Texture texture;
    int screen_width;
    int screen_height;
    bool update_left = false;
    sf::Sprite sprite;
    animated_sprite animation;
    background_particles();
    sf::Sprite render();
    void set_screen_dimensions(const int width, const int height);
    void update();
    void create(const int quantity = 100);
    void destroy();
    void remove_out_of_bounds_particles();
};
