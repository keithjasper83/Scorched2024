#pragma once
#include "Renderer.h"

renderer::renderer(const int screen_width, const int screen_height)
{
    this->screen_width = screen_width;
    this->screen_height = screen_height;
    // Create window settings
    create_settings();
    // Load  Assets here
    load_image_assets();
    load_font_assets();
    // Set Default frame-limits (mostly for development but may become a feature)
    set_frame_limits();
    // Make the render window
    create_window();
    // Set window sizes for terrain scaling
    terrain_obj.set_screen_size(screen_width, screen_height);
    // create 100 elements in frame-time buffer to allow for an average
    frame_time_buffer.resize(100, 0.0f); // Resize the buffer to 100 elements
    // set this from game menu (2 is default)
    set_player_count(2);
}

void renderer::create_window()
{
    if (this->fullscreen)
    {
        window.create(sf::VideoMode::getDesktopMode(), window_title, sf::Style::Fullscreen, settings);
    }
    else
    {
        window.create(sf::VideoMode(screen_width, screen_height), window_title, sf::Style::Default, settings);
    }
    update_window_size();
}

void renderer::load_font_assets()
{
    if (file_system::file_exists(constants::font_file))
    {
        font.loadFromFile(constants::font_file); // Load a font for rendering text
    }
    else
    {
        printf("ERROR: Failed to load font!\n");
    }
}

void renderer::create_settings()
{
    settings.depthBits = 24;        // Depth buffer settings (adjust as needed)
    settings.stencilBits = 8;       // Stencil buffer settings (adjust as needed)
    settings.antialiasingLevel = 8; // Anti-aliasing level (adjust as needed)
    settings.majorVersion = 3;      // Set the major OpenGL version
    settings.minorVersion = 3;      // Set the minor OpenGL version
    settings.sRgbCapable = true;    // Enable sRGB (gamma correction)
}

void renderer::load_image_assets()
{
    load_background_image("Images/sky1.png");
    load_terrain_image("Images/rock1.jpg");
}

void renderer::load_background_image(const std::string &image)
{
    if (file_system::file_exists(image))
    {
        background_texture.loadFromFile(image);
        std::cout << "Background image loaded to memory" << std::endl;
    }
    else
    {
        std::cout << "ERROR: Failed to load background image!" << std::endl;
    }
}

void renderer::set_background_image(const std::string &image)
{
    sf::Sprite background_sprite(background_texture);
    background_sprite.setPosition(0, 0);
    background_sprite.setScale(terrain_obj.get_scale().x, terrain_obj.get_scale().y);
    window.draw(background_sprite, render_state);
    // std::cout << "Background image set and Draw Called" << std::endl;
}

void renderer::load_terrain_image(const std::string &image)
{
    terrain_texture.loadFromFile(image);
}

void renderer::restart_game()
{
    terrain_generator generator(this->screen_width, this->screen_height);
    create_players();
}

void renderer::start()
{
    size_t frame_index = 0;      // Index of the current frame time
    float frame_time_sum = 0.0f; // Sum of the frame times
    create_players();            // Create the players
    set_frame_limits();          // Set the frame limits
    while (window.isOpen())
    {
        delta_time = clock.restart(); // Restart the clock and save the elapsed time into deltaTime
        update_window_size();         // Update the window size
        handle_events();              // Handle events
        for (auto it = explosions.begin(); it != explosions.end();)
        {
            it->update(delta_time); // Update the explosion
            if (!it->is_active())
            {
                it = explosions.erase(it); // Remove inactive explosions
            }
            else
            {
                ++it;
            }
        }
        // Update the frame time buffer and sum
        frame_time_sum -= frame_time_buffer[frame_index];           // Subtract the oldest frame time from the sum
        frame_time_sum += delta_time.asSeconds();                   // Add the new frame time to the sum
        frame_time_buffer[frame_index] = delta_time.asSeconds();    // Store the new frame time in the buffer
        frame_index = (frame_index + 1) % frame_time_buffer.size(); // Increment the frame index and wrap around
        update_fps(frame_time_sum / static_cast<int>(frame_time_buffer.size())); // Pass the average frame time
        // todo: for some reason this function returning false prevents background from loading? only when physics is
        // enabled? so does that even mean its a problem? Physics should always be enabled
        run_gravity_simulation();
        render(); // Run the render function to send information to the window
    }
}

void renderer::run_gravity_simulation()
{
    if (this->enable_physics)
    {
        physics.apply_gravity(players, terrain_obj);
    }
}

void renderer::update_window_size()
{
    this->window_width = fullscreen ? window.getSize().x : this->screen_width;
    this->window_height = fullscreen ? window.getSize().y : this->screen_height;
}

void renderer::set_frame_limits()
{
    window.setFramerateLimit(constants::frame_rate); // Limit the frame rate to 60 FPS
    window.setVerticalSyncEnabled(false);            // Enable vertical sync
}

void renderer::set_fullscreen(bool fullscreen)
{
    this->fullscreen = fullscreen;
}

bool renderer::get_fullscreen() const
{
    return this->fullscreen;
}

void renderer::toggle_full_screen()
{
    set_fullscreen(!get_fullscreen());
    if (this->get_fullscreen())
    {
        window.create(sf::VideoMode::getDesktopMode(), constants::game_title, sf::Style::Fullscreen);
        terrain_obj.set_screen_size(window.getSize().x, window.getSize().y);
        restart_game();
    }
    else
    {
        window.create(sf::VideoMode(screen_width, screen_height), constants::game_title, sf::Style::Default);
        terrain_obj.set_screen_size(window.getSize().x, window.getSize().y);
        restart_game();
    }
    set_frame_limits();
}

void renderer::handle_events()
{
    players[0].active_player = true;
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Resized)
        {
            printf("Window Resized\n");
            update_window_size();
            sf::FloatRect visible_area(0, 0, event.size.width, event.size.height);
            terrain_obj.set_screen_size(event.size.width, event.size.height);
            window.setView(sf::View(visible_area));
        }
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Escape:
                // todo: implement menu system on pause screen
                window.close(); // Close the window
                break;
            case sf::Keyboard::F5:
                toggle_full_screen(); // Toggle fullscreen
                break;
            case sf::Keyboard::P:
                std::cout << "Firing Sound" << std::endl;
                sounds_obj.fire(); // play sound (debug)
                break;
            case sf::Keyboard::F1:
                restart_game(); // Restart the game
                break;
            case sf::Keyboard::G:
                // Enables or disables the grid used for debugging and pixel placement
                if (this->render_grid)
                {
                    this->render_grid = false;
                }
                else
                {
                    this->render_grid = true;
                }
                break;
            case sf::Keyboard::Left:
                players[0].dec_angle(); // Decrease the angle of the player's turret
                break;
            case sf::Keyboard::Right:
                players[0].inc_angle(); // Increase the angle of the player's turret
                break;
            case sf::Keyboard::Up:
                players[0].inc_power(); // Increase the power of the player's turret
                break;
            case sf::Keyboard::Down:
                players[0].dec_power(); // Decrease the power of the player's turret
                break;
            case sf::Keyboard::Space:
                players[0].fire();           // todo: is this needed? (possibly for player rotation?)
                fire_projectile(players[0]); // Fire a projectile
                break;
            case sf::Keyboard::F:
                // Toggle the frame rate display
                if (this->show_fps)
                {
                    this->show_fps = false;
                }
                else
                {
                    this->show_fps = true;
                }
                break;
            case sf::Keyboard::O:
                // Toggle the physics simulation (should stop tanks from falling to the terrain)
                if (this->enable_physics)
                {
                    this->enable_physics = false;
                }
                else
                {
                    this->enable_physics = true;
                }
                break;
            default:
                break;
            }
        }

        // mouse events
        //  TODO: clean up mouse movements and clicks
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            is_dragging = true;
        }
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
        {
            is_dragging = false;
        }
        if ((is_dragging && event.type == sf::Event::MouseMoved) ||
            (sf::Event::MouseButtonPressed == true && event.mouseButton.button == sf::Mouse::Left))
        {
            const sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
            const float scale_x = terrain_obj.get_terrain_sprite().getScale().x;
            const float scale_y = terrain_obj.get_terrain_sprite().getScale().y;
            const sf::Vector2f scaled_mouse_pos(mouse_pos.x / scale_x, mouse_pos.y / scale_y);

            terrain_obj.update_terrain_with_collision(static_cast<int>(scaled_mouse_pos.x),
                                                      static_cast<int>(scaled_mouse_pos.y));
        }
        if (sf::Event::MouseButtonPressed == true && event.mouseButton.button == sf::Mouse::Right)
        {
            sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
            const float scale_x = terrain_obj.get_terrain_sprite().getScale().x;
            const float scale_y = terrain_obj.get_terrain_sprite().getScale().y;
            sf::Vector2f scaled_mouse_pos(mouse_pos.x / scale_x, mouse_pos.y / scale_y);

            terrain_obj.update_terrain_with_collision(static_cast<int>(scaled_mouse_pos.x),
                                                      static_cast<int>(scaled_mouse_pos.y));
        }
    }
}

void renderer::render_explosions()
{
    for (const explosion &explosion : explosions)
    {
        if (explosion.is_active())
        {
            explosion.render(window);
        }
    }
    for (auto &player : players)
    {
        player.set_on_ground(false);
    }
}

void renderer::render_projectiles()
{
    const float gravity = physics.get_gravity(); // Get the gravity value

    for (auto &projectile : projectiles)
    {
        if (projectile.is_active())
        {
            const float time_elapsed = delta_time.asSeconds() * constants::projectile_speed;
            projectile.update_position(time_elapsed, gravity);
            if (projectile.get_y() < window.getSize().y - 10 && projectile.get_y() > 0 + 10)
            {
                const float projectile_x = projectile.get_x() + (projectile.width / 2);
                const float projectile_y = projectile.get_y() + (projectile.height / 2);
                if (projectile.get_start_x() == 0.0f && projectile.get_start_y() == 0.0f)
                {
                    projectile.set_start_x(projectile_x);
                    projectile.set_start_y(projectile_y);
                }
                projectile_path.append(
                    sf::Vertex(sf::Vector2f(projectile.get_x() + (projectile.get_width() / 2),
                                            projectile.get_y() + (projectile.get_height() / 2)),
                               sf::Color::Red)); // add a vertex to the vertex array for the projectile path
                window.draw(projectile_path);    // draw the projectile path
                if (terrain_obj.transparent_at_pixel(projectile_x, projectile_y))
                {
                    queue_render_explosion(projectile);
                    sounds_obj.explode(); // Play sound (debug)
                    projectile.set_active(false);
                    projectile_path.clear();
                }
                sf::CircleShape projectile_shape(projectile.get_width());
                projectile_shape.setPosition(projectile.get_x(), projectile.get_y());
                projectile_shape.setOrigin(projectile.get_width() / 2, projectile.get_width() / 2);
                projectile_shape.setFillColor(sf::Color::Cyan);
                window.draw(projectile_shape);
            }
            else
            {
                std::cout << "Finished rendering projectile, projectile was out of bounds" << std::endl;
            }
        }
    }
}

void renderer::update_fps(const float average_frame_time)
{
    const float frame_rate = 1.0f / average_frame_time;
    fps_text.setString("FPS: " + std::to_string(static_cast<int>(frame_rate)));
}

void renderer::render()
{
    window.clear();
    set_background_image("sky1.png");
    window.draw(terrain_obj.get_terrain_sprite(), render_state);
    for (const auto &player : players)
    {
        generate_tank(player); // Call the function to render the tank
    }
    render_hud.render_player_data(window, players);
    window.draw(render_hud.create_centered_red_window());
    render_projectiles();
    render_explosions();

    // renderHUD(players);
    render_fps();
    render_grid_lines(100); // Render the grid the int specified sets the pixel size of the grid
    window.display();
}

sf::Texture renderer::generate_tank_texture(const int tank_width, const int tank_height)
{
    sf::Image tank_image;
    sf::Texture tank_texture;
    tank_image.create(tank_width, tank_height, sf::Color::Red);
    for (int x = 0; x < tank_width; ++x)
    {
        for (int y = 0; y < tank_height; ++y)
        {
            tank_image.setPixel(x, y, sf::Color::Cyan);
        }
    }
    tank_texture.loadFromImage(tank_image);
    return tank_texture;
}

sf::Texture renderer::generate_turret_texture(const int turret_width, const int turret_height)
{
    // Generate a texture for the turret
    sf::Image turret_image;
    sf::Texture turret_texture;
    turret_image.create(turret_width, turret_height, sf::Color::Red);
    for (int x = 0; x < turret_width; ++x)
    {
        for (int y = 0; y < turret_height; ++y)
        {
            turret_image.setPixel(x, y, sf::Color::Red);
        }
    }
    turret_texture.loadFromImage(turret_image);
    return turret_texture;
}

void renderer::set_player_count(const int player_count)
{
    this->player_count = player_count;
}

int renderer::get_player_count() const
{
    return this->player_count;
}

void renderer::create_players()
{
    players.clear();
    printf("Creating %d players\n", get_player_count());
    const int second_location = window.getSize().x - 50;
    const int player_start_locations[2] = {50, second_location};
    for (int i = 0; i < get_player_count(); ++i)
    {
        auto tanks = tank(player_start_locations[i], 100.0f, i + 1, "Player " + std::to_string(i + 1));
        players.push_back(tanks);
    }
}

void renderer::render_grid_lines(int pixels)
{
    if (this->render_grid)
    {
        // Draw vertical lines
        for (int x = 0; x < window_width; x += pixels)
        {
            sf::RectangleShape line(sf::Vector2f(1.0f, static_cast<float>(window_height)));
            std::string line_label = std::to_string(x);
            sf::Text line_text;
            line_text.setFont(font);
            line_text.setCharacterSize(24);
            line_text.setFillColor(sf::Color::White);
            line_text.setString(line_label);
            line_text.setPosition(x, 0.0f);
            window.draw(line_text);
            line.setPosition(x, 0.0f);
            line.setFillColor(sf::Color::Yellow);
            window.draw(line);
        }
        // Draw horizontal lines
        for (int y = 0; y < window_height; y += pixels)
        {
            sf::RectangleShape line(sf::Vector2f(static_cast<float>(window_width), 1.0f));
            std::string line_label = std::to_string(y);
            sf::Text line_text;
            line_text.setFont(font);
            line_text.setCharacterSize(24);
            line_text.setFillColor(sf::Color::White);
            line_text.setString(line_label);
            line_text.setPosition(0.0f, y);
            window.draw(line_text);
            line.setPosition(0.0f, y);
            line.setFillColor(sf::Color::Yellow);
            window.draw(line);
        }
    }
}

void renderer::render_fps()
{
    if (this->show_fps)
    {
        fps_text.setFont(font);
        fps_text.setCharacterSize(24);
        fps_text.setFillColor(sf::Color::Red);
        const float text_x =
            static_cast<float>(window_width) - (static_cast<int>(window_width) - fps_text.getGlobalBounds().width) / 8;
        fps_text.setPosition(text_x, 10.0f);
        window.draw(fps_text);
    }
}

void renderer::generate_tank(tank tank)
{
    // Generate a tank
    sf::Texture tank_texture = generate_tank_texture(tank.get_body_x(), tank.get_body_y());
    sf::Sprite tank_sprite;
    tank_sprite.setTexture(tank_texture);
    tank_sprite.setPosition(tank.get_x(), tank.get_y());
    tank_sprite.setOrigin(tank.get_origin_x(), tank.get_origin_y());
    // Generate a turret
    sf::Texture turret_texture = generate_turret_texture(tank.get_turret_width(), tank.get_turret_height());
    sf::Sprite turret_sprite;
    turret_sprite.setTexture(turret_texture);
    turret_sprite.setOrigin(tank.get_turret_width() / 2, 0);
    turret_sprite.setPosition(tank.get_x(), tank.get_y() - (tank.get_body_y() / 2.0f) + 5);
    turret_sprite.setRotation(tank.get_angle());
    // todo: perhaps generate to a texture and then render the texture to the screen?
    //  Draw both the tank and turret
    window.draw(tank_sprite);
    window.draw(turret_sprite);
}

void renderer::queue_render_explosion(projectile projectile)
{
    sf::Time duration = sf::seconds(2.0f);
    explosion explosion(projectile.get_x(), projectile.get_y(), 40.0f, duration);
    terrain_obj.update_terrain_with_explosion(projectile.get_x(), projectile.get_y(),
                                              explosion.get_explosion_size() * 3, explosion.get_explosion_size() * 3);
    explosions.push_back(explosion);
}

void renderer::fire_projectile(tank tank)
{
    // Fire a projectile
    // Create a new projectile and add it to the vector
    projectile cannon(tank);
    cannon.set_explosion_duration(3.0f);
    sounds_obj.fire(); // play sound (debug)
    // Determine the initial position (initialX and initialY) and launch angle
    float initial_x = static_cast<int>(calculate_projectile_origin_x(tank)); // Set your desired initial X position
    float initial_y =
        static_cast<int>(calculate_projectile_origin_y(tank, cannon)); // Set your desired initial Y position
    float launch_angle = tank.get_angle();                             // Set your desired launch angle in degrees

    cannon.launch(initial_x, initial_y, launch_angle - 90);
    // Add the projectile to the container
    projectiles.push_back(cannon);
}

int renderer::calculate_projectile_origin_x(tank tank)
{
    // Calculate the origin of the projectile considering turret rotation, width, and the tip of the turret
    const float half_turret_width = tank.get_turret_width() / 2.0f;
    const float turret_tip_offset_x = half_turret_width * std::cos((tank.get_angle() + 90.0f) * M_PI / 180.0f);
    const int origin_x = tank.get_x() - static_cast<int>(turret_tip_offset_x);
    return origin_x;
}

int renderer::calculate_projectile_origin_y(tank tank, projectile &projectile)
{
    // Calculate the origin of the projectile at the tip of the turret
    const float half_turret_width = tank.get_turret_width() / 2.0f;
    const float half_projectile_size = projectile.get_width() / 2.0f;
    const float turret_tip_offset_y = -(tank.get_turret_height() / 2.0f); // Adjusted here
    const int origin_y = tank.get_y() - (tank.get_body_y() - static_cast<int>(turret_tip_offset_y)) -
                         static_cast<int>(half_turret_width + half_projectile_size);
    return origin_y;
}
