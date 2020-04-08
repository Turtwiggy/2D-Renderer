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

    static inline std::vector<vec4f> colours
    {
        srgb_to_lin_approx(vec4f{0x77/255.f, 0xdd/255.f, 0x77/255.f, 1}),
        srgb_to_lin_approx(vec4f{0xff/255.f, 0x69/255.f, 0x61/255.f, 1}),
        srgb_to_lin_approx(vec4f{0xff/255.f, 0x96/255.f, 0x4f/255.f, 1}),
        srgb_to_lin_approx(vec4f{0x89/255.f, 0xcf/255.f, 0xf0/255.f, 1}),
        srgb_to_lin_approx(vec4f{0xfd/255.f, 0xfd/255.f, 0x96/255.f, 1}),
        srgb_to_lin_approx(vec4f{0xf4/255.f, 0xbf/255.f, 0xff/255.f, 1}),
        srgb_to_lin_approx(vec4f{0xf4/255.f, 0x9a/255.f, 0xc2/255.f, 1}),
        srgb_to_lin_approx(vec4f{0xf4/255.f, 0x9a/255.f, 0xc2/255.f, 1}),
        srgb_to_lin_approx(vec4f{0xbe/255.f, 0xfd/255.f, 0x73/255.f, 1}),
    };
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

struct mouse_interactable
{
    bool just_clicked = false;
    bool is_hovered = false;
};

void reset_interactable_state(entt::registry& registry, entt::entity en);

struct battle_tag{};
struct overworld_tag{};

struct unit_group
{
    std::vector<entt::entity> entities;
};

entt::entity create_basic_unit(entt::registry& registry, const team& t, const sprite_handle& handle, const world_transform& transform, const damageable& damage);
entt::entity create_scenery(entt::registry& registry, const sprite_handle& handle, const world_transform& transform, const collidable& coll);

entt::entity create_unit_group(entt::registry& registry, const team& t, const sprite_handle& handle, const world_transform& transform);

#endif // ENTITY_COMMON_HPP_INCLUDED
