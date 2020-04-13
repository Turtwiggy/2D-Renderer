#pragma once

#include "sprite_renderer.hpp"
#include "entity_common.hpp"
#include "tilemap.hpp"
#include "random.hpp"
#include "pathfinding.hpp"

struct wandering_ai
{
    vec2i current_xy;
    vec2i destination_xy;

    float time_between_move_tiles = 1.;
    float time_left_between_move_tiles = time_between_move_tiles;

    void update_ai(entt::registry& registry, float delta_time, render_descriptor& desc, tilemap& tmap, entt::entity en);
    void move_ai(entt::registry& registry, render_descriptor& desc, tilemap& tmap, entt::entity en);
};