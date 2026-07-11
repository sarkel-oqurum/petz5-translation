#pragma once

#include <windows.h>
#include "fonttypes.h"

CreateFontIndirectA_t FindCreateFontIndirectA();

HFONT WINAPI MyCreateFontIndirectA(
    const LOGFONTA* lpLogFont
);

extern CreateFontIndirectA_t RealCreateFontIndirectA;