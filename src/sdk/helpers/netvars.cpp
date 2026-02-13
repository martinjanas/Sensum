#include <vector>
#include "Netvars.h"
#include "../sdk.h"

namespace netvars
{
    void init()
    {
        std::vector<const char*> netvar_modules = { "client.dll", "engine2.dll", "animationsystem.dll"};

        for (const auto& module : netvar_modules)
        {
            CSchemaSystemTypeScope* netvar_class = g::schema_system->FindTypeScopeForModule(module);
            if (!netvar_class)
                continue;

            auto classes = netvar_class->GetClassBindings();
            for (const auto& class_binding : classes.GetElements())
            {
                SchemaClassInfoData_t* class_info = netvar_class->FindDeclaredClass(class_binding->m_pszName);

                for (auto j = 0; j < class_info->m_nFieldSize; j++)
                {
                    SchemaClassFieldData_t* field = &class_info->m_pFields[j];
                    if (!field)
                        continue;

                    std::string name_hashed = std::format(":s->:s", class_binding->m_pszName, field->m_pszName);
                    const auto hash = xxh::get_hash(name_hashed);
                    
                    netvars_data[hash] = field->m_nSingleInheritanceOffset;

                    //if (!strcmp(class_binding->m_pszName, "C_BaseGrenade"))
                    //g_Console->println("DEBUG: %s->%s: 0x%p", class_binding->m_pszName, field->m_pszName, field->m_nSingleInheritanceOffset);
                }
            }
        }
    }

    uintptr_t get_offset_by_hash_cached(const XXH64_hash_t hash)
    {
        uintptr_t offset = 0;
        if (!offset)
            offset = netvars_data[hash];

        return offset;
    }
}

