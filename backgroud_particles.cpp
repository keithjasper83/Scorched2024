#include <functional>

#include "background_particles.h"

background_particles::background_particles()
{
    if (!texture.loadFromFile("Images/snowflake.png"))
    {
        // Handle error
        KJ::debug_output::print(__FILE__, "Failed to load texture!");
    }
    // sprite = sf::Sprite(texture);
    animation = animated_sprite(texture, 5, 1.0f);
};

sf::Sprite background_particles::render()
{
    particle_texture.clear(sf::Color::Transparent);

    for (const auto &particle : particle_vector)
    {
        // sprite.setPosition(particle);
        animation.set_position(particle);
        animation.update(1); // Adjust the elapsed time as needed

        // Draw the animated sprite onto the render texture
        sf::Sprite animatedSprite = animation.draw();
        particle_texture.draw(animatedSprite);
    }

    particle_texture.display();
    sf::Sprite sprite_window(particle_texture.getTexture());
    // Draw the sprite_window using your window.draw method

    // particle_texture.getTexture().copyToImage().saveToFile("Images/snowflake11.png");
    this->update();
    return sprite_window;
}

void background_particles::set_screen_dimensions(const int width, const int height)
{
    this->screen_width = width;
    this->screen_height = height;
    particle_texture.create(width, height);
    particle_texture.setSmooth(true);
    particle_texture.setRepeated(true);
    particle_texture.clear(sf::Color::Transparent);
    this->create(ConfigJSON::getRenderBackgroundParticleCount());
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
        std::normal_distribution<float> random_offset_x(0.0f, 0.25f); // Adjust the standard deviation as needed
        std::normal_distribution<float> random_offset_y(0.5f, 0.75f);

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
    std::normal_distribution<float> normal_z(0.01f, 0.5f);
    int count = ConfigJSON::getRenderBackgroundParticleCount();
    if (particle_vector.size() < count)
    {
        for (int i = 0; i < quantity; i++)
        {
            // Generate random positions with a normal distribution
            const sf::Vector3f particle_position(normal_x(rand_number), normal_y(rand_number), normal_z(rand_number));

            particle_vector.push_back(particle_position);
        }
    }
}

void background_particles::destroy()
{
    KJ::debug_output::print(__FILE__, "destroying background particles");
}

void background_particles::remove_out_of_bounds_particles()
{
    auto it = std::remove_if(particle_vector.begin(), particle_vector.end(), [this](const sf::Vector3f &particle) {
        // Assuming screen_width and screen_height are the dimensions of your screen
        return particle.x > this->screen_width || particle.y > this->screen_height;
    });

    particle_vector.erase(it, particle_vector.end());

    // Add a new particle if needed
    if (particle_vector.size() < ConfigJSON::getRenderBackgroundParticleCount())
    {
        create(1);
    }
}
