#pragma once
#include <string>

class Tank
{
    struct
    {
        float x = 0;
        float y = 0;
    } location;
    struct
    {
        int index = 0;
        std::string playerName = "A";
        int health = 100;
        int ammo = 100;
        std::string weapon = "Cannon";
        float startPower = 50.0f;
        float maxPower = 100.0f;
        float minPower = 1.0f;
    } playerInfo;
    struct
    {
        float bodyX = 64.0f;
        float bodyY = 64.0f;
    } tankSize;
    struct
    {
        float angle = 270.0f;
        float width = 10.0f;
        float height = 60.0f;
    } turret;

public:
    bool activePlayer = false;
    bool onGround = false;

    Tank();

    Tank(float x, float y, int index, std::string playerName);

    void setOnGround(bool onGround);

    bool getOnGround();

    void setAngle(float angle);

    void incAngle();

    void decAngle();

    void incPower();

    void decPower();

    float getPower();

    float getAngle();

    void normalizeAngle();

    void setLocation(float x, float y);

    float getX();

    float getY();

    void Fire();

    float getOriginX();

    float getOriginY();

    float getBodyX();

    float getBodyY();

    float getTurretX();

    float getTurretY();

    float getTurretWidth();

    float getTurretHeight();

    std::string getPlayerName();

    int getIndex();

    int getHealth();

    int getAmmo();

    std::string getWeaponName();
};