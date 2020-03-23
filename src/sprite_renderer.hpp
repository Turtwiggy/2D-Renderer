#ifndef SPRITE_RENDERER_HPP_INCLUDED
#define SPRITE_RENDERER_HPP_INCLUDED

#include <vector>
#include <vec/vec.hpp>
#include <toolkit/texture.hpp>
#include "camera.hpp"
#include <entt/entt.hpp>

struct render_window;
struct camera;

struct sprite_handle
{
    vec2i offset;
    vec4f base_colour = {1,1,1,1};
};

struct world_transform
{
    vec2f position;
};

struct render_descriptor
{
    vec2f pos; //in pixels, centered
    vec4f colour = {1,1,1,1}; //currently srgb but i will fix this with a sledgehammer in the future
    float angle = 0; //in radians, about the origin, which is currently the centre
    vec2f size;
};

struct sprite_renderer
{
    std::vector<std::pair<sprite_handle, render_descriptor>> next_renderables;
    texture sprite_sheet;

    sprite_renderer();

    void add(const sprite_handle& handle, const render_descriptor& descriptor);
    void render(render_window& window, const camera& cam);
};

#endif // SPRITE_RENDERER_HPP_INCLUDED
