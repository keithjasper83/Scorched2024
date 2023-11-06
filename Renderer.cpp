#pragma once
#include "Renderer.h"

Renderer::Renderer(int screenWidth, int screenHeight)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    // Create window settings
    createSettings();
    // Load  Assets here
    LoadImageAssets();
    LoadFontAssets();
    // Set Default framelimits (mostly for development but may become a feature)
    setFrameLimits();
    // Make the render window
    createWindow();
    // Set window sizes for terrain scaling
    terrainObj.setScreenSize(screenWidth, screenHeight);
    // create 100 elements in frametime buffer to allow for an average
    frameTimeBuffer.resize(100, 0.0f); // Resize the buffer to 100 elements
    // set this from game menu (2 is default)
    setPlayerCount(2);
}

void Renderer::createWindow()
{
    if (this->fullscreen)
    {
        window.create(sf::VideoMode::getDesktopMode(), windowTitle, sf::Style::Fullscreen, settings);
    }
    else
    {
        window.create(sf::VideoMode(screenWidth, screenHeight), windowTitle, sf::Style::Default, settings);
    }
    updateWindowSize();
}

void Renderer::LoadFontAssets()
{
    if (FileSystem::fileExists(constants::FONT_FILE))
    {
        font.loadFromFile(constants::FONT_FILE); // Load a font for rendering text
    }
    else
    {
        printf("ERROR: Failed to load font!\n");
    }
}

void Renderer::createSettings()
{
    settings.depthBits = 24;        // Depth buffer settings (adjust as needed)
    settings.stencilBits = 8;       // Stencil buffer settings (adjust as needed)
    settings.antialiasingLevel = 8; // Anti-aliasing level (adjust as needed)
    settings.majorVersion = 3;      // Set the major OpenGL version
    settings.minorVersion = 3;      // Set the minor OpenGL version
    settings.sRgbCapable = true;    // Enable sRGB (gamma correction)
}

void Renderer::LoadImageAssets()
{
    LoadBackgroundImage("Images/sky1.png");
    LoadTerrainImage("Images/rock1.jpg");
}

void Renderer::LoadBackgroundImage(const std::string& image)
{
    if (FileSystem::fileExists(image))
    {
        backgroundTexture.loadFromFile(image);
        std::cout << "Background image loaded to memory" << std::endl;
    }
    else
    {
        std::cout << "ERROR: Failed to load background image!" << std::endl;
    }
}

void Renderer::SetBackgroundImage(const std::string& image)
{
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setPosition(0, 0);
    backgroundSprite.setScale(terrainObj.getScale().x, terrainObj.getScale().y);
    window.draw(backgroundSprite, renderState);
    // cout << "Background image set and Draw Called" << endl;
}

void Renderer::LoadTerrainImage(const std::string& image)
{
    terrainTexture.loadFromFile(image);
}

void Renderer::restartGame()
{
    TerrainGenerator generator(this->screenWidth, this->screenHeight);
    createPlayers();
}

void Renderer::start()
{
    size_t frameIndex = 0;     // Index of the current frame time
    float frameTimeSum = 0.0f; // Sum of the frame times
    createPlayers();           // Create the players
    setFrameLimits();          // Set the frame limits
    while (window.isOpen())
    {
        deltaTime = clock.restart(); // Restart the clock and save the elapsed time into deltaTime
        updateWindowSize();          // Update the window size
        handleEvents();              // Handle events
        for (auto it = explosions.begin(); it != explosions.end();)
        {
            it->update(deltaTime); // Update the explosion
            if (!it->isActive())
            {
                it = explosions.erase(it); // Remove inactive explosions
            }
            else
            {
                ++it;
            }
        }
        // Update the frame time buffer and sum
        frameTimeSum -= frameTimeBuffer[frameIndex];            // Subtract the oldest frame time from the sum
        frameTimeSum += deltaTime.asSeconds();                  // Add the new frame time to the sum
        frameTimeBuffer[frameIndex] = deltaTime.asSeconds();    // Store the new frame time in the buffer
        frameIndex = (frameIndex + 1) % frameTimeBuffer.size(); // Increment the frame index and wrap around
        updateFPS(frameTimeSum / frameTimeBuffer.size());       // Pass the average frame time
        //todo: for some reason this function returning false prevents background from loading? 
        runGravitySimulation();
        render(); // Run the render function to send information to the window
    }
}

void Renderer::runGravitySimulation()
{
    if (this->enablePhysics)
    {
        physics.applyGravity(players, terrainObj, deltaTime);
    }
}

void Renderer::updateWindowSize()
{
    this->windowWidth = fullscreen ? window.getSize().x : this->screenWidth;
    this->windowHeight = fullscreen ? window.getSize().y : this->screenHeight;
}

void Renderer::setFrameLimits()
{
    window.setFramerateLimit(constants::FRAME_RATE); // Limit the frame rate to 60 FPS
    window.setVerticalSyncEnabled(false);            // Enable vertical sync
}

void Renderer::setFullscreen(bool fullscreen)
{
    this->fullscreen = fullscreen;
}

bool Renderer::getFullscreen()
{
    return this->fullscreen;
}

void Renderer::toggleFullScreen()
{
    setFullscreen(!getFullscreen());
    if (this->getFullscreen())
    {
        window.create(sf::VideoMode::getDesktopMode(), constants::GAME_TITLE, sf::Style::Fullscreen);
        terrainObj.setScreenSize(window.getSize().x, window.getSize().y);
        restartGame();
    }
    else
    {
        window.create(sf::VideoMode(screenWidth, screenHeight), constants::GAME_TITLE, sf::Style::Default);
        terrainObj.setScreenSize(window.getSize().x, window.getSize().y);
        restartGame();
    }
    setFrameLimits();
}

void Renderer::handleEvents()
{
    players[0].activePlayer = true;
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Resized)
        {
            printf("Window Resized\n");
            updateWindowSize();
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            terrainObj.setScreenSize(event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));
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
                toggleFullScreen(); // Toggle fullscreen
                break;
            case sf::Keyboard::P:
                std::cout << "Firing Sound" << std::endl;
                SoundsObj.Fire(); // play sound (debug)
                break;
            case sf::Keyboard::F1:
                restartGame(); // Restart the game
                break;
            case sf::Keyboard::G:
                // Enables or disables the grid used for debugging and pixel placement
                if (this->renderGrid)
                {
                    this->renderGrid = false;
                }
                else
                {
                    this->renderGrid = true;
                }
                break;
            case sf::Keyboard::Left:
                players[0].decAngle(); // Decrease the angle of the player's turret
                break;
            case sf::Keyboard::Right:
                players[0].incAngle(); // Increase the angle of the player's turret
                break;
            case sf::Keyboard::Up:
                players[0].incPower(); // Increase the power of the player's turret
                break;
            case sf::Keyboard::Down:
                players[0].decPower(); // Decrease the power of the player's turret
                break;
            case sf::Keyboard::Space:
                players[0].Fire();          // todo: is this needed? (possibly for player rotation?)
                fireProjectile(players[0]); // Fire a projectile
                break;
            case sf::Keyboard::F:
                // Toggle the frame rate display
                if (this->showFPS)
                {
                    this->showFPS = false;
                }
                else
                {
                    this->showFPS = true;
                }
                break;
            case sf::Keyboard::O:
                // Toggle the physics simulation (should stop tanks from falling to the terrain)
                if (this->enablePhysics)
                {
                    this->enablePhysics = false;
                }
                else
                {
                    this->enablePhysics = true;
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
            isDragging = true;
        }
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
        {
            isDragging = false;
        }
        if ((isDragging && event.type == sf::Event::MouseMoved) ||
            (sf::Event::MouseButtonPressed == true && event.mouseButton.button == sf::Mouse::Left))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            float scaleX = terrainObj.getTerrainSprite().getScale().x;
            float scaleY = terrainObj.getTerrainSprite().getScale().y;
            sf::Vector2f scaledMousePos(mousePos.x / scaleX, mousePos.y / scaleY);

            terrainObj.updateTerrainWithCollision(static_cast<int>(scaledMousePos.x),
                                                  static_cast<int>(scaledMousePos.y));
        }
        if (sf::Event::MouseButtonPressed == true && event.mouseButton.button == sf::Mouse::Right)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            float scaleX = terrainObj.getTerrainSprite().getScale().x;
            float scaleY = terrainObj.getTerrainSprite().getScale().y;
            sf::Vector2f scaledMousePos(mousePos.x / scaleX, mousePos.y / scaleY);

            terrainObj.updateTerrainWithCollision(static_cast<int>(scaledMousePos.x),
                                                  static_cast<int>(scaledMousePos.y));
        }
    }
}

void Renderer::renderExplosions()
{
    for (const Explosion &explosion : explosions)
    {
        if (explosion.isActive())
        {
            explosion.render(window);
        }
    }
    for (auto &player : players)
    {
        player.setOnGround(false);
    }
}

void Renderer::renderProjectiles()
{
    float gravity = physics.getGravity(); // Get the gravity value

    for (auto &projectile : projectiles)
    {
        if (projectile.isActive())
        {
            float timeElapsed = deltaTime.asSeconds() * constants::PROJECTILE_SPEED;
            projectile.updatePosition(timeElapsed, gravity);
            if (projectile.getY() < window.getSize().y - 10 && projectile.getY() > 0 + 10)
            {
                int projectileX = projectile.getX() + (projectile.width / 2);
                int projectileY = projectile.getY() + (projectile.height / 2);
                if (projectile.getStartX() == 0.0f && projectile.getStartY() == 0.0f)
                {
                    projectile.setStartX(projectileX);
                    projectile.setStartY(projectileY);
                }
                projectilePath.append(
                    sf::Vertex(sf::Vector2f(projectile.getX() + (projectile.getWidth() / 2),
                                            projectile.getY() + (projectile.getHeight() / 2)),
                               sf::Color::Red)); // add a vertex to the vertex array for the projectile path
                window.draw(projectilePath);     // draw the projectile path
                if (terrainObj.TransparentAtPixel(projectileX, projectileY))
                {
                    queueRenderExplosion(projectile);
                    SoundsObj.Explode(); // Play sound (debug)
                    projectile.setActive(false);
                    projectilePath.clear();
                }
                sf::CircleShape projectileShape(projectile.getWidth());
                projectileShape.setPosition(projectile.getX(), projectile.getY());
                projectileShape.setOrigin(projectile.getWidth() / 2, projectile.getWidth() / 2);
                projectileShape.setFillColor(sf::Color::Cyan);
                window.draw(projectileShape);
            }
            else
            {
                std::cout << "Finished rendering projectile, projectile was out of bounds" << std::endl;
            }
        }
    }
}

void Renderer::updateFPS(float averageFrameTime)
{
    float frameRate = 1.0f / averageFrameTime;
    FPSText.setString("FPS: " + std::to_string(static_cast<int>(frameRate)));
}

void Renderer::render()
{
    window.clear();
    SetBackgroundImage("sky1.png");
    window.draw(terrainObj.getTerrainSprite(), renderState);
    for (auto &player : players)
    {
        generateTank(player); // Call the function to render the tank
    }
    RenderHud.renderPlayerData(window, players);
    window.draw(RenderHud.createCenteredRedWindow());
    renderProjectiles();
    renderExplosions();

    // renderHUD(players);
    renderFPS();
    renderGridLines(100); // Render the grid the int specified sets the pixel size of the grid
    window.display();
}

sf::Texture Renderer::generateTankTexture(int tankWidth, int tankHeight)
{
    sf::Image tankImage;
    sf::Texture tankTexture;
    tankImage.create(tankWidth, tankHeight, sf::Color::Red);
    for (int x = 0; x < tankWidth; ++x)
    {
        for (int y = 0; y < tankHeight; ++y)
        {
            tankImage.setPixel(x, y, sf::Color::Cyan);
        }
    }
    tankTexture.loadFromImage(tankImage);
    return tankTexture;
}

sf::Texture Renderer::generateTurretTexture(int turretWidth, int turretHeight)
{
    // Generate a texture for the turret
    sf::Image turretImage;
    sf::Texture turretTexture;
    turretImage.create(turretWidth, turretHeight, sf::Color::Red);
    for (int x = 0; x < turretWidth; ++x)
    {
        for (int y = 0; y < turretHeight; ++y)
        {
            turretImage.setPixel(x, y, sf::Color::Red);
            // printf("x: %d, y: %d - Setting to red\n", x, y);
        }
    }
    turretTexture.loadFromImage(turretImage);
    return turretTexture;
}

void Renderer::setPlayerCount(int playerCount)
{
    this->playerCount = playerCount;
}

int Renderer::getPlayerCount()
{
    return this->playerCount;
}

void Renderer::createPlayers()
{
    players.clear();
    printf("Creating %d players\n", getPlayerCount());
    int secondLocation = window.getSize().x - 50;
    int playerStartLocations[2] = {50, secondLocation};
    for (int i = 0; i < getPlayerCount(); ++i)
    {
        Tank tank = Tank(playerStartLocations[i], 100.0f, i + 1, "Player " + std::to_string(i + 1));
        players.push_back(tank);
    }
}

void Renderer::renderGridLines(int pixels)
{
    if (this->renderGrid)
    {
        // Draw vertical lines
        for (int x = 0; x < windowWidth; x += pixels)
        {
            sf::RectangleShape line(sf::Vector2f(1.0f, static_cast<float>(windowHeight)));
            std::string LineLabel = std::to_string(x);
            sf::Text LineText;
            LineText.setFont(font);
            LineText.setCharacterSize(24);
            LineText.setFillColor(sf::Color::White);
            LineText.setString(LineLabel);
            LineText.setPosition(x, 0.0f);
            window.draw(LineText);
            line.setPosition(x, 0.0f);
            line.setFillColor(sf::Color::Yellow);
            window.draw(line);
        }
        // Draw horizontal lines
        for (int y = 0; y < windowHeight; y += pixels)
        {
            sf::RectangleShape line(sf::Vector2f(static_cast<float>(windowWidth), 1.0f));
            std::string LineLabel = std::to_string(y);
            sf::Text LineText;
            LineText.setFont(font);
            LineText.setCharacterSize(24);
            LineText.setFillColor(sf::Color::White);
            LineText.setString(LineLabel);
            LineText.setPosition(0.0f, y);
            window.draw(LineText);
            line.setPosition(0.0f, y);
            line.setFillColor(sf::Color::Yellow);
            window.draw(line);
        }
    }
}

void Renderer::renderFPS()
{
    if (this->showFPS)
    {
        FPSText.setFont(font);
        FPSText.setCharacterSize(24);
        FPSText.setFillColor(sf::Color::Red);
        float textX = windowWidth - (windowWidth - FPSText.getGlobalBounds().width) / 8.0f;
        FPSText.setPosition(textX, 10.0f);
        window.draw(FPSText);
    }
}
