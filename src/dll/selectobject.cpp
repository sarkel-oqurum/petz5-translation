#include <windows.h>
#include <stdio.h>
#include <intrin.h>

#include "selectobject.h"
SelectObject_t RealSelectObject = nullptr;
static HFONT gMyFont = NULL;

static void Log(const char* text)
{
    FILE* f = fopen("C:\\Temp\\selectobject.txt", "a");
    if (f)
    {
        fputs(text, f);
        fclose(f);
    }
}

SelectObject_t FindSelectObject()
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

    return (SelectObject_t)GetProcAddress(
        gdi32,
        "SelectObject"
    );
}

HGDIOBJ WINAPI MySelectObject(HDC hdc, HGDIOBJ obj)
{
    if (!gMyFont) {
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
    char buf[256];

    int type = GetObjectType(obj);

    LOGFONTA lf;

    if (GetObjectType(obj) == OBJ_FONT)
    {
        if (GetObjectA(obj, sizeof(lf), &lf))
        {
            void* caller = _ReturnAddress();
            wsprintfA(
                buf,
                "Caller=%p HFONT=%p Face=%s Height=%d\n",
                caller,
                obj,
                lf.lfFaceName,
                lf.lfHeight
            );
            Log(buf);
            if (!lstrcmpA(lf.lfFaceName, "UI Microsoft YaHei")) return RealSelectObject(hdc, gMyFont);
            if (!lstrcmpA(lf.lfFaceName, "SimSun-ExtB")) return RealSelectObject(hdc, gMyFont);
        }
    }

    return RealSelectObject(hdc, obj);
}