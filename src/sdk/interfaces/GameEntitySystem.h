   #pragma once
#include "../classes/CEntityInstance.h"
#include "../helpers/modules.h"
#include "../classes/CBaseEntity.h"

class CEntitySystem
{
public:
	CBaseEntity* GetBaseEntity(uint32_t index)
	{
		uint64_t entity;

		uint64_t temp_entity_ptr = *(uint64_t*)(this + 0x8 * (index >> 9) + 0x10);

		if (!temp_entity_ptr)
			return nullptr;

		int64_t temp_entity = 0x78 * (index & 0x1FF) + temp_entity_ptr;

		if (!temp_entity)
			return nullptr;

		uint32_t entity_index = (*(uint32_t*)(temp_entity + 0x10) & 0x7FFF);

		if (index <= 0x7FFE && (index >> 9) <= 0x3F && temp_entity_ptr && temp_entity && entity_index == index)
			entity = *(uint64_t*)temp_entity;
		else entity = 0;

		return reinterpret_cast<CBaseEntity*>(entity);
	}

	template <typename T = CBaseEntity*>
	T GetLocalPlayerController()
	{
		for (int i = 1; i < 65; ++i)
		{
			T player = reinterpret_cast<T>(GetBaseEntity(i));

			if (!player || !player->IsController())
				continue;

			if (player->m_bIsLocalPlayerController())
				return player;
		}
		return nullptr;
	}

	template <typename T = CBaseEntity*>
	T GetEntityFromHandle(CHandle handle)
	{
		auto base_entity = GetBaseEntity(handle.GetEntryIndex());

		return reinterpret_cast<T>(base_entity);
	}

	CEntityInstance* FindEntityByName(const char* name)
	{
		return VTable::GetThiscall<CEntityInstance*>(this, 12, name); //probably outdated index? +1 ?
	}

	CEntityInstance* OnAddEntity(CEntityInstance* entity_instance, void* handle)
	{
		return VTable::GetThiscall<CEntityInstance*>(this, 15, entity_instance, handle);
	}

	CEntityInstance* OnRemoveEntity(CEntityInstance* entity_instance, void* handle)
	{
		return VTable::GetThiscall<CEntityInstance*>(this, 16, entity_instance, handle);
	}

	int GetHighestEntityIndex()
	{
		return *reinterpret_cast<int*>(this + 0x1520);
	}
};