#pragma once
#include "Renderer.h"

class game
{
  public:
    game(int screen_width, int screen_height);

    void run();

  private:
    renderer renderer_; // Your Renderer class
    int screen_width_;
    int screen_height_;

    // Add other game-related members here

    void update();

    void render();
};
