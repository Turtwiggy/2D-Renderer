#include "battle_map.hpp"

namespace battle_map {

    entt::entity create_battle(entt::registry& registry, random_state& rng, vec2i dim, level_info::types type)
    {
        entt::entity res = registry.create();

        tilemap tmap;
        tmap.create(dim);

        //Create background tiles
        {
            for (int y = 0; y < dim.y(); y++)
            {
                for (int x = 0; x < dim.x(); x++)
                {
                    sprite_handle handle = get_sprite_handle_of(rng, tiles::BASE);
                    handle.base_colour = clamp(rand_det_s(rng.rng, 0.7, 1.3) * handle.base_colour * 0.1, 0, 1);
                    handle.base_colour.w() = 1;

                    render_descriptor desc;
                    desc.pos = vec2f{ x, y } *TILE_PIX + vec2f{ TILE_PIX / 2, TILE_PIX / 2 };
                    desc.depress_on_hover = true;
                    //desc.angle = rand_det_s(rng.rng, 0.f, 2 * M_PI);

                    entt::entity base = registry.create();

                    registry.assign<sprite_handle>(base, handle);
                    registry.assign<render_descriptor>(base, desc);
                    registry.assign<battle_tag>(base, battle_tag());

                    tmap.add(base, { x, y });
                }
            }
        }

        //Decor
        std::vector<tiles::type> decoration =
        {
            tiles::GRASS
        };
        //distribute_entities(registry, tmap, rng, dim, type, 20, decoration, 0);

        //Scenery
        std::vector<tiles::type> scenery =
        {
            tiles::TREE_1, tiles::TREE_2, tiles::TREE_ROUND, tiles::ROCKS, tiles::BRAMBLE
        };
        //distribute_entities(registry, tmap, rng, dim, type, 1, scenery, -1);

        registry.assign<tilemap>(res, tmap);
        registry.assign<battle_tag>(res, battle_tag());

        return res;
    }

    void distribute_entities(entt::registry& registry, tilemap& tmap, random_state& rng, vec2i dim, level_info::types type, int percentage, const std::vector<tiles::type>& scenery, float path_cost)
    {
        for (int y = 0; y < dim.y(); y++)
        {
            for (int x = 0; x < dim.x(); x++)
            {
                if (!(rand_det_s(rng.rng, 0, 100) < percentage))
                    continue;

                int random_element = rand_det_s(rng.rng, 0.f, scenery.size());

                random_element = clamp(random_element, 0, (int)scenery.size() - 1);

                tiles::type type = scenery[random_element];

                sprite_handle handle = get_sprite_handle_of(rng, type);
                handle.base_colour.w() = 1;

                world_transform trans;
                trans.position = vec2f{ x, y } *TILE_PIX + vec2f{ TILE_PIX / 2, TILE_PIX / 2 };

                collidable coll;
                coll.cost = path_cost;

                auto base = create_scenery(registry, handle, trans, coll);

                tmap.add(base, { x, y });
            }
        }
    }

    entt::entity create_battle_unit( 
        entt::registry& registry, 
        sprite_handle handle, 
        world_transform transform,
        team t)
    {
        entt::entity res = registry.create();

        render_descriptor desc;
        desc.pos = transform.position;
        desc.depress_on_hover = true;

        registry.assign<sprite_handle>(res, handle);
        registry.assign<world_transform>(res, transform);
        registry.assign<render_descriptor>(res, desc);
        registry.assign<mouse_interactable>(res, mouse_interactable());

        registry.assign<damageable>(res, damageable());
        registry.assign<team>(res, t);
        registry.assign<battle_tag>(res, battle_tag());

        return res;
    }

    entt::entity create_battle_unit_at(entt::registry& registry, random_state& rng, vec2i pos, int team_id)
    {
        world_transform transform;
        transform.position = vec2f{pos.x(), pos.y()} *TILE_PIX + vec2f{ TILE_PIX / 2, TILE_PIX / 2 };

        team base_team;
        base_team.type = team::NUMERIC;
        base_team.t = team_id;

        sprite_handle handle = get_sprite_handle_of(rng, tiles::SOLDIER_SPEAR);

        handle.base_colour *= team::colours.at(team_id);

        entt::entity unit = create_battle_unit(registry, handle, transform, base_team);

        return unit;
    }


    void debug_combat(entt::registry& registry, entt::entity battle, random_state& rng)
    {
        ImGui::Begin("Battle Editor");

        if (ImGui::Button("Add units to map"))
        {
            tilemap& tmap = registry.get<tilemap>(battle);

            vec2i half = tmap.dim / 2;

            sprite_handle handle = get_sprite_handle_of(rng, tiles::type::SOLDIER_SPEAR);

            battle_unit_info info;
            info.hp = 100;

            world_transform transform;
            transform.position = { half.x(), half.y() - 1 };

            entt::entity unit = create_battle_unit_at(registry, rng, { half.x(), half.y() - 1 }, 0);
            entt::entity enemy_unit = create_battle_unit_at(registry, rng, { half.x() + 4, half.y() - 1 }, 1);

            tmap.add(unit, { half.x(), half.y() - 1 });
            tmap.add(enemy_unit, { half.x() + 4, half.y() - 1 });
        }

        ImGui::End();
    }
}
