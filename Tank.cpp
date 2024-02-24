#include "Tank.h"

tank::tank(const float x, const float y, const int index, const std::string &player_name)
{
    location_.x = x;
    location_.y = y;
    player_info_.player_name = player_name;
    player_info_.index = index;
}

void tank::set_on_ground(const bool on_ground)
{
    this->on_ground = on_ground;
}

bool tank::get_on_ground() const
{
    return this->on_ground;
}

void tank::set_angle(const float angle)
{
    KJ::debug_output::print(__FILE__, "Player: " + get_player_name() + " setAngle: " + std::to_string(angle),
                            KJ::debug_output::MessageType::INFO);
    turret_.angle = angle;
}

void tank::inc_angle()
{
    turret_.angle++;
    normalize_angle();
}

void tank::dec_angle()
{
    turret_.angle--;
    normalize_angle();
}

void tank::inc_power()
{
    player_info_.start_power++;
}

void tank::dec_power()
{
    player_info_.start_power--;
}

float tank::get_power()
{
    if (player_info_.start_power > player_info_.max_power)
    {
        player_info_.start_power = player_info_.max_power;
    }
    else if (player_info_.start_power < player_info_.min_power)
    {
        player_info_.start_power = player_info_.min_power;
    }
    return player_info_.start_power;
}

float tank::get_angle() const
{
    return turret_.angle;
}

void tank::normalize_angle()
{
    if (turret_.angle > 360)
    {
        turret_.angle = 360;
    }
    if (turret_.angle < 0)
    {
        turret_.angle = 0;
    }
}

void tank::set_location(const float x, const float y)
{
    // printf("Tank.cpp - Player: %s setLocation: %f, %f\n", getPlayerName().c_str(), x, y);
    location_.x = x;
    location_.y = y;
}

float tank::get_x() const
{
    return location_.x;
}

float tank::get_y() const
{
    return location_.y;
}

void tank::fire()
{
    KJ::debug_output::print(__FILE__, "FIRE!", KJ::debug_output::MessageType::GOOD);
    // Create a new projectile and add it to the container

    // Now, you can tell the renderer to draw the projectiles
    // renderer.drawProjectiles(projectiles);
}

float tank::get_origin_x() const
{
    return tank_size_.body_x;
}

float tank::get_origin_y() const
{
    return tank_size_.body_y;
}

float tank::get_body_x() const
{
    return tank_size_.body_x;
}

float tank::get_body_y() const
{
    return tank_size_.body_y;
}

float tank::get_turret_x() const
{
    return tank_size_.body_x / 2.0f;
}

float tank::get_turret_y() const
{
    return -tank_size_.body_y - 5.0f;
}

float tank::get_turret_width() const
{
    return turret_.width;
}

float tank::get_turret_height() const
{
    return turret_.height;
}

std::string tank::get_player_name() const
{
    return player_info_.player_name;
}

int tank::get_index() const
{
    return player_info_.index;
}

int tank::get_health() const
{
    return player_info_.health;
}

int tank::reduce_health_by(int amount)
{
    player_info_.health -= amount;
    return player_info_.health;
}

int tank::get_ammo() const
{
    return player_info_.ammo;
}

std::string tank::get_weapon_name() const
{
    return player_info_.weapon;
}

// Function to get the position of the tip of the turret
sf::Vector2f tank::get_turret_tip_position() const
{
    // Calculate the position of the tip of the turret based on tank position and turret dimensions
    const float tip_x = location_.x + get_body_x() - turret_.height * std::cos(turret_.angle);
    const float tip_y = location_.y + get_body_y() - turret_.height * std::sin(turret_.angle);

    return {tip_x, tip_y};
}

sf::Texture tank::get_tank_texture()
{
    sf::Texture tank_texture;
    if (!tank_texture.loadFromFile("Images/tank-army.png"))
    {
        KJ::debug_output::print(__FILE__, "failed to load image", KJ::debug_output::MessageType::FATAL);
        // exit(1);
    }
    return tank_texture;
}

sf::Texture tank::get_turret_texture()
{
    sf::Texture turret_texture;
    if (!turret_texture.loadFromFile("Images/turret-army.png"))
    {
        KJ::debug_output::print(__FILE__, "failed to load image");
        // exit(1);
    }
    return turret_texture;
}

int tank::get_active_projectile() const
{
    return active_projectile_;
}
void tank::set_active_projectile(int projectile)
{
    active_projectile_ = projectile;
}
