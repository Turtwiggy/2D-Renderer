#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include <vec/vec.hpp>

#define TILE_PIX 16
#define TILE_SEP 1

struct render_window;

struct camera
{
    vec2f pos;

    vec2f tile_to_screen(render_window& win, vec2f tile_pos);
    vec2f screen_to_tile(render_window& win, vec2f screen_pos);
};

#endif // CAMERA_HPP_INCLUDED