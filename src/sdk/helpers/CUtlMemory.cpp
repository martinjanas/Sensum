#include "CUtlMemory.h"
#include "../../core/core.h"
#include "../../core/contexts/EngineContext.h"

template <class T, class I>
CUtlMemory<T, I>::CUtlMemory(int nGrowSize, int nInitAllocationCount) : m_pMemory(0), m_nAllocationCount(nInitAllocationCount), m_nGrowSize(nGrowSize) {
    ValidateGrowSize();
    assert(nGrowSize >= 0);
    if (m_nAllocationCount) {
        auto& ctx = core::contexts().GetContext<EngineContext>();
        m_pMemory = (T*)ctx.MemAlloc()->Alloc(m_nAllocationCount * sizeof(T));
    }
}

template <class T, class I>
void CUtlMemory<T, I>::Init(int nGrowSize /*= 0*/, int nInitSize /*= 0*/) {
    Purge();

    m_nGrowSize = nGrowSize;
    m_nAllocationCount = nInitSize;
    ValidateGrowSize();
    assert(nGrowSize >= 0);
    if (m_nAllocationCount) {
        auto& ctx = core::contexts().GetContext<EngineContext>();

        m_pMemory = (T*)ctx.MemAlloc()->Alloc(m_nAllocationCount * sizeof(T));
    }
}

template <class T, class I>
void CUtlMemory<T, I>::ConvertToGrowableMemory(int nGrowSize) {
    if (!IsExternallyAllocated())
        return;

    m_nGrowSize = nGrowSize;
    if (m_nAllocationCount) {
        int nNumBytes = m_nAllocationCount * sizeof(T);
        auto& ctx = core::contexts().GetContext<EngineContext>();
        T* pMemory = (T*)ctx.MemAlloc()->Alloc(nNumBytes);
        memcpy(pMemory, m_pMemory, nNumBytes);
        m_pMemory = pMemory;
    }
    else {
        m_pMemory = NULL;
    }
}

template <class T, class I>
void CUtlMemory<T, I>::Grow(int num) {
    assert(num > 0);

    if (IsExternallyAllocated()) {
        // Can't grow a buffer whose memory was externally allocated
        assert(0);
        return;
    }

    auto oldAllocationCount = m_nAllocationCount;
    // Make sure we have at least numallocated + num allocations.
    // Use the grow rules specified for this memory (in m_nGrowSize)
    int nAllocationRequested = m_nAllocationCount + num;

    int nNewAllocationCount = UtlMemory_CalcNewAllocationCount(m_nAllocationCount, m_nGrowSize, nAllocationRequested, sizeof(T));

    // if m_nAllocationRequested wraps index type I, recalculate
    if ((int)(I)nNewAllocationCount < nAllocationRequested) {
        if ((int)(I)nNewAllocationCount == 0 && (int)(I)(nNewAllocationCount - 1) >= nAllocationRequested) {
            --nNewAllocationCount; // deal w/ the common case of m_nAllocationCount == MAX_USHORT + 1
        }
        else {
            if ((int)(I)nAllocationRequested != nAllocationRequested) {
                // we've been asked to grow memory to a size s.t. the index type can't address the requested amount of memory
                assert(0);
                return;
            }
            while ((int)(I)nNewAllocationCount < nAllocationRequested) {
                nNewAllocationCount = (nNewAllocationCount + nAllocationRequested) / 2;
            }
        }
    }

    m_nAllocationCount = nNewAllocationCount;

    auto& ctx = core::contexts().GetContext<EngineContext>();
    if (m_pMemory) {
        auto ptr = ctx.MemAlloc()->Alloc(m_nAllocationCount * sizeof(T));

        memcpy(ptr, m_pMemory, oldAllocationCount * sizeof(T));
        m_pMemory = (T*)ptr;
    }
    else {
        m_pMemory = (T*)ctx.MemAlloc()->Alloc(m_nAllocationCount * sizeof(T));
    }
}

template <class T, class I>
inline void CUtlMemory<T, I>::EnsureCapacity(int num) {
    if (m_nAllocationCount >= num)
        return;

    if (IsExternallyAllocated()) {
        // Can't grow a buffer whose memory was externally allocated
        assert(0);
        return;
    }
    m_nAllocationCount = num;

    auto& ctx = core::contexts().GetContext<EngineContext>();

    if (m_pMemory) {
        m_pMemory = (T*)ctx.MemAlloc()->ReAlloc(m_pMemory, m_nAllocationCount * sizeof(T));
    }
    else {
        m_pMemory = (T*)ctx.MemAlloc()->Alloc(m_nAllocationCount * sizeof(T));
    }
}

template <class T, class I>
void CUtlMemory<T, I>::Purge() {
    if (!IsExternallyAllocated()) {
        if (m_pMemory) {
            auto& ctx = core::contexts().GetContext<EngineContext>();
            ctx.MemAlloc()->Free((void*)m_pMemory);
            m_pMemory = 0;
        }
        m_nAllocationCount = 0;
    }
}

template <class T, class I>
void CUtlMemory<T, I>::Purge(int numElements) {
    assert(numElements >= 0);

    if (numElements > m_nAllocationCount) {
        // Ensure this isn't a grow request in disguise.
        assert(numElements <= m_nAllocationCount);
        return;
    }

    // If we have zero elements, simply do a purge:
    if (numElements == 0) {
        Purge();
        return;
    }

    if (IsExternallyAllocated()) {
        // Can't shrink a buffer whose memory was externally allocated, fail silently like purge
        return;
    }

    // If the number of elements is the same as the allocation count, we are done.
    if (numElements == m_nAllocationCount) {
        return;
    }

    if (!m_pMemory) {
        // Allocation count is non zero, but memory is null.
        assert(m_pMemory);
        return;
    }
    m_nAllocationCount = numElements;
    auto& ctx = core::contexts().GetContext<EngineContext>();
    m_pMemory = (T*)ctx.MemAlloc()->ReAlloc(m_pMemory, m_nAllocationCount * sizeof(T));
}