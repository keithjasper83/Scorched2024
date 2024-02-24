#pragma once
#include "SFML/Graphics.hpp"
#include "debug_output.h"
#include <cmath>
#include <iostream>
#include <string>

#include "ConfigJSON.h"

class tank
{
    struct
    {
        float x = 0;
        float y = 0;
    } location_;
    struct
    {
        int index = 0;
        std::string player_name = "A";
        int health = 100;
        int ammo = 100;
        std::string weapon = "Cannon";
        float start_power = 50.0f;
        float max_power = 100.0f;
        float min_power = 1.0f;
    } player_info_;
    struct
    {
        float body_x = 64.0f * ConfigJSON::getRenderTankScale();
        float body_y = 64.0f * ConfigJSON::getRenderTankScale();
    } tank_size_;
    struct
    {
        float angle = 225.0f;
        float width = 10.0f;
        float height = 60.0f;
    } turret_;

  public:
    bool active_player = false;
    bool on_ground = false;

    tank() = default;

    tank(float x, float y, int index, const std::string &player_name);

    void set_on_ground(bool on_ground);

    bool get_on_ground() const;

    void set_angle(float angle);

    void inc_angle();

    void dec_angle();

    void inc_power();

    void dec_power();

    float get_power();

    float get_angle() const;

    void normalize_angle();

    void set_location(float x, float y);

    float get_x() const;

    float get_y() const;

    static void fire();

    float get_origin_x() const;

    float get_origin_y() const;

    float get_body_x() const;

    float get_body_y() const;

    float get_turret_x() const;

    float get_turret_y() const;

    float get_turret_width() const;

    float get_turret_height() const;

    std::string get_player_name() const;

    int get_index() const;

    int get_health() const;
    int reduce_health_by(int amount);

    int get_ammo() const;

    sf::Vector2f get_turret_tip_position() const;
    static sf::Texture get_tank_texture();
    static sf::Texture get_turret_texture();
    std::string get_weapon_name() const;

    int get_active_projectile() const;
    void set_active_projectile(int projectileType);

    int active_projectile_ = 0;
};
