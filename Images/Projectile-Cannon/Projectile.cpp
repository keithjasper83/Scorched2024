#include "../../IProjectile.h"

class Projectile : public IProjectile {
public:
    void update(float deltaTime) {
        // Update the projectile's position
		// Update the projectile's rotation
		// Update the projectile's texture
    }
private:
    float x;
    float y;
    float velocity;
    int damage = 1;
    // Add other attributes like texture, rotation, etc.
};