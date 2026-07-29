#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "fonttypes.h"

CreateFontA_t RealCreateFontA = nullptr;

CreateFontA_t FindCreateFontA()
{
    HMODULE gdi = GetModuleHandleA("gdi32.dll");

    if (!gdi)
    {MessageBoxA(
            nullptr,
            "did not find gdis",
            "Petz",
            MB_OK
        );
        return nullptr;
    }

    return (CreateFontA_t)GetProcAddress(
        gdi,
        "CreateFontA"
    );
}

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
)
{
    if (pszFaceName)
    {
        pszFaceName = "Impact";
    }

    if (iCharSet)
    {
        iCharSet = 204; // RUSSIAN
    }
    return RealCreateFontA(
        cHeight,
        cWidth,
        cEscapement,
        cOrientation,
        cWeight,
        bItalic,
        bUnderline,
        bStrikeOut,
        iCharSet,
        iOutPrecision,
        iClipPrecision,
        iQuality,
        iPitchAndFamily,
        pszFaceName
    );

}


/*HFONT CreateFontA(
  [in] int    cHeight,          0?
  [in] int    cWidth,           0?
  [in] int    cEscapement,      GM_COMPATIBLE?
  [in] int    cOrientation,     GM_COMPATIBLE
  [in] int    cWeight,          FW_DONTCARE
  [in] DWORD  bItalic,          FALSE
  [in] DWORD  bUnderline,       FALSE
  [in] DWORD  bStrikeOut,       FALSE
  [in] DWORD  iCharSet,         0? ANSI_CHARSET? whatever the default is
  [in] DWORD  iOutPrecision,    OUT_DEFAULT_PRECISs
  [in] DWORD  iClipPrecision,   CLIP_DEFAULT_PRECIS
  [in] DWORD  iQuality,         CLEARTYPE_QUALITY or ANTIALIASED_QUALITY or PROOF_QUALITY idk
  [in] DWORD  iPitchAndFamily,  FF_DONTCARE
  [in] LPCSTR pszFaceName       "Chuvash256"
);*/