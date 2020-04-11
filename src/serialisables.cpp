#include "serialisables.hpp"
#include "tilemap.hpp"
#include <networking/serialisable.hpp>

DEFINE_SERIALISE_FUNCTION(tilemap)
{
    SERIALISE_SETUP();

    DO_FSERIALISE(selected);
    DO_FSERIALISE(dim);
    DO_FSERIALISE(all_entities);
}

DEFINE_ENTT_TYPES(tilemap);

DEFINE_ENTT_SERIALISE();
