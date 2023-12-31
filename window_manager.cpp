#include "window_manager.h"

window_manager::window_manager()
{
}

window_manager::display_resolution window_manager::get_display_resolution() const
{
    if (display.height <= 1 || display.width <= 1)
    {
        KJ::debug_output::print(__FILE__, "Display resolution not set", KJ::debug_output::MessageType::FATAL);

        exit(1);
    }
    return display;
}

void window_manager::set_display_resolution(const int width, const int height)
{
    display.height = height;
    display.width = width;
}

window_manager::canvas_resolution window_manager::get_canvas_resolution() const
{
    if (render.height <= 1.0f || render.width <= 1)
    {
        KJ::debug_output::print(__FILE__, "Canvas resolution not set", KJ::debug_output::MessageType::FATAL);
        exit(1);
    }
    return render;
}

void window_manager::set_canvas_resolution(const int width, const int height)
{
    render.height = height;
    render.width = width;
}

window_manager::canvas_scale window_manager::get_scale() const
{
    // Calculate the scale factors for the terrain sprite
    const window_manager::canvas_resolution canvas = window_manager::get_canvas_resolution();
    const window_manager::display_resolution display = window_manager::get_display_resolution();

    canvas_scale return_scale;
    return_scale.x = canvas.width / display.width;
    return_scale.y = canvas.height / display.height;

    KJ::debug_output::print(__FILE__,
                            "Canvas Scale: " + std::to_string(return_scale.x) + "x" + std::to_string(return_scale.y),
                            KJ::debug_output::MessageType::WARNING);
    return return_scale;
}
