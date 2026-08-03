#pragma once

#include <windows.h>

class WinMenu;

typedef LRESULT (__thiscall* DoWMDrawItemGotoMenu_t)(
    WinMenu*,
    HWND,
    WPARAM,
    LPARAM
);

extern DoWMDrawItemGotoMenu_t RealDoWMDrawItemGotoMenu;

DoWMDrawItemGotoMenu_t FindDoWMDrawItemGotoMenu();

LRESULT __fastcall MyDoWMDrawItemGotoMenu(
    WinMenu* self,
    void*,
    HWND hwnd,
    WPARAM wParam,
    LPARAM lParam
);