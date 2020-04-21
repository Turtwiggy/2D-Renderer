#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include <vec/vec.hpp>

#define TILE_PIX 16
#define TILE_SEP 1

struct render_window;

struct camera
{
    vec2f pos;
    float zoom_level = 0;

    vec2f tile_to_screen(render_window& win, vec2f tile_pos) const;
    vec2f screen_to_tile(render_window& win, vec2f screen_pos) const;
    vec2f world_to_screen(render_window& win, vec2f world_pos) const;
    vec2f screen_to_world(render_window& win, vec2f screen_pos) const;
    static vec2f tile_to_world(vec2f pos);
    //static vec2f world_to_tile(vec2f pos);

    void translate(vec2f amount);

    void zoom(float number_of_levels);

    float calculate_scale() const;
};

#endif // CAMERA_HPP_INCLUDED
