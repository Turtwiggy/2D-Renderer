#ifndef ENTITY_COMMON_HPP_INCLUDED
#define ENTITY_COMMON_HPP_INCLUDED

#include "tilemap.hpp"
#include <entt/entt.hpp>

struct collidable
{
    /// -1 = blocked
    int cost = 0;
};

struct damageable
{
    float health = 1;
};

struct team
{
    int t = 0;
};

struct battle_tag{};
struct overworld_tag{};

#endif // ENTITY_COMMON_HPP_INCLUDED
