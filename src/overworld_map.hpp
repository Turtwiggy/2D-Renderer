#ifndef OVERWORLD_MAP_HPP_INCLUDED
#define OVERWORLD_MAP_HPP_INCLUDED

#include <entt/entt.hpp>
#include "tilemap.hpp"

entt::entity create_overworld_unit(entt::registry& registry, sprite_handle handle, tilemap_position transform);
void debug_overworld(entt::registry& registry, entt::entity en, random_state& rng);
entt::entity start_battle(entt::registry& registry, const std::vector<entt::entity>& armies);

#endif // OVERWORLD_MAP_HPP_INCLUDED
