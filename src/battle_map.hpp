#pragma once

#include <entt/entt.hpp>
#include <stdint.h>
#include "sprite_renderer.hpp"
#include "tilemap.hpp"

namespace battle_map {

    struct battle_unit_info
    {
        float hp = 1;
    };

    void create_background_tiles(entt::registry& registry, vec2i dim, tilemap& tmap, random_state& rng);

    void distribute_entities(entt::registry& registry, tilemap& tmap, random_state& rng, vec2i dim, level_info::types type, int percentage, const std::vector<tiles::type>& scenery, float path_cost);

    entt::entity create_battle_unit(entt::registry& registry, sprite_handle handle, world_transform transform, battle_unit_info info);
    entt::entity create_battle(entt::registry& registry, random_state& rng, vec2i dim, level_info::types type);

}