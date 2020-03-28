#include "overworld_map.hpp"
#include "entity_common.hpp"

entt::entity create_overworld_unit(entt::registry& registry, sprite_handle handle, world_transform transform)
{
    entt::entity res = registry.create();

    render_descriptor desc;
    desc.pos = transform.position;

    registry.assign<sprite_handle>(res, handle);
    registry.assign<world_transform>(res, transform);
    registry.assign<overworld_tag>(res, overworld_tag());

    return res;
}

entt::entity create_overworld(entt::registry& registry, random_state& rng, vec2i dim)
{
     entt::entity res = registry.create();

    tilemap tmap;
    tmap.create(dim);

    for (int y = 0; y < dim.y(); y++)
    {
        for (int x = 0; x < dim.x(); x++)
        {
            auto render_type = tiles::BASE;

            sprite_handle handle = get_sprite_handle_of(rng, tiles::BASE);
            handle.base_colour = clamp(rand_det_s(rng.rng, 0.5, 1.5) * handle.base_colour * 0.2, 0, 1);
            handle.base_colour.w() = 1;

            render_descriptor desc;
            desc.pos = vec2f{ x, y } * TILE_PIX + vec2f{ TILE_PIX / 2, TILE_PIX / 2 };

            entt::entity base = registry.create();

            registry.assign<sprite_handle>(base, handle);
            registry.assign<render_descriptor>(base, desc);
            registry.assign<overworld_tag>(base, overworld_tag());

            tmap.add(base, { x, y });
        }
    }

    registry.assign<tilemap>(res, tmap);
    registry.assign<overworld_tag>(res, overworld_tag());

    return res;
}
