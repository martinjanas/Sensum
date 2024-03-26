#pragma once

#include <Windows.h>
#include <cstdint>

#define PAGE_EXECUTABLE ( PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY )

struct VTableInfo_t
{
	uintptr_t** original_vtable;
	uintptr_t* fake_vtable;
};

class ShadowVMT
{
public:
	ShadowVMT(void* class_ptr)
	{
		this->original_vtable = *reinterpret_cast<uintptr_t**>(class_ptr);


	}

	size_t GetVTableSize()
	{
		MEMORY_BASIC_INFORMATION mbi{};

		size_t i{};

		while (VirtualQuery(reinterpret_cast<LPCVOID>(this->original_vtable[i]), &mbi, sizeof(mbi)))
		{
			if (mbi.State != MEM_COMMIT)
				break;

			if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
				break;

			if (!(mbi.Protect & PAGE_EXECUTABLE))
				break;

			++i;
		}

		return i;
	}

private:
	uintptr_t* original_vtable;
	size_t vtable_size = 0;
};

