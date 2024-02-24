#include "Renderer.h"

renderer::renderer(const int screen_width, const int screen_height)
{
    initializeRenderer(screen_width, screen_height);
}

void renderer::initializeRenderer(const int screen_width, const int screen_height)
{
    try
    {
        ConfigJSON::loadConfigText();
        validateResolutions(screen_width, screen_height);
        loadImageAssets();
        loadFontAsset();
        engineSetFrameLimits(window_);
        create_window();
        terrain_obj.set_screen_size(canvas_resolution.width, canvas_resolution.height);
        frame_time_buffer.resize(100, 0.0f); // Resize the buffer to 100 elements
        game_state.set_players(ConfigJSON::getEngineDefaultStartPlayers());
        if (ConfigJSON::getDefaultFullScreen() == true)
        {
            toggle_full_screen(true);
        }
    }
    catch (std::exception &e)
    {
        KJ::debug_output::print(__FILE__, e.what(), KJ::debug_output::MessageType::FATAL);
    }
}

void renderer::validateResolutions(const int screen_width, const int screen_height)
{
    _window_manager.set_display_resolution(screen_width, screen_height);
    _window_manager.set_canvas_resolution(screen_width * 2, screen_height * 2);
    canvas_resolution = _window_manager.get_canvas_resolution();
    display_resolution = _window_manager.get_display_resolution();

    if (canvas_resolution.height <= 1.0f || canvas_resolution.width <= 1.0f || display_resolution.height <= 1.0f ||
        display_resolution.width <= 1.0f)
    {
        throw std::runtime_error("Error: invalid resolution");
    }
    KJ::debug_output::print(__FILE__, "Window Manager Initialized", KJ::debug_output::MessageType::INFO);
}

void renderer::create_window()
{
    window_.create(sf::VideoMode(display_resolution.width, display_resolution.height), window_title, sf::Style::Default,
                   settings);
    setIcon(window_);
}

void renderer::setIcon(sf::RenderWindow &window)
{
    if (iconImage_.getSize().x == 0 || iconImage_.getSize().y == 0)
    {
        // Load icon image if it hasn't been loaded before
        if (!loadIconImage())
        {
            // Failed to load icon, handle error or use a default icon
            KJ::debug_output::print(__FILE__, "Failed to load icon", KJ::debug_output::MessageType::ERROR);
            // Optionally, set a default icon or handle the error gracefully
        }
    }
    window.setIcon(iconImage_.getSize().x, iconImage_.getSize().y, iconImage_.getPixelsPtr());
    KJ::debug_output::print(__FILE__, "Icon Set", KJ::debug_output::MessageType::GOOD);
}

bool renderer::loadIconImage()
{
    if (!file_system::file_exists("Images/icon-master.png"))
    {
        // Optionally, provide a default icon or handle the absence of the icon file
        return false;
    }
    if (!iconImage_.loadFromFile("Images/icon-master.png"))
    {
        // Failed to load icon image
        return false;
    }
    return true;
}

void renderer::loadFontAsset()
{
    if (file_system::file_exists(ConfigJSON::getEngineFontFile()))
    {
        if (!font.loadFromFile(ConfigJSON::getEngineFontFile()))
        {
            KJ::debug_output::print(__FILE__, "ERROR: Failed to load font: " + ConfigJSON::getEngineFontFile(),
                                    KJ::debug_output::MessageType::FATAL);
            // Handle failure gracefully or use a default font
        }
    }
    else
    {
        KJ::debug_output::print(__FILE__, "ERROR: Font file not found: " + ConfigJSON::getEngineFontFile(),
                                KJ::debug_output::MessageType::FATAL);
        // Handle absence of font file gracefully or use a default font
    }
}

void renderer::createSettings()
{
    // Modify settings here based on requirements or user-defined parameters
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.sRgbCapable = true;
}

void renderer::loadImageAssets()
{
    loadImage("Images/sky1.png", background_texture);
    loadImage("Images/rock1.jpg", terrain_texture);
}

void renderer::loadImage(const std::string &image, sf::Texture &texture)
{
    if (file_system::file_exists(image))
    {
        if (!texture.loadFromFile(image))
        {
            // Failed to load texture
            KJ::debug_output::print(__FILE__, "Failed to load image: " + image, KJ::debug_output::MessageType::FATAL);
        }
        else
        {
            // Successfully loaded texture
            KJ::debug_output::print(__FILE__, "Image loaded: " + image, KJ::debug_output::MessageType::INFO);
        }
    }
    else
    {
        // File doesn't exist
        KJ::debug_output::print(__FILE__, "Image file not found: " + image, KJ::debug_output::MessageType::FATAL);
    }
}

void renderer::renderBackground(sf::RenderWindow &renderWindow)
{
    // Assuming background_texture is already loaded
    sf::Sprite background_sprite(background_texture);
    background_sprite.setPosition(0, 0);
    background_sprite.setScale(terrain_obj.get_scale().x, terrain_obj.get_scale().y);
    renderWindow.draw(background_sprite, render_state);
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
    engineCreatePlayers();
}

void renderer::engineUpdate(sf::RenderWindow &renderWindow)
{
    delta_time = clock.restart();          // Restart the clock and save the elapsed time into deltaTime
    engineHandleInputEvents(renderWindow); // Handle events

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
    fpsFrameTimeSum -= frame_time_buffer[fpsCount];       // Subtract the oldest frame time from the sum
    fpsFrameTimeSum += delta_time.asSeconds();            // Add the new frame time to the sum
    frame_time_buffer[fpsCount] = delta_time.asSeconds(); // Store the new frame time in the buffer
    fpsCount = (fpsCount + 1) % frame_time_buffer.size(); // Increment the frame index and wrap around
    update_fps(fpsFrameTimeSum / static_cast<int>(frame_time_buffer.size())); // Pass the average frame time
}

void renderer::engineSetup(sf::RenderWindow &renderWindow)
{

    engineCreatePlayers(); // Create the players
    engineSetFrameLimits(renderWindow);
    particles.set_screen_dimensions(display_resolution.width, display_resolution.height);
    engineLoadProjectiles();
}

void renderer::frameUpdate()
{
}

void renderer::start()
{
    ConfigJSON::loadConfigText();

    engineSetup(window_);
    while (window_.isOpen())
    {
        engineUpdate(window_);
        KJ::debug_output::print(__FILE__, "Starting Physics", KJ::debug_output::MessageType::INFO);
        physicsRun();    // Run the gravity simulation
        render(window_); // Run the render function to send information to the window
    }
}

void renderer::physicsRun()
{
    if (this->enable_physics)
    {
        physics.apply_gravity(players, terrain_obj);
    }
}

void renderer::engineSetFrameLimits(sf::RenderWindow &renderWindow)
{
    renderWindow.setFramerateLimit(ConfigJSON::getMaxFPS()); // Limit the frame rate to 60 FPS
    renderWindow.setVerticalSyncEnabled(false);              // Enable vertical sync
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
        window_.create(desktopMode, "Game Title", sf::Style::Fullscreen);
        display_resolution.width = window_.getSize().x;
        display_resolution.height = window_.getSize().y;
        KJ::debug_output::print(__FILE__,
                                "Window Object Resolution: " + std::to_string(window_.getSize().x) + " x " +
                                    std::to_string(window_.getSize().y),
                                KJ::debug_output::MessageType::GOOD);
    }
    else
    {
        window_.create(sf::VideoMode(ConfigJSON::getWindowedResolutionX(), ConfigJSON::getWindowedResolutionY()),
                       "Game Title", sf::Style::Default);
        display_resolution.width = window_.getSize().x;
        display_resolution.height = window_.getSize().y;
        KJ::debug_output::print(__FILE__,
                                "Window Object Resolution: " + std::to_string(window_.getSize().x) + " x " +
                                    std::to_string(window_.getSize().y),
                                KJ::debug_output::MessageType::GOOD);
    }
    terrain_obj.set_screen_size(window_.getSize().x, window_.getSize().y);

    particles.set_screen_dimensions(window_.getSize().x, window_.getSize().y);
    setIcon(window_);
    restart_game();
    engineSetFrameLimits(window_);
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

void renderer::renderWinScreen(sf::RenderWindow &renderWindow, std::string winMessage)
{
    sf::Text text = render_hud.set_winner(winMessage);
    text.setPosition(renderWindow.getSize().x / 2.0f - text.getLocalBounds().width / 2.0f,
                     renderWindow.getSize().y / 2.0f - text.getLocalBounds().height / 2.0f);
    sf::RectangleShape modalMenu(sf::Vector2f(400, 200));
    modalMenu.setFillColor(sf::Color::White);
    modalMenu.setPosition(renderWindow.getSize().x / 2.0f - modalMenu.getSize().x / 2.0f,
                          renderWindow.getSize().y / 2.0f - modalMenu.getSize().y / 2.0f);
    MenuManager winStateButtons(renderWindow, font);

    renderWindow.draw(modalMenu);
    renderWindow.draw(text);
    winStateButtons.renderWinStateButtons(std::bind(&renderer::restart_game, this),
                                          std::bind(&renderer::engineQuitGame, this));
}

void renderer::checkWinState(sf::RenderWindow &renderWindow)
{
    if (players[0].get_health() <= 0 || players[1].get_health() <= 0)
    {
        // this->display_menu = true;
        // this->display_settings_menu = false;
        MenuModalWindow modalWindow(renderWindow, font);

        if (players[0].get_health() <= 0)
        {
            renderWinScreen(renderWindow, "Player 2 Wins");
        }
        else
        {
            renderWinScreen(renderWindow, "Player 1 Wins");
        }
    }
}

void renderer::engineHandleWindowResize(sf::RenderWindow &renderWindow, sf::Event &event)
{
    printf("Window Resized\n");
    _window_manager.set_canvas_resolution(renderWindow.getSize().x, renderWindow.getSize().y);
    sf::FloatRect visible_area(0, 0, event.size.width, event.size.height);
    terrain_obj.set_screen_size(event.size.width, event.size.height);
    renderWindow.setView(sf::View(visible_area));
}

void renderer::engineHandleWindowClose(sf::RenderWindow &renderWindow)
{
    renderWindow.close();
}

void renderer::engineHandleMouseMove(sf::RenderWindow &renderWindow, sf::Vector2i &mousePosition)
{
    mousePosition = sf::Mouse::getPosition(renderWindow);
}

void renderer::engineHandleUserInputFire(sf::RenderWindow &renderWindow)
{
    terrain_obj.cache_terrain_image();
    if (projectiles.size() < 1)
    {
        players[currentPlayer].fire();

        // Assuming 'selectedProjectileType' is the user's choice
        ProjectileTypes::ProjectileType selectedProjectileType =
            static_cast<ProjectileTypes::ProjectileType>(players[currentPlayer].get_active_projectile());
        // Create a projectile using the factory
        std::unique_ptr<ProjectileInterface> projectile = ProjectileFactory::CreateProjectile(
            ProjectileFactory::projectileTypeToString(selectedProjectileType), players[currentPlayer]);

        sounds_obj.fire(); // play sound (debug)

        if (projectile)
        {
            const float launch_angle = players[currentPlayer].get_angle();
            try
            {
                // Use a temporary variable to store the origin, avoiding duplicate function calls
                sf::Vector2f origin = get_turret_projectile_origin(players[currentPlayer]);
                projectile->launch(origin.x, origin.y, launch_angle, players[currentPlayer].get_power());

                projectile->set_active(true);
                std::cout << "Start Coordinates: " << projectile->get_start_x() << ", " << projectile->get_start_y()
                          << std::endl;
                KJ::debug_output::print(__FILE__, "Projectile launched", KJ::debug_output::MessageType::GOOD);
            }
            catch (const std::exception &e)
            {
                KJ::debug_output::print(__FILE__, e.what(), KJ::debug_output::MessageType::FATAL);
                return; // Exit the function if launching fails
            }

            projectiles.emplace_back(std::move(projectile));
        }
        else
        {
            // Handle the case where projectile creation fails
            KJ::debug_output::print(__FILE__, "Failed to create projectile", KJ::debug_output::MessageType::ERROR);
        }
    }
}

void renderer::engineHandleUserInputRotateLeft(sf::RenderWindow &renderWindow)
{
    if (projectiles.size() >= 1)
    {
        return;
    }
    sounds_obj.rotate();
    players[currentPlayer].dec_angle(); // Decrease the angle of the player's turret
};
void renderer::engineHandleUserInputRotateRight(sf::RenderWindow &renderWindow)
{
    if (projectiles.size() >= 1)
    {
        return;
    }
    sounds_obj.rotate();
    players[currentPlayer].inc_angle(); // Increase the angle of the player's turret
};
void renderer::engineHandleUserInputPowerUp(sf::RenderWindow &renderWindow)
{
    if (projectiles.size() >= 1)
    {
        return;
    }
    players[currentPlayer].inc_power(); // Increase the power of the player's turret
};
void renderer::engineHandleUserInputPowerDown(sf::RenderWindow &renderWindow)
{
    if (projectiles.size() >= 1)
    {
        return;
    }
    players[currentPlayer].dec_power(); // Decrease the power of the player's turret
};

void renderer::engineHandleUserInputSettingsSave(sf::RenderWindow &renderWindow)
{
    ConfigJSON::saveConfigText();
    std::cout << "intConfigData size: " << ConfigJSON::intConfigData.size() << std::endl;
    std::cout << "boolConfigData size: " << ConfigJSON::boolConfigData.size() << std::endl;
    std::cout << "stringConfigData size: " << ConfigJSON::stringConfigData.size() << std::endl;
    // exit(0);
}

void renderer::engineHandleUserInputConfigToConsole(sf::RenderWindow &renderWindow)
{
    std::cout << "intConfigData size: " << ConfigJSON::intConfigData.size() << std::endl;
    std::cout << "boolConfigData size: " << ConfigJSON::boolConfigData.size() << std::endl;
    std::cout << "stringConfigData size: " << ConfigJSON::stringConfigData.size() << std::endl;
    for (const auto &item : ConfigJSON::intConfigData)
    {
        std::cout << item.first << " = " << item.second << std::endl;
    }
    for (const auto &item : ConfigJSON::boolConfigData)
    {
        std::cout << item.first << " = " << item.second << std::endl;
    }
    for (const auto &item : ConfigJSON::stringConfigData)
    {
        std::cout << item.first << " = " << item.second << std::endl;
    }
    ConfigJSON::dumpMapsToConsole();
}

void renderer::engineHandleUserInputSettingsLoad(sf::RenderWindow &renderWindow)
{
    ConfigJSON::loadConfigText();
    std::cout << "intConfigData size: " << ConfigJSON::intConfigData.size() << std::endl;
    std::cout << "boolConfigData size: " << ConfigJSON::boolConfigData.size() << std::endl;
    std::cout << "stringConfigData size: " << ConfigJSON::stringConfigData.size() << std::endl;
    // exit(0);
}

void renderer::engineHandleUserInputChangeWeapon(const bool increment)
{
    const int numProjectiles =
        static_cast<int>(ProjectileTypes::ProjectileType::Count); // Update this if you add more projectile types
    int activeProjectile = static_cast<int>(players[currentPlayer].get_active_projectile());

    if (increment)
    {
        activeProjectile = (activeProjectile + 1) % numProjectiles;
    }
    else
    {
        activeProjectile = (activeProjectile - 1 + numProjectiles) % numProjectiles;
    }

    players[currentPlayer].set_active_projectile(activeProjectile);

    const ProjectileTypes::ProjectileType activeType = static_cast<ProjectileTypes::ProjectileType>(activeProjectile);

    KJ::debug_output::print(__FILE__,
                            "Switched to projectile type: " + ProjectileFactory::projectileTypeToString(activeType),
                            KJ::debug_output::MessageType::GOOD);
}

void renderer::engineHandleUserInput(sf::RenderWindow &renderWindow, sf::Event &event)
{
    switch (event.key.code)
    {
    case sf::Keyboard::Left:
        engineHandleUserInputRotateLeft(renderWindow);
        break;
    case sf::Keyboard::Right:
        engineHandleUserInputRotateRight(renderWindow);
        break;
    case sf::Keyboard::Up:
        engineHandleUserInputPowerUp(renderWindow);
        break;
    case sf::Keyboard::Down:
        engineHandleUserInputPowerDown(renderWindow);
        break;
    case sf::Keyboard::Space:
        engineHandleUserInputFire(renderWindow);
        break;

    case sf::Keyboard::PageUp:
        engineHandleUserInputChangeWeapon(true);
        break;
    case sf::Keyboard::PageDown:
        engineHandleUserInputChangeWeapon(false);
        break;

        /*
            DEBUG AND MENU CONTROLS

          */
    case sf::Keyboard::F2:
        engineHandleUserInputSettingsSave(renderWindow);
        break;
    case sf::Keyboard::F3:
        engineHandleUserInputConfigToConsole(renderWindow);
        break;
    case sf::Keyboard::F6:
        engineHandleUserInputSettingsLoad(renderWindow);
        break;

    case sf::Keyboard::Escape:
        // todo: implement menu system on pause screen
        this->display_settings_menu = false;
        this->display_menu = !this->display_menu;
        break;

    case sf::Keyboard::Q:
        // todo: implement menu system on pause screen
        renderWindow.close(); // Close the window
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

void renderer::engineHandleInputEvents(sf::RenderWindow &renderWindow)
{
    // todo: create constants::audio enabled toggle
    players[currentPlayer].active_player = true;

    sf::Event event;
    while (renderWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Resized)
        {
            engineHandleWindowResize(renderWindow, event);
        }
        if (event.type == sf::Event::Closed)
        {
            engineHandleWindowClose(renderWindow);
        }
        else if (event.type == sf::Event::MouseMoved)
        {
            engineHandleMouseMove(renderWindow, mousePosition);
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            engineHandleUserInput(renderWindow, event);
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
        if (event.type == sf::Event::MouseMoved)
        {
            if (ConfigJSON::getVerboseMouseDebugging() == true)
            {
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(renderWindow);
                std::cout << "Mouse Position: " << mouse_pos.x << ", " << mouse_pos.y << std::endl;
            }
        }
        if ((is_dragging && event.type == sf::Event::MouseMoved) ||
            (sf::Event::MouseButtonPressed == true && event.mouseButton.button == sf::Mouse::Left))
        {
            const sf::Vector2i mouse_pos = sf::Mouse::getPosition(renderWindow);
            const float scale_x = terrain_obj.get_terrain_sprite().getScale().x;
            const float scale_y = terrain_obj.get_terrain_sprite().getScale().y;
            const sf::Vector2f scaled_mouse_pos(mouse_pos.x / scale_x, mouse_pos.y / scale_y);

            terrain_obj.update_terrain_with_collision(static_cast<int>(scaled_mouse_pos.x),
                                                      static_cast<int>(scaled_mouse_pos.y));
        }
        if (sf::Event::MouseButtonPressed == true && event.mouseButton.button == sf::Mouse::Right)
        {

            KJ::debug_output::print(__FILE__, "Mouse Right Clicked", KJ::debug_output::MessageType::GOOD);

            sf::Vector2i mouse_pos = sf::Mouse::getPosition(renderWindow);
            const float scale_x = terrain_obj.get_terrain_sprite().getScale().x;
            const float scale_y = terrain_obj.get_terrain_sprite().getScale().y;
            sf::Vector2f scaled_mouse_pos(mouse_pos.x / scale_x, mouse_pos.y / scale_y);

            terrain_obj.update_terrain_with_collision(static_cast<int>(scaled_mouse_pos.x),
                                                      static_cast<int>(scaled_mouse_pos.y));
        }
    }
}

void renderer::renderExplosions(sf::RenderWindow &renderTarget)
{
    for (const explosion &explosion : explosions)
    {
        if (explosion.is_active())
        {
            explosion.render(renderTarget);
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

sf::FloatRect renderer::collisonDetectionGenerateHitbox(sf::RenderWindow &renderWindow, tank &tank)
{
    // Get the tank's position and dimensions
    float tankWidth = -tank.get_body_x() * 2;
    float tankHeight = -tank.get_body_y();
    float tankX = tank.get_x() - tankWidth / 2;
    float tankY = tank.get_y();

    // Create a rectangle shape for the hitbox
    sf::FloatRect hitbox_rect(tankX, tankY, tankWidth, tankHeight);
    sf::RectangleShape hitbox(sf::Vector2f(tankWidth, tankHeight));

    if (hitbox_rect.contains(sf::Vector2f(static_cast<float>(sf::Mouse::getPosition(renderWindow).x),
                                          static_cast<float>(sf::Mouse::getPosition(renderWindow).y))))
    {
        KJ::debug_output::print(__FILE__, "Mouse is in hitbox", KJ::debug_output::MessageType::GOOD);
    }

    if (ConfigJSON::getRenderDebugHitboxes() == true)
    {
        hitbox.setPosition(tankX, tankY);
        hitbox.setFillColor(sf::Color::Transparent);
        hitbox.setOutlineColor(sf::Color::Red);
        hitbox.setOutlineThickness(2.0f);
        // Draw the hitbox
        renderWindow.draw(hitbox);
    }
    return hitbox_rect;
}

void renderer::renderDebugCollisionDetectionShowHitboxes(sf::RenderWindow &renderWindow)
{
    for (auto &player : players)
    {
        collisonDetectionGenerateHitbox(renderWindow, player);
    }
}

void renderer::renderProjectiles(sf::RenderWindow &renderTarget)
{
    const float gravity = physics.get_gravity(); // Get the gravity value

    for (auto it = projectiles.begin(); it != projectiles.end();)
    {
        auto &projectile = *it->get(); // Dereference the smart pointer

        if (projectile.is_active())
        {
            KJ::debug_output::print(__FILE__, "Rendering projectile", KJ::debug_output::MessageType::INFO);
            const float time_elapsed = delta_time.asSeconds() * ConfigJSON::getEngineProjectileSpeed();
            projectile.update_position(time_elapsed, gravity, renderTarget);

            const float projectile_x = projectile.get_x() + (projectile.get_width() / 2.0f);
            const float projectile_y = projectile.get_y() + (projectile.get_height() / 2.0f);

            if (projectile_x < -2000 || projectile_x > ConfigJSON::getWindowedResolutionX() + 2000)
            {
                // If the projectile goes 2000 pixels off-screen, mark it as inactive
                projectile.set_active(false);
            }

            projectile_path.append(sf::Vertex(sf::Vector2f(projectile_x, projectile_y), sf::Color::Red));
            renderTarget.draw(projectile_path);

            if (terrain_obj.transparent_at_pixel(projectile_x, projectile_y))
            {
                collide_with_terrain(projectile);
            }
            detect_tank(projectile);

            if (projectile.is_visible(renderTarget.getSize().x, renderTarget.getSize().y))
            {
                const sf::Drawable *render_property = projectile.get_render_property();
                if (render_property)
                {
                    renderTarget.draw(*dynamic_cast<const sf::Drawable *>(render_property));
                }
            }
        }
        else
        {
            KJ::debug_output::print(__FILE__, "Finished rendering projectile");
        }
        if (!projectile.is_active())
        {
            KJ::debug_output::print(__FILE__, "Finished rendering projectile");
            it = projectiles.erase(it); // Erase the projectile and update the iterator
        }
        else
        {
            ++it; // Move the iterator to the next element
        }
    }
}

void renderer::collide_with_tank(int player_index, ProjectileInterface &projectile)
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

void renderer::detect_tank(ProjectileInterface &projectile)
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

void renderer::collide_with_terrain(ProjectileInterface &projectile)
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

void renderer::renderTerrain(sf::RenderWindow &renderWindow)
{
    terrain_obj.set_screen_size(window_.getSize().x, window_.getSize().y);

    renderWindow.draw(terrain_obj.get_terrain_sprite(), render_state);
}

void renderer::renderPlayers(sf::RenderWindow &renderWindow)
{
    for (const auto &player : players)
    {
        renderPlayer(renderWindow, player); // Call the function to render the tank
    }
}

void renderer::renderParticles(sf::RenderWindow &renderWindow)
{
    if (this->enable_particles)
    {
        renderWindow.draw(particles.render());
    }
}

void renderer::renderPlayerHud(sf::RenderWindow &renderWindow)
{
    render_hud.render_player_data(renderWindow, players); // Render the player data
}

void renderer::renderDebugData(sf::RenderWindow &renderWindow)
{
    renderDebugGrid(renderWindow); // Render the grid lines
    renderDebugFPS(renderWindow);  // Render the FPS
    renderDebugCollisionDetectionShowHitboxes(renderWindow);
}

void renderer::renderMenuScreens(sf::RenderWindow &renderWindow)
{
    renderPauseMenu(renderWindow);
}

void renderer::render(sf::RenderWindow &renderWindow)
{
    renderWindow.clear(); // Clear the window
    // todo: move console output to a debug function - based on flag? seperate class?
    renderBackground(renderWindow);
    renderTerrain(renderWindow);
    renderPlayers(renderWindow);
    renderProjectiles(renderWindow);
    renderParticles(renderWindow);
    renderExplosions(renderWindow);
    renderPlayerHud(renderWindow);
    renderDebugData(renderWindow);
    renderMenuScreens(renderWindow);
    checkWinState(renderWindow);

    renderWindow.display(); // Display the window
}

void renderer::renderPauseMenu(sf::RenderWindow &renderTarget)
{
    if (display_menu)
    {
        MenuManager menuManager(renderTarget, font);
        menuManager.renderPauseMenu([this]() { engineResumeGame(); }, [this]() { engineOpenSettingsMenu(); },
                                    [this]() { engineQuitGame(); });
    }
}

void renderer::engineHandleMenuSelection(MenuOption selectedOption)
{
    switch (selectedOption)
    {
    case MenuOption::Resume:
        // Perform action for Resume option (e.g., resume the game)
        engineResumeGame();
        break;
    case MenuOption::Settings:
        // Perform action for Settings option (e.g., open settings menu)
        engineOpenSettingsMenu();
        break;
    case MenuOption::Quit:
        // Perform action for Quit option (e.g., quit the game)
        engineQuitGame();
        break;
    default:
        // Do nothing for other cases
        break;
    }
}

void renderer::engineResumeGame()
{
    // Logic to resume the game
    display_menu = false; // Hide the pause menu
    // Additional actions to resume the game...
}

void renderer::engineOpenSettingsMenu()
{
    // Logic to open the settings menu
    // Additional actions to display settings...
}

void renderer::engineQuitGame()
{
    // Logic to quit the game
    // Additional actions to quit the game...
    window_.close(); // Close the game window or exit the application
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
// Initialize the static map
std::map<std::string, ProjectileFactory::ProjectileCreator> ProjectileFactory::projectileTypeRegistry;

void renderer::engineLoadProjectiles()
{

    ProjectileFactory::RegisterProjectileType("DefaultProjectile",
                                              [](const tank &t) { return std::make_unique<DefaultProjectile>(t); });
    ProjectileFactory::RegisterProjectileType("CannonProjectile",
                                              [](const tank &t) { return std::make_unique<CannonProjectile>(t); });
}

void renderer::engineCreatePlayers()
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

void renderer::renderDebugGrid(sf::RenderWindow &renderTarget, int pixels)
{
    if (this->render_grid)
    {
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
            renderTarget.draw(line_text);
            line.setPosition(x, 0.0f);
            line.setFillColor(sf::Color::Yellow);
            renderTarget.draw(line);
        }
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
            renderTarget.draw(line_text);
            line.setPosition(0.0f, y);
            line.setFillColor(sf::Color::Yellow);
            renderTarget.draw(line);
        }
    }
}

void renderer::renderDebugFPS(sf::RenderWindow &renderWindow)
{
    if (this->show_fps)
    {
        fps_text.setFont(font);
        fps_text.setCharacterSize(24);
        fps_text.setFillColor(sf::Color::Red);
        const float text_x = (display_resolution.width - fps_text.getGlobalBounds().width - 40.0f);
        fps_text.setPosition(text_x, 10.0f);
        renderWindow.draw(fps_text);
    }
}

void renderer::renderPlayer(sf::RenderWindow &renderWindow, tank tank)
{
    sf::Sprite tank_sprite;
    float tank_scale = ConfigJSON::getRenderTankScale();
    tank_sprite.setScale(tank_scale, tank_scale);
    tank_sprite.setTexture(tank_texture);
    tank_sprite.setPosition(tank.get_x(), tank.get_y() - 5.0f);
    tank_sprite.setOrigin(tank.get_origin_x(), tank.get_origin_y());
    sf::Sprite turret_sprite;
    turret_sprite.setTexture(turret_texture);
    turret_sprite.setOrigin(tank.get_turret_width() / 2 + 7.5f, 4.0f);
    turret_sprite.setPosition(tank.get_x(), tank.get_y() - (tank.get_body_y() * tank_scale) + 5.0f);
    turret_sprite.setRotation(tank.get_angle());
    renderWindow.draw(turret_sprite);
    renderWindow.draw(tank_sprite);
    get_turret_projectile_origin(tank);
}

void renderer::queue_render_explosion(const ProjectileInterface &projectile)
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
    const float turret_angle_radians = -tank.get_angle() * (3.14159265f / 180.0f);
    const float turret_length = tank.get_turret_height();
    const sf::Vector2f turret_tip_offset(turret_length * std::sin(turret_angle_radians),
                                         turret_length * std::cos(turret_angle_radians));

    const sf::Vector2f turret_tip_position = tank_top_center + turret_tip_offset;
    return turret_tip_position;
}

// TODO: PROJECTILES
void renderer::engineFireProjectile(tank tank, std::unique_ptr<ProjectileInterface> projectile)
{
}

// void renderer::fire_projectile(tank tank)
//{
//
//     projectile cannon(tank);
//     cannon.set_explosion_duration(3.0f);
//     sounds_obj.fire(); // play sound (debug)
//     const float launch_angle =
//         tank.get_angle() - 90.0f; // Set your desired launch angle in degrees and correct 90 degrees
//
//     cannon.launch(get_turret_projectile_origin(tank).x, get_turret_projectile_origin(tank).y, launch_angle);
//     projectiles.push_back(cannon);
//
// }

///
/// GETTERS AND SETTERS
///
void renderer::set_fullscreen(bool fullscreen)
{
    this->fullscreen = fullscreen;
}

bool renderer::get_fullscreen() const
{
    return this->fullscreen;
}
