#include "ShadowVMT.h"
#include "../../../sdk/helpers/console.h"

ShadowVMT::ShadowVMT(void* interface_obj)
{
	m_pInterfaceObj = interface_obj;
	m_pOriginalVtable = *static_cast<uintptr_t**>(m_pInterfaceObj);

	m_nVtableSize = get_vtable_size();

	m_pFakeVtable = new uintptr_t[m_nVtableSize];
	for (size_t i = 0; i < m_nVtableSize; i++)
		m_pFakeVtable[i] = m_pOriginalVtable[i];

	*static_cast<uintptr_t**>(m_pInterfaceObj) = m_pFakeVtable;
}

bool ShadowVMT::apply(uint32_t index, uintptr_t* hook_func, void** original_fn)
{
	//Get the pointer to original fn
	*original_fn = reinterpret_cast<void*>(m_pOriginalVtable[index]);

	// Replace the function pointer in the fake VMT with the hook function
	m_pFakeVtable[index] = reinterpret_cast<uintptr_t>(hook_func);
	
	return true;
}

bool ShadowVMT::restore_vtable()
{
	*static_cast<uintptr_t**>(m_pInterfaceObj) = m_pOriginalVtable;

	delete[] m_pFakeVtable;
	m_pFakeVtable = nullptr;

	return true;
}

size_t ShadowVMT::get_vtable_size()
{
	size_t size = 0;
	MEMORY_BASIC_INFORMATION mbi{};
	const DWORD PAGE_EXECUTABLE = (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
	while (LI_FN(VirtualQuery).cached()(reinterpret_cast<const void*>(this->m_pOriginalVtable[size]), &mbi, sizeof(mbi)))
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
