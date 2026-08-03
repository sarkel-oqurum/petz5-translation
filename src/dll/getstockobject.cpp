#include <windows.h>
#include <stdio.h>

#include "getstockobject.h"
static HFONT gMyFont = NULL;

GetStockObject_t RealGetStockObject = nullptr;

GetStockObject_t FindGetStockObject()
{
    HMODULE gdi32 = GetModuleHandleA("gdi32.dll");

    if (!gdi32)
    {
        MessageBoxA(
            nullptr,
            "Could not find gdi32.dll",
            "Petz",
            MB_OK
        );
        return nullptr;
    }

    return (GetStockObject_t)GetProcAddress(
        gdi32,
        "GetStockObject"
    );
}

HGDIOBJ WINAPI MyGetStockObject(int fnObject)
{
    char buf[128];
    FILE* f = fopen("C:\\temp\\petzlog.txt", "a");
if (f)
{
    fprintf(f, "GetStockObject(%d)\n", fnObject);
    fclose(f);
}

    if (fnObject == SYSTEM_FONT ||
    fnObject == DEFAULT_GUI_FONT ||
    fnObject == ANSI_VAR_FONT ||
    fnObject == DEVICE_DEFAULT_FONT)
    {
        if (!gMyFont)
        {
            gMyFont = CreateFontA(
                -16, 0,0,0,
                FW_NORMAL,
                FALSE,FALSE,FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY,
                DEFAULT_PITCH,
                "Impact");
        }

        return gMyFont;
    }

    return RealGetStockObject(fnObject);
}