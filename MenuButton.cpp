#include "MenuButton.h"

MenuButton::MenuButton(const sf::Font &font, const std::string &text, const sf::Vector2f &position,
                       const sf::Vector2f &size, bool fromCenter)
    : text_(text), font_(font), position_(position), size_(size), fromCenter_(fromCenter)
{
    initialize();
}

void MenuButton::render(sf::RenderWindow &target)
{
    target.draw(buttonBackground_);
    target.draw(buttonShape_);
    target.draw(buttonText_);
}

void MenuButton::onClick(std::function<void()> action)
{
    onClickAction_ = std::move(action);
}

bool MenuButton::contains(const sf::Vector2f &point) const
{
    return buttonShape_.getGlobalBounds().contains(point);
}

void MenuButton::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePosition(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
        if (contains(mousePosition) && onClickAction_)
        {
            onClickAction_();
        }
    }
}

void MenuButton::initialize()
{
    buttonShape_.setFillColor(sf::Color::Blue); // Set button color
    buttonShape_.setSize(size_);

    if (!fromCenter_)
    {
        // If not from center, use the given position directly
        buttonShape_.setPosition(position_);
    }
    else
    {
        // Center the button and calculate the position from the center
        float xPos = position_.x - size_.x / 2.0f;
        float yPos = position_.y - size_.y / 2.0f;
        buttonShape_.setPosition(xPos, yPos);
    }

    buttonShape_.setOutlineThickness(2);
    buttonShape_.setOutlineColor(sf::Color::Blue);

    buttonText_.setFont(font_);
    buttonText_.setString(text_);
    buttonText_.setCharacterSize(18); // Text size
    buttonText_.setFillColor(sf::Color::Black);

    // Center the text within the button shape
    sf::FloatRect textBounds = buttonText_.getLocalBounds();
    buttonText_.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    buttonText_.setPosition(buttonShape_.getPosition() +
                            sf::Vector2f(buttonShape_.getSize().x / 2.0f, buttonShape_.getSize().y / 2.0f));
}

sf::Vector2f MenuButton::getSize() const
{
    return buttonShape_.getSize();
}

void MenuButton::setPosition(const sf::Vector2f &position)
{
    position_ = position;

    if (!fromCenter_)
    {
        buttonShape_.setPosition(position_);
    }
    else
    {
        float xPos = position_.x - size_.x / 2.0f;
        float yPos = position_.y - size_.y / 2.0f;
        buttonShape_.setPosition(xPos, yPos);
    }

    buttonText_.setPosition(position_ + size_ / 2.0f);
}
