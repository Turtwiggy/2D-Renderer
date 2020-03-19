#pragma once

#include "vec/vec.hpp"
#include "sprite_renderer.hpp"
#include "random.hpp"

struct particle_props
{
	sprite_handle sprite;
	vec2f position;
	vec2f velocity, velocity_variation;
	vec4f colour_begin, colour_end;
	vec1f size_begin, size_end, size_variation;
	float life_time = 1.f;
};

struct particle
{
	sprite_handle sprite;
	vec2f position;
	vec2f velocity;
	vec4f colour_begin, colour_end;
	vec1f rotation = 0.0f;
	vec1f size_begin, size_end;

	float life_time = 1.0f;
	float life_remaining = 0.0f;

	bool active = false;
};

struct particle_system {

public:
	particle_system();

	void emit(const particle_props& props);

	void update(float delta_time);

	void render(sprite_renderer& renderer);

private:

	std::vector<particle> particle_pool;
	uint32_t max_particles = 1000;
	uint32_t pool_index = max_particles-1;

	random_state rnd;
};