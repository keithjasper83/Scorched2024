#include "MenuModalWindow.h"

MenuModalWindow::MenuModalWindow(sf::RenderWindow &window, const sf::Font &font)
    : window_(window), font_(font), isModalVisible_(false), result_(false)
{
    initialize();
}

void MenuModalWindow::show(const std::string &message)
{
    isModalVisible_ = true;
    modalText_.setString(message);
    centerText();
}

bool MenuModalWindow::getResult() const
{
    return result_;
}

void MenuModalWindow::handleEvent(const sf::Event &event)
{
    if (!isModalVisible_)
        return;

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePosition(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

        if (quitButton_.contains(mousePosition))
        {
            result_ = true;
            isModalVisible_ = false;
        }
        else if (cancelButton_.contains(mousePosition))
        {
            result_ = false;
            isModalVisible_ = false;
        }
    }
}

bool MenuModalWindow::isModalVisible() const
{
    return isModalVisible_;
}

void MenuModalWindow::render()
{
    if (!isModalVisible_)
        return;

    window_.draw(modalBackground_);
    window_.draw(modalText_);
    quitButton_.render(window_);
    cancelButton_.render(window_);
}

void MenuModalWindow::initialize()
{
    modalBackground_.setSize(sf::Vector2f(400.f, 200.f));
    modalBackground_.setFillColor(sf::Color(0, 0, 0, 200));
    centerModalBackground();

    modalText_.setFont(font_);
    modalText_.setCharacterSize(24);
    modalText_.setFillColor(sf::Color::White);

    quitButton_ = createButton("Quit", sf::Vector2f(150.f, 120.f), sf::Vector2f(100.f, 40.f));
    cancelButton_ = createButton("Cancel", sf::Vector2f(250.f, 120.f), sf::Vector2f(100.f, 40.f));
}

void MenuModalWindow::centerModalBackground()
{
    sf::Vector2u windowSize = window_.getSize();
    modalBackground_.setPosition((windowSize.x - modalBackground_.getSize().x) / 2.f,
                                 (windowSize.y - modalBackground_.getSize().y) / 2.f);
}

void MenuModalWindow::centerText()
{
    sf::FloatRect textBounds = modalText_.getLocalBounds();
    modalText_.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    sf::Vector2u windowSize = window_.getSize();
    modalText_.setPosition(windowSize.x / 2.f, (windowSize.y - modalBackground_.getSize().y) / 2.f + 20.f);
}

MenuButton MenuModalWindow::createButton(const std::string &text, const sf::Vector2f &position,
                                         const sf::Vector2f &size)
{
    MenuButton button(font_, text, position, size);
    button.onClick([text]() { std::cout << "Button clicked: " << text << std::endl; });
    return button;
}
