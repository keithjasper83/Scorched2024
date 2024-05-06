#include "MenuManager.h"

MenuManager::MenuManager(sf::RenderWindow &renderWindow, sf::Font &font) : window_(renderWindow), font_(font)
{
}

void MenuManager::renderPauseMenu(std::function<void()> resumeFunction, std::function<void()> settingsFunction,
                                  std::function<void()> quitFunction)
{

    sf::RectangleShape backgroundRect(sf::Vector2f(window_.getSize().x, window_.getSize().y));
    backgroundRect.setFillColor(sf::Color(200, 200, 200, 128)); // Light grey with transparency
    window_.draw(backgroundRect);

    Menu pauseMenu(window_);

    // Use MenuButton instances instead of directly creating MenuButton objects
    MenuButton resumeButton(font_, "Resume", sf::Vector2f(window_.getSize().x / 2.0f, window_.getSize().y / 2.0f - 80),
                            sf::Vector2f(200, 50), true);
    MenuButton settingsButton(font_, "Settings",
                              sf::Vector2f(window_.getSize().x / 2.0f, window_.getSize().y / 2.0f - 20),
                              sf::Vector2f(200, 50), true);
    MenuButton quitButton(font_, "Quit", sf::Vector2f(window_.getSize().x / 2.0f, window_.getSize().y / 2.0f + 40),
                          sf::Vector2f(200, 50), true);

    resumeButton.onClick(resumeFunction);
    settingsButton.onClick(settingsFunction);
    quitButton.onClick(quitFunction);

    // Add buttons to the menu
    pauseMenu.addButton(resumeButton);
    pauseMenu.addButton(settingsButton);
    pauseMenu.addButton(quitButton);

    sf::Event event;
    while (window_.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window_.close();
        }
        pauseMenu.handleEvent(event);
    }

    pauseMenu.render();
}

void MenuManager::renderWinStateButtons(std::function<void()> restartFunction, std::function<void()> quitFunction)
{
    sf::RectangleShape backgroundRect(sf::Vector2f(window_.getSize().x, window_.getSize().y));
    backgroundRect.setFillColor(sf::Color(200, 200, 200, 128)); // Light grey with transparency
    window_.draw(backgroundRect);

    Menu winMenu(window_);

    // Calculate the X-coordinate for centering the buttons horizontally
    float centerX = window_.getSize().x / 2.0f; // Center of the window

    // Calculate the width of both buttons
    float buttonWidth = 200.0f;

    // Calculate the gap between the buttons
    float buttonGap = 20.0f;

    // Calculate the total width occupied by both buttons and the gap
    float totalWidth = 2.0f * buttonWidth + buttonGap;

    // Calculate the starting X-coordinate for the first button to center them
    float startX = (centerX - totalWidth / 2.0f);

    // Create the restart button centered horizontally and positioned at the bottom 2/3 on the Y-axis
    MenuButton restartButton(font_, "Restart", sf::Vector2f(startX, window_.getSize().y * 2.0f / 3.0f),
                             sf::Vector2f(buttonWidth, 50), true);

    // Position the quit button next to the restart button with the calculated gap
    MenuButton quitButton(font_, "Quit",
                          sf::Vector2f(startX + buttonWidth + buttonGap, window_.getSize().y * 2.0f / 3.0f),
                          sf::Vector2f(buttonWidth, 50), true);

    restartButton.onClick(restartFunction);
    quitButton.onClick(quitFunction);

    winMenu.addButton(restartButton);
    winMenu.addButton(quitButton);

    sf::Event event;
    while (window_.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window_.close();
        }
        winMenu.handleEvent(event);
    }

    winMenu.render();
}

void MenuManager::renderSettingsMenu(std::function<void()> backFunction, std::function<void()> applyFunction)
{
    sf::RectangleShape backgroundRect(sf::Vector2f(window_.getSize().x, window_.getSize().y));
    backgroundRect.setFillColor(sf::Color(200, 200, 200, 128)); // Light grey with transparency
    window_.draw(backgroundRect);

    Menu settingsMenu(window_);

    // Use MenuButton instances instead of directly creating MenuButton objects
    MenuButton backButton(font_, "Back", sf::Vector2f(window_.getSize().x / 2.0f, window_.getSize().y / 2.0f - 80),
                          sf::Vector2f(200, 50), true);
    MenuButton applyButton(font_, "Apply", sf::Vector2f(window_.getSize().x / 2.0f, window_.getSize().y / 2.0f - 20),
                           sf::Vector2f(200, 50), true);

    backButton.onClick(backFunction);
    applyButton.onClick(applyFunction);

    // Add buttons to the menu
    settingsMenu.addButton(backButton);
    settingsMenu.addButton(applyButton);

    sf::Event event;
    while (window_.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window_.close();
        }
        settingsMenu.handleEvent(event);
    }

    settingsMenu.render();
}
