#include "particle_system.hpp"

#include "tilemap.hpp"

particle_system::particle_system()
{
	particle_pool.resize(max_particles);
}

void particle_system::emit(const particle_props& props)
{
	particle& particle = particle_pool[pool_index];

	float random = rand_det_s(rnd.rng, 0.0, 1.0);

	//sprite
	particle.sprite = props.sprite;

	particle.active = true;
	particle.position = props.position;
	particle.rotation = random * 2.0f * 3.141;

	// Velocity
	particle.velocity = props.velocity;
	particle.velocity.x() += props.velocity_variation.x() * (random - 0.5f);
	particle.velocity.y() += props.velocity_variation.y() * (random - 0.5f);

	// Color
	particle.colour_begin = props.colour_begin;
	particle.colour_end = props.colour_end;

	// Size
	particle.size_begin = props.size_begin + props.size_variation * (random - 0.5f);
	particle.size_end = props.size_end;

	// Life
	particle.life_time = props.life_time;
	particle.life_remaining = props.life_time;

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

		if (particle.life_remaining <= 0.0f)
		{
			particle.active = false;
			continue;
		}

		particle.life_remaining -= delta_time;
		particle.position += particle.velocity * delta_time;
		particle.rotation += 0.01f * delta_time;
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

		float life = particle.life_remaining / particle.life_time;
		vec4f color = slerp(particle.colour_end, particle.colour_begin, life);
		color.z() = color.z() * life;

		float size = slerp(particle.size_begin, particle.size_end, life);

		render_descriptor desc;
		desc.colour = color;
		desc.pos = particle.position;
		
		renderer.add(particle.sprite, desc);
	}
}