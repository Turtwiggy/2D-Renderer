#pragma once

#include <entt/entt.hpp>

#include "vec/vec.hpp"
#include "sprite_renderer.hpp"
#include "random.hpp"

struct particle
{
    vec2f velocity;         //world-units per second
    float time_total;       //seconds
    float time_left;        //seconds
    vec2f size_begin;
    vec2f size_end;
    vec4f colour_begin;
    vec4f colour_end;
    //float emission_rate;    //particles per second
};

struct particle_system {

public:
    //emits a single particle at pos
    void emit(random_state& rng, vec2f pos);

    void update(float delta_time);

    void render(sprite_renderer& renderer);

private:

    entt::registry registry;
    //std::vector<entt::entity> particles; //alternative to registry?

};