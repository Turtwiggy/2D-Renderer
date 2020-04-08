#ifndef OVERWORLD_BUILDING_HPP_INCLUDED
#define OVERWORLD_BUILDING_HPP_INCLUDED

#include <entt/entt.hpp>

struct sprite_handle;
struct world_transform;

struct building_tag
{

};

entt::entity create_overworld_building(entt::registry& registry, const sprite_handle& handle, const world_transform& transform);

void do_building_ui(entt::registry& registry, entt::entity en);

#endif // OVERWORLD_BUILDING_HPP_INCLUDED
