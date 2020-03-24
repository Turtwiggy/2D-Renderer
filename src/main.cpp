
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

int main(int argc, char* argv[])
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
    cam.pos = vec2f{ win.get_window_size().x() / 2, win.get_window_size().y() / 2 };

    sprite_renderer sprite_render;

    random_state rng;

    /*sprite_handle dummy;
    dummy = get_sprite_handle_of(rng, tiles::TREE_1);
    render_descriptor desc;
    desc.pos = {15, 35};
    desc.colour = get_colour_of(tiles::TREE_1, level_info::GRASS);*/

    particle_system particle_sys;

    entt::registry registry;

    tilemap dummy_battle = create_battle(registry, rng, { 100, 100 }, level_info::GRASS);


    while (!win.should_close())
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

        //Emit particle
        ImGui::Begin("Particle Window");

        if (ImGui::Button("Emit particle"))
        {
            particle_sys.emit(rng);
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

        dummy_battle.render(registry, sprite_render);

        particle_sys.update(delta_time);
        particle_sys.render(sprite_render);

        sprite_render.render(win, cam);
        win.display();
    }

    return 0;
}
