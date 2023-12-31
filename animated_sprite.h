#pragma once
#include "debug_output.h"
#include <SFML/Graphics.hpp>
class animated_sprite
{
  public:
    animated_sprite();
    animated_sprite(const sf::Texture &texture, int num_frames, float frame_duration);
    void set_position(const sf::Vector3f &position);
    void update(float dt);
    sf::Sprite draw();

  private:
    sf::Sprite sprite_;
    int num_frames_;
    int current_frame_;
    float frame_duration_;
    float elapsed_;
    int frame_width_;
};
