#include "Renderer.h"

renderer::renderer(const int screen_width, const int screen_height)
{
    KJ::debug_output::print(__FILE__, "Renderer Constructor", KJ::debug_output::MessageType::GOOD);
    try
    {
        _window_manager.set_display_resolution(screen_width, screen_height);
        _window_manager.set_canvas_resolution(screen_width * 2, screen_height * 2);
    }
    catch (std::exception &e)
    {
        KJ::debug_output::print(__FILE__, static_cast<std::string>(e.what()), KJ::debug_output::MessageType::FATAL);
    }
    canvas_resolution = _window_manager.get_canvas_resolution();
    display_resolution = _window_manager.get_display_resolution();

    if (canvas_resolution.height <= 1.0f || canvas_resolution.width <= 1.0f || display_resolution.height <= 1.0f ||
        display_resolution.width <= 1.0f)
    {
        KJ::debug_output::print(__FILE__, "Error: resolutions not set", KJ::debug_output::MessageType::FATAL);
        exit(1);
    }
    KJ::debug_output::print(__FILE__, "Window Manager Initialized", KJ::debug_output::MessageType::INFO);

    load_image_assets();
    load_font_assets();
    set_frame_limits();
    create_window();
    terrain_obj.set_screen_size(canvas_resolution.width, canvas_resolution.height);
    frame_time_buffer.resize(100, 0.0f); // Resize the buffer to 100 elements
    game_state.set_players(constants::default_starting_players);
    background_particles particles;
    if (constants::fullscreen == true)
    {
        toggle_full_screen(true);
    }
}

void renderer::create_window()
{
    window.create(sf::VideoMode(display_resolution.width, display_resolution.height), window_title, sf::Style::Default,
                  settings);
    set_icon(window);
}

void renderer::set_icon(sf::RenderWindow &window)
{
    sf::Image image = sf::Image{};
    if (!image.loadFromFile("Images/icon-master.png"))
    {
        KJ::debug_output::print(__FILE__, "Failed to load icon", KJ::debug_output::MessageType::FATAL);
    }
    window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
    KJ::debug_output::print(__FILE__, "Icon Set", KJ::debug_output::MessageType::GOOD);
}

void renderer::load_font_assets()
{
    if (file_system::file_exists(constants::font_file))
    {
        font.loadFromFile(constants::font_file); // Load a font for rendering text
    }
    else
    {
        KJ::debug_output::print(__FILE__, "ERROR: Failed to load font!\n", KJ::debug_output::MessageType::FATAL);
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
        KJ::debug_output::print(__FILE__, "Background image loaded to memory", KJ::debug_output::MessageType::INFO);
    }
    else
    {
        KJ::debug_output::print(__FILE__, "Failed to load background image!", KJ::debug_output::MessageType::FATAL);
    }
}

void renderer::set_background_image(const std::string &image)
{
    sf::Sprite background_sprite(background_texture);
    background_sprite.setPosition(0, 0);
    background_sprite.setScale(terrain_obj.get_scale().x, terrain_obj.get_scale().y);
    window.draw(background_sprite, render_state);
}

void renderer::load_terrain_image(const std::string &image)
{
    terrain_texture.loadFromFile(image);
}

void renderer::restart_game()
{
    this->display_settings_menu = false;
    this->display_menu = false;
    terrain_generator generator(display_resolution.width, display_resolution.height);
    terrain_obj.reset_terrain();
    start();
    create_players();
}

void renderer::start()
{
    size_t frame_index = 0;      // Index of the current frame time
    float frame_time_sum = 0.0f; // Sum of the frame times
    create_players();            // Create the players
    set_frame_limits();
    particles.set_screen_dimensions(display_resolution.width, display_resolution.height);

    while (window.isOpen())
    {
        delta_time = clock.restart(); // Restart the clock and save the elapsed time into deltaTime
        if (!this->display_menu)
        {
            handle_events(); // Handle events
        }
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
        frame_time_sum -= frame_time_buffer[frame_index];           // Subtract the oldest frame time from the sum
        frame_time_sum += delta_time.asSeconds();                   // Add the new frame time to the sum
        frame_time_buffer[frame_index] = delta_time.asSeconds();    // Store the new frame time in the buffer
        frame_index = (frame_index + 1) % frame_time_buffer.size(); // Increment the frame index and wrap around
        update_fps(frame_time_sum / static_cast<int>(frame_time_buffer.size())); // Pass the average frame time
        KJ::debug_output::print(__FILE__, "Starting Physics", KJ::debug_output::MessageType::INFO);
        run_gravity_simulation(); // Run the gravity simulation
        render();                 // Run the render function to send information to the window
    }
}

void renderer::run_gravity_simulation()
{
    if (this->enable_physics)
    {
        physics.apply_gravity(players, terrain_obj);
    }
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

void renderer::toggle_full_screen(bool startFull)
{
    if (startFull == true)
    {
        set_fullscreen(true);
    }
    else
    {
        set_fullscreen(!get_fullscreen());
    }
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::Vector2i mousePosition = sf::Mouse::getPosition();
    for (unsigned int i = 0; i < sf::VideoMode::getFullscreenModes().size(); ++i)
    {
        sf::VideoMode mode = sf::VideoMode::getFullscreenModes()[i];
        sf::IntRect screenRect(0, 0, mode.width, mode.height); // Create IntRect with top-left coordinates at (0, 0)
        if (screenRect.contains(mousePosition))
        {
            desktopMode = mode;
            break;
        }
    }
    if (get_fullscreen())
    {
        window.create(desktopMode, "Game Title", sf::Style::Fullscreen);
        display_resolution.width = window.getSize().x;
        display_resolution.height = window.getSize().y;
        KJ::debug_output::print(__FILE__,
                                "Window Object Resolution: " + std::to_string(window.getSize().x) + " x " +
                                    std::to_string(window.getSize().y),
                                KJ::debug_output::MessageType::GOOD);
    }
    else
    {
        window.create(sf::VideoMode(constants::window_screen_width, constants::window_screen_height), "Game Title",
                      sf::Style::Default);
        display_resolution.width = window.getSize().x;
        display_resolution.height = window.getSize().y;
        KJ::debug_output::print(__FILE__,
                                "Window Object Resolution: " + std::to_string(window.getSize().x) + " x " +
                                    std::to_string(window.getSize().y),
                                KJ::debug_output::MessageType::GOOD);
    }
    terrain_obj.set_screen_size(window.getSize().x, window.getSize().y);
    particles.set_screen_dimensions(window.getSize().x, window.getSize().y);
    set_icon(window);
    restart_game();
    set_frame_limits();
}

void renderer::rotatePlayerTurn()
{
    for (auto &player : players)
    {
        player.active_player = false;
    }
    if (currentPlayer == 0)
    {
        currentPlayer = 1;
    }
    else
    {
        currentPlayer = 0;
    }
}

void renderer::checkWinState()
{
    if (players[0].get_health() <= 0 || players[1].get_health() <= 0)
    {
        // this->display_menu = true;
        // this->display_settings_menu = false;
        if (players[0].get_health() <= 0)
        {
            sf::Text text = render_hud.set_winner(players[0].get_player_name());
            text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
            sf::RectangleShape modalMenu(sf::Vector2f(400, 200));
            modalMenu.setFillColor(sf::Color::White);
            modalMenu.setPosition(window.getSize().x / 2.0f - 200, window.getSize().y / 2.0f - 100);
            window.draw(modalMenu);
            window.draw(text);
            KJ::debug_output::print(__FILE__, "Player 2 Wins", KJ::debug_output::MessageType::GOOD);
        }
        else
        {
            sf::Text text = render_hud.set_winner(players[1].get_player_name());
            text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
            sf::RectangleShape modalMenu(sf::Vector2f(400, 200));
            modalMenu.setFillColor(sf::Color::White);
            modalMenu.setPosition(window.getSize().x / 2.0f - 200, window.getSize().y / 2.0f - 100);
            window.draw(modalMenu);
            window.draw(text);
            KJ::debug_output::print(__FILE__, "Player 1 Wins", KJ::debug_output::MessageType::GOOD);
        }
    }
}

void renderer::handle_events()
{
    // todo: create constants::audio enabled toggle
    players[currentPlayer].active_player = true;

    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Resized)
        {
            printf("Window Resized\n");
            _window_manager.set_canvas_resolution(window.getSize().x, window.getSize().y);
            sf::FloatRect visible_area(0, 0, event.size.width, event.size.height);
            terrain_obj.set_screen_size(event.size.width, event.size.height);
            window.setView(sf::View(visible_area));
        }
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        else if (event.type == sf::Event::MouseMoved)
        {
            mousePosition = sf::Mouse::getPosition(window);
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Left:
                if (projectiles.size() >= 1)
                {
                    break;
                }
                sounds_obj.rotate();
                players[currentPlayer].dec_angle(); // Decrease the angle of the player's turret
                break;
            case sf::Keyboard::Right:
                if (projectiles.size() >= 1)
                {
                    break;
                }
                sounds_obj.rotate();
                players[currentPlayer].inc_angle(); // Increase the angle of the player's turret
                break;
            case sf::Keyboard::Up:
                if (projectiles.size() >= 1)
                {
                    break;
                }
                players[currentPlayer].inc_power(); // Increase the power of the player's turret
                break;
            case sf::Keyboard::Down:
                if (projectiles.size() >= 1)
                {
                    break;
                }
                players[currentPlayer].dec_power(); // Decrease the power of the player's turret
                break;
            case sf::Keyboard::Space:
                if (projectiles.size() < 1)
                {
                    players[currentPlayer].fire();           // todo: is this needed? (possibly for player rotation?)
                    fire_projectile(players[currentPlayer]); // Fire a projectile
                }

                break;

            /*
                DEBUG AND MENU CONTROLS

              */
            case sf::Keyboard::Escape:
                // todo: implement menu system on pause screen
                this->display_settings_menu = false;
                this->display_menu = !this->display_menu;
                break;

            case sf::Keyboard::Q:
                // todo: implement menu system on pause screen
                window.close(); // Close the window
                break;
            case sf::Keyboard::F5:
                toggle_full_screen(); // Toggle fullscreen
                break;
            case sf::Keyboard::P:
                KJ::debug_output::print(__FILE__, "Firing Sound");
                sounds_obj.fire(); // play sound (debug)
                break;
            case sf::Keyboard::F1:
                restart_game(); // Restart the game
                break;
            case sf::Keyboard::G:
                // Enables or disables the grid used for debugging and pixel placement
                this->render_grid = !this->render_grid;
                break;
            case sf::Keyboard::F:
                // Toggle the frame rate display
                this->show_fps = !this->show_fps;
                break;
            case sf::Keyboard::O:
                // Toggle the physics simulation (should stop tanks from falling to the terrain)
                this->enable_physics = !this->enable_physics;
                break;
            case sf::Keyboard::I:
                this->enable_particles = !this->enable_particles;
                break;
            case sf::Keyboard::M:
                // KJ::debug_output::print(terrain_obj.get_pixel_map_size().to_string());
                break;
            case sf::Keyboard::N:
                terrain_obj.update_pixel_array(terrain_obj.get_scale().x, terrain_obj.get_scale().y);
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
            // Handling Menu Click
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
        else
        {
            explosions.erase(explosions.begin());
        }
    }
    for (auto &player : players)
    {
        player.set_on_ground(false);
    }
}

sf::FloatRect renderer::render_tank_hitbox(tank &tank)
{
    // Get the tank's position and dimensions
    float tankWidth = -tank.get_body_x() * 2;
    float tankHeight = -tank.get_body_y();
    float tankX = tank.get_x() - tankWidth / 2;
    float tankY = tank.get_y();

    // Create a rectangle shape for the hitbox
    sf::FloatRect hitbox_rect(tankX, tankY, tankWidth, tankHeight);
    sf::RectangleShape hitbox(sf::Vector2f(tankWidth, tankHeight));

    if (hitbox_rect.contains(sf::Vector2f(static_cast<float>(sf::Mouse::getPosition(window).x),
                                          static_cast<float>(sf::Mouse::getPosition(window).y))))
    {
        KJ::debug_output::print(__FILE__, "Mouse is in hitbox", KJ::debug_output::MessageType::GOOD);
    }

    if (constants::debug_hitboxes == true)
    {
        hitbox.setPosition(tankX, tankY);
        hitbox.setFillColor(sf::Color::Transparent);
        hitbox.setOutlineColor(sf::Color::Red);
        hitbox.setOutlineThickness(2.0f);
        // Draw the hitbox
        window.draw(hitbox);
    }
    return hitbox_rect;
}

void renderer::debug_display_hitboxes()
{
    for (auto &player : players)
    {
        render_tank_hitbox(player);
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
                    collide_with_terrain(projectile);
                }
                detect_tank(projectile);

                sf::CircleShape projectile_shape(projectile.get_width());
                projectile_shape.setPosition(projectile.get_x(), projectile.get_y());
                projectile_shape.setOrigin(projectile.get_width() / 2, projectile.get_width() / 2);
                projectile_shape.setFillColor(sf::Color::Cyan);
                window.draw(projectile_shape);
            }
            else
            {
                KJ::debug_output::print(__FILE__, "Finished rendering projectile, projectile was out of bounds");
                projectile.set_active(false);
            }
        }
        else
        {
            projectiles.erase(projectiles.begin());
        }
    }
}

void renderer::collide_with_tank(int player_index, projectile projectile)
{
    // players[player_index].get_health();
    KJ::debug_output::print(__FILE__, "Player: " + players[player_index].get_player_name() + " hit!",
                            KJ::debug_output::MessageType::GOOD);
    projectile.set_active(false);
    projectile_path.clear();
    players[player_index].reduce_health_by(projectile.get_damage());
    queue_render_explosion(projectile);
    sounds_obj.explode(); // Play sound (debug)
    rotatePlayerTurn();
}

void renderer::detect_tank(projectile &projectile)
{
    for (auto &player : players)
    {
        // Extract tank hitbox parameters
        float tankWidth = -player.get_body_x() * 2;
        float tankHeight = -player.get_body_y();
        float tankX = player.get_x() - tankWidth / 2;
        float tankY = player.get_y();
        float tankTop = tankY + tankHeight;
        float tankBottom = tankY - tankHeight;
        float tankLeft = tankX;
        float tankRight = tankX + tankWidth;

        // Create a rectangle shape for the hitbox
        sf::FloatRect hitbox_rect(tankX, tankY, tankWidth, tankHeight);
        sf::RectangleShape hitbox(sf::Vector2f(tankWidth, tankHeight));

        hitbox.setPosition(tankX, tankY);
        hitbox.setOutlineColor(sf::Color::Yellow);
        hitbox.setOutlineThickness(2.0f);
        // debug Draw the hitbox
        // window.draw(hitbox);

        // Extract projectile parameters
        float projectileX = projectile.get_x();
        float projectileY = projectile.get_y();
        float projectileWidth = projectile.get_width();
        float projectileHeight = projectile.get_height();
        float projectileTop = projectileY - projectileHeight;
        float projectileBottom = projectileY;
        float projectileLeft = projectileX;
        float projectileRight = projectileX + projectileWidth;

        if (!hitbox_rect.contains(sf::Vector2f(projectileX, projectileY)))
        {
            // Projectile is outside tank hitbox
            KJ::debug_output::print(
                __FILE__, "Tank Top:" + std::to_string(tankTop) + " Projectile Y: " + std::to_string(projectileY),
                KJ::debug_output::MessageType::INFO);
        }
        else
        {
            // Projectile entered tank hitbox
            KJ::debug_output::print(__FILE__,
                                    "Tank Top:" + std::to_string(tankTop) + " Tank Bottom: " +
                                        std::to_string(tankBottom) + " Projectile Y: " + std::to_string(projectileY),
                                    KJ::debug_output::MessageType::WARNING);
            projectile.set_active(false);
            collide_with_tank(player.get_index() - 1, projectile);
        }
    }
}

void renderer::collide_with_terrain(projectile &projectile)
{
    queue_render_explosion(projectile);
    sounds_obj.explode(); // Play sound (debug)
    projectile.set_active(false);
    projectile_path.clear();
    rotatePlayerTurn();
    KJ::debug_output::print(__FILE__, "Projectile hit terrain", KJ::debug_output::MessageType::GOOD);
}

void renderer::update_fps(const float average_frame_time)
{
    const float frame_rate = 1.0f / average_frame_time;
    fps_text.setString("FPS " + std::to_string(static_cast<int>(frame_rate)));
}

void renderer::render()
{
    window.clear(); // Clear the window
    // todo: move console output to a debug function - based on flag? seperate class?
    set_background_image("sky1.png");
    window.draw(terrain_obj.get_terrain_sprite(), render_state);
    for (const auto &player : players)
    {
        generate_tank(player); // Call the function to render the tank
    }

    render_projectiles();
    if (this->enable_particles)
    {
        window.draw(particles.render());
    }

    render_pixel_map_size();
    debug_display_hitboxes();
    render_explosions();                            // Render the explosions
    render_hud.render_player_data(window, players); // Render the player data
    render_fps();                                   // Render the FPS
    render_grid_lines(100);                         // Render the grid the int specified sets the pixel size of the grid
    display_menu_render();
    checkWinState();
    window.display(); // Display the window
}

void renderer::render_pixel_map_size()
{
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    text.setString("MapSize: " + std::to_string(terrain_obj.get_pixel_map_size()));
    text.setPosition(window.getSize().x / 2.0f, window.getSize().y - 100.0f);
    window.draw(text);
}

void renderer::display_menu_render()
{
    ModalWindow modal(window, font);

    if (this->display_menu)
    {
        // Create a button
        Button sampleButton(font, "Click me!", sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 50.f));
        sampleButton.onClick([]() { std::cout << "Button clicked!" << std::endl; });

        // Create a menu and add the button to it
        Menu sampleMenu(window);
        sampleMenu.addButton(std::move(sampleButton));

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    this->display_menu = false;
                }
                else if (event.key.code == sf::Keyboard::Q)
                {
                    modal.show("Are you sure you want to quit?");
                    modal.render();
                    KJ::debug_output::print(__FILE__, "Modal Q Pressed", KJ::debug_output::MessageType::ERROR);
                }
            }

            modal.handleEvent(event);

            sampleMenu.handleEvent(event);

            if (!modal.isModalVisible())
            {
                if (modal.getResult())
                {
                    // User clicked "Quit"
                    std::cout << "Quit game!" << std::endl;
                    KJ::debug_output::print(__FILE__, "Modal Confirm", KJ::debug_output::MessageType::ERROR);

                    window.close();
                }
                else
                {
                    // User has not yet responsed!
                    KJ::debug_output::print(__FILE__, "Modal Cancel", KJ::debug_output::MessageType::ERROR);
                    // User clicked "Cancel"
                    std::cout << "Canceled quitting!" << std::endl;
                    // Handle other actions if needed
                }
            }
        }
    }
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

void renderer::create_players()
{
    players.clear();
    KJ::debug_output::print(__FILE__, "Creating Players: #" + std::to_string(game_state.get_players()),
                            KJ::debug_output::MessageType::INFO);

    int player_count = game_state.get_players();
    int first_location = 80;
    int last_location = 160;
    int play_start_location_delta = 0;
    if (game_state.get_players() == 2)
    {

        play_start_location_delta = display_resolution.width - last_location;
    }
    else
    {
        play_start_location_delta = display_resolution.width / game_state.get_players();
    }

    const int player_start_angles = 180;
    for (int i = 0; i < player_count; ++i)
    {
        int x_location = 0;
        if (i == 0)
        {
            x_location = first_location;
        }
        else
        {
            x_location = first_location + play_start_location_delta;
        }
        auto tanks = tank(x_location, 100.0f, i + 1, "Player " + std::to_string(i + 1));
        tanks.set_angle(player_start_angles);
        players.push_back(tanks);
    }
}

void renderer::render_grid_lines(int pixels)
{
    if (this->render_grid)
    {
        // Draw vertical lines
        for (int x = 0; x < display_resolution.width; x += pixels)
        {
            sf::RectangleShape line(sf::Vector2f(1.0f, display_resolution.height));
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
        for (int y = 0; y < display_resolution.height; y += pixels)
        {
            sf::RectangleShape line(sf::Vector2f(display_resolution.width, 1.0f));
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
        const float text_x = (display_resolution.width - fps_text.getGlobalBounds().width - 40.0f);
        fps_text.setPosition(text_x, 10.0f);
        window.draw(fps_text);
    }
}

void renderer::generate_tank(tank tank)
{
    // Generate a tank
    // sf::Texture tank_texture = generate_tank_texture(tank.get_body_x(), tank.get_body_y());
    sf::Sprite tank_sprite;
    float tank_scale = constants::tank_scale;
    tank_sprite.setScale(tank_scale, tank_scale);
    tank_sprite.setTexture(tank_texture);
    tank_sprite.setPosition(tank.get_x(), tank.get_y() - 5.0f);
    tank_sprite.setOrigin(tank.get_origin_x(), tank.get_origin_y());
    //  Generate a turret
    /* sf::Texture turret_texture =
         generate_turret_texture(static_cast<int>(tank.get_turret_width()),
       static_cast<int>(tank.get_turret_height()));*/

    sf::Sprite turret_sprite;
    turret_sprite.setTexture(turret_texture);
    turret_sprite.setOrigin(tank.get_turret_width() / 2 + 7.5f, 4.0f);
    turret_sprite.setPosition(tank.get_x(), tank.get_y() - (tank.get_body_y() * tank_scale) + 5.0f);
    turret_sprite.setRotation(tank.get_angle());
    // todo: perhaps generate to a texture and then render the texture to the screen?
    //  Draw both the tank and turret
    window.draw(turret_sprite);
    window.draw(tank_sprite);
    get_turret_projectile_origin(tank);
}

void renderer::queue_render_explosion(const projectile &projectile)
{
    const sf::Time duration = sf::seconds(2.0f);
    const explosion explosion(projectile.get_x(), projectile.get_y(), 40.0f, duration);
    terrain_obj.update_terrain_with_explosion(projectile.get_x(), projectile.get_y(),
                                              explosion.get_explosion_size() * 3, explosion.get_explosion_size() * 3);
    explosions.push_back(explosion);
}

sf::Vector2f renderer::get_turret_projectile_origin(tank &tank)
{

    const sf::Vector2f tank_top_center(tank.get_x(), tank.get_y() - tank.get_body_y());
    // debug function for projectile origin when spawned
    /*sf::CircleShape circle(5.0f);
    circle.setPosition(tank_top_center);
    circle.setFillColor(sf::Color::Red);
    window.draw(circle);*/
    // Calculate the offset based on the turret's angle
    const float turret_angle_radians = -tank.get_angle() * (3.14159265f / 180.0f);
    const float turret_length = tank.get_turret_height();
    const sf::Vector2f turret_tip_offset(turret_length * std::sin(turret_angle_radians),
                                         turret_length * std::cos(turret_angle_radians));

    // Calculate the position of the second point relative to the top center of the tank
    const sf::Vector2f turret_tip_position = tank_top_center + turret_tip_offset;
    return turret_tip_position;
}

void renderer::fire_projectile(tank tank)
{
    // Fire a projectile
    // Create a new projectile and add it to the vector
    projectile cannon(tank);
    cannon.set_explosion_duration(3.0f);
    sounds_obj.fire(); // play sound (debug)
    const float launch_angle =
        tank.get_angle() - 90.0f; // Set your desired launch angle in degrees and correct 90 degrees

    cannon.launch(get_turret_projectile_origin(tank).x, get_turret_projectile_origin(tank).y, launch_angle);
    // Add the projectile to the container
    projectiles.push_back(cannon);
}
