#include "snow_effect.hpp"

#include "tilemap.hpp"
#include "sprite_renderer.hpp"

void snow_effect::start()
{
    if (active) 
        return;

    active = true;
}

void snow_effect::update(float delta_time,
    render_window& win,
    random_state& rng,
    particle_system& particle_sys)
{
    if (!active) 
        return;

    time_left_before_new_particle -= delta_time;

    if (time_left_before_new_particle > 0.f)
        return;

    printf("spawning particle");
    time_left_before_new_particle = time_between_particles;

    auto win_size = win.get_window_size();

    //Spawn new particle!
    float max_x = rand_det_s(rng.rng, 0.0, 1.0) * win_size.x();
    float max_y = rand_det_s(rng.rng, 0.0, 1.0) * win_size.y();

    particle p;
    p.sprite = get_sprite_handle_of(rng, tiles::TREE_1);

    render_descriptor desc;
    desc.pos = { max_x, max_y };
    //todo update scale over time
    //todo update colour over time
    //{ 140 / 255.f, 29 / 255.f, 7 / 255.f, 0.8f },    //start colour
    //{ 1.f, 0.f, 0.f, 0.f }                          //end colour
    //todo update angle over time
    p.desc = desc;

    p.velocity = { -1.f, -1.f };

    p.time_total = 4.0f;
    p.time_left = p.time_total;

    particle_sys.emit(p);
    //particles.push_back(spawned_particle);

    //goes from 1 to 0
    float life = p.time_left / p.time_total;

//    //Update position
//    desc.pos += p.velocity * delta_time;

//    //Update angle
//    float seconds_to_complete_360_degrees = 1.f;
//    float angle_this_frame = delta_time * M_PIf
//        / seconds_to_complete_360_degrees;
//    desc.angle += angle_this_frame;

//    //Update colour
//    vec4f lerped_colour = mix(
//        p.colour_end,
//        p.colour_begin, life);
//    desc.colour = lerped_colour;

//    //Update size
//    vec2f lerped_size = mix(
//        p.size_end,
//        p.size_begin, life);
//    desc.scale = lerped_size;
//}
}

void snow_effect::stop()
{
    if (!active)
        return;

    active = false;

    //todo cleanup particles
}
