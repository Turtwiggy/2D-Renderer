#include "battle_map.hpp"

entt::entity create_battle_unit(entt::registry& registry, sprite_handle handle, world_transform transform, battle_unit_info info)
{
    entt::entity res = registry.create();

    render_descriptor desc;
    desc.pos = transform.position;

    registry.assign<sprite_handle>(res, handle);
    registry.assign<world_transform>(res, transform);
    registry.assign<battle_unit_info>(res, info);

    return res;
}

tilemap create_battle(entt::registry& registry, random_state& rng, vec2i dim, level_info::types type)
{
    tilemap ret;
    ret.create(dim);

    for (int y = 0; y < dim.y(); y++)
    {
        for (int x = 0; x < dim.x(); x++)
        {
            auto render_type = tiles::BASE;

            /*renderable_object robj;
            robj.tile_id = get_tile_of(render_type);
            robj.lin_colour = get_colour_of(render_type, type);

            robj.lin_colour = clamp(rand_det_s(rng, 0.7, 1.3) * robj.lin_colour, 0, 1);

            tile_object obj;
            obj.obj = robj;

            ret.add([y * dim.x() + x].push_back(obj);*/

            sprite_handle handle = get_sprite_handle_of(rng, tiles::BASE);
            handle.base_colour = clamp(rand_det_s(rng.rng, 0.3, 1.7) * handle.base_colour * 0.2, 0, 1);
            handle.base_colour.w() = 1;

            render_descriptor desc;
            desc.pos = vec2f{ x, y } *TILE_PIX + vec2f{ TILE_PIX / 2, TILE_PIX / 2 };
            //desc.angle = rand_det_s(rng.rng, 0.f, 2 * M_PI);

            entt::entity base = registry.create();

            registry.assign<sprite_handle>(base, handle);
            registry.assign<render_descriptor>(base, desc);

            ret.add(base, { x, y });
        }
    }

    return ret;
}
