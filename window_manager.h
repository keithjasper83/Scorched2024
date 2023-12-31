#pragma once
#include "SFML/Graphics.hpp"
#include "debug_output.h"
#include <iostream>

class window_manager
{
  public:
    struct display_resolution
    {
        int height = 0, width = 0;
        std::string to_string()
        {
            return std::to_string(width) + "x" + std::to_string(height);
        }
    };
    struct canvas_resolution
    {
        int height = 0, width = 0;
        std::string to_string()
        {
            return std::to_string(width) + "x" + std::to_string(height);
        }
    };
    struct canvas_scale
    {
        float x, y = 0.0f;
    };

    window_manager();

    display_resolution get_display_resolution() const;
    void set_display_resolution(const int width, const int height);
    canvas_resolution get_canvas_resolution() const;
    void set_canvas_resolution(const int width, const int height);
    canvas_scale get_scale() const;

  private:
    display_resolution display;
    canvas_resolution render;
};
