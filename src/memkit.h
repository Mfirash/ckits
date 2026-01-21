#ifndef MEMKIT_H
#define MEMKIT_H

#include <windows.h>

class memkit {
private:
    HANDLE hHeap;

public:
    memkit()
    {
        hHeap = GetProcessHeap();
    }

    void *m_malloc(size_t size)
    {
        return HeapAlloc(hHeap, HEAP_ZERO_MEMORY, size);
    }

    void *m_calloc(size_t num, size_t size)
    {
        return HeapAlloc(hHeap, HEAP_ZERO_MEMORY, num * size);
    }

    void *m_realloc(void *ptr, size_t new_size)
    {
        if (!ptr)
            return m_malloc(new_size);
        return HeapReAlloc(hHeap, 0, ptr, new_size);
    }

    void m_free(void *ptr)
    {
        if (ptr)
            HeapFree(hHeap, 0, ptr);
    }

    void *m_aligned_alloc(size_t alignment, size_t size)
    {
        return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    }
};

#endif