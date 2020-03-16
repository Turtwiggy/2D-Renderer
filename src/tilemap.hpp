#ifndef TILEMAP_HPP_INCLUDED
#define TILEMAP_HPP_INCLUDED

#include <vector>
#include <vec/vec.hpp>
#include <map>
#include "sprite_renderer.hpp"
#include "random.hpp"

namespace ai_info
{
    enum type
    {
        ACTIVE,
        NONE,
    };
}

namespace tiles
{
    enum type
    {
        BASE,
        DIRT,
        GRASS,
        TREE_1,
        TREE_2,
        TREE_DENSE,
        TREE_ROUND,
        CACTUS,
        VINE,
        SHRUB,
        ROCKS,
        BRAMBLE,
        CIVILIAN,
        SOLDIER,
        GROUND_BUG,
        FLYING_BUG,
        ARMOURED_BUG,
        SCORPION,
        SMALL_PINCHY,
        LAND_ANIMAL,
        SEA_ANIMAL,
        CROCODILE,
        FACE_MALE,
        FACE_WOMAN,
        THIN_DOOR_CLOSED,
        THIN_DOOR_OPEN,
        DOOR_CLOSED,
        DOOR_OPEN,
        GRAVE,
        WOOD_FENCE_FULL,
        WOOD_FENCE_HALF,
        TILING_WALL,
        CULTIVATION,
    };
}

namespace level_info
{
    enum types
    {
        BARREN, ///dirt, some grass
        DESERT,
        GRASS
    };
}

std::map<tiles::type, std::vector<vec2i>>& get_locations();
sprite_handle get_sprite_handle_of(random_state& rng, tiles::type type);
vec4f get_colour_of(tiles::type type, level_info::types level_type);

struct tilemap
{
    vec2i dim;
    // x * y, back to front rendering
    std::vector<std::vector<entt::entity>> all_entities;

    void create(vec2i dim);
    void add(entt::entity en, vec2i pos);
    void render(entt::registry& reg, sprite_renderer& renderer);
};

#endif