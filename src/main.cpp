
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
#include <SFML/Graphics.hpp>
#include "sprite_renderer.hpp"

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
    sett.is_srgb = true;
    sett.viewports = !no_viewports;

    render_window win(sett, "hello");

    camera cam;
    cam.pos = (vec2f){win.get_window_size().x()/2, win.get_window_size().y()/2};

    sprite_renderer sprite_render;

    while(!win.should_close())
    {
        win.poll();

        ImGui::Begin("New window");

        ImGui::Button("I am a button");

        ImGui::End();

        vec2i win_size = win.get_window_size();

        sprite_handle dummy;
        dummy.offset = {0, 1};

        render_descriptor desc;
        desc.pos = {15, 35};

        sprite_render.add(dummy, desc);

        sprite_render.render(win, cam);
        win.display();
    }

    return 0;
}
