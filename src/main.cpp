
#include <iostream>
#include <string>

#include <imgui/imgui.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Engine/Core.hpp"
#include <toolkit/render_window.hpp>
 

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

    while(!win.should_close())
    {
        win.poll();

        ImGui::Begin("New window");

        ImGui::Button("I am a button");

        ImGui::End();

        win.display();
    }

    return 0;
}
