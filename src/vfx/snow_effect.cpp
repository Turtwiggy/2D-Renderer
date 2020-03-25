#include "snow_effect.hpp"


void snow_effect::start()
{
    if (active) 
    {
        return;
    }

    active = true;
}

void snow_effect::update(float delta_time,
    render_window& win,
    random_state& rng,
    particle_system& particle_sys)
{
    if (!active) 
    {
        return;
    }

    time_left_before_new_particle -= delta_time;

    if (time_left_before_new_particle <= 0.f)
    {
        time_left_before_new_particle = time_between_particles;

        auto win_size = win.get_window_size();

        printf("spawning particle");

        //Spawn new particle!
        float max_x = rand_det_s(rng.rng, 0.0, 1.0) * win_size.x();
        float max_y = rand_det_s(rng.rng, 0.0, 1.0) * win_size.y();

        particle_sys.emit(
            rng,                //random
            { max_x, max_y },   //pos
            { -50.0f, -50.f },  //velocity
            { 140 / 255.f, 29 / 255.f, 7 / 255.f, 1.f },    //start colour
            { 1.f, 0.f, 0.f, 0.f }                          //end colour
        );
    }
}

void snow_effect::stop()
{
    if (!active)
        return;

    active = false;

    //todo cleanup particles
}
