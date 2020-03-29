#include "entity_common.hpp"
#include "sprite_renderer.hpp"

void damageable::reset()
{
    cur_hp = max_hp;
}

void damageable::damage_amount(float amount)
{
    cur_hp -= amount;
    cur_hp = clamp(cur_hp, 0.f, max_hp);
}

void damageable::damage_total_fraction(float frac)
{
    cur_hp = cur_hp - max_hp * frac;
    cur_hp = clamp(cur_hp, 0.f, max_hp);
}

void damageable::damage_remaining_fraction(float frac)
{
    cur_hp *= frac;
    cur_hp = clamp(cur_hp, 0.f, max_hp);
}

entt::entity create_basic_unit(entt::registry& registry, const team& t, const sprite_handle& handle, const world_transform& transform, const damageable& damage)
{
    entt::entity res = registry.create();

    render_descriptor desc;
    desc.pos = transform.position;

    registry.assign<team>(res, t);
    registry.assign<sprite_handle>(res, handle);
    registry.assign<world_transform>(res, transform);
    registry.assign<damageable>(res, damage);

    registry.assign<render_descriptor>(res, desc);

    return res;
}

entt::entity create_scenery(entt::registry& registry, const sprite_handle& handle, const world_transform& transform, const collidable& coll)
{
    entt::entity res = registry.create();

    render_descriptor desc;
    desc.pos = transform.position;

    team t;
    t.type = team::NEUTRAL;

    registry.assign<render_descriptor>(res, desc);
    registry.assign<team>(res, t);
    registry.assign<sprite_handle>(res, handle);
    registry.assign<world_transform>(res, transform);
    registry.assign<collidable>(res, coll);

    return res;
}
