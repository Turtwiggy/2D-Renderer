#ifndef OVERWORLD_MAP_HPP_INCLUDED
#define OVERWORLD_MAP_HPP_INCLUDED

#include <entt/entt.hpp>
#include "tilemap.hpp"

entt::entity create_overworld_unit(entt::registry& registry, sprite_handle handle, world_transform transform);
entt::entity create_overworld(entt::registry& registry, random_state& rng, vec2i dim);

#endif // OVERWORLD_MAP_HPP_INCLUDED
