#pragma once

#include <windows.h>

typedef HFONT (WINAPI* CreateFontIndirectA_t)(
    const LOGFONTA*
);