#include <list>
#include <algorithm>
#include "../hooks.h"
#include "../../sdk/helpers/entity_data.h"

CEntityInstance* __fastcall hooks::on_add_entity::hooked(void* rcx, CEntityInstance* instance, CHandle handle)
{
    if (instance)
    {
        if (!instance->IsController())
        {
            bool entity_exists = std::any_of(entity_data::entity_instances.begin(), entity_data::entity_instances.end(), [&](const entity_data::EntityInstance_t& e) { return e.handle.GetEntryIndex() == handle.GetEntryIndex(); });
            if (!entity_exists)
                entity_data::entity_instances.emplace_back(instance, handle);
        }
    }

    return original_fn(rcx, instance, handle);
}

