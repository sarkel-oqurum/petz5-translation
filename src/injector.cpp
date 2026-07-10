#include "injector.h"
#include <iostream>
#include <windows.h>
#include <string.h>

bool InjectDLL(HANDLE hProcess, const char* dllPath)
{
    SIZE_T size = strlen(dllPath) + 1;

    LPVOID remoteMem = VirtualAllocEx(
        hProcess,
        NULL,
        size,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );

    if (!remoteMem)
    {
        std::cout << "VirtualAllocEx failed: " << GetLastError() << '\n';
        return false;
    }
    
    if (!WriteProcessMemory(hProcess, remoteMem, dllPath, size, NULL))
    {
        std::cout << "WriteProcessMemory failed: " << GetLastError() << '\n';
        return false;
    }

    LPVOID loadLib = GetProcAddress(
        GetModuleHandleA("kernel32.dll"),
        "LoadLibraryA"
    );

    if (!loadLib)
    {
        std::cout << "GetProcAddress failed.\n";
        return false;
    }

    HANDLE hThread = CreateRemoteThread(
        hProcess,
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)loadLib,
        remoteMem,
        0,
        NULL
    );

    if (!hThread)
    {
        std::cout << "CreateRemoteThread failed: " << GetLastError() << '\n';
        return false;
    }

    CloseHandle(hThread);
    return true;
}