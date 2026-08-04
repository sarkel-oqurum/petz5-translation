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
        MessageBoxA(
            nullptr,
            "Font hook installed",
            "Petz",
            MB_OK
        );
        
    }
    else
    {
        MessageBoxA(
            nullptr,
            "Hook failed",
            "Petz",
            MB_OK
        );
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

        MessageBoxA(
            nullptr,
            "DLL loaded!",
            "debug",
            MB_OK
        );

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