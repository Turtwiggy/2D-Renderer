#pragma once

#include "vec/vec.hpp"
#include "sprite_renderer.hpp"
#include "random.hpp"

struct particle_settings
{
    sprite_handle sprite;

    vec2f velocity_variation;
    vec4f colour_begin;
    vec4f colour_end;
    vec2f size_begin;
    vec2f size_end;
    vec1f size_variation;
    float life_time = 1.f;
};

struct particle
{
    sprite_handle sprite;
    particle_settings props;

    //State
    vec2f position = { 400.f, 400.f };
    vec2f velocity = { -5.f, 0 };
    vec1f rotation = 0.0f;
    vec2f size = { 1.f, 1.f };

    float time_active = 0.0f;
    bool active = false;
};

struct particle_system {

public:
    particle_system();

    void emit(const particle_settings& props,
        vec2f position, vec2f velocity, vec2f size, vec1f rotation);

    void update(float delta_time);

    void render(sprite_renderer& renderer);

private:

    std::vector<particle> particle_pool;
    uint32_t max_particles = 1000;
    uint32_t pool_index = max_particles - 1;

    random_state rnd;
};