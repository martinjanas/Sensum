#include "CUtlVector.h"
#include "../../core/core.h"
#include "../../core/contexts/EngineContext.h"

template <typename T>
void CUtlVector<T>::GrowVector(int num)
{
    auto& ctx = core::contexts().GetContext<EngineContext>();

    m_Size += num;
    if (m_pElements)
        m_pElements = (T*)ctx.MemAlloc()->ReAlloc(m_pElements, m_Size * sizeof(T));
    else
        m_pElements = (T*)ctx.MemAlloc()->Alloc(m_Size * sizeof(T));
}