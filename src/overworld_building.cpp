#include "overworld_building.hpp"

#include "entity_common.hpp"
#include "tilemap.hpp"
#include "sprite_renderer.hpp"
#include <imgui/imgui.h>

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

void do_building_ui(entt::registry& registry, entt::entity en)
{
    ImGui::Begin("Building UI", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("Building text");

    ImGui::End();
}
