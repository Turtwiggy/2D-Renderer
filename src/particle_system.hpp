#pragma once

#include <entt/entt.hpp>

#include "vec/vec.hpp"
#include "sprite_renderer.hpp"
#include "random.hpp"

struct particle
{
    vec2f velocity;         //(VALIDATE) pixels per second?
    float time_total;       //seconds
    float time_left;        //seconds
    vec2f size_begin;
    vec2f size_end;
    vec4f colour_begin;
    vec4f colour_end;
};

struct particle_system {

public:

    void emit(random_state rng);

    void update(float delta_time);

    void render(sprite_renderer& renderer);

private:

    entt::registry registry;
    //std::vector<entt::entity> particles; //alternative to registry?

};