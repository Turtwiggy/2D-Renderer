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

void reset_interactable_state(entt::registry& registry,entt::entity en)
{
    auto& mouse = registry.get<mouse_interactable>(en);

    mouse.is_hovered = false;
    mouse.just_clicked = false;
}

entt::entity create_basic_unit(entt::registry& registry, const team& t, const sprite_handle& handle, const world_transform& transform, const damageable& damage)
{
    entt::entity res = registry.create();

    render_descriptor desc;
    desc.pos = transform.position;
    desc.depress_on_hover = true;

    registry.assign<team>(res, t);
    registry.assign<sprite_handle>(res, handle);
    registry.assign<world_transform>(res, transform);
    registry.assign<damageable>(res, damage);

    registry.assign<render_descriptor>(res, desc);
    registry.assign<mouse_interactable>(res, mouse_interactable());

    return res;
}

entt::entity create_scenery(entt::registry& registry, const sprite_handle& handle, const world_transform& transform, const collidable& coll)
{
    entt::entity res = registry.create();

    render_descriptor desc;
    desc.pos = transform.position;
    desc.depress_on_hover = true;

    team t;
    t.type = team::NEUTRAL;

    registry.assign<render_descriptor>(res, desc);
    registry.assign<team>(res, t);
    registry.assign<sprite_handle>(res, handle);
    registry.assign<world_transform>(res, transform);
    registry.assign<collidable>(res, coll);
    registry.assign<mouse_interactable>(res, mouse_interactable());

    return res;
}


entt::entity create_unit_group(entt::registry& registry, const team& t, const sprite_handle& handle, const world_transform& transform)
{
    entt::entity res = registry.create();

    render_descriptor desc;
    desc.pos = transform.position;
    desc.depress_on_hover = true;

    registry.assign<render_descriptor>(res, desc);
    registry.assign<team>(res, t);
    registry.assign<sprite_handle>(res, handle);
    registry.assign<world_transform>(res, transform);

    unit_group ugroup;

    registry.assign<unit_group>(res, ugroup);

    registry.assign<mouse_interactable>(res, mouse_interactable());

    return res;
}


vec2f convert_xy_to_world(const vec2i pos)
{
    return vec2f{ pos.x(), pos.y() } *TILE_PIX + vec2f{ TILE_PIX / 2, TILE_PIX / 2 };
}

vec2i convert_world_to_xy(vec2f pos, camera& cam, render_window& win)
{
    vec2f mouse_tile = cam.screen_to_tile(win, pos);

    vec2i i_tile = { mouse_tile.x(), mouse_tile.y() };

    return i_tile;
}