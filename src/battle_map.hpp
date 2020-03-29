#pragma once

#include <entt/entt.hpp>
#include <stdint.h>
#include "sprite_renderer.hpp"
#include "tilemap.hpp"

struct battle_unit_info
{
    float hp = 1;
};

entt::entity create_battle_unit(entt::registry& registry, sprite_handle handle, world_transform transform, battle_unit_info info);
entt::entity create_battle(entt::registry& registry, random_state& rng, vec2i dim, level_info::types type);
