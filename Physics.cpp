#include "Physics.h"

physics::physics()
{
    this->gravity_ = constants::gravity;
}
physics::physics(const float gravity) : gravity_(gravity)
{
    // Constructor logic, if any
    this->gravity_ = gravity;
}

float physics::get_gravity() const
{
    return this->gravity_;
}

void physics::apply_gravity(std::vector<tank> &tanks, terrain_generator &terrain)
{
    for (tank &tank : tanks)
    {
        if (!tank.get_on_ground())
        {
            constexpr float pixels_per_second = 10.0f; // Adjust this value to control the falling
            terrain.update_scale();
            const float scale = terrain.get_scale().y;
            const float x_scale = terrain.get_scale().x;
            const float ground_height =
                terrain.get_first_non_transparent_pixel_in_x(static_cast<int>(tank.get_x() / x_scale));
            const float scaled_ground_height = ground_height * scale;
            const float old_location = tank.get_y();
            const float new_location = old_location + pixels_per_second;
            if (scaled_ground_height <= old_location)
            {
                tank.set_on_ground(true);
            }
            else
            {
                tank.set_location(tank.get_x(), new_location);
                tank.set_on_ground(false);
            }
        }
    }
}
