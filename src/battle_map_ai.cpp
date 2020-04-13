#include "battle_map_ai.hpp"

void wandering_ai::update_ai(entt::registry& registry, float delta_time, render_descriptor& desc, tilemap& tmap,  entt::entity en)
{
    time_left_between_move_tiles -= delta_time;

    if (time_left_between_move_tiles > 0.)
        return;

    time_left_between_move_tiles = time_between_move_tiles;

    move_ai(registry, desc, tmap, en);
}

void wandering_ai::move_ai( entt::registry& registry, render_descriptor& desc, tilemap& tmap, entt::entity en)
{
    vec2i prev = current_xy;

    std::optional<std::vector<vec2i>> path = a_star(registry, tmap, current_xy, destination_xy);

    if (path.has_value())
    {
        //next step
        std::vector p = path.value();
        p.erase(p.begin());
        vec2i next_p = p.front();

        //update renderer
        desc.pos = convert_xy_to_world(next_p);
        //update map
        tmap.move(en, current_xy, next_p);
        //update position
        current_xy = next_p;
    }
}
