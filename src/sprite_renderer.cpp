#include "sprite_renderer.hpp"
#include <toolkit/render_window.hpp>
#include <toolkit/vertex.hpp>
#include <SFML/Graphics.hpp>
#include <toolkit/fs_helpers.hpp>
#include "camera.hpp"
#include <math.h>

sprite_renderer::sprite_renderer()
{
    std::string spritesheet_name = "res/monochrome_transparent.png";
    std::string spritesheet_data = file::read(spritesheet_name, file::mode::BINARY);

    assert(spritesheet_data.size() > 0);

    sf::Image img;
    img.loadFromMemory(spritesheet_data.c_str(), spritesheet_data.size());

    texture_settings tex_sett;
    tex_sett.width = img.getSize().x;
    tex_sett.height = img.getSize().y;
    tex_sett.is_srgb = true;

    sprite_sheet.load_from_memory(tex_sett, img.getPixelsPtr());
}

void sprite_renderer::add(const sprite_handle& handle, const render_descriptor& descriptor)
{
    next_renderables.push_back({handle, descriptor});
}

void sprite_renderer::render(render_window& window, const camera& cam)
{
    std::vector<vertex> vertices;
    vertices.reserve(next_renderables.size() * 6);

    vec2i screen_dimensions = window.get_window_size();

    vec2f tl_visible = cam.screen_to_world(window, {0,0}) - vec2f{TILE_PIX, TILE_PIX};
    vec2f br_visible = cam.world_to_screen(window, {screen_dimensions.x(), screen_dimensions.y()}) + vec2f{TILE_PIX, TILE_PIX};

    vec2f uv_scale = {1.f/sprite_sheet.dim.x(), 1.f/sprite_sheet.dim.y()};

    for(auto [handle, desc] : next_renderables)
    {
        //if(desc.pos.x() < tl_visible.x() || desc.pos.y() < tl_visible.y() || desc.pos.x() > br_visible.x() || desc.pos.y() > br_visible.y())
        //    continue;

        vec2f real_pos = cam.world_to_screen(window, desc.pos);
        vec2f real_dim = {TILE_PIX, TILE_PIX}; // TODO: SCALE

        vertex tl, tr, br, bl;
        tl.position = real_pos - real_dim/2.f;
        tr.position = real_pos + vec2f{real_dim.x()/2.f, -real_dim.y()/2.f};
        br.position = real_pos + real_dim/2.f;
        bl.position = real_pos + vec2f{-real_dim.x()/2.f, real_dim.y()/2.f};

        tl.position = round(tl.position);
        tr.position = round(tr.position);
        br.position = round(br.position);
        bl.position = round(bl.position);

        vec2i texture_coordinate = handle.offset * (TILE_PIX + TILE_SEP);

        vec2f tltx = {texture_coordinate.x(), texture_coordinate.y()};
        vec2f trtx = {texture_coordinate.x() + TILE_PIX, texture_coordinate.y()};
        vec2f brtx = {texture_coordinate.x() + TILE_PIX, texture_coordinate.y() + TILE_PIX};
        vec2f bltx = {texture_coordinate.x(), texture_coordinate.y() + TILE_PIX};

        tltx = tltx * uv_scale;
        trtx = trtx * uv_scale;
        brtx = brtx * uv_scale;
        bltx = bltx * uv_scale;

        tl.uv = tltx;
        tr.uv = trtx;
        br.uv = brtx;
        bl.uv = bltx;

        float shade = 0.05;

        vec4f base_colour = handle.base_colour * desc.colour;

        vec4f tl_col = clamp(base_colour*(1 + shade), 0, 1);
        vec4f tr_col = clamp(base_colour, 0, 1);
        vec4f br_col = clamp(base_colour*(1 - shade), 0, 1);
        vec4f bl_col = clamp(base_colour, 0, 1);

        tl.colour = tl_col;
        tr.colour = tr_col;
        br.colour = br_col;
        bl.colour = bl_col;

        vertices.push_back(tl);
        vertices.push_back(bl);
        vertices.push_back(tr);

        vertices.push_back(tr);
        vertices.push_back(bl);
        vertices.push_back(br);
    }

    window.render(vertices, &sprite_sheet);

    next_renderables.clear();
}