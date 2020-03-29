#ifndef ENTITY_COMMON_HPP_INCLUDED
#define ENTITY_COMMON_HPP_INCLUDED

#include <entt/entt.hpp>
#include <vector>
#include <vec/vec.hpp>

struct sprite_handle;
struct world_transform;

struct collidable
{
    /// -1 = blocked
    int cost = 0;
};

struct team
{
    enum team_type
    {
        NUMERIC,
        NEUTRAL,
    };

    team_type type = NUMERIC;

    int t = 0;
};

struct damageable
{
    float max_hp = 1;
    float cur_hp = max_hp;

    void reset();
    void damage_amount(float amount);
    void damage_total_fraction(float frac);
    void damage_remaining_fraction(float frac);
};

struct battle_tag{};
struct overworld_tag{};

struct army
{
    std::vector<entt::entity> entities;
};

entt::entity create_basic_unit(entt::registry& registry, const team& t, const sprite_handle& handle, const world_transform& transform, const damageable& damage);
entt::entity create_scenery(entt::registry& registry, const sprite_handle& handle, const world_transform& transform, const collidable& coll);

#endif // ENTITY_COMMON_HPP_INCLUDED
