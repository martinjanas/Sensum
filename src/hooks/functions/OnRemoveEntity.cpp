#include <algorithm>
#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../sdk/helpers/entity_data.h"

CEntityInstance* __fastcall hooks::on_remove_entity::hooked(void* rcx, CEntityInstance* instance, CHandle handle)
{
    if (instance && instance->IsController())
    {
        auto it = std::remove_if(entity_data::player_instances.begin(), entity_data::player_instances.end(), [&](const entity_data::EntityInstance_t& e) { return e.handle.GetEntryIndex() == handle.GetEntryIndex(); });

        entity_data::player_instances.erase(it, entity_data::player_instances.end());
    }

    return original_fn(rcx, instance, handle);
}


