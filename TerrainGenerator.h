#pragma once

#include "Config.h"
#include <SFML/Graphics.hpp>

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
    sf::Image terrain_image;
    scale window_scale;

    terrain_generator();
    terrain_generator(int width, int height);
    void set_screen_size(int width, int height);
    void update_sprite_size();
    sf::Image get_terrain_image();
    int get_first_non_transparent_pixel_in_x(int x) const;
    void update_terrain_with_collision(int pos_x, int pos_y);
    float get_screen_width() const;
    float get_screen_height() const;
    scale get_scale() const;
    void update_scale();
    void update_terrain_with_explosion(int pos_x, int pos_y, int explosion_size_x, int explosion_size_y);
    bool transparent_at_pixel(int x, int y) const;
    sf::Sprite get_terrain_sprite();

  private:
    int width_ = constants::screen_width;
    int height_ = constants::screen_height;
};
