#pragma once

#include <toolkit/render_window.hpp>

#include "random.hpp"
#include "particle_system.hpp"

struct snow_effect {

    void start();

    void update(float delta_time,
        render_window& win,
        random_state& rng,
        particle_system& particle_sys);

    void stop();

private:
    bool active = false;

    float time_between_particles = 0.3f;
    float time_left_before_new_particle = time_between_particles;
};