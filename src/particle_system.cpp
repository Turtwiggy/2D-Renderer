#include "particle_system.hpp"

entt::entity particle_system::emit(particle& p)
{
    auto base = registry.create();

    registry.assign<particle>(base, p);

    return base;
}

void particle_system::update(float delta_time)
{
    auto view = registry.view<particle>();

    for (auto ent : view)
    {
        auto& p = view.get<particle>(ent);

        p.desc.pos += p.velocity * delta_time;
        p.time_left -= delta_time;

        if (p.time_left <= 0.0f)
        {
            registry.destroy(ent);
        }
    }
}

void particle_system::render(sprite_renderer& renderer)
{
    auto view = registry.view<particle>();

    for (auto ent : view)
    {
        auto& p = view.get<particle>(ent);

        renderer.add(p.sprite, p.desc);
    }
}
