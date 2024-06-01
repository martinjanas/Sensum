#pragma once

#include <Windows.h>
#include <cstdint>
#include "sdk/helpers/importer.h"
#include <unordered_map>

#define PAGE_EXECUTABLE ( PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY )

class ShadowVMT 
{
public:
	ShadowVMT(void* object);
	ShadowVMT() {};
	//~ShadowVMT();

	bool Apply(int index, uintptr_t* hook_function, void** original_fn);
	void Remove(int index);

private:
	void* m_ptr_object;
	uintptr_t* m_ptr_object_vtable;
	size_t m_object_vtable_size;
	uintptr_t* m_ptr_object_fake_vtable;
	std::unordered_map<int, uintptr_t*> m_object_hooks;

	size_t GetVTableSize();
};
