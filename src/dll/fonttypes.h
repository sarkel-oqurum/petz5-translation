#pragma once

#include <windows.h>

typedef HFONT (WINAPI* CreateFontIndirectA_t)(
    const LOGFONTA*
);

typedef HFONT (WINAPI* CreateFontA_t)(
    int, int, int, int, int,
    DWORD, DWORD, DWORD,
    DWORD, DWORD, DWORD,
    DWORD, DWORD,
    LPCSTR
);