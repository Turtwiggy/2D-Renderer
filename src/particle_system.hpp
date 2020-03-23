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
	vec1f size_begin;
	vec1f size_end;
	vec1f size_variation;
	float life_time = 1.f;
};

struct particle
{
	render_descriptor desc;
	sprite_handle sprite;

	//State
	vec2f position_pixels = { 400.f, 400.f };
	vec2f velocity_pixels_per_second = { -5.f, 0 };
	vec1f rotation_degrees = 0.0f;

	float time_active = 0.0f;
	bool active = false;
};

struct particle_system {

public:
	particle_system();

	void emit(const particle_settings& props);

	void update(float delta_time);

	void render(sprite_renderer& renderer);

private:

	std::vector<particle> particle_pool;
	uint32_t max_particles = 1000;
	uint32_t pool_index = max_particles-1;

	random_state rnd;
};