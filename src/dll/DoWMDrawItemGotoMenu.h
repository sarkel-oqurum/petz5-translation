#pragma once

#include <windows.h>

// WinMenu::DoWMDrawItemGotoMenu(WinMenu *this,HWND__ *param_1,uint param_2,long param_3)
struct WinMenu;

typedef long (__thiscall* DoWMDrawItemGotoMenu_t)(
    WinMenu*, HWND, UINT, DRAWITEMSTRUCT* 
);

DoWMDrawItemGotoMenu_t FindDoWMDrawItemGotoMenu();

long __fastcall MyDoWMDrawItemGotoMenu(
    WinMenu* hMenu,
    void* edx,
    HWND item,
    UINT fByPosition,
    DRAWITEMSTRUCT* lpmii
);

extern DoWMDrawItemGotoMenu_t RealDoWMDrawItemGotoMenu;