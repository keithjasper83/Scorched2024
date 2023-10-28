#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include "FileSystem.cpp"
#include "SoundManager.cpp"
#include "Config.h"
#include "Tank.cpp"
#include "Physics.cpp"
#include <random>
#include "TerrainGenerator.cpp"

#include "ProjectileCannon.cpp"

class Renderer {
public:
    bool fullscreen = constants::FULLSCREEN;
    bool renderGrid = constants::RENDER_GRID;
    bool showFPS = constants::SHOW_FPS;
    bool betaTerrain = constants::BETA_TERRAIN;
    TerrainGenerator terrainObj;

    Renderer(int screenWidth, int screenHeight, const std::string& windowTitle) {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;

        sf::ContextSettings settings;
        settings.depthBits = 24; // Depth buffer settings (adjust as needed)
        settings.stencilBits = 8; // Stencil buffer settings (adjust as needed)
        settings.antialiasingLevel = 8; // Anti-aliasing level (adjust as needed)
        settings.majorVersion = 3; // Set the major OpenGL version
        settings.minorVersion = 3; // Set the minor OpenGL version

        

        //move to gamestate functions -> restart ??
        


        setFrameLimits();

        if (this->fullscreen) {
			window.create(sf::VideoMode::getDesktopMode(), windowTitle, sf::Style::Fullscreen, settings);
		}
		else {
			window.create(sf::VideoMode(screenWidth, screenHeight), windowTitle, sf::Style::Default, settings);
		}
        updateWindowSize();

        terrainObj.setScreenSize(screenWidth, screenHeight);

        std::string FontFile = FileSystem::fileExists(constants::FONT_FILE);
        font.loadFromFile(FontFile); // Load a font for rendering text
        

        frameTimeBuffer.resize(100, 0.0f); // Resize the buffer to 100 elements

        //set this from game menu (2 is default)
        setPlayerCount(2);

    }
    sf::Texture backgroundTexture;

    void LoadBackgroundImage(const std::string& image) {
        backgroundTexture.loadFromFile(image);
    }

    void SetBackgroundImage(const std::string& image) {
        sf::Sprite backgroundSprite(backgroundTexture);
        backgroundSprite.setPosition(0, 0);
        window.draw(backgroundSprite);

    }

    sf::Texture terrainTexture;
    void LoadTerrainImage(const std::string& image) {
		terrainTexture.loadFromFile(image);
	}


    void restartGame() {
        TerrainGenerator generator(this->screenWidth, this->screenHeight);
        //generator.generateTerrain();
        //std::vector<int> terrain = generator.getTerrain();
        //setTerrain(terrain);
        createPlayers();

    }

    void start() {

        //TerrainGenerator generator(this->screenWidth, this->screenHeight);
        //generator.generateTerrain();
        //std::vector<int> terrain = generator.getTerrain();
        //setTerrain(terrain);
        

        size_t frameIndex = 0;
        float frameTimeSum = 0.0f;
        //LoadBackgroundImage("sky1.png");
        //LoadTerrainImage("rock1.jpg");
        createPlayers();
        setFrameLimits();
        while (window.isOpen()) {
            deltaTime = clock.restart();
            updateWindowSize();

            handleEvents();

            for (auto it = explosions.begin(); it != explosions.end();) {
                it->update(deltaTime);
                if (!it->isActive()) {
                    it = explosions.erase(it); // Remove inactive explosions
                }
                else {
                    ++it;
                }
            }
			
            // Update the frame time buffer and sum
            frameTimeSum -= frameTimeBuffer[frameIndex];
            frameTimeSum += deltaTime.asSeconds();
            frameTimeBuffer[frameIndex] = deltaTime.asSeconds();
            frameIndex = (frameIndex + 1) % frameTimeBuffer.size();

            updateFPS(frameTimeSum / frameTimeBuffer.size()); // Pass the average frame time

            render();
        }
    }


   /* void setTerrain(std::vector<int>& heightmap) {
        terrain = heightmap;
    }*/

    //void renderTerrain() {
    //    if (!this->terrain.empty()) {
    //        int windowHeight = fullscreen ? window.getSize().y : this->screenHeight;
    //        int windowWidth = fullscreen ? window.getSize().x : this->screenWidth;
    //        float xScale = static_cast<float>(windowWidth) / this->screenWidth;
    //        float yScale = static_cast<float>(windowHeight) / this->screenHeight;

    //        for (int x = 0; x < this->terrain.size(); x++) {
    //            int terrainHeight = this->terrain[x];
    //            float terrainX = static_cast<float>(x) * xScale;
    //            float terrainY = fullscreen ? static_cast<float>(windowHeight - terrainHeight) : static_cast<float>(this->screenHeight - terrainHeight);
    //            float terrainWidth = 1.0f * xScale;

    //            sf::RectangleShape terrainBlock(sf::Vector2f(terrainWidth, terrainHeight));
    //            terrainBlock.setPosition(terrainX, terrainY);
    //            terrainBlock.setTexture(&terrainTexture);
    //            terrainBlock.setFillColor(sf::Color(139, 69, 19));

    //            window.draw(terrainBlock);
    //        }
    //    }
    //    else {
    //        printf("ERROR: Terrain data has not been set!\n");
    //    }
    //}

    //void renderTerrain2() {
    //    if (!this->terrain.empty()) {
    //        sf::Shader terrainShader;
    //        if (!terrainShader.loadFromFile("terrainFragment.frag", sf::Shader::Fragment)) {
    //            printf("ERROR: Failed to load terrain shader!\n");
    //        }

    //        // Set the mask texture as a uniform for the shader
    //        terrainShader.setUniform("terrainTexture", terrainTexture);

    //        int windowHeight = fullscreen ? window.getSize().y : this->screenHeight;
    //        int windowWidth = fullscreen ? window.getSize().x : this->screenWidth;
    //        float xScale = static_cast<float>(windowWidth) / this->screenWidth;
    //        float yScale = static_cast<float>(windowHeight) / this->screenHeight;

    //        // Create a vertex array to draw the terrain
    //        sf::VertexArray terrainShape(sf::TriangleStrip, this->terrain.size() * 2);

    //        for (int x = 0; x < this->terrain.size(); x++) {
    //            // Calculate the position of the terrain point
    //            float terrainX = static_cast<float>(x) * xScale;
    //            float terrainY = fullscreen ? static_cast<float>(windowHeight - terrain[x]) : static_cast<float>(this->screenHeight - terrain[x]);

    //            // Set the positions of the vertices
    //            terrainShape[x * 2].position = sf::Vector2f(terrainX, terrainY);
    //            terrainShape[x * 2 + 1].position = sf::Vector2f(terrainX, windowHeight); // Extend to the bottom of the screen

    //            // Set the texture coordinates for the vertices
    //            terrainShape[x * 2].texCoords = sf::Vector2f(terrainX, 0); // Map to the top of the texture
    //            terrainShape[x * 2 + 1].texCoords = sf::Vector2f(terrainX, terrainTexture.getSize().y); // Map to the bottom of the texture

    //            // Set the color for the terrain
    //            terrainShape[x * 2].color = sf::Color::White;
    //            terrainShape[x * 2 + 1].color = sf::Color::White;
    //        }

    //        // Set the shader and draw the terrain
    //        window.draw(terrainShape, &terrainShader);
    //    }
    //    else {
    //        printf("ERROR: Terrain data has not been set!\n");
    //    }
    //}




 



private:
    sf::Clock clock;
    sf::Time deltaTime;
    sf::RenderWindow window;
    sf::Font font;
    sf::Text text;
    std::vector<int> terrain;

    //FrameRate Buffer - This is updated to 100 values in the constructor
    std::vector<float> frameTimeBuffer; // Buffer to store frame times for averaging

    int screenWidth;
    int screenHeight;

    int windowWidth;
    int windowHeight;

    Physics physics;
    SoundManager SoundsObj;

    bool needsRedraw = true;

    bool projectileInFlight = false;

    void setNeedsRedraw(bool needsRedraw) {
		this->needsRedraw = needsRedraw;
	}

    //update start locations to be a maximum of 4 players split accross the screen with range of 300px of divisions
    // create function to run before create players to ensure this is set to current screen (taking note of resolition to ensure 
    // it maximises the use of the available screen space)
    int startLocations[2] = { 50, 1220 };



    void updateWindowSize() {
		windowWidth = fullscreen ? window.getSize().x : this->screenWidth;
		windowHeight = fullscreen ? window.getSize().y : this->screenHeight;
	}

    void setFrameLimits() {
        window.setFramerateLimit(constants::FRAME_RATE); // Limit the frame rate to 60 FPS
		window.setVerticalSyncEnabled(false); // Enable vertical sync
    }

    // implement logic to respawn players when screen resizes ? perhaps restart game session.
    void setFullscreen(bool fullscreen) {
        this->fullscreen = fullscreen;
    }

    bool getFullscreen() {
        return this->fullscreen;
    }

    void toggleFullScreen() {

        setFullscreen(!getFullscreen());

        if (this->getFullscreen()) {
            window.create(sf::VideoMode::getDesktopMode(), constants::GAME_TITLE, sf::Style::Fullscreen);
            restartGame();
        }
        else {
            window.create(sf::VideoMode(screenWidth, screenHeight), constants::GAME_TITLE, sf::Style::Default);
            restartGame();
        }
        setFrameLimits();

    }

    bool isDragging = false;


    //implement turn based events controlling one tank at a time
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            /*if (event.type == sf::Event::Resized) {
                printf("Window Resized\n");
				updateWindowSize();
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
			}*/
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                setNeedsRedraw(true);
                switch (event.key.code) {
                    case sf::Keyboard::Escape:
                        //implement pause menu and reset game
					    window.close();
				   	break;
                    case sf::Keyboard::F5:
                        toggleFullScreen();
                        break;
                    case sf::Keyboard::P:
                        // press p to fire a sample sound!
                        SoundsObj.Fire();
                    break;
                    case sf::Keyboard::F1:
                        // press f1 to restart the game
						restartGame();
					break;
                    case sf::Keyboard::G:
						// press g to toggle gravity
                        if (this->renderGrid) {
							this->renderGrid = false;
						}
                        else {
							this->renderGrid = true;
						}
                    break;
                    case sf::Keyboard::T:
                        if (this->betaTerrain) {
							this->betaTerrain = false;
                            restartGame();
						}
                        else {
							this->betaTerrain = true;   
                            restartGame();
						}

                    /// <summary>
                    /// Player Controls
                    /// </summary>
                    /// <returns></returns>
                    case sf::Keyboard::Left:
                        //printf("Left\n");
                        players[0].decAngle();
                        //printf("Player Angle: %f\n", players[0].getAngle());
						break;
                    case sf::Keyboard::Right:
                        //printf("Right\n");
                        players[0].incAngle();
                        //printf("Player Angle: %f\n", players[0].getAngle());
                        break;
                    case sf::Keyboard::Up:
                        //printf("Up\n");
                        players[0].incPower();
                        //printf("Player Power: %f\n", players[0].getPower());
                        break;
                    case sf::Keyboard::Down:
                        //printf("Down\n");
                        players[0].decPower();
                        //printf("Player Power: %f\n", players[0].getPower());
                        break;
                    case sf::Keyboard::Space:
                        //printf("Space\n");
                        players[0].Fire();
                        fireProjectile(players[0]);
                        break;
                    case sf::Keyboard::F:
                        if (this->showFPS) {
                            this->showFPS = false;
                        }
                        else {
                            this->showFPS = true;
                        }
                        break;

					default:
						break;
				}
            }
            

            //mouse events
            // TODO: clean up mouse movements and clicks
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                isDragging = true;
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                isDragging = false;
            }
            if (isDragging && event.type == sf::Event::MouseMoved || sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                terrainObj.updateTerrainWithCollision(mousePos.x, mousePos.y);
            }
            if (sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                terrainObj.updateTerrainWithCollision(mousePos.x, mousePos.y);
            }

        }
    }

    std::vector<Projectile> projectiles;
   

    int calculateProjectileOriginX(Tank tank) {
        // Calculate the origin of the projectile considering turret rotation, width, and the tip of the turret
        float halfTurretWidth = tank.getTurretWidth() / 2;
        float turretTipOffsetX = halfTurretWidth * std::cos((tank.getAngle() + 90) * M_PI / 180.0f);
        int originX = tank.getX() - static_cast<int>(turretTipOffsetX);
        return originX;
    }

    int calculateProjectileOriginY(Tank tank, Projectile& projectile) {
        // Calculate the origin of the projectile at the tip of the turret
        float halfTurretWidth = tank.getTurretWidth() / 2;
        float halfProjectileSize = projectile.getWidth() / 2;
        float turretTipOffsetY = -(tank.getTurretHeight() / 2); // Adjusted here
        int originY = tank.getY() - (tank.getBodyY() - static_cast<int>(turretTipOffsetY)) - static_cast<int>(halfTurretWidth + halfProjectileSize);
        return originY;
    }

    void renderExplosions() {
        for (const Explosion& explosion : explosions) {
            if (explosion.isActive()) {
                explosion.render(window);
            }
        }

    }

    void modifyTerrainWithExplosion(const Explosion& explosion, std::vector<int>& terrain) {
        // Get the explosion position and size
        sf::Vector2f explosionPosition = explosion.getPosition();
        float explosionSize = explosion.getExplosionSize();

        // Calculate the explosion radius
        float explosionRadius = explosionSize;

        // Iterate through the terrain data and check if each point is within the explosion area
        for (int x = 0; x < terrain.size(); x++) {
            // Calculate the position of the terrain point
            sf::Vector2f terrainPoint(static_cast<float>(x), static_cast<float>(terrain[x]));

            // Calculate the distance between the terrain point and the explosion center
            float distance = std::sqrt(std::pow(terrainPoint.x - explosionPosition.x, 2) + std::pow(terrainPoint.y - explosionPosition.y, 2));

            // Check if the terrain point is within the explosion radius
            if (distance <= explosionRadius) {
                // Remove terrain points within the explosion radius by setting their height to zero
                terrain[x] = 0;
            }
        }
    }







    void fireProjectile(Tank tank) {

        

        // Launch the projectile
		// Fire a projectile
        // Create a new projectile and add it to the container
        Projectile cannon(tank);

        cannon.setExplosionDuration(3.0f);

        // Determine the initial position (initialX and initialY) and launch angle
        float initialX = calculateProjectileOriginX(tank); // Set your desired initial X position
        float initialY = calculateProjectileOriginY(tank, cannon); // Set your desired initial Y position
        float launchAngle = tank.getAngle(); // Set your desired launch angle in degrees

        cannon.launch(initialX, initialY, launchAngle - 90);

        // Add the projectile to the container
        projectiles.push_back(cannon);
    }

    int getTerrainHeightAt(int x) {
        int terrainInt = x;
        int terrainData = terrain[terrainInt];

        // Calculate the inverted terrain height
        int invertedTerrainHeight = windowHeight - terrainData;

        // Return the inverted terrain height
        return invertedTerrainHeight;
    }

    std::vector<Explosion> explosions;

    void renderProjectiles() {
        // Define gravity constant
        float gravity = physics.getGravity(); // Adjust as needed

        for (auto& projectile : projectiles) {
            sf::VertexArray tracerLines(sf::LinesStrip);

            if (projectile.isActive()) {
                //printf("Renderer.cpp - renderProjectiles() - Projectile is active\n");
                // Calculate time elapsed
                float timeElapsed = deltaTime.asSeconds();

                // Update the projectile's position
                //printf("Renderer.cpp - renderProjectiles() - Updating projectile position\n");
                projectile.updatePosition(timeElapsed, gravity);
                //printf("Renderer.cpp - renderProjectiles() - timeElapsed: %f, gravity: %f\n", timeElapsed, gravity);
                //printf("Renderer.cpp - Projectile Position updated\n");

                // Check for collision with terrain
                int projectileX = projectile.getX() + (projectile.width / 2);
                int projectileY = projectile.getY() + (projectile.height / 2);
                int terrainHeight = getTerrainHeightAt(projectileX);

                if (projectileY >= terrainHeight) { // Inverted comparison
                    // Handle collision with terrain
                    queueRenderExplosion(projectile);
                    projectile.setActive(false);
                    projectileInFlight = false;
                }

                tracerLines.append(sf::Vertex(sf::Vector2f(projectile.getX(), projectile.getY()), sf::Color::Red));


                // Render the projectile
                //("Renderer.cpp - renderProjectiles() - Projectile Width: %f\n", projectile.getWidth());
                sf::CircleShape projectileShape(projectile.getWidth());
                
                //printf("Renderer.cpp - renderProjectiles() - Projectile X: %f, Y: %f\n", projectile.getX(), projectile.getY());
                projectileShape.setPosition(projectile.getX(), projectile.getY());
                projectileShape.setOrigin(projectile.getWidth() / 2, projectile.getWidth() / 2);
                projectileShape.setFillColor(sf::Color::Cyan);
                
                window.draw(projectileShape);
                window.draw(tracerLines);

            }
        }

    }

    std::vector<bool> unstableTerrain;

    void queueRenderExplosion(Projectile projectile) {
        sf::Time duration = sf::seconds(2.0f);
        //printf("Renderer.cpp - renderExplosion() - X: %f, Y: %f\n", projectile.getX(), projectile.getY());
        Explosion explosion(projectile.getX(), projectile.getY(), 40.0f, duration);
        modifyTerrainWithExplosion(explosion, terrain);
        explosions.push_back(explosion);
    }





    void updateFPS(float averageFrameTime) {
        // Update your game objects here
        // Calculate frame rate
        float frameRate = 1.0f / averageFrameTime;
        text.setString("FPS: " + std::to_string(static_cast<int>(frameRate)));
    }



    void render() {

        //todo FIX GRAVITY TO TAKE ACCOUNT OF NEW TERRAIN SYSTEM (Image Based)
        /*if (physics.applyGravity(players, terrain, windowHeight, deltaTime)) {
            setNeedsRedraw(true);
        }*/

        if (projectileInFlight) {
			setNeedsRedraw(true);
		}
        /*if (!needsRedraw) {
			return;
		}*/
        window.clear();
        SetBackgroundImage("sky1.png");

        if (this->renderGrid) {
            drawPixelGrid(100);
        }

        //NOTE: ADD YOUR RENDERING CODE HERE
        // 
        // 
        // Render your game objects here
        /*if (!this->betaTerrain) {
            renderTerrain();
        }
        else {
            renderTerrain2();
        }*/
        window.draw(terrainObj.getTerrainSprite());


        //switch for a loop of players
        for (auto & player : players) {
            generateTank(player); // Call the function to generate a tank
            generatePlayerData(player); // Call the function to generate a label for the player name
        }
        // Render the frame rate text
        if (this->showFPS) {
            renderFPS();
		}
        
        renderProjectiles();
        renderExplosions();

        

        window.display();
        setNeedsRedraw(false);
    }

    sf::Texture generateTankTexture(int tankWidth, int tankHeight) {
        // Generate a texture for the tank
		sf::Image tankImage;
        sf::Texture tankTexture;
		tankImage.create(tankWidth, tankHeight, sf::Color::Red);
        for (int x = 0; x < tankWidth; ++x) {
            for (int y = 0; y < tankHeight; ++y) {
					tankImage.setPixel(x, y, sf::Color::Cyan);
			}
		}
		tankTexture.loadFromImage(tankImage);
        return tankTexture;
    }

   

    sf::Texture generateTurretTexture(int turretWidth, int turretHeight) {
        // Generate a texture for the turret
        sf::Image turretImage;
        sf::Texture turretTexture;
        turretImage.create(turretWidth, turretHeight, sf::Color::Red);
        for (int x = 0; x < turretWidth; ++x) {
            for (int y = 0; y < turretHeight; ++y) {
                turretImage.setPixel(x, y, sf::Color::Red);
                //printf("x: %d, y: %d - Setting to red\n", x, y);
            }
        }
        turretTexture.loadFromImage(turretImage);
        return turretTexture;
    }

    void generateTank(Tank tank) {
        // Generate a tank
        sf::Texture tankTexture = generateTankTexture(tank.getBodyX(), tank.getBodyY());
        sf::Sprite tankSprite;
        tankSprite.setTexture(tankTexture);
        tankSprite.setPosition(tank.getX(), tank.getY());
        tankSprite.setOrigin(tank.getOriginX(), tank.getOriginY());

        // Generate a turret
        sf::Texture turretTexture = generateTurretTexture(tank.getTurretWidth(), tank.getTurretHeight());
        sf::Sprite turretSprite;
        turretSprite.setTexture(turretTexture);
        turretSprite.setOrigin(tank.getTurretWidth() / 2 , 0);
        turretSprite.setPosition(tank.getX(), tank.getY() - (tank.getBodyY() / 2) + 5);
        turretSprite.setRotation(tank.getAngle());

        // Draw both the tank and turret
        window.draw(tankSprite);
        window.draw(turretSprite);
    }


    
    
    void setPlayerCount(int playerCount) {
		this->playerCount = playerCount;
	}
    int getPlayerCount() {
		return this->playerCount;
	}

    void createPlayers() {
        players.clear();
        printf("Creating %d players\n", getPlayerCount());
        for (int i = 0; i < getPlayerCount(); ++i) {
            Tank tank = Tank(startLocations[i], 100.0f, i + 1, "Player " + std::to_string(i + 1));
            players.push_back(tank);
        }
    }

	int playerCount = 0;
    std::vector<Tank> players;


    // DEBUG DRAWING FUNCTION
    // to display grind of pixels
    // for debugging and drawing purposes
    void drawPixelGrid(int pixels) {
        // Draw yellow lines every 50 pixels
        

        // Draw vertical lines
        for (int x = 0; x < windowWidth; x += pixels) {
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
        for (int y = 0; y < windowHeight; y += pixels) {
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

    void generatePlayerData(Tank tank) {
        // Generate a label for the player name
		sf::Text playerHud;
        playerHud.setFont(font);
        playerHud.setCharacterSize(24);
        playerHud.setFillColor(sf::Color::White);
        std::string playerName = "Name: " + tank.getPlayerName();

        std::string playerHealth = "  Health: " + std::to_string(tank.getHealth());
        std::string playerPower = "  Power: " + std::to_string(tank.getPower());
        std::string playerAngle = "  Angle: " + std::to_string(tank.getAngle());
        std::string playerWeapon = "  Weapon: " + tank.getWeaponName();
        std::string playerAmmo = "  Ammo: " + std::to_string(tank.getAmmo());

        std::string playerData = playerName + playerHealth + playerPower + playerAngle + playerAmmo + playerWeapon;
        playerHud.setString(playerData);

        if (tank.getIndex() == 1) {
            playerHud.setPosition(10.0f, 10.0f);
        }
        else {
            playerHud.setPosition(10.0f, playerHud.getGlobalBounds().height + 20.0f);
        }
		window.draw(playerHud);
    }

    void renderFPS() {
        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Red);
        float textX = windowWidth - (windowWidth - text.getGlobalBounds().width) / 8.0f;
        text.setPosition(textX, 10.0f);
		window.draw(text);
	}

    

    std::vector<UnstableTerrainArea> unstableAreas;

    void handleExplosion(const Explosion& explosion, std::vector<int>& terrain) {
        // Mark the terrain as unstable within the explosion radius
        int explosionRadius = static_cast<int>(explosion.getExplosionSize());
        int explosionX = static_cast<int>(explosion.getPosition().x);

        for (int x = explosionX - explosionRadius; x <= explosionX + explosionRadius; x++) {
            if (x >= 0 && x < terrain.size()) {
                int terrainHeight = terrain[x];
                UnstableTerrainArea unstableArea;
                unstableArea.startX = x;
                unstableArea.endX = x;
                unstableArea.startY = terrainHeight;
                unstableArea.endY = terrainHeight - explosionRadius;
                unstableArea.isSupported = false;
                unstableAreas.push_back(unstableArea);

                // Modify the terrain within the explosion radius
                terrain[x] = unstableArea.endY;
            }
        }

        // Check for support and handle terrain collapse
        handleTerrainCollapse(terrain);
    }

    void handleTerrainCollapse(std::vector<int>& terrain) {
        for (int i = 0; i < unstableAreas.size(); i++) {
            UnstableTerrainArea& area = unstableAreas[i];

            // Check if the area is supported from below
            if (area.startY >= 0) {
                area.isSupported = true;
                for (int x = area.startX; x <= area.endX; x++) {
                    if (x >= 0 && x < terrain.size()) {
                        if (terrain[x] < area.startY) {
                            area.isSupported = false;
                            break;
                        }
                    }
                }
            }

            // If the area is not supported, let it fall
            if (!area.isSupported) {
                for (int x = area.startX; x <= area.endX; x++) {
                    if (x >= 0 && x < terrain.size()) {
                        terrain[x] += 1; // Adjust this value to control the falling speed
                    }
                }
            }
        }

        // Remove areas that have stabilized
        unstableAreas.erase(
            std::remove_if(unstableAreas.begin(), unstableAreas.end(),
                [](const UnstableTerrainArea& area) {
                    return area.isSupported;
                }
            ),
            unstableAreas.end()
        );
    }



};

