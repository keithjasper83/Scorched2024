#include "HUD.h"


HUD::HUD()
{
    if (!font.loadFromFile("Roboto-Bold.ttf"))
    {
        std::cout << "ERROR: Font not found" << std::endl;
    }

    if (!backgroundImageTexture.loadFromFile("Images/hudBackground.png"))
    {
        std::cout << "ERROR: Background image not found" << std::endl;
    }
    backgroundImageSprite.setTexture(backgroundImageTexture);
}

void HUD::setFont(const std::string& fontPath)
{
    if (!font.loadFromFile(fontPath))
    {
        std::cout << "ERROR: Font not found" << std::endl;
    }
}

sf::Text HUD::generatePlayerData(Tank tank)
{
    sf::Text playerHud;
    playerHud.setFont(font);
    playerHud.setCharacterSize(24);
    if (tank.activePlayer)
    {
        playerHud.setFillColor(sf::Color::Red);
    }
    else
    {
        playerHud.setFillColor(sf::Color::Black);
    }

    std::string playerName = "Name: " + tank.getPlayerName();
    std::string playerHealth = " Health: " + std::to_string(tank.getHealth());
    std::string playerPower = " Power: " + std::to_string(static_cast<int>(tank.getPower()));
    std::string playerAngle = " Angle: " + std::to_string(static_cast<int>(tank.getAngle()));
    std::string playerWeapon = " Weapon: " + tank.getWeaponName();
    std::string playerAmmo = " Ammo: " + std::to_string(tank.getAmmo());

    std::string playerData = playerName + playerHealth + playerPower + playerAngle + playerAmmo + playerWeapon;
    playerHud.setString(playerData);

    // Set the position based on player index or other criteria
    // playerHud.setPosition(x, y);

    return playerHud;
}

void HUD::renderPlayerData(sf::RenderWindow& window, const std::vector<Tank>& players)
{
    window.draw(backgroundImageSprite);

    float yPosition = 10.0f; // Initial Y position
    for (auto& player : players)
    {
        sf::Text playerLabel = generatePlayerData(player);
        playerLabel.setPosition(10.0f, yPosition);
        window.draw(playerLabel);
        yPosition += 30.0f; // Increase Y position for the next player label
    }
}

sf::RectangleShape HUD::createCenteredRedWindow()
{
    sf::RectangleShape redWindow(sf::Vector2f(40, 40));
    redWindow.setFillColor(sf::Color::Red);

    constexpr float centerX = (1920 - 40) / 2.0f;
    constexpr float centerY = (1280 - 40) / 2.0f;
    redWindow.setPosition(centerX, centerY);

    return redWindow;
}