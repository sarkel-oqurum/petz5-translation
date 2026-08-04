#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "hooks.h"
#include "path.h"

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

DWORD WINAPI MainThread(LPVOID)
{
    auto fontPath = GetExeDirectory() + "\\Chuvash256.ttf";
    AddFontResourceExA(
        fontPath.c_str(),
        FR_PRIVATE,
        nullptr
    );


    if (InstallFontHook())
    {
        printf("[src/dll/dll.cpp] Font hook installed.\n");
    }
    else
    {
        printf("[src/dll/dll.cpp] Hook failed.\n");
    }


    return 0;
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD reason,
    LPVOID
)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);
        CreateThread(
            nullptr,
            0,
            MainThread,
            nullptr,
            0,
            nullptr
        );
    }

    return TRUE;
}