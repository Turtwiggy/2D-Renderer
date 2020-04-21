#include "battle_map_ai.hpp"

#include "camera.hpp"
#include "battle_map.hpp"

void wandering_ai::tick_ai
(
    entt::registry&     registry,
    float               delta_time,
    tilemap&            tmap,
    entt::entity        en,
    random_state&       rng
)
{
    time_left_before_move_tiles -= delta_time;

    if (time_left_before_move_tiles > 0.)
        return;

    time_left_before_move_tiles = time_between_move_tiles;

    move_ai(registry, tmap, en, rng);
}

void wandering_ai::move_ai
(
    entt::registry&     registry,
    tilemap&            tmap,
    entt::entity        en,
    random_state&       rng
)
{
    tilemap_position& my_pos = registry.get<tilemap_position>(en);
    render_descriptor& my_desc = registry.get<render_descriptor>(en);
    damageable& my_health = registry.get<damageable>(en);
    battle_map::battle_unit_info& my_info = registry.get<battle_map::battle_unit_info>(en);

    if (my_health.cur_hp <= 0) 
    {
        //printf("I am dead! \n");
        sprite_handle& my_sprite = registry.get<sprite_handle>(en);
        my_sprite = get_sprite_handle_of(rng, tiles::CACTUS);
        return;
    }

    std::optional<entt::entity> nearest = closest_alive_entity(registry, en);

    if (!nearest.has_value())
        return;

    vec2i nearest_entity_pos = registry.get<tilemap_position>(nearest.value()).pos;
    destination_xy = nearest_entity_pos;

    int dist = abs(my_pos.pos.x() - destination_xy.x()) + abs(my_pos.pos.y() - destination_xy.y());
    printf("distance from other entity: %i ", dist);

    if (dist < 2)
    {
        damageable& your_health = registry.get<damageable>(nearest.value());
        float my_damage = my_info.damage;

        your_health.damage_amount(my_damage);

        printf("gonna attack u bro for %f! \n", my_damage);
        printf("your new health is %f!", your_health.cur_hp);
    }

    std::optional<std::vector<vec2i>> path = a_star(registry, tmap, my_pos.pos, destination_xy);

    if (path.has_value())
    {
        std::vector<vec2i> points = path.value();

        //next step
        points.erase(points.begin());
        vec2i next_p = points.front();
        next_p = clamp(next_p, vec2i{ 0, 0 }, tmap.dim - 1);

        //update renderer
        my_desc.pos = camera::tile_to_world(vec2f{ next_p.x(), next_p.y() });
        //update map
        tmap.move(en, my_pos.pos, next_p);
        //update position
        my_pos.pos = next_p;
    }
}

void wandering_ai::tick_animation
(
    float delta_time,
    render_descriptor& desc
)
{
    time_left_before_animation_update -= delta_time;

    if (time_left_before_animation_update > 0.)
        return;

    time_left_before_animation_update = time_between_animation_updates;

    update_animation(desc);
}

void wandering_ai::update_animation
(   
    render_descriptor& desc 
)
{
    if (on_max_scale)
        desc.scale = min_scale;
    else
        desc.scale = max_scale;

    on_max_scale = !on_max_scale;
}


void wandering_ai::show_path_colours_on_tilemap(tilemap& tmap, entt::registry& registry, std::vector<vec2i> points, vec2i destination)
{
    for (int i = 0; i < points.size(); i++)
    {
        vec2i pos = points[i];

        std::vector<entt::entity> es =
            tmap.all_entities[pos.y() * tmap.dim.x() + pos.x()];

        //get the top entity, which is the tile at this debugging moment
        render_descriptor& desc = registry.get<render_descriptor>(es[0]);

        if (pos == destination)
            desc.colour = { 0, 0, 1, 1 };   //blue
        else
            desc.colour = { 1, 0, 0, 1 };   //red
    }
}

void wandering_ai::reset_tilemap_colours(tilemap& tmap, entt::registry& registry)
{
    //Change all the colours of all the tiles for the path!
    for (int y = 0; y < tmap.dim.y(); y++)
    {
        for (int x = 0; x < tmap.dim.x(); x++)
        {
            std::vector<entt::entity> es =
                tmap.all_entities[y * tmap.dim.x() + x];

            //get the top entity, which is the tile at this debugging moment
            render_descriptor& desc = registry.get<render_descriptor>(es[0]);

            //reset tile look
            desc.colour = { 1, 1, 1, 1 };
        }
    }
}


std::optional<entt::entity> closest_alive_entity(entt::registry& registry, entt::entity en)
{
    tilemap_position& my_pos = registry.get<tilemap_position>(en);

    //Gets the closest target by looping over all entities
    //this could probably be optimized
    std::optional<entt::entity> closest_entity = std::nullopt;
    int max_dist = std::numeric_limits<int>::max();

    auto view = registry.view<team, battle_tag, damageable, wandering_ai, tilemap_position>();

    for (auto ent : view)
    {
        if (ent == en)
            continue;

        damageable other_ai_health = view.get<damageable>(ent);

        if (other_ai_health.cur_hp <= 0)
            continue;

        tilemap_position other_ai_pos = view.get<tilemap_position>(ent);

        int distance_from_current_squared = abs(my_pos.pos.squared_length() - other_ai_pos.pos.squared_length());

        if (distance_from_current_squared < max_dist)
        {
            closest_entity = ent;
            max_dist = distance_from_current_squared;
        }
    }

    return closest_entity;
}

