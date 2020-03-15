#include "camera.hpp"
#include <toolkit/render_window.hpp>

vec2f camera::tile_to_screen(render_window& win, vec2f tile_pos)
{
    vec2i screen_dim = win.get_window_size();

    vec2f half_dim = (vec2f){screen_dim.x(), screen_dim.y()}/2.f;

    vec2f relative = (vec2f){tile_pos.x(), tile_pos.y()} * TILE_PIX - pos + half_dim;

    return relative;
}
 
vec2f camera::screen_to_tile(render_window& win, vec2f screen_pos)
{
    vec2i screen_dim = win.get_window_size();

    vec2f half_dim = (vec2f){screen_dim.x(), screen_dim.y()}/2.f;

    vec2f relative = screen_pos - half_dim + pos;

    vec2f tile_coord = relative / (float)TILE_PIX;

    return tile_coord;
}