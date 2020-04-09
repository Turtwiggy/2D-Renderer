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
#include "vfx/particle_system.hpp"
#include "vfx/effects/snow_effect.hpp"

#include <GLFW/glfw3.h>

#include "overworld_generation.hpp"

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

void battle_starter(entt::registry& registry)
{
    ImGui::Begin("Potential Battles");

    {
        auto overworld_view = registry.view<tilemap, overworld_tag>();

        int idx = 0;

        for (auto ent : overworld_view)
        {
            tilemap& tmap = registry.get<tilemap>(ent);

            for(int y=0; y < tmap.dim.y(); y++)
            {
                for(int x=0; x < tmap.dim.x(); x++)
                {
                    std::vector<entt::entity> root;
                    std::vector<entt::entity> others;

                    for(auto en2 : tmap.all_entities[y * tmap.dim.x() + x])
                    {
                        if(!registry.has<unit_group>(en2))
                            continue;

                        root.push_back(en2);
                    }

                    if(root.size() == 0)
                        continue;

                    ///only check right, and down tiles
                    for(int dy = 0; dy <= 1; dy++)
                    {
                        for(int dx = 0; dx <= 1; dx++)
                        {
                            if(dx == 0 && dy == 0)
                                continue;

                            if(abs(dx) == abs(dy))
                                continue;

                            int ox = dx + x;
                            int oy = dy + y;

                            if(ox < 0 || oy < 0 || ox >= tmap.dim.x() || oy >= tmap.dim.y())
                                continue;

                            for(auto en2 : tmap.all_entities[oy * tmap.dim.x() + ox])
                            {
                                if(!registry.has<unit_group>(en2))
                                    continue;

                                others.push_back(en2);
                            }
                        }
                    }

                    for(int i=0; i < (int)root.size(); i++)
                    {
                        for(int j=0; j < (int)others.size(); j++)
                        {
                            if(ImGui::Button(("Txt? " + std::to_string(i) + " yep " + std::to_string(j)).c_str()))
                            {
                                start_battle(registry, {root[i], others[j]});
                            }
                        }
                    }
                }
            }

            /*if (ImGui::Button(std::to_string(idx).c_str()))
            {
                printf("Hi\n");
            }

            idx++;*/
        }
    }

    ImGui::End();
}

void battle_generator(entt::registry& registry, random_state& rng, std::array<int, 2>& xy)
{
    ImGui::Begin("Start a new battle");

    float xy_c[2] = { xy[0], xy[1] };
    ImGui::SliderFloat2("Battle Size", xy_c, 0, 500);
    xy[0] = xy_c[0];
    xy[1] = xy_c[1];

    if (ImGui::Button("Generate Battle"))
    {
        battle_map::create_battle(registry, rng, { xy[0], xy[1] }, level_info::GRASS);
    }

    ImGui::End();
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

    vfx::particle_system particle_sys;
    vfx::snow_effect snow;

    entt::registry registry;

    entt::entity overworld = create_overworld(registry, rng, {150, 150});
    entt::entity default_battle = battle_map::create_battle(registry, rng, { 30, 30 }, level_info::GRASS);
    entt::entity& focused_tilemap = overworld;

    //battle editor variables
    std::array<int, 2> battle_size = { 50, 50 };

    debug_overworld(registry, overworld, rng);

    #ifdef TEST_OVERWORLD
    focused_tilemap = overworld;

    cam.pos = {32 * TILE_PIX, 32 * TILE_PIX};
    #endif // TEST_OVERWORLD

    while (!win.should_close())
    {
        win.poll();

        vec2f mpos = {ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y};

        if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            auto win_pos = win.get_window_position();

            mpos = mpos - vec2f{win_pos.x(), win_pos.y()};
        }

        ////hack to hold the console
        //if (ImGui::IsKeyDown(GLFW_KEY_C)) {
        //    win.display();
        //    continue;
        //}

        ImGuiIO& io = ImGui::GetIO();
        float delta_time = io.DeltaTime;

        if(io.MouseWheel > 0)
            cam.zoom(1);

        if(io.MouseWheel < 0)
            cam.zoom(-1);

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

        if(ImGui::IsKeyDown(GLFW_KEY_LEFT_SHIFT))
        {
            dx_dt *= 3;
            dy_dt *= 3;
        }

        cam.translate({dx_dt, dy_dt});

        //UI
        battle_generator(registry, rng, battle_size);
        battle_map::debug_combat(registry, default_battle, rng);

        //Update systems
        if (auto val = scene_selector(registry); val.has_value())
        {
            focused_tilemap = val.value();

            //centre camera on the new scene
            tilemap& tmap = registry.get<tilemap>(focused_tilemap);
            cam.pos = vec2f{ tmap.dim.x()/2, tmap.dim.y()/2 } * TILE_PIX;
        }

        //ai
        battle_map::update_ai(registry, focused_tilemap, delta_time);

        //map
        battle_starter(registry);


        tilemap& focused = registry.get<tilemap>(focused_tilemap);
        focused.render(registry, win, cam, sprite_render, mpos);

        //vfx
        snow.editor();
        snow.update(delta_time, win, rng, particle_sys);

        particle_sys.render(sprite_render);

        //renderer
        sprite_render.render(win, cam);
        win.display();
    }

    return 0;
}
