#pragma once

#include "TerrainGenerator.h"

struct terrain_generator::scale scale;

terrain_generator::terrain_generator()
{
    if (!terrain_texture.loadFromFile("Images/overlay.png"))
    {
        printf("Failed to load texture"); // Error handling
    }

    // Create a sprite for the terrain
    const sf::Sprite terrain_sprite(terrain_texture);
    this->terrain_sprite = terrain_sprite;

    const sf::Image terrain_image = terrain_texture.copyToImage();
    this->terrain_image = terrain_image;
}
terrain_generator::terrain_generator(const int width, const int height)
{
    // init height
    this->width_ = width;
    this->height_ = height;
    set_screen_size(width, height);
}

void terrain_generator::set_screen_size(const int width, const int height)
{
    // Update the screen dimensions
    this->width_ = width;
    this->height_ = height;

    // Update the terrain sprite size to fill the screen
    update_sprite_size();

    // Optionally, update the terrain image size to match the new screen size
    // updateImageSize();
}

void terrain_generator::update_sprite_size()
{
    // Get the original texture size
    const sf::Vector2u texture_size = terrain_texture.getSize();

    // Calculate the scale factors for the terrain sprite
    const float scale_x = static_cast<float>(width_) / texture_size.x;
    const float scale_y = static_cast<float>(height_) / texture_size.y;

    // Set the scale for the terrain sprite
    terrain_sprite.setScale(scale_x, scale_y);
    // this->terrainSprite.setScale(this->width / 800.0f, this->height / 600.0f);
}

sf::Image terrain_generator::get_terrain_image()
{
    // for (int x = 0; x < this->terrainImage.getSize().x; x++) {
    // int floorheight = getFirstNonTransparentPixelinX(x);
    // terrainImage.setPixel(x, floorheight, sf::Color::Red);
    //}
    return this->terrain_image;
}

float terrain_generator::get_screen_width() const
{
    return this->width_;
}
float terrain_generator::get_screen_height() const
{
    return this->height_;
}

terrain_generator::scale terrain_generator::get_scale() const
{
    return this->window_scale;
}

void terrain_generator::update_scale()
{
    const sf::Vector2u image = get_terrain_image().getSize();
    const auto y_scale = static_cast<float>(get_screen_height() / image.y);
    const auto x_scale = static_cast<float>(get_screen_width() / image.x);
    this->window_scale.x = x_scale;
    this->window_scale.y = y_scale;
}

int terrain_generator::get_first_non_transparent_pixel_in_x(const int x) const
{
    int ret_value = -1;
    for (int y = 0; y < terrain_image.getSize().y; ++y)
    {
        const sf::Color pixel = terrain_image.getPixel(x, y);
        if (pixel.a != 0)
        {
            // printf("this pixel is not transparent Y: %d %d\n", x, y);
            ret_value = y;
            break;
        }
    } // end for
    return ret_value;
}

void terrain_generator::update_terrain_with_collision(const int pos_x, const int pos_y)
{
    const float terrain_width = (static_cast<float>(terrain_image.getSize().x));
    const float terrain_height = (static_cast<float>(terrain_image.getSize().y));
    for (int x = 0; x < 80; x++)
    {
        for (int y = 0; y < 80; y++)
        {
            const int circle_x = x - 20; // Centering the circle at (20, 20)
            const int circle_y = y - 20;
            if (circle_x * circle_x + circle_y * circle_y <= 20 * 20)
            {
                // If the pixel is within the circle's radius, make it transparent
                if (pos_x + x - 20 < 0 || pos_x + x - 20 >= terrain_width || pos_y + y - 20 < 0 ||
                    pos_y + y - 20 >= terrain_height)
                {
                    continue;
                }
                terrain_image.setPixel((pos_x) + x - 20, (pos_y) + y - 20, sf::Color::Transparent);
            }
        }
    }
    terrain_texture.loadFromImage(terrain_image);
    terrain_sprite.setTexture(terrain_texture);
}

// BUG BUG explosion not fully affecting terrain but effect seems to be quite nice :) needs resolving at some point.
//  CURRENTLY faked using multiply X 4 when calling this function
void terrain_generator::update_terrain_with_explosion(int pos_x, int pos_y, const int explosion_size_x,
                                                      const int explosion_size_y)
{
    const float terrain_width = static_cast<float>(terrain_image.getSize().x) / window_scale.x;
    const float terrain_height = static_cast<float>(terrain_image.getSize().y) / window_scale.y;

    for (int x = 0; x < explosion_size_x; x++)
    {
        for (int y = 0; y < explosion_size_y; y++)
        {
            const int explosion_x_half = explosion_size_x / 2;
            const int explosion_y_half = explosion_size_y / 2;
            const int circle_x = x - explosion_x_half;
            const int circle_y = y - explosion_y_half;

            if (circle_x * circle_x + circle_y * circle_y <= explosion_x_half * explosion_y_half)
            {
                const int terrain_x = static_cast<int>(pos_x / window_scale.x + x - explosion_x_half);
                const int terrain_y = static_cast<int>(pos_y / window_scale.y + y - explosion_y_half);

                // Check if the terrain coordinates are within bounds
                if (terrain_x >= 0 && terrain_x < terrain_width && terrain_y >= 0 && terrain_y < terrain_height)
                {
                    terrain_image.setPixel(terrain_x, terrain_y, sf::Color::Transparent);
                }
            }
        }
    }

    terrain_texture.loadFromImage(terrain_image);
    terrain_sprite.setTexture(terrain_texture);
}

bool terrain_generator::transparent_at_pixel(const int x, const int y) const
{
    // cout << "TerrainGenerator.cpp - Pixel: " << x << ", " << y << endl;
    const sf::Color pixel = terrain_texture.copyToImage().getPixel((x / window_scale.x), (y / window_scale.y));
    // printf("TerrainGenerator.cpp - TransparentAtPixel() - X: %d, Y: %d Aplha: %d\n", x, y, pixel.a);
    return pixel.a != 0;
}

sf::Sprite terrain_generator::get_terrain_sprite()
{
    return this->terrain_sprite;
}
