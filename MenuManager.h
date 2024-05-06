// MenuManager.h
#pragma once

#include "Menu.h"
#include <SFML/Graphics.hpp>
#include <functional>

class MenuManager
{
  public:
    MenuManager(sf::RenderWindow &window, sf::Font &font);
    void renderPauseMenu(std::function<void()> resumeFunction, std::function<void()> settingsFunction,
                         std::function<void()> quitFunction);
    void renderWinStateButtons(std::function<void()> restartFunction, std::function<void()> quitFunction);

    void renderSettingsMenu(std::function<void()> backFunction, std::function<void()> applyFunction);

  private:
    sf::RenderWindow &window_;
    sf::Font &font_;
};

// MenuManager.cpp
