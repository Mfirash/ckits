#ifndef SYNCKIT_H
#define SYNCKIT_H

#include <windows.h>

class synckit
{
private:
    CRITICAL_SECTION cs;
    HANDLE hSemaphore;
    HANDLE hMutex;

public:
    synckit()
    {
        InitializeCriticalSection(&cs);
        hSemaphore = NULL;
        hMutex = NULL;
    }
    ~synckit()
    {
        DeleteCriticalSection(&cs);
        CloseHandle(hSemaphore);
        CloseHandle(hMutex);
    }
    void lock()
    {
        EnterCriticalSection(&cs);
    }
    bool attemptlock()
    {
        return TryEnterCriticalSection(&cs);
    }
    void unlock()
    {
        LeaveCriticalSection(&cs);
    }
    bool createmutex(const char *name = nullptr)
    {
        hMutex = CreateMutexA(NULL, FALSE, name);
        return hMutex != NULL;
    }
    void wait_mutex()
    {
        WaitForSingleObject(hMutex, INFINITE);
    }
    void release_mutex()
    {
        ReleaseMutex(hMutex);
    }
    bool synckit::create_semaphore(long initial_count, long max_count)
    {
        hSemaphore = CreateSemaphoreA(NULL, initial_count, max_count, NULL);
        return hSemaphore != NULL;
    }

    void synckit::wait_semaphore()
    {
        WaitForSingleObject(hSemaphore, INFINITE);
    }

    void synckit::release_semaphore(long count)
    {
        ReleaseSemaphore(hSemaphore, count, NULL);
    }
};

#endif