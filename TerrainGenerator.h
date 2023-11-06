#pragma once

#include "Config.h"
#include <SFML/Graphics.hpp>

class TerrainGenerator
{
  public:
    struct Scale
    {
        float x;
        float y;
    };

    sf::RenderWindow renderer;
    sf::Texture terrainTexture;
    sf::Sprite terrainSprite;
    sf::Image terrainImage;
    Scale scale;

    TerrainGenerator();
    TerrainGenerator(int width, int height);
    void setScreenSize(int width, int height);
    void updateSpriteSize();
    sf::Image getTerrainImage();
    int getFirstNonTransparentPixelinX(int x);
    void updateTerrainWithCollision(int posX, int posY);
    float getScreenWidth();
    float getScreenHeight();
    Scale getScale();
    void updateScale();
    void updateTerrainWithExlosion(int posX, int posY, int explosionSizeX, int explosionSizeY);
    bool TransparentAtPixel(int x, int y);
    sf::Sprite getTerrainSprite();

  private:
    int width = constants::SCREEN_WIDTH;
    int height = constants::SCREEN_HEIGHT;
};
