#pragma once

#include <cassert>

#include "../../sdk/helpers/interfaces.h"

template <class T>
class CUtlVector {
public:
    CUtlVector() {
        m_Size = 0;
        m_pElements = nullptr;
    }

    T& operator[](int i) {
        return m_pElements[i];
    }

    T& operator[](int i) const
    {
        return m_pElements[i];
    }

    [[nodiscard]] int Count() const {
        return m_Size;
    }

    int m_Size;
    T* m_pElements;

    template <typename T>
    inline T & Element(int i) 
    {
        assert(i < m_Size);
        return m_pElements[i];
    }

    T* AtPtr(int i) //I dont know what I did but it works - MJ
    {
        assert(i < m_Size);
        return &*m_pElements + i;
    }

    template <typename T>
    inline const T& Element(int i) const {
        assert(i < m_Size);
        return m_pElements[i];
    }

    template <typename T>
    void GrowVector(int num)
    {
        m_Size += num;
        if (m_pElements)
            m_pElements = (T*)mem_alloc_in::mem_alloc->ReAlloc(m_pElements, m_Size * sizeof(T));
        else
            m_pElements = (T*)mem_alloc_in::mem_alloc->Alloc(m_Size * sizeof(T));
    }

    //-----------------------------------------------------------------------------
    // Adds an element, uses default constructor
    //-----------------------------------------------------------------------------
    template <typename T>
    inline int AddToHead() {
        return InsertBefore(0);
    }

    template <typename T>
    inline int AddToTail() {
        return InsertBefore(m_Size);
    }

    template <typename T>
    inline int InsertAfter(int elem) {
        return InsertBefore(elem + 1);
    }

    template <typename T>
    int InsertBefore(int elem) {
        // Can insert at the end
        GrowVector();
        ShiftElementsRight(elem);
        Construct(&Element(elem));
        return elem;
    }

    //-----------------------------------------------------------------------------
    // Adds an element, uses copy constructor
    //-----------------------------------------------------------------------------
    template <typename T>
    inline int AddToHead(const T& src) {
        return InsertBefore(0, src);
    }

    template <typename T>
    inline int AddToTail(const T& src) {
        return InsertBefore(m_Size, src);
    }

    template <typename T>
    inline int InsertAfter(int elem, const T& src) {
        return InsertBefore(elem + 1, src);
    }

    template <typename T>
    int InsertBefore(int elem, const T& src) {
        GrowVector();
        ShiftElementsRight(elem);
        CopyConstruct(&Element(elem), src);
        return elem;
    }

    //-----------------------------------------------------------------------------
    // Shifts elements
    //-----------------------------------------------------------------------------
    template <typename T>
    void ShiftElementsRight(int elem, int num) {
        int numToMove = m_Size - elem - num;
        if ((numToMove > 0) && (num > 0))
            memmove(&Element(elem + num), &Element(elem), numToMove * sizeof(T));
    }

    template <typename T>
    void ShiftElementsLeft(int elem, int num) {
        int numToMove = m_Size - elem - num;
        if ((numToMove > 0) && (num > 0)) {
            memmove(&Element(elem), &Element(elem + num), numToMove * sizeof(T));

#ifdef _DEBUG
            memset(&Element(m_Size - num), 0xDD, num * sizeof(T));
#endif
        }
    }

    //-----------------------------------------------------------------------------
    // Element removal
    //-----------------------------------------------------------------------------
    template <typename T>
    void FastRemove(int elem) {
        Destruct(&Element(elem));
        if (m_Size > 0) {
            if (elem != m_Size - 1)
                memcpy(&Element(elem), &Element(m_Size - 1), sizeof(T));
            --m_Size;
        }
    }

    template <typename T>
    void Remove(int elem) {
        Destruct(&Element(elem));
        ShiftElementsLeft(elem);
        --m_Size;
    }

    template <typename T>
    bool FindAndRemove(const T& src) {
        int elem = GetOffset(src);
        if (elem != -1) {
            Remove(elem);
            return true;
        }
        return false;
    }

    template <typename T>
    bool FindAndFastRemove(const T& src) {
        int elem = GetOffset(src);
        if (elem != -1) {
            FastRemove(elem);
            return true;
        }
        return false;
    }

    //-----------------------------------------------------------------------------
    // Finds an element (element needs operator== defined)
    //-----------------------------------------------------------------------------
    template <typename T>
    int GetOffset(const T& src) const {
        for (auto i = 0; i < Count(); ++i) {
            if (Element(i) == src)
                return i;
        }
        return -1;
    }
};

