#include "battle_map.hpp"

#include "battle_map_ai.hpp"


entt::entity battle_map::create_battle(entt::registry& registry, random_state& rng, vec2i dim, level_info::types type)
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
                //handle.base_colour = clamp(rand_det_s(rng.rng, 0.7, 1.3) * handle.base_colour * 0.1, 0, 1);
                //handle.base_colour.w() = 1;

                render_descriptor desc;
                desc.pos = vec2f{ x, y } *TILE_PIX + vec2f{ TILE_PIX / 2, TILE_PIX / 2 };
                desc.depress_on_hover = true;
                //desc.angle = rand_det_s(rng.rng, 0.f, 2 * M_PI);

                entt::entity base = registry.create();

                registry.assign<sprite_handle>(base, handle);
                registry.assign<render_descriptor>(base, desc);
                registry.assign<battle_tag>(base, battle_tag());
                //registry.assign<mouse_interactable>(base, mouse_interactable());

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
    registry.assign<battle_map_state>(res, battle_map_state());

    return res;
}

void battle_map::distribute_entities(entt::registry& registry, tilemap& tmap, random_state& rng, vec2i dim, level_info::types type, int percentage, const std::vector<tiles::type>& scenery, float path_cost)
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


entt::entity battle_map::create_battle_unit(entt::registry& registry, sprite_handle handle, world_transform transform, team t)
{
    entt::entity res = registry.create();

    render_descriptor desc;
    desc.pos = transform.position;
    desc.depress_on_hover = true;

    registry.assign<sprite_handle>(res, handle);
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
    transform.position = convert_xy_to_world(pos);

    team base_team;
    base_team.type = team::NUMERIC;
    base_team.t = team_id;

    sprite_handle handle = get_sprite_handle_of(rng, tiles::SOLDIER_SPEAR);
    handle.base_colour *= team::colours.at(team_id);

    entt::entity unit = battle_map::create_battle_unit(registry, handle, transform, base_team);

    return unit;
}


entt::entity battle_map::create_obstacle(entt::registry& registry, sprite_handle handle, world_transform transform, int path_cost)
{
    entt::entity res = registry.create();

    render_descriptor desc;
    desc.pos = transform.position;
    desc.depress_on_hover = true;

    registry.assign<sprite_handle>(res, handle);
    registry.assign<render_descriptor>(res, desc);
    registry.assign<mouse_interactable>(res, mouse_interactable());

    collidable c;
    c.cost = path_cost;
    registry.assign<collidable>(res, c);
    // registry.assign<battle_unit>(res, battle_unit());

    return res;
}

entt::entity create_obstacle_at(entt::registry& registry, random_state& rng, vec2i pos, tilemap& map, sprite_handle handle, int path_cost)
{
    world_transform transform;
    transform.position = convert_xy_to_world(pos);

    entt::entity obstacle = battle_map::create_obstacle(registry, handle, transform, path_cost);

    map.add(obstacle, pos);
}


void battle_map::battle_map_state::update_ai(entt::registry& registry, entt::entity& map, random_state& rng, float delta_time, camera& cam, render_window& win)
{
    auto view = registry.view < battle_tag, render_descriptor, sprite_handle, wandering_ai> ();

    tilemap& tmap = registry.get<tilemap>(map);

    for (auto ent : view)
    {
        auto& ai = view.get<wandering_ai>(ent);
        auto& desc = view.get<render_descriptor>(ent);

        ai.tick_ai(registry, delta_time, desc, tmap, ent, cam, win);
        ai.tick_animation(delta_time, desc);
    }
}

void battle_map::battle_map_state::debug_combat(entt::registry& registry, entt::entity& map, random_state& rng, render_window& win, camera& cam, vec2f mpos)
{
    battle_map_state& state = registry.get<battle_map::battle_map_state>(map);

    bool mouse_clicked = ImGui::IsMouseClicked(0) && !ImGui::IsAnyWindowHovered();
    bool mouse_hovering = !ImGui::IsAnyWindowHovered();

    if (mouse_clicked)
    {
        tilemap& tmap = registry.get<tilemap>(map);

        vec2i clamped_i_tile = clamp
        (
            convert_world_to_xy(mpos, cam, win), 
            vec2i{ 0, 0 }, 
            tmap.dim - 1
        );
        printf(" clicked tile: %d %d \n", clamped_i_tile.x(), clamped_i_tile.y());

        if (state.current_item == "Obstacles")
        {
            sprite_handle handle = get_sprite_handle_of(rng, tiles::type::CACTUS);
            create_obstacle_at(registry, rng, clamped_i_tile, tmap, handle, -1);
        }
        
        if (state.current_item == "Enemies")
        {
            vec2i half = tmap.dim / 2;

            //add enemy
            vec2i start_pos = clamped_i_tile;
            vec2i dest_pos = tmap.dim - 1;

            entt::entity enemy_unit = create_battle_unit_at(registry, rng, start_pos, 1);

            wandering_ai ai;
            ai.current_xy = start_pos;
            ai.destination_xy = dest_pos;
            registry.assign<wandering_ai>(enemy_unit, ai);

            collidable coll;
            coll.cost = 10;
            registry.assign<collidable>(enemy_unit, coll);

            tmap.add(enemy_unit, start_pos);
        }

        if (state.current_item == "Player")
        {
            entt::entity unit = create_battle_unit_at(registry, rng, clamped_i_tile, 0);

            collidable coll;
            coll.cost = 0;
            registry.assign<collidable>(unit, coll);

            tmap.add(unit, clamped_i_tile);
        }
    }

    ImGui::Begin("Battle Editor");

    if (ImGui::BeginCombo("##combo", state.current_item.c_str() )) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < state.items.size(); n++)
        {
            bool is_selected = (state.current_item == state.items[n]); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(state.items[n].c_str(), is_selected)) 
            {
                printf("Selected! \n");
                state.current_item = state.items[n];
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }

    ImGui::End();

    ImGui::Begin("Unit Editor");

    auto view = registry.view<battle_tag, render_descriptor, sprite_handle, damageable, wandering_ai>();

    for (auto ent : view)
    {
        auto& ai = view.get<wandering_ai>(ent);
        auto& health = view.get<damageable>(ent);

        if (health.cur_hp <= 0)
            continue;

        std::string name = "Unit name: STEVE";
        ImGui::Text(name.c_str());

        std::string hp = "Unit hp: " + std::to_string(health.cur_hp);
        ImGui::Text(hp.c_str());

        std::string attack = "Unit attack: " + std::to_string(1);
        ImGui::Text(attack.c_str());;

        if (ImGui::Button("Destroy unit!"))
        {
            tilemap& tmap = registry.get<tilemap>(map);
            tmap.remove( ent, ai.current_xy );

            health.damage_amount(health.max_hp);
        };
    }

    ImGui::End();
}


