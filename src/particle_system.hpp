#pragma once

#include <entt/entt.hpp>

#include "vec/vec.hpp"
#include "sprite_renderer.hpp"
#include "random.hpp"

struct particle
{
    render_descriptor desc;
    sprite_handle sprite;

    vec2f velocity = { 0.f, 0.f };  //world-units per second
    float time_total = 1.f;         //seconds
    float time_left = 1.f;          //seconds
};

struct particle_system 
{
public:
    entt::entity emit(particle& p);

    void update(float delta_time);
    void render(sprite_renderer& renderer);

private:
    entt::registry registry;
};