
#include <iostream>
#include <string>
#include <vector>

#include <imgui/imgui.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core.hpp"
#include "random.hpp"
#include <toolkit/render_window.hpp>
#include <toolkit/texture.hpp>
#include <toolkit/vertex.hpp>
#include <toolkit/fs_helpers.hpp>
#include "sprite_renderer.hpp"

#include <entt/entt.hpp>
#include "tilemap.hpp"
#include "battle_map.hpp"
#include "Editor/imgui_bezier.hpp"

#include "particle_system.hpp"
#include "vfx/snow_effect.hpp"

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
    snow_effect snow;

    entt::registry registry;

    tilemap dummy_battle = create_battle(registry, rng, { 100, 100 }, level_info::GRASS);

    //Bezier
    float dx = 0.5f;
    float start_point[2] = { 0.f, 0.f };
    float end_point[2] = { 1.f, 1.f };
    static float v[5] = { 0.390f, 0.575f, 0.565f, 1.000f };

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

        //UI
        ImGui::Begin("New window");

        ImGui::Button("I am a button");

        if (ImGui::Button("Start Snow"))
            snow.start();

        if (ImGui::Button("Stop Snow"))
            snow.stop();

        ImGui::SliderFloat("Bezier Sample Value", &dx, 0.0f, 1.0f, "ratio = %.3f");

        ImGui::SliderFloat2("Bezier Start Point", start_point, 0.f, 1.f, "ratio = %.3f");

        ImGui::SliderFloat2("Bezier End Point", end_point, 0.f, 1.f, "ratio = %.3f");

        ImGui::Bezier("easeOutSine", v, start_point, end_point, dx);       // draw

        float y = ImGui::BezierValue(dx, v, start_point, end_point); // x delta in [0..1] range
        std::cout << "Y: " << y <<
            " v0:" << v[0] <<
            " v1:" << v[1] <<
            " v2:" << v[2] <<
            " v3:" << v[3] << std::endl;

        ImGui::End();

        //Update systems

        dummy_battle.render(registry, sprite_render);

        snow.update(delta_time, win, rng, particle_sys);

        particle_sys.update(delta_time);
        particle_sys.render(sprite_render);

        sprite_render.render(win, cam);
        win.display();
    }

    return 0;
}
