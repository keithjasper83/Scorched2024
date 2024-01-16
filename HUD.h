#pragma once
#include "Tank.h"
#include "debug_output.h"
#include <SFML/Graphics.hpp>
#include <iostream>
class hud
{
  private:
    sf::Font font_;
    sf::Texture background_image_texture_;
    sf::Sprite background_image_sprite_;

  public:
    hud();
    void set_font(const std::string &font_path);
    sf::Text generate_player_data(tank tank) const;
    void render_player_data(sf::RenderWindow &window, const std::vector<tank> &players) const;
    static sf::RectangleShape create_centered_red_window();
    sf::Text set_winner(std::string playerName) const;
};
