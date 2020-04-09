#ifndef OVERWORLD_BUILDING_HPP_INCLUDED
#define OVERWORLD_BUILDING_HPP_INCLUDED

#include <entt/entt.hpp>
#include <vector>

struct sprite_handle;
struct world_transform;

struct building_feature;

struct building_tag
{
    enum categories
    {
        VILLAGE,
        CASTLE,
        TOWN,
        CHURCH,
    };

    categories cat = categories::VILLAGE;
    std::vector<building_feature> built;

    void show_build_ui();

    //std::vector<building_feature> get_buildable();
};

//Not every building category will be able to build all features
struct building_feature
{
    enum type
    {
        WALLS,
        BARRACKS,
        STABLE,
        MINE,
        COUNT
    };

    type t = type::COUNT;
    int level = 0;
    int max_level = 0;
    std::vector<building_tag::categories> buildable_on;

    bool can_build_on(building_tag::categories cat);
};

building_feature get_feature_info(building_feature::type type);
std::vector<building_feature> get_buildable_for(building_tag::categories cat);

entt::entity create_overworld_building(entt::registry& registry, const sprite_handle& handle, const world_transform& transform);

#endif // OVERWORLD_BUILDING_HPP_INCLUDED
