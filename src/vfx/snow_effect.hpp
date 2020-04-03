#pragma once

#include <toolkit/render_window.hpp>
#include <vector>
#include <entt/entt.hpp>

#include "random.hpp"
#include "particle_system.hpp"
#include "tilemap.hpp"

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

    bool paused = true;

    float time_between_particles = 0.1f;
    float time_left_before_new_particle = time_between_particles;

    tiles::type particle_sprite = tiles::type::EFFECT_10; //change this it is fun
    particle_type my_type = particle_type::SNOW;

    vec2f velocity = { -50.f, -50.f };
    vec4f colour_begin{ 140 / 255.f, 29 / 255.f, 7 / 255.f, 0.8f };
    vec4f colour_end{ 1.f, 0.f, 0.f, 0.f };
    vec2f size_begin{ 1.f, 1.f };
    vec2f size_end{ 0.5f, 0.5f };


};