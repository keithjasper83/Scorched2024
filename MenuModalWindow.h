#pragma once

#include "MenuButton.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

class MenuModalWindow
{
  public:
    MenuModalWindow(sf::RenderWindow &window, const sf::Font &font);

    void show(const std::string &message);
    bool getResult() const;
    void handleEvent(const sf::Event &event);
    bool isModalVisible() const;
    void render();

  private:
    void initialize();
    void centerModalBackground();
    void centerText();
    MenuButton createButton(const std::string &text, const sf::Vector2f &position, const sf::Vector2f &size);

    sf::RenderWindow &window_;
    sf::Font font_;
    sf::RectangleShape modalBackground_;
    sf::Text modalText_;
    MenuButton quitButton_;
    MenuButton cancelButton_;
    bool isModalVisible_;
    bool result_;
};
