#pragma once
#include "Config.h"
#include "debug_output.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

class terrain_generator
{
  public:
    struct scale
    {
        float x;
        float y;
    };

    sf::RenderWindow renderer;
    sf::Texture terrain_texture;
    sf::Sprite terrain_sprite;
    sf::Sprite terrain_sprite_master;
    sf::Image terrain_image;
    sf::Image terrain_image_master;
    scale window_scale;
    bool update_pixel_array_flag = false;

    terrain_generator();
    terrain_generator(int width, int height);
    void reset_terrain();
    void set_screen_size(int width, int height);
    void update_sprite_size();
    sf::Image get_terrain_image() const;
    int get_first_non_transparent_pixel_in_x(int x) const;
    void update_terrain_with_collision(int pos_x, int pos_y);
    float get_screen_width() const;
    float get_screen_height() const;
    scale get_scale() const;
    void update_scale();
    void update_terrain_with_explosion(int pos_x, int pos_y, int explosion_size_x, int explosion_size_y);
    bool transparent_at_pixel(int x, int y) const;
    sf::Sprite get_terrain_sprite() const;
    void update_pixel_array(float x_scale, float y_scale);
    int get_pixel_map_size();

  private:
    int width_;
    int height_;
    std::vector<sf::Vector3f> pixel_map;
};
