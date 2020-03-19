
#include <iostream>
#include <string>
#include <vector>

#include <imgui/imgui.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Engine/Core.hpp"
#include <toolkit/render_window.hpp>
#include <toolkit/texture.hpp>
#include <toolkit/vertex.hpp>
#include <toolkit/fs_helpers.hpp>
#include "sprite_renderer.hpp"
#include <entt/entt.hpp>
#include "random.hpp"
#include "tilemap.hpp"
#include "battle_map.hpp"
#include "particle_system.hpp"

int main(int argc, char *argv[])
{
    bool no_viewports = false;

    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            std::string sarg = argv[i];

            if (sarg == "-noviewports" || sarg == "-noviewport")
            {
                no_viewports = true; 

                printf("Viewports are disabled\n");
            }
        }
    }

    render_settings sett;
    sett.width = 800;
    sett.height = 600;
    sett.is_srgb = false;
    sett.viewports = !no_viewports;

    render_window win(sett, "Dwarf and Blade");

    camera cam;
    cam.pos = vec2f{win.get_window_size().x()/2, win.get_window_size().y()/2};

    sprite_renderer sprite_render;

    particle_system particles;

    random_state rng;

    // Smoke
    particle_props smoke_particle;
    smoke_particle.sprite = get_sprite_handle_of(rng, tiles::DIRT);
    smoke_particle.position = { 400.0f, 400.0f };
    smoke_particle.velocity = { -2.0f, 0.0f }; smoke_particle.velocity_variation = { 4.0f, 2.0f };
    smoke_particle.size_begin = 0.35f, smoke_particle.size_end = 0.0f, smoke_particle.size_variation = 0.15f;
    smoke_particle.colour_begin = { 0.8f, 0.8f, 0.8f, 1.0f };
    smoke_particle.colour_end = { 0.6f, 0.6f, 0.6f, 1.0f };
    smoke_particle.life_time = 4.0f;

    // Flames
    particle_props engine_particle;
    engine_particle.sprite = get_sprite_handle_of(rng, tiles::DIRT);
    engine_particle.position = { 400.0f, 400.0f };
    engine_particle.velocity = { -10.0f, 0.0f }, engine_particle.velocity_variation = { 3.0f, 1.0f };
    engine_particle.size_begin = 0.5f, engine_particle.size_end = 0.0f, engine_particle.size_variation = 0.3f;
    engine_particle.colour_begin = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
    engine_particle.colour_end = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f , 1.0f };
    engine_particle.life_time = 1.0f;

    /*sprite_handle dummy;
    dummy = get_sprite_handle_of(rng, tiles::TREE_1);

    render_descriptor desc;
    desc.pos = {15, 35};
    desc.colour = get_colour_of(tiles::TREE_1, level_info::GRASS);*/

    entt::registry registry;

    tilemap dummy_battle = create_battle(registry, rng, {100, 100}, level_info::GRASS);


    while(!win.should_close())
    {
        win.poll();

        ImGuiIO io = ImGui::GetIO();
        float delta_time = io.DeltaTime;

        //Input
        if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_C)))
        {
            auto mpos = vec2f{ io.MousePos.x, io.MousePos.y };
            std::cout << "mpos: " << mpos << std::endl;
        }

        //Delta Time
        if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_A)))
            std::cout << delta_time << std::endl;

        //Emit flames at centre
        ImGui::Begin("Particle Window");
        if (ImGui::Button("Emit particle"))
        {
            std::cout << "emitting particle" << std::endl;
            particles.emit(engine_particle);
        }
        ImGui::End();

        ImGui::Begin("New window");

        ImGui::Button("I am a button");

        ImGui::End();

        vec2i win_size = win.get_window_size();
        
        /* Dummy Unit */
        //base_colour = clamp(rand_det_s(rng.rng, 0.7, 1.3) * dirt.base_colour * 0.5, 0, 1);
        //render_descriptor desc;
        //desc.pos = vec2f{ 400.f, 400.f };
        //entt::entity base = registry.create();
        //registry.assign<sprite_handle>(base, handle);
        //registry.assign<render_descriptor>(base, desc);
        //sprite_render.add(dirt, desc);
        /* End dummy sprite */

        //dummy_battle.render(registry, sprite_render);

        particles.update(delta_time);
        particles.render(sprite_render);

        sprite_render.render(win, cam);
        win.display();
    }

    return 0;
}
