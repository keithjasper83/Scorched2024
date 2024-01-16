#include "Menu.h"

Menu::Menu(sf::RenderWindow &window) : window_(window)
{
}

void Menu::addButton(MenuButton button)
{
    buttons_.push_back(std::move(button));
}

void Menu::render()
{
    for (auto &button : buttons_)
    {
        button.render(window_);
    }
}

void Menu::handleEvent(const sf::Event &event)
{
    for (auto &button : buttons_)
    {
        button.handleEvent(event);
    }
}
