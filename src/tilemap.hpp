#ifndef TILEMAP_HPP_INCLUDED
#define TILEMAP_HPP_INCLUDED

#include <vector>
#include <vec/vec.hpp>
#include <map>

namespace ai_info
{
    enum type
    {
        ACTIVE,
        NONE,
    };
}

struct entity
{
    ai_info::type ai_type = ai_info::NONE;
};

namespace tiles
{
    enum types
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

std::map<tiles::types, std::vector<vec2i>>& get_locations();

struct tilemap
{
    // x * y, back to front rendering
    std::vector<std::vector<entity>> all_entities;

    void create(vec2i dim);
};

#endif