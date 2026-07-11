#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "fonttypes.h"

CreateFontIndirectA_t RealCreateFontIndirectA = nullptr;

// Looks for CreateFontIndirectA where the fonts are initialized. We will hook this.
CreateFontIndirectA_t FindCreateFontIndirectA()
{
    HMODULE gdi = GetModuleHandleA("gdi32.dll");

    //FARPROC addr = GetProcAddress(
    //    gdi,
    //    "CreateFontIndirectA"
    //);
    //char buf[100];
    //sprintf_s(
    //    buf,
    //    sizeof(buf),
    //    "CreateFontIndirectA: %p",
    //    addr
    //);

    if (!gdi)
    {MessageBoxA(
            nullptr,
            "did not find font",
            "Petz",
            MB_OK
        );
        return nullptr;}

    return (CreateFontIndirectA_t)GetProcAddress(
        gdi,
        "CreateFontIndirectA"
    );
}

HFONT WINAPI MyCreateFontIndirectA(
    const LOGFONTA* lpLogFont
)
{
    OutputDebugStringA(
    "CreateFontIndirectA hook hit\n"
);
    LOGFONTA modified = *lpLogFont;

    strcpy_s(
        modified.lfFaceName,
        "Chuvash256"
    );
    //modified.lfCharSet = EASTEUROPE_CHARSET; // 238

    //modified.lfCharSet = ANSI_CHARSET;

    char buf[256];
    sprintf_s(
    buf,
    "Face='%s'\nHeight=%ld\nCharset=%u\nWeight=%ld",
    lpLogFont->lfFaceName,
    lpLogFont->lfHeight,
    lpLogFont->lfCharSet,
    lpLogFont->lfWeight
);

MessageBoxA(nullptr, buf, "Original LOGFONT", MB_OK);
    
    MessageBoxA(
    nullptr,
    modified.lfFaceName,
    "Font requested",
    MB_OK
    );
    HFONT test = CreateFontA(
    -16,
    0,
    0,
    0,
    FW_NORMAL,
    FALSE,
    FALSE,
    FALSE,
    DEFAULT_CHARSET,
    OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,
    DEFAULT_QUALITY,
    DEFAULT_PITCH,
    "256Chuvash"
);

if (test)
{
    MessageBoxA(nullptr, "Font exists", "Test", MB_OK);
}
else
{
    MessageBoxA(nullptr, "Font missing", "Test", MB_OK);
}

if (!RealCreateFontIndirectA)
{
    MessageBoxA(nullptr, "NULL trampoline", "error", MB_OK);
    return nullptr;
}

    return RealCreateFontIndirectA(&modified);
}