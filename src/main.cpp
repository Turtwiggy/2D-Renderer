#include <iostream>
#include <string>
#include <vector>
#include <optional>

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
#include "entity_common.hpp"
#include "tilemap.hpp"
#include "battle_map.hpp"
#include "overworld_map.hpp"

#include "Editor/imgui_bezier.hpp"
#include "particle_system.hpp"
#include "vfx/snow_effect.hpp"

#include <GLFW/glfw3.h>

std::optional<entt::entity> scene_selector(entt::registry& registry)
{
    std::optional<entt::entity> ret;

    ImGui::Begin("Scenes");

    int idx = 0;

    {
        auto overworld_view = registry.view<tilemap, overworld_tag>();

        ImGui::Text("Overworld(s):");

        for (auto ent : overworld_view)
        {
            if (ImGui::Button(std::to_string(idx).c_str()))
            {
                ret = ent;
            }

            idx++;
        }
    }

    {
        auto battle_view = registry.view<tilemap, battle_tag>();

        ImGui::Text("Battle(s)");

        for (auto ent : battle_view)
        {
            if (ImGui::Button(std::to_string(idx).c_str()))
            {
                ret = ent;
            }

            idx++;
        }
    }

    ImGui::End();

    return ret;
}

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
    sett.width = 1400;
    sett.height = 1000;
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

    //Bezier
    static float sample_x = 0.5f;
    static float start_point[2] = { 0.f, 0.f };
    static float middle_points[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
    static float end_point[2] = { 1.f, 0.f };

    entt::entity overworld = create_overworld(registry, rng, {64, 64});
    entt::entity focused_tilemap = create_battle(registry, rng, { 100, 100 }, level_info::GRASS);

    #ifdef TEST_OVERWORLD
    focused_tilemap = overworld;

    cam.pos = {32 * TILE_PIX, 32 * TILE_PIX};
    #endif // TEST_OVERWORLD

    while (!win.should_close())
    {
        win.poll();

        //hack to hold the console
        if (ImGui::IsKeyDown(GLFW_KEY_C)) {
            win.display();
            continue;
        }

        ImGuiIO& io = ImGui::GetIO();
        float delta_time = io.DeltaTime;

        //Input
        if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_C)))
        {
            auto mpos = vec2f{ io.MousePos.x, io.MousePos.y };
            std::cout << "mpos: " << mpos << std::endl;
        }

        //Delta Time
        if (ImGui::IsKeyDown(GLFW_KEY_N))
            std::cout << delta_time << std::endl;

        //Camera
        float dx = ImGui::IsKeyDown(GLFW_KEY_D) - ImGui::IsKeyDown(GLFW_KEY_A);
        float dy = ImGui::IsKeyDown(GLFW_KEY_S) - ImGui::IsKeyDown(GLFW_KEY_W);

        float dx_dt = dx * delta_time * 400;
        float dy_dt = dy * delta_time * 400;

        cam.pos.x() += dx_dt;
        cam.pos.y() += dy_dt;

        //UI
        ImGui::Begin("New window");

        ImGui::Button("I am a button");

        if (ImGui::Button("Start Snow"))
            snow.start();

        if (ImGui::Button("Stop Snow"))
            snow.stop();

        //Draw Bezier Curve
        ImGui::SliderFloat("Bezier Sample Value", &sample_x, 0.0f, 1.0f, "ratio = %.3f");
        ImGui::SliderFloat2("Bezier Start Point", start_point, 0.f, 1.f, "ratio = %.3f");
        ImGui::SliderFloat2("Bezier End Point", end_point, 0.f, 1.f, "ratio = %.3f");
        ImGui::Bezier("Bezier Control", sample_x, middle_points, start_point, end_point);    

        ImGui::End();

        //Update systems

        if (auto val = scene_selector(registry); val.has_value())
        {
            focused_tilemap = val.value();
        }

        //map
        tilemap& focused = registry.get<tilemap>(focused_tilemap);
        //focused.render(registry, sprite_render);

        //vfx
        snow.update(delta_time, win, rng, particle_sys);
        particle_sys.update(delta_time);
        particle_sys.render(sprite_render);

        //renderer
        sprite_render.render(win, cam);
        win.display();
    }

    return 0;
}
