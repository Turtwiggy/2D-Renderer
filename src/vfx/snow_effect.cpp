#include "snow_effect.hpp"

#include "tilemap.hpp"
#include "sprite_renderer.hpp"

void snow_effect::start()
{
    paused = false;
}

void snow_effect::update(float delta_time,
    render_window& win,
    random_state& rng,
    particle_system& particle_sys)
{
    if (paused)
        return;

    update_snow_particles(delta_time, particle_sys);

    //See if we need to spawn a new particle!
    time_left_before_new_particle -= delta_time;

    if (time_left_before_new_particle > 0.f)
        return;

    printf("spawning particle");
    time_left_before_new_particle = time_between_particles;

    spawn_snow_particle(win, rng, particle_sys);
}

void snow_effect::stop()
{
    paused = true;
}

void snow_effect::spawn_snow_particle(
    render_window& win,
    random_state& rng,
    particle_system& particle_sys)
{
    auto win_size = win.get_window_size();

    //Spawn new particle!
    float max_x = rand_det_s(rng.rng, 0.0, 1.0) * win_size.x();
    float max_y = rand_det_s(rng.rng, 0.0, 1.0) * win_size.y();

    particle p;
    p.sprite = get_sprite_handle_of(rng, tiles::TREE_1);
    p.type = my_type;
    p.time_total = 4.0f;
    p.time_left = p.time_total;

    //set initial info
    render_descriptor desc;
    desc.pos = { max_x, max_y };
    p.desc = desc;

    particle_sys.emit(p);
}

void snow_effect::update_snow_particles(float delta_time, particle_system& particle_sys)
{
    entt::registry& reg = particle_sys.get_particle_registry();

    auto view = reg.view<particle>();

    for (auto ent : view)
    {
        auto& p = view.get<particle>(ent);

        if (p.type != my_type)
            return;

        //update all snow particles!

        //goes from 1 to 0
        float life = p.time_left / p.time_total;

        //data
        vec2f velocity = { -50.f, -50.f };
        vec4f colour_begin{ 140 / 255.f, 29 / 255.f, 7 / 255.f, 0.8f };
        vec4f colour_end{ 1.f, 0.f, 0.f, 0.f };
        vec2f size_begin{ 1.f, 1.f };
        vec2f size_end{ 0.5f, 0.5f };

        render_descriptor desc = p.desc;

        //Update position
        desc.pos += velocity * delta_time;

        //Update angle
        float seconds_to_complete_360_degrees = 1.f;
        float angle_this_frame = delta_time * M_PIf
            / seconds_to_complete_360_degrees;
        desc.angle += angle_this_frame;

        //Update colour
        vec4f lerped_colour = mix(colour_end, colour_begin, life);
        desc.colour = lerped_colour;

        //Update size
        vec2f lerped_size = mix(size_end, size_begin, life);
        desc.scale = lerped_size;

        p.desc = desc;
    }
}

