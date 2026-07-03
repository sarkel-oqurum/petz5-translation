#include <windows.h>
#include <string.h>

#pragma comment(lib, "user32.lib")

DWORD WINAPI MainThread(LPVOID)
{
    MessageBoxA(nullptr, "Main thread running", "PetzChuvash", MB_OK);
    return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, MainThread, nullptr, 0, nullptr);
    }
    return TRUE;
}