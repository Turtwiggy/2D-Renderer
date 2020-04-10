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
        vec2i current_xy;
        vec2i destination_xy;

        float time_between_move_tiles = 1.;
        float time_left_between_move_tiles = time_between_move_tiles;

        void update(float delta_time, render_descriptor& desc, tilemap& tmap)
        {
            time_left_between_move_tiles -= delta_time;

            if (time_left_between_move_tiles > 0.)
                return;

            time_left_between_move_tiles = time_between_move_tiles;

            move_ai(desc, tmap);
        }

        void move_ai(render_descriptor& desc, tilemap& tmap)
        {
            vec2i prev_pos = current_xy;

            //Update pos
            vec2i new_pos = current_xy;
            new_pos.x() -= 1;

            //Clamp pos
            vec2i clamped_pos = clamp(new_pos, vec2i{ 0, 0 }, tmap.dim );
            current_xy = clamped_pos;
            
            //update render position
            desc.pos = convert_xy_to_world(current_xy);
            //update tmap position
            tmap.move(prev_pos, current_xy);
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

    entt::entity create_battle_unit( entt::registry& registry, sprite_handle handle, world_transform transform, team t);

    void debug_combat(entt::registry& registry, entt::entity battle, random_state& rng);
}