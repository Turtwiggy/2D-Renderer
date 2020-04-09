#pragma once

#include <entt/entt.hpp>
#include <stdint.h>
#include "sprite_renderer.hpp"
#include "entity_common.hpp"
#include "tilemap.hpp"
#include "random.hpp"

#include <imgui/imgui.h>

namespace battle_map {

    vec2f convert_xy_to_world(const vec2i pos);

    struct wandering_ai
    {
        vec2i current_xy = { 15, 15 };
        vec2i destination_xy = { 0, 0 };

        float time_between_move_tiles = 1.;
        float time_left_between_move_tiles = time_between_move_tiles;

        vec2f convert_current_xy_to_pos()
        {
            return convert_xy_to_world(current_xy);
        };

        void update(float delta_time)
        {
            time_left_between_move_tiles -= delta_time;

            if (time_left_between_move_tiles > 0.)
                return;

            time_left_between_move_tiles = time_between_move_tiles;

            printf("moving ai");
            move_ai();
        }

        void move_ai()
        {
            vec2i clamped_pos = clamp(current_xy, vec2i{ 0, 0 }, vec2i{ 30, 30 });

            //move left
            current_xy.x() = clamped_pos.x() - 1;
            //current_xy.y() = clamped_pos.y() - 1;
        }
    };

    void update_ai(entt::registry& registry, entt::entity& map, float delta_time);


    struct battle_unit_info
    {
        float hp = 1;
    };
    struct battle_unit {};

    entt::entity create_battle(entt::registry& registry, random_state& rng, vec2i dim, level_info::types type);
    
    void distribute_entities(entt::registry& registry, tilemap& tmap, random_state& rng, vec2i dim, level_info::types type, int percentage, const std::vector<tiles::type>& scenery, float path_cost);

    entt::entity create_battle_unit(
        entt::registry& registry,
        sprite_handle handle,
        world_transform transform,
        team t);

    void debug_combat(entt::registry& registry, entt::entity battle, random_state& rng);
}