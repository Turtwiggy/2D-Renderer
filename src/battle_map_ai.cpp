#include "battle_map_ai.hpp"

void wandering_ai::tick_ai(
    entt::registry& registry, 
    float delta_time, 
    render_descriptor& desc, 
    sprite_handle& handle, 
    random_state& rng,
    tilemap& tmap,  
    entt::entity en)
{
    time_left_before_move_tiles -= delta_time;

    if (time_left_before_move_tiles > 0.)
        return;

    time_left_before_move_tiles = time_between_move_tiles;

    move_ai(registry, desc, handle, rng, tmap, en);
}

void wandering_ai::tick_animation(
    entt::registry& registry,
    float delta_time,
    render_descriptor& desc,
    sprite_handle& handle,
    random_state& rng,
    tilemap& tmap,
    entt::entity en )
{
    time_left_before_animation_update -= delta_time;

    if (time_left_before_animation_update > 0.)
        return;

    time_left_before_animation_update = time_between_animation_updates;

    update_animation(registry, delta_time, desc, handle, rng, tmap, en);
}

void wandering_ai::update_animation(
    entt::registry& registry,
    float delta_time,
    render_descriptor& desc,
    sprite_handle& handle,
    random_state& rng,
    tilemap& tmap,
    entt::entity e )
{
    if (on_max_scale)
        desc.scale = min_scale;
    else
        desc.scale = max_scale;

    on_max_scale = !on_max_scale;
}

void wandering_ai::move_ai( 
    entt::registry& registry,
    render_descriptor& desc, 
    sprite_handle& handle, 
    random_state& rng,
    tilemap& tmap, 
    entt::entity en)
{
    //TODO only update ai destination to closest target
    auto view = registry.view<ai_destination_tag>();
    for (auto entity : view)
    {
        ai_destination_tag& dest_tag = view.get<ai_destination_tag>(entity);

        //just replaces the destination with any old destination tag object
        destination_xy = dest_tag.destination;
    }

    std::optional<std::vector<vec2i>> path = a_star(registry, tmap, current_xy, destination_xy);

    if (path.has_value())
    {
        std::vector<vec2i> points = path.value();

        //next step
        points.erase(points.begin());
        vec2i next_p = points.front();

        next_p = clamp(next_p, vec2i{ 0, 0 }, tmap.dim);

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
