#ifndef PATHFINDING_HPP_INCLUDED
#define PATHFINDING_HPP_INCLUDED

#include <vector>
#include <map>
#include <optional>
#include <vec/vec.hpp>
#include <entt/entt.hpp>

struct tilemap;

std::optional<std::vector<vec2i>> a_star(entt::registry& registry, tilemap& tmap, vec2i first, vec2i finish);

#endif // PATHFINDING_HPP_INCLUDED
