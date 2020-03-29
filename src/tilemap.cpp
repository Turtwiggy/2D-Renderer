#include "tilemap.hpp"
#include <vec/vec.hpp>

template<typename T>
void add_to(T& in, vec2i loc)
{
    in.push_back(loc);
}

std::map<tiles::type, std::vector<vec2i>>& get_locations()
{
    static std::map<tiles::type, std::vector<vec2i>> ret;

    using namespace tiles;

    ///consider adding skull and crossbones to dirt
    //add_to(ret[BASE], {0, 0});
    add_to(ret[BASE], {8, 5});

    add_to(ret[DIRT], {1, 0});
    add_to(ret[DIRT], {2, 0});
    add_to(ret[DIRT], {3, 0});
    add_to(ret[DIRT], {4, 0});

    add_to(ret[GRASS], {5, 0});
    add_to(ret[GRASS], {6, 0});
    add_to(ret[GRASS], {7, 0});

    add_to(ret[TREE_1], {0, 1});
    add_to(ret[TREE_1], {1, 1});
    add_to(ret[TREE_2], {2, 1});
    add_to(ret[TREE_2], {4, 1});
    add_to(ret[TREE_DENSE], {3, 1});
    add_to(ret[TREE_DENSE], {3, 2});
    add_to(ret[TREE_ROUND], {5, 1});
    add_to(ret[TREE_ROUND], {4, 2});

    add_to(ret[CACTUS], {6, 1});
    add_to(ret[CACTUS], {7, 1});

    add_to(ret[VINE], {2, 2});
    add_to(ret[SHRUB], {0, 2});

    add_to(ret[ROCKS], {5, 2});

    add_to(ret[BRAMBLE], {0, 2});
    add_to(ret[BRAMBLE], {6, 2});

    ///top left man is 24, 0

    add_to(ret[CIVILIAN], {25, 0});
    add_to(ret[CIVILIAN], {26, 0});
    add_to(ret[CIVILIAN], {26, 1});
    add_to(ret[CIVILIAN], {27, 1});
    add_to(ret[CIVILIAN], {29, 1});
    add_to(ret[CIVILIAN], {30, 1});
    add_to(ret[CIVILIAN], {30, 3});
    add_to(ret[CIVILIAN], {31, 3});
    add_to(ret[CIVILIAN], {30, 4});
    add_to(ret[CIVILIAN], {31, 4});
    ///top left scorpion is 25, 5

    add_to(ret[SOLDIER], {26, 4});
    add_to(ret[SOLDIER], {30, 9});

    add_to(ret[GROUND_BUG], {28, 5});
    add_to(ret[GROUND_BUG], {29, 5});
    add_to(ret[GROUND_BUG], {30, 5});
    add_to(ret[SMALL_PINCHY], {31, 5});
    add_to(ret[FLYING_BUG], {26, 5});
    add_to(ret[ARMOURED_BUG], {27, 5});
    add_to(ret[SCORPION], {24, 5});

    add_to(ret[LAND_ANIMAL], {25, 7});
    add_to(ret[LAND_ANIMAL], {26, 7});
    add_to(ret[LAND_ANIMAL], {27, 7});
    add_to(ret[LAND_ANIMAL], {28, 7});
    //add_to(ret[LAND_ANIMAL], {29, 7});
    add_to(ret[LAND_ANIMAL], {30, 7});
    add_to(ret[LAND_ANIMAL], {31, 7});
    add_to(ret[LAND_ANIMAL], {26, 8}); //bat

    add_to(ret[SEA_ANIMAL], {25, 8});
    add_to(ret[SEA_ANIMAL], {28, 8});
    add_to(ret[CROCODILE], {29, 8});

    add_to(ret[FACE_MALE], {24, 10});
    add_to(ret[FACE_MALE], {26, 10});
    add_to(ret[FACE_MALE], {27, 10});
    add_to(ret[FACE_MALE], {28, 10});
    add_to(ret[FACE_MALE], {29, 10});

    add_to(ret[FACE_WOMAN], {25, 10});
    add_to(ret[FACE_WOMAN], {30, 10});
    add_to(ret[FACE_WOMAN], {31, 10});

    add_to(ret[THIN_DOOR_CLOSED], {3, 4});
    add_to(ret[THIN_DOOR_OPEN], {4, 4});

    add_to(ret[DOOR_CLOSED], {3, 3});
    add_to(ret[THIN_DOOR_OPEN], {4, 3});

    add_to(ret[GRAVE], {0, 14});
    add_to(ret[GRAVE], {1, 14});
    add_to(ret[GRAVE], {2, 14});

    add_to(ret[WOOD_FENCE_FULL], {1, 3});
    add_to(ret[WOOD_FENCE_FULL], {2, 3});
    add_to(ret[WOOD_FENCE_HALF], {0, 3});

    add_to(ret[TILING_WALL], {16, 19});
    add_to(ret[TILING_WALL], {17, 19});
    add_to(ret[TILING_WALL], {18, 19});

    add_to(ret[TILING_WALL], {16, 20});
    add_to(ret[TILING_WALL], {17, 20});
    add_to(ret[TILING_WALL], {18, 20});

    add_to(ret[TILING_WALL], {16, 21});
    add_to(ret[TILING_WALL], {17, 21});
    add_to(ret[TILING_WALL], {18, 21});

    add_to(ret[CULTIVATION], {13, 6});
    add_to(ret[CULTIVATION], {14, 6});
    add_to(ret[CULTIVATION], {15, 6});
    add_to(ret[CULTIVATION], {16, 6});
    add_to(ret[CULTIVATION], {17, 6});

    return ret;
}

sprite_handle get_sprite_handle_of(random_state& rng, tiles::type type)
{
    auto& tiles = get_locations();

    auto& which = tiles[type];

    if(which.size() == 0)
        throw std::runtime_error("No tiles for type " + std::to_string(type));

    int len = which.size();

    int iwhich = (int)rand_det_s(rng.rng, 0, len);

    if(iwhich >= len || iwhich < 0)
        throw std::runtime_error("Rng is bad");

    sprite_handle handle;
    handle.offset = which[iwhich];
    handle.base_colour = get_colour_of(type, level_info::GRASS); //???

    return handle;
}

vec4f get_colour_of(tiles::type tile_type, level_info::types level_type)
{
    vec4f barren_col = srgb_to_lin_approx(vec4f{122, 68, 74, 255} / 255.f);
    vec4f grass_col = srgb_to_lin_approx(vec4f{56, 217, 115, 255} / 255.f);

    if(tile_type == tiles::BRAMBLE || tile_type == tiles::SHRUB || tile_type == tiles::BASE)
    {
        if(level_type == level_info::GRASS)
            return grass_col;
        else
            return barren_col;
    }

    if(tile_type == tiles::DIRT)
        return barren_col;

    if(tile_type == tiles::GRASS)
        return grass_col;

    if(tile_type == tiles::TREE_1 || tile_type == tiles::TREE_2 || tile_type == tiles::TREE_DENSE ||
       tile_type == tiles::TREE_ROUND || tile_type == tiles::CACTUS || tile_type == tiles::VINE ||
       tile_type == tiles::CULTIVATION || tile_type == tiles::CROCODILE)
       return grass_col;

    vec4f wood_col = srgb_to_lin_approx(vec4f{191, 121, 88, 255} / 255.f);
    vec4f building_gray = srgb_to_lin_approx(vec4f{207, 198, 184, 255} / 255.f);
    vec4f generic_red = srgb_to_lin_approx(vec4f{230, 72, 46, 255} / 255.f);

    if(tile_type == tiles::ROCKS || tile_type == tiles::GRAVE || tile_type == tiles::TILING_WALL)
        return building_gray;

    if(tile_type == tiles::LAND_ANIMAL)
        return building_gray;

    if(tile_type == tiles::SEA_ANIMAL)
        return building_gray;

    if(tile_type == tiles::CIVILIAN || tile_type == tiles::SOLDIER ||
       tile_type == tiles::GROUND_BUG || tile_type == tiles::FLYING_BUG || tile_type == tiles::ARMOURED_BUG || tile_type == tiles::SMALL_PINCHY)
        return building_gray;

    if(tile_type == tiles::SCORPION)
        return generic_red;

    if(tile_type == tiles::FACE_MALE || tile_type == tiles::FACE_WOMAN)
        return building_gray;

    if(tile_type == tiles::WOOD_FENCE_FULL || tile_type == tiles::WOOD_FENCE_HALF || tile_type == tiles::TILING_WALL ||
       tile_type == tiles::THIN_DOOR_CLOSED || tile_type == tiles::THIN_DOOR_OPEN ||
       tile_type == tiles::DOOR_CLOSED || tile_type == tiles::DOOR_OPEN)
        return wood_col;

    throw std::runtime_error("Did not find " + std::to_string(tile_type));
}

void tilemap::create(vec2i _dim)
{
    dim = _dim;
    all_entities.resize(dim.x() * dim.y());
}

void tilemap::add(entt::entity en, vec2i pos)
{
    if(pos.x() < 0 || pos.y() < 0 || pos.x() >= dim.x() || pos.y() >= dim.y())
        throw std::runtime_error("Add out of bounds");

    all_entities[pos.y() * dim.x() + pos.x()].push_back(en);
}

void tilemap::render(entt::registry& registry, sprite_renderer& renderer)
{
    for(auto lst : all_entities)
    {
        for(auto en : lst)
        {
            sprite_handle& handle = registry.get<sprite_handle>(en);
            render_descriptor& desc = registry.get<render_descriptor>(en);

            renderer.add(handle, desc);
        }
    }
}
