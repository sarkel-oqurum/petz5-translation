#pragma once

#include <windows.h>
#include "fonttypes.h"

CreateFontA_t FindCreateFontA();

HFONT WINAPI MyCreateFontA(
    int cHeight,
    int cWidth,
    int cEscapement,
    int cOrientation,
    int cWeight,
    DWORD bItalic,
    DWORD bUnderline,
    DWORD bStrikeOut,
    DWORD iCharSet,
    DWORD iOutPrecision,
    DWORD iClipPrecision,
    DWORD iQuality,
    DWORD iPitchAndFamily,
    LPCSTR pszFaceName
);

extern CreateFontA_t RealCreateFontA;