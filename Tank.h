#pragma once
#include <string>

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
        float body_x = 64.0f;
        float body_y = 64.0f;
    } tank_size_;
    struct
    {
        float angle = 270.0f;
        float width = 10.0f;
        float height = 60.0f;
    } turret_;

  public:
    bool active_player = false;
    bool on_ground = false;

    tank() = default;

    tank(float x, float y, int index, const std::string& player_name);

    void set_on_ground(bool on_ground);

    bool get_on_ground() const;

    void set_angle(float angle);

    void inc_angle();

    void dec_angle();

    void inc_power();

    void dec_power();

    float get_power();

    float get_angle();

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

    std::string get_player_name();

    int get_index() const;

    int get_health() const;

    int get_ammo() const;

    std::string get_weapon_name();
};
