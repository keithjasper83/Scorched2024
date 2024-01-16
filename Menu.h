#pragma once

#include "MenuButton.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

class Menu
{
  public:
    Menu(sf::RenderWindow &window);

    void addButton(MenuButton button);
    void render();
    void handleEvent(const sf::Event &event);

  private:
    sf::RenderWindow &window_;
    std::vector<MenuButton> buttons_;
};
