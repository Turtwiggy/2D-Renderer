#pragma once

#include "sprite_renderer.hpp"
#include "tilemap.hpp"
#include "random.hpp"
#include "pathfinding.hpp"

struct wandering_ai
{
    //pathfinding
    vec2i current_xy;
    vec2i destination_xy;
    float time_between_move_tiles = 1.;
    float time_left_before_move_tiles = time_between_move_tiles;

    //animation
    float time_between_animation_updates = 0.3f;
    float time_left_before_animation_update = time_between_animation_updates;
    bool on_max_scale = true;
    vec2f max_scale = { 1., 1. };
    vec2f min_scale = { 0.95f, 0.9f };

    void tick_ai
    (
        entt::registry&     registry, 
        float               delta_time, 
        render_descriptor&  desc, 
        tilemap&            tmap, 
        entt::entity        en,
        camera& cam,
        render_window& win
    );

    void move_ai
    (
        entt::registry&     registry,
        render_descriptor&  desc,
        tilemap&            tmap,
        entt::entity        en,
        camera&             cam, 
        render_window&      win
    );

    void tick_animation
    (
        float               delta_time,
        render_descriptor&  desc
    );

    void update_animation
    (
        render_descriptor&  desc
    );
   
    void reset_tilemap_colours(tilemap& tmap, entt::registry& registry);
    void show_path_colours_on_tilemap(tilemap& tmap, entt::registry& registry, std::vector<vec2i> points, vec2i destination);
};

