#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "hooks.h"

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

DWORD WINAPI MainThread(LPVOID)
{
    AddFontResourceExA(
        "C:\\Games\\Petz 5\\Chuvash256.ttf",
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