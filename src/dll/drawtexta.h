#pragma once

#include <windows.h>

//should be in types.h probably
typedef int (WINAPI* DrawTextA_t)(
    HDC, LPCSTR, int, LPRECT, UINT
);


DrawTextA_t FindDrawTextA();

int WINAPI MyDrawTextA(
    HDC hdc,
    LPCSTR lpchText,
    int cchText,
    LPRECT lprc,
    UINT format
);

extern DrawTextA_t RealDrawTextA;