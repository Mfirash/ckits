#ifndef THREADKIT_H
#define THREADKIT_H

#include <windows.h>

class threadkit
{
private:
    HANDLE hThread;
    DWORD threadId;

public:
    threadkit();
    bool tcreate(LPTHREAD_START_ROUTINE func, LPVOID param)
    {
        hThread = CreateThread(NULL, 0, func, param, 0, &threadId);
        return hThread != NULL;
    }
    void tjoin()
    {
        if (hThread)
        {
            WaitForSingleObject(hThread, INFINITE);
            CloseHandle(hThread);
            hThread = NULL;
        }
    }
    void tdetach()
    {
        if (hThread)
        {
            CloseHandle(hThread);
            hThread = NULL;
        }
    }
    DWORD tfetchid() { return threadId; }
    bool tjoinable() { return hThread != NULL; }
    threadkit tfork()
    {
        threadkit cloned;
        if (hThread)
        {
            DuplicateHandle(GetCurrentProcess(), hThread, GetCurrentProcess(),
                            &cloned.hThread, 0, FALSE, DUPLICATE_SAME_ACCESS);
            cloned.threadId = threadId;
        }
        return cloned;
    }
    ~threadkit() { tdetach(); }
};

#endif