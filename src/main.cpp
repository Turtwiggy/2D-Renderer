
#include <iostream>
#include <string>

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
#include <vector>

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

    texture spritesheet;
    
    std::string spritesheet_name = "res/colored_transparent.png";
    std::string spritesheet_data = file::read(spritesheet_name, file::mode::BINARY);

    assert(spritesheet_data.size() > 0);

    sf::Image img;
    img.loadFromMemory(spritesheet_data.c_str(), spritesheet_data.size());

    spritesheet.load_from_memory(img.getPixelsPtr(), {img.getSize().x, img.getSize().y});

    while(!win.should_close())
    {
        win.poll();

        ImGui::Begin("New window");

        ImGui::Button("I am a button");

        ImGui::End();

        vec2i win_size = win.get_window_size();

        vec2f rect[] = {{0, 0}, {win_size.x(), 0}, {0, win_size.y()}, {win_size.x(), win_size.y()}};
        vec2f uvs[] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};

        std::vector<vertex> vertices;
        vertices.resize(6);

        vertices[0].position = rect[0];
        vertices[0].uv = uvs[0];
        vertices[1].position = rect[2];
        vertices[1].uv = uvs[2];
        vertices[2].position = rect[1];        
        vertices[2].uv = uvs[1];
        
        vertices[3].position = rect[1];
        vertices[3].uv = uvs[1];
        vertices[4].position = rect[2];
        vertices[4].uv = uvs[2];
        vertices[5].position = rect[3];
        vertices[5].uv = uvs[3];

        for(auto& i : vertices)
        {
            i.colour = (vec4f){1,1,1,1};
        }

        win.render(vertices, &spritesheet);

        win.display();
    }

    return 0;
}
