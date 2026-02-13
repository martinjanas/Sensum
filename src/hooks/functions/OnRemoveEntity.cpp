#include <algorithm>
#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../sdk/helpers/entity_data.h"

CEntityInstance* __fastcall hooks::on_remove_entity::hooked(void* rcx, CEntityInstance* instance, CHandle handle)
{
    if (instance)
        entity_data::entity_instances.erase(std::remove_if(entity_data::entity_instances.begin(), entity_data::entity_instances.end(), [&](const auto& e) { return e.handle == handle; }), entity_data::entity_instances.end());

    return original_fn(rcx, instance, handle);
}


