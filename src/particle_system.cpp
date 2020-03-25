#include "particle_system.hpp"

#include "tilemap.hpp"

void particle_system::emit(random_state rnd)
{
    auto base = registry.create();
    float random = rand_det_s(rnd.rng, 0.0, 1.0);

    sprite_handle handle = get_sprite_handle_of(rnd, tiles::TREE_1);

    render_descriptor desc;
    desc.pos = { 400.f, 400.f };
    desc.angle = 0;
    desc.size = { 1.f, 1.f };
    desc.colour = { 1.f, 0.f, 0.f, 1.f };

    particle p;
    p.velocity = { -10.f, 0.f };
    p.time_left = 2.f;
    p.time_total = p.time_left;
    p.size_begin = { 1.f, 1.f };
    p.size_end = { 0.5f, 0.5f };
    p.colour_begin = { 140 / 255.f, 29 / 255.f, 7 / 255.f, 1.f };
    p.colour_end = { 1.f, 0.f, 0.f, 0.f };

    registry.assign<sprite_handle>(base, handle);
    registry.assign<render_descriptor>(base, desc);
    registry.assign<particle>(base, p);
}

void particle_system::update(float delta_time)
{
    auto view = registry.view<sprite_handle, render_descriptor, particle>();

    for (auto ent : view)
    {
        auto& handle = view.get<sprite_handle>(ent);
        auto& desc = view.get<render_descriptor>(ent);
        auto& p = view.get<particle>(ent);

        //Update life
        if (p.time_left <= 0.f)
        {
            registry.destroy(ent);  //inefficient?
            continue;
        }

        //update life
        p.time_left -= delta_time;
        //goes from 1 to 0
        float life = p.time_left / p.time_total;

        //Update position
        desc.pos += p.velocity * delta_time;

        //Update angle
        float seconds_to_complete_360_degrees = 1.f;
        float angle_this_frame = delta_time * M_PIf
            / seconds_to_complete_360_degrees;
        desc.angle += angle_this_frame;

        //Update colour
        vec4f lerped_colour = mix(
            p.colour_end,
            p.colour_begin, life);
        desc.colour = lerped_colour;

        //Update size
        vec2f lerped_size = mix(
            p.size_end,
            p.size_begin, life);
        desc.size = lerped_size;
    }
}

void particle_system::render(sprite_renderer& renderer)
{
    auto view = registry.view<sprite_handle, render_descriptor, particle>();

    for (auto ent : view)
    {
        auto& handle = view.get<sprite_handle>(ent);
        auto& desc = view.get<render_descriptor>(ent);

        renderer.add(handle, desc);
    }
}
