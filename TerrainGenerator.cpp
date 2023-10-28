#pragma once
#include <vector>
#include <ctime>
#include <random>
#include <SFML/Graphics.hpp>

class TerrainGenerator {
    sf::RenderWindow renderer;
    sf::Texture terrainTexture;
    sf::Sprite terrainSprite;
    sf::Image terrainImage;
public:
    TerrainGenerator() {
        if (!terrainTexture.loadFromFile("overlay.png")) {
            printf("Failde to load texture"); // Error handling
        }

        // Create a sprite for the terrain
        sf::Sprite terrainSprite(terrainTexture);
        this->terrainSprite = terrainSprite;

        sf::Image terrainImage = terrainTexture.copyToImage();
        this->terrainImage = terrainImage;
    }
    TerrainGenerator(int width, int height) {
        // init height
        this->width = width;
        this->height = height;
    }

    void setScreenSize(int width, int height) {
		this->width = width;
		this->height = height;
        updateSpriteSize();
        //updateImageSize();
	}

    void updateSpriteSize() {
		//this->terrainSprite.setScale(this->width / 800.0f, this->height / 600.0f);
	} 


    void updateTerrainWithCollision(int posX, int posY) {
        int terrainWidth = terrainImage.getSize().x;
        int terrainHeight = terrainImage.getSize().y;
        for (int x = 0; x < 40; x++) {
            for (int y = 0; y < 40; y++) {
                int circleX = x - 20; // Centering the circle at (20, 20)
                int circleY = y - 20;
                if (circleX * circleX + circleY * circleY <= 20 * 20) {
                    // If the pixel is within the circle's radius, make it transparent
                    if (posX + x - 20 < 0 || posX + x - 20 >= terrainWidth || posY + y - 20 < 0 || posY + y - 20 >= terrainHeight) {
						continue;
					}
                    terrainImage.setPixel(posX + x - 20, posY + y - 20, sf::Color::Cyan);
                }
            }
        }
        terrainTexture.loadFromImage(terrainImage);
        terrainSprite.setTexture(terrainTexture);
    }
 

    sf::Sprite getTerrainSprite() {
		return this->terrainSprite;
	}

    //void generateTerrain() {
    //    // Seed the random number generator
    //    std::mt19937 rng(static_cast<unsigned>(time(0)));

    //    // Set the number of hills and their properties
    //    int numHills = 8; // Adjust the number of hills
    //    int minHeight = 5; // Adjust the minimum height
    //    int maxHeight = static_cast<int>(1 * this->height);

    //    for (int i = 0; i < numHills; ++i) {
    //        int hillWidth = 100 + rng() % 200; // Adjust the width of the hills
    //        int hillHeight = minHeight + rng() % (maxHeight - minHeight);
    //        int startX = rng() % (width - hillWidth);

    //        for (int x = startX; x < startX + hillWidth; ++x) {
    //            if (x < 0 || x >= width) {
    //                continue; // Skip out-of-bounds values
    //            }
    //            float normalizedX = static_cast<float>(x - startX) / static_cast<float>(hillWidth);
    //            float heightOffset = hillHeight * (normalizedX * normalizedX - normalizedX * normalizedX * normalizedX); // Combination of quadratic and cubic functions
    //            int newHeight = static_cast<int>(floorHeight + heightOffset);
    //            terrain[x] = std::max(terrain[x], newHeight);
    //        }
    //    }

    //    // Smooth out the terrain with a lower smoothing factor
    //    for (int x = 1; x < width - 1; ++x) {
    //        terrain[x] = (terrain[x - 1] + terrain[x] + terrain[x + 1]) / 1.4; // Reduce the smoothing effect by using / 2
    //    }
    //}

    



  /*  std::vector<int> getTerrain() {
        return this->terrain;
    }*/

private:
    int width = 800;
    int height  = 600;
    std::vector<int> terrain;
    int floorHeight = 5; // Minimum height for the floor
};

class UnstableTerrainArea {
public:
    int startX;
    int endX;
    int startY;
    int endY;
    bool isSupported;
};