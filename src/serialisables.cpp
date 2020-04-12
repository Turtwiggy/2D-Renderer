#include "serialisables.hpp"
#include "tilemap.hpp"
#include "sprite_renderer.hpp"
#include <networking/serialisable.hpp>

//Now need to get the registry out
DEFINE_SERIALISE_FUNCTION(tilemap)
{
    SERIALISE_SETUP();

    DO_FSERIALISE(selected);
    DO_FSERIALISE(dim);
    DO_FSERIALISE(all_entities);
}

DEFINE_SERIALISE_FUNCTION(sprite_handle)
{
    SERIALISE_SETUP();

    DO_FSERIALISE(offset);
    DO_FSERIALISE(base_colour);
}

DEFINE_SERIALISE_FUNCTION(world_transform)
{
    SERIALISE_SETUP();

    DO_FSERIALISE(position);
}

DEFINE_SERIALISE_FUNCTION(render_descriptor)
{
    SERIALISE_SETUP();

    DO_FSERIALISE(pos);
    DO_FSERIALISE(colour);
    DO_FSERIALISE(angle);
    DO_FSERIALISE(scale);
    DO_FSERIALISE(depress_on_hover);
}

DEFINE_ENTT_TYPES(tilemap, sprite_handle, world_transform, render_descriptor);

DEFINE_ENTT_SERIALISE();
