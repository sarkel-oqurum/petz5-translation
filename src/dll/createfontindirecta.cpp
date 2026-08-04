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
    {
        //std::cout << "[src/dll/createfontindirecta.cpp] GDI32 not found.\n";
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
        LF_FACESIZE,
        "Chuvash256"
    );
    //modified.lfCharSet = EASTEUROPE_CHARSET; // 238

    modified.lfCharSet = 204; //RUSSIAN
    

    char buf[256];
    sprintf_s(
    buf,
    "Face='%s'\nHeight=%ld\nCharset=%u\nWeight=%ld",
    lpLogFont->lfFaceName,
    lpLogFont->lfHeight,
    lpLogFont->lfCharSet,
    lpLogFont->lfWeight
);

    //printf("[src/dll/createfontindirecta.cpp] " << buf << "\n");
    //std::cout << "[src/dll/createfontindirecta.cpp] Font requested." << modified.lfFaceName << "\n";
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
        "Chuvash256"
);

if (test)
{
    printf("[src/dll/createfontindirecta.cpp] Font exists.\n");
}
else
{
    printf("[src/dll/createfontindirecta.cpp] Font missing.\n");
}

if (!RealCreateFontIndirectA)
{
    printf("[src/dll/createfontindirecta.cpp] Error: NULL trampoline.\n");
    return nullptr;
}

    return RealCreateFontIndirectA(&modified);
}