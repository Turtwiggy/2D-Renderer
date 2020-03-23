#include "particle_system.hpp"

#include <toolkit/fs_helpers.hpp>

//Todo move to vec lib
template<int N, typename T>
inline vec<N, T> lerp(const vec<N, T>& v1, const vec<N, T>& v2, float t)
{
    return v1 * (1 - t) + v2 * t;
}

particle_system::particle_system()
{
    particle_pool.resize(max_particles);
}

void particle_system::emit(
    const particle_settings& props,
    vec2f position, vec2f velocity, vec2f size, vec1f rotation)
{
    particle& particle = particle_pool[pool_index];

    float random = rand_det_s(rnd.rng, 0.0, 1.0);

    //sprite
    particle.sprite = props.sprite;

    particle.active = true;

    //Position, Rotation and Size
    particle.position = position;
    particle.rotation = rotation;
    particle.size = size;

    //velocity
    particle.velocity = velocity;
    particle.velocity.x() += props.velocity_variation.x() * (random - 0.5f);
    particle.velocity.y() += props.velocity_variation.y() * (random - 0.5f);

    particle.props = props;

    particle.time_active = props.life_time;

    pool_index = --pool_index % particle_pool.size();
}

void particle_system::update(float delta_time)
{
    for (auto& particle : particle_pool)
    {
        if (!particle.active)
        {
            continue;
        }

        if (particle.time_active <= 0.0f)
        {
            particle.active = false;
            continue;
        }

        particle.time_active -= delta_time;
        particle.position += particle.velocity * delta_time;

        float seconds_to_complete_360_degrees = 1.f;
        float angle_this_frame = delta_time * M_PIf
            / seconds_to_complete_360_degrees;

        particle.rotation += angle_this_frame;
    }
}

void particle_system::render(sprite_renderer& renderer)
{
    for (auto& particle : particle_pool)
    {
        if (!particle.active)
        {
            continue;
        }

        //goes from 1 to 0
        float life = particle.time_active / particle.props.life_time;
        vec4f begin_colour = particle.props.colour_begin;

        vec4f lerped_colour = lerp(
            particle.props.colour_end,
            particle.props.colour_begin, life);

        vec2f lerped_size = lerp(
            particle.props.size_end,
            particle.props.size_begin, life);

        //convert particle to render descriptor
        render_descriptor desc;
        desc.pos = particle.position;
        desc.angle = particle.rotation;
        desc.colour = lerped_colour;
        desc.size = lerped_size;

        renderer.add(particle.sprite, desc);
    }
}
