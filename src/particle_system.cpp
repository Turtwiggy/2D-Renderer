#include "particle_system.hpp"

#include <toolkit/fs_helpers.hpp>

//particle_props load_particle(std::string path)
//{
//	std::string particle_source = file::read(path, file::mode::TEXT);
//
//	return compile(particle_source);
//}
//
//particle_props compile(std::string source)
//{
//	return particle_props();
//}

particle_system::particle_system()
{
	particle_pool.resize(max_particles);
}

void particle_system::emit(const particle_settings& props)
{
	particle& particle = particle_pool[pool_index];

	float random = rand_det_s(rnd.rng, 0.0, 1.0);

	//sprite
	particle.sprite = props.sprite;

	particle.active = true;
	particle.position = props.position;
	particle.rotation = random * 2.0f * 3.141f;

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
		particle.position_pixels += particle.velocity_pixels_per_second * delta_time;
		particle.rotation_degrees += 0.01f * delta_time;
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

		float life = particle.time_active / particle.life_time;
		vec4f begin_colour =  particle.colour_begin;
		vec4f color = lerp(particle.colour_end, particle.colour_begin, life);

		float size = lerp(particle.size_begin, particle.size_end, life);

		//update render descriptor
		render_descriptor desc = particle.desc;
		desc.colour = color;
		desc.size = vec2f{ 1, 1 } * size;
		desc.rotation_degrees = 0;

		particle.desc = desc;
		
		renderer.add(particle.sprite, particle.desc);
	}
}
