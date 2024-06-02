#include "ShadowVMT.h"

ShadowVMT::ShadowVMT(void* object)
	: m_ptr_object(object), m_ptr_object_vtable(*reinterpret_cast<uintptr_t**>(object)), m_object_vtable_size(0), m_ptr_object_fake_vtable(nullptr) {
	// Calculate the VMT Size
	m_object_vtable_size = GetVTableSize();

	// Create the Fake VMT
	m_ptr_object_fake_vtable = new uintptr_t[m_object_vtable_size];

	// Fill the Fake Virtual Method Table with the Function Pointers from Original
	for (size_t i = 0; i < m_object_vtable_size; ++i)
		m_ptr_object_fake_vtable[i] = m_ptr_object_vtable[i];

	// Swap VTable Pointer
	*reinterpret_cast<uintptr_t**>(m_ptr_object) = m_ptr_object_fake_vtable;
}

//ShadowVMT::~ShadowVMT() {
//	// Restore the original VMT pointer
//	*reinterpret_cast<uintptr_t**>(m_ptr_object) = m_ptr_object_vtable;
//	delete[] m_ptr_object_fake_vtable;
//}

size_t ShadowVMT::GetVTableSize()
{
	MEMORY_BASIC_INFORMATION mbi{};

	size_t size = 0;
	while (LI_FN(VirtualQuery).cached()(reinterpret_cast<const void*>(this->m_ptr_object_vtable[size]), &mbi, sizeof(mbi)))
	{
		if (mbi.State != MEM_COMMIT)
			break;

		if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
			break;

		if (!(mbi.Protect & PAGE_EXECUTABLE))
			break;

		++size;
	}

	return size;
}

bool ShadowVMT::Apply(int index, uintptr_t* hook_function, void** original_fn) {
	// Get the Pointer to Original Func
	*original_fn = reinterpret_cast<void*>(m_ptr_object_fake_vtable[index]);

	// Swap pointer to Original Function to Hook Function Pointer
	*reinterpret_cast<uintptr_t**>(&m_ptr_object_fake_vtable[index]) = hook_function;

	// Insert the hook function to the list
	m_object_hooks[index] = *(uintptr_t**)*original_fn;

	return true;
}

void ShadowVMT::Remove(int index) {
	// Swap the pointer from hook function to original function using the hook list
	*reinterpret_cast<uintptr_t**>(m_ptr_object_fake_vtable[index]) = m_object_hooks[index];

	// Removes the hook function from hook list
	m_object_hooks.erase(index);
}
