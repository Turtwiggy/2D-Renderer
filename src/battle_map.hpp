#pragma once

#include <stdint.h>

#include "sprite_renderer.hpp"
#include "entity_common.hpp"
#include "tilemap.hpp"
#include "random.hpp"

#include <entt/entt.hpp>
#include <imgui/imgui.h>

namespace battle_map {

    struct battle_unit_info
    {
        float damage = 1;
    };

    struct battle_map_state
    {
        //Combobox options
        std::vector<std::string> items = {
            "Enemies", "Obstacles", "Player"
        };
        std::string current_item = "Enemies";

        void update_ai(entt::registry& registry, entt::entity& map, float delta_time, random_state& rng);
        void debug_combat(entt::registry& registry, entt::entity& map, random_state& rng, render_window& win, camera& cam, vec2f mpos);
    
        //void reset_tilemap_colours(tilemap& tmap, entt::registry& registry);    
    }; 
    
    entt::entity create_battle(entt::registry& registry, random_state& rng, vec2i dim, level_info::types type);
    void distribute_entities(entt::registry& registry, tilemap& tmap, random_state& rng, vec2i dim, level_info::types type, int percentage, const std::vector<tiles::type>& scenery, float path_cost);
    entt::entity create_battle_unit( entt::registry& registry, sprite_handle handle, tilemap_position transform, team t);
    entt::entity create_obstacle(entt::registry& registry, sprite_handle handle, tilemap_position transform, int path_cost);

}