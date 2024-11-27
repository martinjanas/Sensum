#pragma once

#include <Windows.h>
#include <cstdint>
#include "../../helpers/importer.h"
#include <unordered_map>

class ShadowVMT
{
public:
	ShadowVMT(void* inteface_obj);
	ShadowVMT() = default;

	bool apply(uint32_t index, uintptr_t* hook_func, void** original_fn);
	bool restore_vtable();

private:
	void* m_pInterfaceObj;
	uintptr_t* m_pOriginalVtable;
	uintptr_t* m_pFakeVtable;
	size_t m_nVtableSize;

	size_t get_vtable_size();
};
