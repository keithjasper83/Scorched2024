#include <functional>

#include "Config.h"
#include "background_particles.h"

background_particles::background_particles()
{
}

sf::Sprite background_particles::render()
{
    particle_texture.clear(sf::Color::Transparent);
    // Create a circle shape for the dust particle
    sf::CircleShape dust_particle(5.0f);
    dust_particle.setFillColor(sf::Color::Yellow);
    // Move and draw dust particles
    // std::cout << "new Frame rendering background particles" << std::endl;
    for (const auto &particle : particle_vector)
    {
        // Set the position of the particle
        dust_particle.setPosition(particle);
        // std::cout << "rendering particle - x: " << particle.x << " y: " << particle.y << std::endl;

        // Draw the particle onto the render texture
        particle_texture.draw(dust_particle);
    }

    // Display the render texture
    particle_texture.display();
    sf::Sprite sprite(particle_texture.getTexture());
    /*sprite.setOrigin(this->screen_width / 2.f, this->screen_height / 2.f);
        sprite.setPosition(this->screen_width / 2.f, this->screen_height / 2.f);
        sprite.setScale(1.0f, 1.0f);*/
    // sprite.setColor(sf::Color(0, 255, 255, 255));
    this->update();
    return sprite;
}

void background_particles::set_screen_dimensions(const int width, const int height)
{
    this->screen_width = width;
    this->screen_height = height;
    particle_texture.create(this->screen_width, this->screen_height);
    particle_texture.setSmooth(true);
    particle_texture.setRepeated(true);
    particle_texture.clear(sf::Color::Transparent);
    this->create(constants::particle_count);
}

void background_particles::update()
{
    // Seed the random number generator only once
    static std::random_device rd;
    static std::default_random_engine rand_number(rd());

    // Loop through particle vector and update locations
    for (auto &particle : particle_vector)
    {
        const float old_position_x = particle.x;
        const float old_position_y = particle.y;

        // Initialize a normal distribution for particle generation
        std::normal_distribution<float> random_offset_x(0.0f, 0.5f); // Adjust the standard deviation as needed
        std::normal_distribution<float> random_offset_y(1.0f, 0.5f);

        const float offset_x = random_offset_x(rand_number);
        const float offset_y = random_offset_y(rand_number);

        const float new_position_x = old_position_x + offset_x;
        const float new_position_y = old_position_y + offset_y;

        // Update the particle position
        particle.x = new_position_x;
        particle.y = new_position_y;
    }

    // Remove out-of-bounds particles after updating all particles
    remove_out_of_bounds_particles();
}

void background_particles::create(const int quantity)
{
    std::random_device rd;
    std::default_random_engine rand_number(rd());

    // Initialize a normal distribution for particle generation
    std::normal_distribution<float> normal_x(this->screen_width / 2.0f, this->screen_width / 4.0f);
    std::normal_distribution<float> normal_y(this->screen_height / 10.0f, this->screen_height / 4.0f);

    for (int i = 0; i < quantity; i++)
    {
        // Generate random positions with a normal distribution
        const sf::Vector2f particle_position(normal_x(rand_number), normal_y(rand_number));

        particle_vector.push_back(particle_position);
    }
}

void background_particles::destroy()
{
    std::cout << "destroying background particles" << std::endl;
}

void background_particles::remove_out_of_bounds_particles()
{
    auto it = std::remove_if(particle_vector.begin(), particle_vector.end(), [this](const sf::Vector2f &particle) {
        // Assuming screen_width and screen_height are the dimensions of your screen
        return particle.x > this->screen_width || particle.y > this->screen_height;
    });

    particle_vector.erase(it, particle_vector.end());

    // Add a new particle if needed
    if (particle_vector.size() < constants::particle_count)
    {
        create(1);
    }
}
