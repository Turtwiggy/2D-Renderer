#ifndef OVERWORLD_GENERATION_HPP_INCLUDED
#define OVERWORLD_GENERATION_HPP_INCLUDED

#include <entt/entt.hpp>
#include <vec/vec.hpp>

struct random_state;

entt::entity create_overworld(entt::registry& registry, random_state& rng, vec2i dim);

#endif // OVERWORLD_GENERATION_HPP_INCLUDED
