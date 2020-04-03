#pragma once

#include <toolkit/render_window.hpp>
#include <vector>
#include <entt/entt.hpp>

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

    void spawn_snow_particle(
        render_window& win,
        random_state& rng,
        particle_system& sys);

    void update_snow_particles(
        float delta_time,
        particle_system& particle_sys);

    bool paused = false;

    float time_between_particles = 0.1f;
    float time_left_before_new_particle = time_between_particles;

    particle_type my_type = particle_type::SNOW;

};