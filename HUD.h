#pragma once

#include "Tank.h"
#include <SFML/Graphics.hpp>
#include <iostream>
class HUD {
private:
    sf::Font font;
    sf::Texture backgroundImageTexture;
    sf::Sprite backgroundImageSprite;

public:
    HUD();
    void setFont(const std::string& fontPath);
    sf::Text generatePlayerData(Tank tank);
    void renderPlayerData(sf::RenderWindow& window, const std::vector<Tank>& players);
    sf::RectangleShape createCenteredRedWindow();
};