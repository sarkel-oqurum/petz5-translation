#pragma once

#include <windows.h>

//should be in types.h probably
typedef BOOL (WINAPI* InsertMenuItemA_t)(
    HMENU, int, int, LPCMENUITEMINFOA 
);

InsertMenuItemA_t FindInsertMenuItemA();

BOOL WINAPI MyInsertMenuItemA(
    HMENU hMenu,
    UINT item,
    BOOL fByPosition,
    LPCMENUITEMINFOA lpmii
);

extern InsertMenuItemA_t RealInsertMenuItemA;