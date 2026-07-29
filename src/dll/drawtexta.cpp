#include <windows.h>
#include <winuser.h>
#include <string.h>
#include <stdio.h>

typedef int (WINAPI* DrawTextA_t)(
    HDC, LPCSTR, int, LPRECT, UINT
);

DrawTextA_t RealDrawTextA = nullptr;

DrawTextA_t FindDrawTextA()
{
    HMODULE gdi = GetModuleHandleA("gdi32.dll");

    if (!gdi)
    {
        MessageBoxA(
            nullptr,
            "did not find gdis",
            "Petz",
            MB_OK
        );
        return nullptr;
    }

    return (DrawTextA_t)GetProcAddress(
        gdi,
        "DrawTextA"
    );
}

int WINAPI MyDrawTextA(
    HDC hdc,
    LPCSTR lpchText,
    int cchText,
    LPRECT lprc,
    UINT format
)
{
    /*char msgBuf[512];
        sprintf_s(
        msgBuf,
        sizeof(msgBuf),
        "lpchText = %s\n",
        lpchText
    );
        MessageBoxA(nullptr, msgBuf, "DrawTextA", MB_OK);*/

    return RealDrawTextA(
        hdc,
        lpchText,
        cchText,
        lprc,
        format
    );

}