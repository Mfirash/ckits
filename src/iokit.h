#ifndef IOKIT_H
#define IOKIT_H

#include <windows.h>
#include "strkit.h"

class iokit
{
public:
    void intToA(int value, char *buffer, int &pos)
    {
        if (value == 0)
        {
            buffer[pos++] = '0';
            return;
        }
        if (value < 0)
        {
            buffer[pos++] = '-';
            value = -value;
        }

        char temp[12];
        int i = 0;
        while (value > 0)
        {
            temp[i++] = (value % 10) + '0';
            value /= 10;
        }
        while (i > 0)
        {
            buffer[pos++] = temp[--i];
        }
    }

    void printout(const char *format, ...)
    {
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        char buffer[1024];
        int bIdx = 0;

        va_list args;
        va_start(args, format);

        for (int i = 0; format[i] != '\0' && bIdx < 1023; i++)
        {
            if (format[i] == '%' && format[i + 1] != '\0')
            {
                i++;
                if (format[i] == 's')
                {
                    const char *s = va_arg(args, const char *);
                    while (*s && bIdx < 1023)
                        buffer[bIdx++] = *s++;
                }
                else if (format[i] == 'd')
                {
                    int d = va_arg(args, int);
                    intToA(d, buffer, bIdx);
                }
                else
                {
                    buffer[bIdx++] = format[i];
                }
            }
            else
            {
                buffer[bIdx++] = format[i];
            }
        }

        va_end(args);

        DWORD bytesWritten = 0;
        WriteFile(hStdOut, buffer, bIdx, &bytesWritten, NULL);
    }
    strkit input()
    {
        strkit result;
        HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
        char charBuffer[128];
        DWORD bytesRead = 0;
        if (ReadFile(hStdIn, charBuffer, sizeof(charBuffer), &bytesRead, NULL))
        {
            for (DWORD i = 0; i < bytesRead; i++)
            {
                if (charBuffer[i] != '\r' && charBuffer[i] != '\n')
                {
                    result.append(charBuffer[i]);
                }
            }
        }
        return result;
    }

    HANDLE fileopen(const char *filename, const char *mode)
    {
        DWORD dwDesiredAccess = 0;
        DWORD dwCreationDisposition = 0;
        if (mode[0] == 'r')
        {
            dwDesiredAccess = GENERIC_READ;
            dwCreationDisposition = OPEN_EXISTING;
        }
        else if (mode[0] == 'w')
        {
            dwDesiredAccess = GENERIC_WRITE;
            dwCreationDisposition = CREATE_ALWAYS;
        }
        else if (mode[0] == 'a')
        {
            dwDesiredAccess = FILE_APPEND_DATA;
            dwCreationDisposition = OPEN_ALWAYS;
        }
        HANDLE hFile = CreateFileA(
            filename,
            dwDesiredAccess,
            FILE_SHARE_READ,
            NULL,
            dwCreationDisposition,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        return hFile;
    }
    strkit fileread(HANDLE hFile)
    {
        strkit result;
        if (hFile == INVALID_HANDLE_VALUE)
            return result;

        DWORD fileSize = GetFileSize(hFile, NULL);
        if (fileSize == INVALID_FILE_SIZE || fileSize == 0)
            return result;
        result.strreserve(fileSize);
        HANDLE hHeap = GetProcessHeap();
        char *tempBuf = (char *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, fileSize);
        DWORD bytesRead = 0;

        if (ReadFile(hFile, tempBuf, fileSize, &bytesRead, NULL))
        {
            for (DWORD i = 0; i < bytesRead; i++)
            {
                result.append(tempBuf[i]);
            }
        }

        HeapFree(hHeap, 0, tempBuf);
        return result;
    }
    size_t filewrite(HANDLE hFile, const char *input)
    {
        if (hFile == INVALID_HANDLE_VALUE || input == NULL)
            return 0;

        DWORD bytesWritten = 0;
        DWORD len = 0;
        while (input[len] != '\0')
            len++;

        if (WriteFile(hFile, input, len, &bytesWritten, NULL))
        {
            return (size_t)bytesWritten;
        }
        return 0;
    }

    int fileclose(HANDLE hFile)
    {
        if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
            return -1;

        if (CloseHandle(hFile))
        {
            return 0;
        }
        return GetLastError();
    }
    int mkdir(LPCSTR name)
    {
        if (CreateDirectoryA(name, NULL))
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    int rmdir(LPCSTR name)
    {
        if (RemoveDirectoryA(name))
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    int listdir(LPCSTR directory)
    {
        char search_path[MAX_PATH];
        wsprintfA(search_path, "%s\\*", directory);
        WIN32_FIND_DATAA find_data;
        HANDLE hFind = FindFirstFileA(search_path, &find_data);
        if (hFind == INVALID_HANDLE_VALUE)
        {
            return;
        }
        do
        {
            if (lstrcmpA(find_data.cFileName, ".") == 0 || lstrcmpA(find_data.cFileName, "..") == 0)
            {
                continue;
            }
            if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
            }
            else
            {
            }

        } while (FindNextFileA(hFind, &find_data));
        FindClose(hFind);
    }
    void chdir(char *ndir)
    {
        SetCurrentDirectoryA(ndir);
    }
    char *getcwd()
    {
        DWORD size = GetCurrentDirectoryA(0, NULL);
        if (size == 0)
            return NULL;
        HANDLE hHeap = GetProcessHeap();
        char *buffer = (char *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, size);
        GetCurrentDirectoryA(size, buffer);
        return buffer;
    }
};

#endif