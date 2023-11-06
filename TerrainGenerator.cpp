#pragma once

#include "TerrainGenerator.h"

struct TerrainGenerator::Scale scale;

TerrainGenerator::TerrainGenerator()
{
    if (!terrainTexture.loadFromFile("Images/overlay.png"))
    {
        printf("Failde to load texture"); // Error handling
    }

    // Create a sprite for the terrain
    sf::Sprite terrainSprite(terrainTexture);
    this->terrainSprite = terrainSprite;

    sf::Image terrainImage = terrainTexture.copyToImage();
    this->terrainImage = terrainImage;
}
TerrainGenerator::TerrainGenerator(int width, int height)
{
    // init height
    this->width = width;
    this->height = height;
    setScreenSize(width, height);
}

void TerrainGenerator::setScreenSize(int width, int height)
{
    // Update the screen dimensions
    this->width = width;
    this->height = height;

    // Update the terrain sprite size to fill the screen
    updateSpriteSize();

    // Optionally, update the terrain image size to match the new screen size
    // updateImageSize();
}

void TerrainGenerator::updateSpriteSize()
{
    // Get the original texture size
    sf::Vector2u textureSize = terrainTexture.getSize();

    // Calculate the scale factors for the terrain sprite
    float scaleX = static_cast<float>(width) / textureSize.x;
    float scaleY = static_cast<float>(height) / textureSize.y;

    // Set the scale for the terrain sprite
    terrainSprite.setScale(scaleX, scaleY);
    // this->terrainSprite.setScale(this->width / 800.0f, this->height / 600.0f);
}

sf::Image TerrainGenerator::getTerrainImage()
{
    // for (int x = 0; x < this->terrainImage.getSize().x; x++) {
    // int floorheight = getFirstNonTransparentPixelinX(x);
    // terrainImage.setPixel(x, floorheight, sf::Color::Red);
    //}
    return this->terrainImage;
}

float TerrainGenerator::getScreenWidth()
{
    return this->width;
}
float TerrainGenerator::getScreenHeight()
{
    return this->height;
}

TerrainGenerator::Scale TerrainGenerator::getScale()
{
    return this->scale;
}

void TerrainGenerator::updateScale()
{
    sf::Vector2u Image = getTerrainImage().getSize();
    float yScale = static_cast<float>(getScreenHeight() / Image.y);
    float xScale = static_cast<float>(getScreenWidth() / Image.x);
    this->scale.x = xScale;
    this->scale.y = yScale;
}

int TerrainGenerator::getFirstNonTransparentPixelinX(int x)
{
    int retValue = -1;
    for (int y = 0; y < terrainImage.getSize().y; ++y)
    {
        sf::Color pixel = terrainImage.getPixel(x, y);
        if (pixel.a != 0)
        {
            // printf("this pixel is not transparent Y: %d %d\n", x, y);
            retValue = y;
            break;
        }
    } // end for
    return retValue;
}

void TerrainGenerator::updateTerrainWithCollision(int posX, int posY)
{
    float terrainWidth = (static_cast<float>(terrainImage.getSize().x));
    float terrainHeight = (static_cast<float>(terrainImage.getSize().y));
    for (int x = 0; x < 80; x++)
    {
        for (int y = 0; y < 80; y++)
        {
            int circleX = x - 20; // Centering the circle at (20, 20)
            int circleY = y - 20;
            if (circleX * circleX + circleY * circleY <= 20 * 20)
            {
                // If the pixel is within the circle's radius, make it transparent
                if (posX + x - 20 < 0 || posX + x - 20 >= terrainWidth || posY + y - 20 < 0 ||
                    posY + y - 20 >= terrainHeight)
                {
                    continue;
                }
                terrainImage.setPixel((posX) + x - 20, (posY) + y - 20, sf::Color::Transparent);
            }
        }
    }
    terrainTexture.loadFromImage(terrainImage);
    terrainSprite.setTexture(terrainTexture);
}

// BUG BUG explosion not fully affecting terrain but effect seems to be quite nice :) needs resolving at some point.
//  CURRENTLY faked using multiply X 4 when calling this function
void TerrainGenerator::updateTerrainWithExlosion(int posX, int posY, int explosionSizeX, int explosionSizeY)
{
    float terrainWidth = static_cast<float>(terrainImage.getSize().x) / scale.x;
    float terrainHeight = static_cast<float>(terrainImage.getSize().y) / scale.y;

    for (int x = 0; x < explosionSizeX; x++)
    {
        for (int y = 0; y < explosionSizeY; y++)
        {
            int explosionXHalf = explosionSizeX / 2;
            int explosionYHalf = explosionSizeY / 2;
            int circleX = x - explosionXHalf;
            int circleY = y - explosionYHalf;

            if (circleX * circleX + circleY * circleY <= explosionXHalf * explosionYHalf)
            {
                int terrainX = static_cast<int>(posX / scale.x + x - explosionXHalf);
                int terrainY = static_cast<int>(posY / scale.y + y - explosionYHalf);

                // Check if the terrain coordinates are within bounds
                if (terrainX >= 0 && terrainX < terrainWidth && terrainY >= 0 && terrainY < terrainHeight)
                {
                    terrainImage.setPixel(terrainX, terrainY, sf::Color::Transparent);
                }
            }
        }
    }

    terrainTexture.loadFromImage(terrainImage);
    terrainSprite.setTexture(terrainTexture);
}

bool TerrainGenerator::TransparentAtPixel(int x, int y)
{
    // cout << "TerrainGenerator.cpp - Pixel: " << x << ", " << y << endl;
    sf::Color pixel = terrainTexture.copyToImage().getPixel((x / scale.x), (y / scale.y));
    // printf("TerrainGenerator.cpp - TransparentAtPixel() - X: %d, Y: %d Aplha: %d\n", x, y, pixel.a);
    return pixel.a != 0;
}

sf::Sprite TerrainGenerator::getTerrainSprite()
{
    return this->terrainSprite;
}
