#include "overworld_map.hpp"
#include "entity_common.hpp"
#include "overworld_building.hpp"

entt::entity create_overworld_unit(entt::registry& registry, sprite_handle handle, world_transform transform)
{
    entt::entity res = registry.create();

    render_descriptor desc;
    desc.pos = transform.position;
    desc.depress_on_hover = true;

    registry.assign<sprite_handle>(res, handle);
    registry.assign<world_transform>(res, transform);
    registry.assign<overworld_tag>(res, overworld_tag());
    registry.assign<render_descriptor>(res, desc);
    registry.assign<mouse_interactable>(res, mouse_interactable());

    return res;
}

entt::entity create_dummy_army_at(entt::registry& registry, random_state& rng, vec2i pos, int team_id)
{
    world_transform transform;
    transform.position = vec2f{pos.x(), pos.y()} * TILE_PIX + vec2f{TILE_PIX/2, TILE_PIX/2};

    team base_team;
    base_team.type = team::NUMERIC;
    base_team.t = team_id;

    sprite_handle handle = get_sprite_handle_of(rng, tiles::SOLDIER_SPEAR);

    handle.base_colour *= team::colours.at(team_id);

    entt::entity army = create_unit_group(registry, base_team, handle, transform);

    int unit_count = 10;

    for(int i=0; i < unit_count; i++)
    {
        world_transform trans;
        trans.position = vec2f{pos.x(), pos.y()} * TILE_PIX + vec2f{TILE_PIX/2, TILE_PIX/2};

        damageable damage;

        entt::entity en = create_basic_unit(registry, base_team, get_sprite_handle_of(rng, tiles::SOLDIER_SPEAR), trans, damage);

        unit_group& ugroup = registry.get<unit_group>(army);

        ugroup.entities.push_back(en);
    }

    return army;
}

void debug_overworld(entt::registry& registry, entt::entity en, random_state& rng)
{
    tilemap& tmap = registry.get<tilemap>(en);

    vec2i half = tmap.dim/2;

    entt::entity army1 = create_dummy_army_at(registry, rng, {half.x(), half.y() - 1}, 0);
    entt::entity army2 = create_dummy_army_at(registry, rng, {half.x()+1, half.y() - 1}, 1);

    tmap.add(army1, {half.x(), half.y() - 1});
    tmap.add(army2, {half.x()+1, half.y() - 1});
}

entt::entity start_battle(entt::registry& registry, const std::vector<entt::entity>& armies)
{
    entt::entity ret = registry.create();

    return ret;
}
