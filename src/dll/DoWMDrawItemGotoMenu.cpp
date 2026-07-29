#include <windows.h>
#include <string.h>
#include <stdio.h>

struct WinMenu;

typedef long (__thiscall* DoWMDrawItemGotoMenu_t)(
    WinMenu*, HWND, UINT, DRAWITEMSTRUCT* 
);

DoWMDrawItemGotoMenu_t RealDoWMDrawItemGotoMenu = nullptr;

DoWMDrawItemGotoMenu_t FindDoWMDrawItemGotoMenu()
{
    return (DoWMDrawItemGotoMenu_t)0x004023D2;
}

long __fastcall MyDoWMDrawItemGotoMenu(
    WinMenu* self,
    void* edx,
    HWND hwnd,
    UINT param2,
    DRAWITEMSTRUCT* dis)
{
    MessageBoxA(nullptr, "Hook!", "Petz", MB_OK);

    return RealDoWMDrawItemGotoMenu(self, hwnd, param2, dis);
}