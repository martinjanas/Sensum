#pragma once
#include "../classes/CEntityInstance.h"
#include "../helpers/modules.h"
#include "../classes/CBaseEntity.h"

class CEntitySystem
{
public:
	CBaseEntity* GetBaseEntity(uint32_t index)
	{
		if (index < 0 || index > 0x7FFE)
			return nullptr;

		const int chunk_idx = index >> 9;
		if (chunk_idx > 0x3F)
			return nullptr;

		const uintptr_t chunk_base = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(this) + 0x10 + chunk_idx * 8);
		if (!chunk_base)
			return nullptr;

		const uintptr_t entry = chunk_base + (index & 0x1FF) * 0x70;

		const uint32_t entry_identity = *reinterpret_cast<uint32_t*>(entry + 0x10);
		if ((entry_identity & 0x7FFF) != index)
			return nullptr;

		return *reinterpret_cast<CBaseEntity**>(entry);
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
		return VTable::GetThiscall<CEntityInstance*>(this, 13, name);
	}

	CEntityInstance* OnAddEntity(CEntityInstance* entity_instance, void* handle)
	{
		return VTable::GetThiscall<CEntityInstance*>(this, 16, entity_instance, handle);
	}

	CEntityInstance* OnRemoveEntity(CEntityInstance* entity_instance, void* handle)
	{
		return VTable::GetThiscall<CEntityInstance*>(this, 17, entity_instance, handle);
	}

	int GetHighestEntityIndex()
	{
		return *reinterpret_cast<int*>(this + 0x20F0);
	}
};