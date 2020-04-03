#include "snow_effect.hpp"

#include "sprite_renderer.hpp"
#include <imgui/imgui.h>


namespace vfx {

    void snow_effect::snow_effect_editor()
    {
        ImGui::Begin("Snow effect editor");

        float list[2] = { velocity.x(), velocity.y() };
        ImGui::SliderFloat2("Velocity", list, -100., 100.);
        velocity.x() = list[0];
        velocity.y() = list[1];

        ImGui::End();
    }

    void snow_effect::spawn_particle_check(float delta_time,
        particle_system& particle_sys,
        render_window& win,
        random_state& rng)
    {
        time_left_before_new_particle -= delta_time;

        if (time_left_before_new_particle > 0.f)
            return;

        time_left_before_new_particle = time_between_particles;

        spawn_particle(win, rng, particle_sys);
    }

    void snow_effect::spawn_particle(
        render_window& win,
        random_state& rng,
        particle_system& particle_sys)
    {
        auto win_size = win.get_window_size();

        float max_x = rand_det_s(rng.rng, 0.0, 1.0) * win_size.x();
        float max_y = rand_det_s(rng.rng, 0.0, 1.0) * win_size.y();

        particle p;
        p.sprite = get_sprite_handle_of(rng, sprite);
        p.type = type;
        p.time_total = particle_time_total;
        p.time_left = particle_time_total;

        render_descriptor desc;
        desc.pos = { max_x, max_y };
        p.desc = desc;

        particle_sys.emit(p);
    }



    void snow_effect::update_all_particles(float delta_time, particle_system& particle_sys)
    {
        snow_effect_editor();

        entt::registry& reg = particle_sys.get_particle_registry();

        auto view = reg.view<particle>();

        for (auto ent : view)
        {
            auto& p = view.get<particle>(ent);

            if (p.type != type)
                return;

            //reduce time_left
            p.time_left -= delta_time;
            if (p.time_left < 0.0f)
            {
                reg.destroy(ent);
                return;
            }

            //goes from 1 to 0
            float life = p.time_left / p.time_total;

            render_descriptor desc = p.desc;

            //Update position
            desc.pos += velocity * delta_time;
               
            //Update angle
            desc.angle += angle_every_frame * delta_time;

            //Update colour (todo)
            //vec4f endCol = srgb_to_lin_approx(vec4f{ 0xff / 255.f, 0x00 / 255.f, 0x00 / 255.f, 1 });
            //vec4f begCol = srgb_to_lin_approx(vec4f{ 0xff / 255.f, 0xff / 255.f, 0xff / 255.f, 1 });
            //vec4f lerped_colour = mix(endCol, begCol, life);
            //p.sprite.base_colour *= lerped_colour;
            //p.desc.colour = lerped_colour;

            //Update size
            vec2f lerped_size = mix(size_end, size_begin, life);
            desc.scale = lerped_size;

            p.desc = desc;
        }
    }

}

