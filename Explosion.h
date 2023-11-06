#pragma once
#include <SFML/Graphics.hpp>

class Explosion
{
public:
    Explosion(float x, float y, float size, sf::Time duration);
    void update(sf::Time deltaTime);
    bool isActive() const;
    void render(sf::RenderWindow& window) const;
    sf::Vector2f getPosition() const;
    float getExplosionSize() const;

private:
    sf::Vector2f position;
    float explosionSize;
    sf::Time explosionDuration;
    bool active;
    sf::CircleShape explosionShape; // Used for rendering
};
