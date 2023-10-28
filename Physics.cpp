#include <vector>
#include "Tank.cpp"
#include "Config.h"
#include "SFML/Graphics.hpp"

class Physics {
public:
    Physics() {
        this->gravity = constants::GRAVITY;
    }
    Physics(float gravity) : gravity(gravity) {
    	// Constructor logic, if any
        this->gravity = gravity;
    }

    float getGravity() {
		return this->gravity;
	}

    bool applyGravity(std::vector<Tank>& tanks, const std::vector<int>& terrain, int screenHeight, sf::Time deltaTime) {
        bool tankFell = false;
        int pixelsPerSecond = 1;  // Adjust this value to control the falling speed

        for (Tank& tank : tanks) {
            int tankX = tank.getX();
            float tankY = tank.getY() + tank.getBodyY();
            int terrainHeight = screenHeight - terrain[tankX];

            if (tankY < terrainHeight) {
                float deltaTimeSeconds = deltaTime.asSeconds();
                float newTankY = tankY + (pixelsPerSecond * deltaTimeSeconds);

                if (newTankY >= terrainHeight) {
                    newTankY = terrainHeight;
                }

                tank.setLocation(tankX, newTankY);
                tankFell = true;
            }
        }

        return tankFell;
    }

private:
    float gravity;
};
