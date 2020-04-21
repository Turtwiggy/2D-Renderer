#include "overworld_map.hpp"
#include "entity_common.hpp"
#include "overworld_building.hpp"

entt::entity create_overworld_unit(entt::registry& registry, sprite_handle handle, tilemap_position transform)
{
    entt::entity res = registry.create();

    render_descriptor desc;
    desc.pos = camera::tile_to_world(vec2f{ transform.pos.x(), transform.pos.y() });
    desc.depress_on_hover = true;

    registry.assign<sprite_handle>(res, handle);
    registry.assign<tilemap_position>(res, transform);
    registry.assign<overworld_tag>(res, overworld_tag());
    registry.assign<render_descriptor>(res, desc);
    registry.assign<mouse_interactable>(res, mouse_interactable());

    return res;
}

entt::entity create_dummy_army_at(entt::registry& registry, random_state& rng, vec2i pos, int team_id)
{
    tilemap_position transform;
    transform.pos = vec2i{ pos.x(), pos.y() };

    team base_team;
    base_team.type = team::NUMERIC;
    base_team.t = team_id;

    sprite_handle handle = get_sprite_handle_of(rng, tiles::SOLDIER_SPEAR);

    handle.base_colour *= team::colours.at(team_id);

    entt::entity army = create_unit_group(registry, base_team, handle, transform);

    int unit_count = 10;

    for(int i=0; i < unit_count; i++)
    {
        tilemap_position trans;
        trans.pos = vec2i{ pos.x(), pos.y() };

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
