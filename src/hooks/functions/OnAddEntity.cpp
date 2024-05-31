#include <list>
#include <algorithm>
#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../sdk/helpers/entity_data.h"

CEntityInstance* __fastcall hooks::on_add_entity::hooked(void* rcx, CEntityInstance* instance, CHandle handle)
{
    if (instance && instance->IsController())
    {
        bool exists = std::any_of(entity_data::player_instances.begin(), entity_data::player_instances.end(), [&](const entity_data::instance_t& e) { return e.handle.GetEntryIndex() == handle.GetEntryIndex(); });

        if (!exists)
            entity_data::player_instances.emplace_back(instance, handle);
    }

    return original_fn(rcx, instance, handle);
}

