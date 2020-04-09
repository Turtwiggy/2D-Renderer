#include "overworld_building.hpp"

#include "entity_common.hpp"
#include "tilemap.hpp"
#include "sprite_renderer.hpp"
#include <imgui/imgui.h>

void building_tag::show_build_ui()
{
    ImGui::Begin("I am a building");

    ImGui::Text("Building text");

    ImGui::End();
}

bool building_feature::can_build_on(building_tag::categories cat)
{
    for(auto& i : buildable_on)
    {
        if(i == cat)
            return true;
    }

    return false;
}

template<typename T>
void def(T& in, const building_feature& f)
{
    in[(int)f.t] = f;
}

building_feature get_feature_info(building_feature::type type)
{
    std::array<building_feature, building_feature::COUNT> var;

    def(var, {building_feature::WALLS, 0, 2, {building_tag::VILLAGE, building_tag::CASTLE, building_tag::TOWN, building_tag::CHURCH}});
    def(var, {building_feature::BARRACKS, 0, 1, {building_tag::VILLAGE, building_tag::CASTLE, building_tag::TOWN}});
    def(var, {building_feature::STABLE, 0, 2, {building_tag::VILLAGE, building_tag::CASTLE, building_tag::TOWN}});
    def(var, {building_feature::MINE, 0, 3, {building_tag::VILLAGE, building_tag::TOWN}});

    return var[(int)type];
}

std::vector<building_feature> get_buildable_for(building_tag::categories cat)
{
    std::vector<building_feature> vec;

    for(int i=0; i < (int)building_feature::type::COUNT; i++)
    {
        building_feature next = get_feature_info((building_feature::type)i);

        if(!next.can_build_on(cat))
            continue;

        vec.push_back(next);
    }

    return vec;
}

entt::entity create_overworld_building(entt::registry& registry, const sprite_handle& handle, const world_transform& transform)
{
    entt::entity res = registry.create();

    render_descriptor desc;
    desc.pos = transform.position;
    desc.depress_on_hover = true;

    registry.assign<sprite_handle>(res, handle);
    registry.assign<world_transform>(res, transform);
    registry.assign<overworld_tag>(res, overworld_tag());
    registry.assign<render_descriptor>(res, desc);
    registry.assign<mouse_interactable>(res, mouse_interactable());
    registry.assign<building_tag>(res, building_tag());

    return res;
}

