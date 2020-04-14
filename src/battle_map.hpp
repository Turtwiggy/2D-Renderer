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
        float hp = 1;
    };
    struct battle_unit {};

    //enum unit_placing_state
    //{
    //    POSITION_NEW_UNIT,
    //    SET_UNIT_DESTINATION
    //};
    //inline std::vector<std::string> unit_states {
    //    "Positining new unit",
    //    "Setting unit destination"
    //};
    //std::string unit_placing_state_to_string(unit_placing_state& state);

    struct battle_map_state
    {
        //unit_placing_state place_state = unit_placing_state::POSITION_NEW_UNIT;

        //std::optional<entt::entity> last_placed_unit;

        //Combobox options
        //std::vector<std::string> items = {
        //    "Units", "Obstacles"
        //};
        //std::string current_item = items[0];

        void update_ai(entt::registry& registry, entt::entity& map, float delta_time);
        void debug_combat(entt::registry& registry, entt::entity& map, random_state& rng, render_window& win, camera& cam, vec2f mpos);
    }; 
    
    entt::entity create_battle(entt::registry& registry, random_state& rng, vec2i dim, level_info::types type);
    void distribute_entities(entt::registry& registry, tilemap& tmap, random_state& rng, vec2i dim, level_info::types type, int percentage, const std::vector<tiles::type>& scenery, float path_cost);
    entt::entity create_battle_unit( entt::registry& registry, sprite_handle handle, world_transform transform, team t);
    entt::entity create_obstacle(entt::registry& registry, sprite_handle handle, world_transform transform);



}