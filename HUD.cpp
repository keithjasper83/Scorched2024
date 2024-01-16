#include "HUD.h"

hud::hud()
{
    if (!font_.loadFromFile(ConfigJSON::getEngineFontFile()))
    {
        KJ::debug_output::print(__FILE__, " Font not found", KJ::debug_output::MessageType::FATAL);
    }

    if (!background_image_texture_.loadFromFile("Images/hudBackground.png"))
    {
        KJ::debug_output::print(__FILE__, "Background image not found", KJ::debug_output::MessageType::FATAL);
    }
    background_image_sprite_.setTexture(background_image_texture_);
}

void hud::set_font(const std::string &font_path)
{
    if (!font_.loadFromFile(font_path))
    {
        KJ::debug_output::print(__FILE__, "Font not found");
    }
}

sf::Text hud::generate_player_data(tank tank) const
{
    sf::Text player_hud;
    player_hud.setFont(font_);
    player_hud.setCharacterSize(24);
    if (tank.active_player)
    {
        player_hud.setFillColor(sf::Color::Red);
    }
    else
    {
        player_hud.setFillColor(sf::Color::Black);
    }

    std::string player_name = "Name: " + tank.get_player_name();
    std::string player_health = " Health: " + std::to_string(tank.get_health());
    std::string player_power = " Power: " + std::to_string(static_cast<int>(tank.get_power()));
    std::string player_angle = " Angle: " + std::to_string(static_cast<int>(tank.get_angle()));
    std::string player_weapon = " Weapon: " + tank.get_weapon_name();
    std::string player_ammo = " Ammo: " + std::to_string(tank.get_ammo());

    std::string player_data = player_name + player_health + player_power + player_angle + player_ammo + player_weapon;
    player_hud.setString(player_data);

    // Set the position based on player index or other criteria
    // playerHud.setPosition(x, y);

    return player_hud;
}

void hud::render_player_data(sf::RenderWindow &window, const std::vector<tank> &players) const
{
    window.draw(background_image_sprite_);

    float y_position = 10.0f; // Initial Y position
    for (auto &player : players)
    {
        sf::Text player_label = generate_player_data(player);
        player_label.setPosition(10.0f, y_position);
        window.draw(player_label);
        y_position += 30.0f; // Increase Y position for the next player label
    }
}

sf::RectangleShape hud::create_centered_red_window()
{
    sf::RectangleShape red_window(sf::Vector2f(40, 40));
    red_window.setFillColor(sf::Color::Red);

    constexpr float center_x = (1920 - 40) / 2.0f;
    constexpr float center_y = (1280 - 40) / 2.0f;
    red_window.setPosition(center_x, center_y);

    return red_window;
}

sf::Text hud::set_winner(std::string winMessage) const
{
    sf::Text winner;
    winner.setFont(font_);
    winner.setCharacterSize(24);
    winner.setFillColor(sf::Color::Red);
    winner.setString(winMessage);
    return winner;
}
