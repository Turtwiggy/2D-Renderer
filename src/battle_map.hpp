#pragma once

#include <stdint.h>
#include "sprite_renderer.hpp"
#include "entity_common.hpp"
#include "tilemap.hpp"
#include "random.hpp"
#include "pathfinding.hpp"

#include <entt/entt.hpp>
#include <imgui/imgui.h>

namespace battle_map {

    vec2f convert_xy_to_world(const vec2i pos);

    struct wandering_ai
    {
        vec2i current_xy;
        vec2i destination_xy;

        float time_between_move_tiles = 1.;
        float time_left_between_move_tiles = time_between_move_tiles;

        void update(
            entt::registry& registry, 
            float delta_time, 
            render_descriptor& desc, 
            tilemap& tmap, 
            entt::entity en)
        {
            time_left_between_move_tiles -= delta_time;

            if (time_left_between_move_tiles > 0.)
                return;

            time_left_between_move_tiles = time_between_move_tiles;

            move_ai(registry, desc, tmap, en);
        }

        void move_ai(entt::registry& registry, render_descriptor& desc, tilemap& tmap, entt::entity en)
        {
            vec2i prev = current_xy;

            std::optional<std::vector<vec2i>> path = a_star(registry, tmap, current_xy, destination_xy);

            //printf("start dest value: %d %d \n", current_xy.x(), current_xy.y());
            //printf("end dest value: %d %d \n", destination_xy.x(), destination_xy.y());

            if (path.has_value())
            {
                printf("moving ai");

                //next step
                std::vector p = path.value();
                p.erase(p.begin());
                vec2i next_p = p.front();
                //printf("next_pos: %d %d \n", next_p.x(), next_p.y() );

                //update renderer
                desc.pos = convert_xy_to_world(next_p);
                //update map
                tmap.move(en, current_xy, next_p);
                //update position
                current_xy = next_p;
            } 
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