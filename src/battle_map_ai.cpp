#include "battle_map_ai.hpp"

#include "entity_common.hpp"

void wandering_ai::tick_ai(
    entt::registry& registry, 
    float delta_time, 
    render_descriptor& desc, 
    tilemap& tmap,  
    entt::entity en,
    camera& cam,
    render_window& win )
{
    time_left_before_move_tiles -= delta_time;

    if (time_left_before_move_tiles > 0.)
        return;

    time_left_before_move_tiles = time_between_move_tiles;

    move_ai(registry, desc, tmap, en, cam, win);
}

void wandering_ai::move_ai
(
    entt::registry&     registry,
    render_descriptor&  desc,
    tilemap&            tmap,
    entt::entity        en,
    camera&             cam,
    render_window&      win
)
{
    //Gets the closest target by looping over all entities
    //this could probably be optimized
    vec2i closest_entity = { -1, -1 };
    int max_dist = std::numeric_limits<int>::max();

    auto view = registry.view<team, battle_tag, damageable, wandering_ai>();

    for (auto ent : view)
    {
        if (ent == en)
            continue;

        wandering_ai ai = view.get<wandering_ai>(ent);

        vec2i tile_xy = ai.current_xy;

        int distance_from_current_squared = abs(current_xy.squared_length() - tile_xy.squared_length());

        if (closest_entity == vec2i{-1, -1} || distance_from_current_squared < max_dist)
        {
            closest_entity = tile_xy;
            max_dist = distance_from_current_squared;
        }
    }

    if (closest_entity == vec2i{ -1, -1 })
        closest_entity = current_xy;

    destination_xy = closest_entity;

    std::optional<std::vector<vec2i>> path = a_star(registry, tmap, current_xy, destination_xy);

    if (path.has_value())
    {
        std::vector<vec2i> points = path.value();

        //next step
        points.erase(points.begin());
        vec2i next_p = points.front();

        next_p = clamp(next_p, vec2i{ 0, 0 }, tmap.dim - 1);

        // debugging colours
        //reset_tilemap_colours(tmap, registry);
        //show_path_colours_on_tilemap(tmap, registry, points, destination_xy);

        //update renderer
        desc.pos = convert_xy_to_world(next_p);
        //update map
        tmap.move(en, current_xy, next_p);
        //update position
        current_xy = next_p;
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

void wandering_ai::update_animation( render_descriptor& desc )
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