#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

class MenuButton
{
  public:
    MenuButton() = default;
    /// <summary>
    /// Create Menu button
    /// </summary>
    /// <param name="font"></param>
    /// <param name="text"></param>
    /// <param name="position"></param>
    /// <param name="size"></param>
    MenuButton(const sf::Font &font, const std::string &text, const sf::Vector2f &position, const sf::Vector2f &size,
               bool fromCenter = false);
    void render(sf::RenderWindow &target);
    void onClick(std::function<void()> action);
    bool contains(const sf::Vector2f &point) const;
    void handleEvent(const sf::Event &event);
    void resetClick();
    sf::Vector2f getSize() const;
    void setPosition(const sf::Vector2f& position);


        private : 
            std::string text_;
    sf::RectangleShape buttonShape_;
    sf::RectangleShape buttonBackground_;
    sf::Text buttonText_;
    sf::Font font_;
    sf::Vector2f position_;
    sf::Vector2f size_;
    std::function<void()> onClickAction_;
    bool isClicked_ = false;
    bool fromCenter_ = false;

    void initialize();
};
