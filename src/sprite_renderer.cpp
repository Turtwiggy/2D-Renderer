#include "sprite_renderer.hpp"
#include <toolkit/render_window.hpp>
#include <toolkit/vertex.hpp>
#include <SFML/Graphics.hpp>
#include <toolkit/fs_helpers.hpp>
#include "camera.hpp"
#include <math.h>

sprite_renderer::sprite_renderer()
{
    std::string spritesheet_name = "res/colored_transparent.png";
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

    vec2f window_half_dim = (vec2f){screen_dimensions.x(), screen_dimensions.y()}/2.f;

    vec2f tl_visible = cam.pos - window_half_dim;
    vec2f br_visible = cam.pos + window_half_dim;

    int x_start = floor(tl_visible.x() / TILE_PIX) - 1;
    int y_start = floor(tl_visible.y() / TILE_PIX) - 1;

    int x_end = ceil(br_visible.x() / TILE_PIX) + 1;
    int y_end = ceil(br_visible.y() / TILE_PIX) + 1;

    /*x_start = clamp(x_start, 0, level_size.x());
    x_end = clamp(x_end, 0, level_size.x());
    y_start = clamp(y_start, 0, level_size.y());
    y_end = clamp(y_end, 0, level_size.y());*/

    //auto mouse_tile_opt = screen_to_tile(mpos, screen_dimensions);

    vec2f uv_scale = {1.f/sprite_sheet.dim.x(), 1.f/sprite_sheet.dim.y()};

    for(auto [handle, desc] : next_renderables)
    {
        
    }

    next_renderables.clear();
}