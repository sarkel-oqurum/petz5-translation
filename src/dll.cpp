#include <windows.h>
#include <string.h>
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hinstDLL);

  int added = AddFontResourceExA(
    "C:\\Users\\Lighty\\Documents\\projects\\petzChuvash\\font\\256Chuvash_test.ttf",
    FR_PRIVATE,
    NULL);

char buffer[64];
wsprintfA(buffer, "Fonts added: %d", added);

MessageBoxA(NULL, buffer, "Result", MB_OK);

HFONT hFont = CreateFontA(
    -16,
    0, 0, 0,
    FW_NORMAL,
    FALSE, FALSE, FALSE,
    DEFAULT_CHARSET,
    OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,
    DEFAULT_QUALITY,
    DEFAULT_PITCH,
    "256Chuvash"   // Replace with the actual internal family name
);

MessageBoxA(
    NULL,
    hFont ? "CreateFont succeeded" : "CreateFont failed",
    "Test",
    MB_OK);
        break;
    }

    return TRUE;
}