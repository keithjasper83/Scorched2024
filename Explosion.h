#pragma once
#include "debug_output.h"
#include <SFML/Graphics.hpp>

class explosion
{
  public:
    explosion(float x, float y, float size, sf::Time duration);
    void update(sf::Time delta_time);
    bool is_active() const;
    void render(sf::RenderWindow &window) const;
    sf::Vector2f get_position() const;
    float get_explosion_size() const;

  private:
    sf::Vector2f position_;
    float explosion_size_;
    sf::Time explosion_duration_;
    bool active_;
    sf::CircleShape explosion_shape_; // Used for rendering
};
