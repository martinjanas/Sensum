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

            CUtlTSHash<CSchemaClassBinding*> classes = netvar_class->GetClassBindings();

            for (const auto& class_binding : classes.GetElements())
            {
                CSchemaClassInfo* class_info = netvar_class->FindDeclaredClass(class_binding->GetName());

                for (auto j = 0; j < class_info->m_nFieldSize; j++)
                {
                    SchemaClassFieldData_t* field = &class_info->m_pFields[j];//&class_info->GetFields()[j];

                    if (!field)
                        continue;

                    char name_hashed[256];

                    strcpy_s(name_hashed, class_binding->m_pszName);
                    strcat_s(name_hashed, "->");
                    strcat_s(name_hashed, field->m_pszName);

                    const auto hash = fnv::hash_runtime(name_hashed);

                    netvars_data[hash] = field->m_nSingleInheritanceOffset;

                    //if (!strstr(class_binding->m_name, "CEntityInstance"));
                        //g_Console->println("DEBUG: %s->%s: 0x%p", class_binding->m_pszName, field->m_pszName, (uintptr_t)field->m_nSingleInheritanceOffset);
                }
            }
        }
    }

    uintptr_t get_offset_by_hash_cached(const fnv::hash hash)
    {
        uintptr_t offset(0);

        if (!offset)
            offset = netvars_data[hash];

        return offset;
    }
}

