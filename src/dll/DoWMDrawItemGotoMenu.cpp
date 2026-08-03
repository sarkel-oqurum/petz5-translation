#include <windows.h>
#include <stdio.h>

struct WinMenu;

typedef LRESULT (__thiscall* DoWMDrawItemGotoMenu_t)(
    WinMenu*,
    HWND,
    WPARAM,
    LPARAM
);

DoWMDrawItemGotoMenu_t RealDoWMDrawItemGotoMenu = nullptr;

DoWMDrawItemGotoMenu_t FindDoWMDrawItemGotoMenu()
{
    return reinterpret_cast<DoWMDrawItemGotoMenu_t>(0x00407540);
}

LRESULT __fastcall MyDoWMDrawItemGotoMenu(
    WinMenu* self,
    void*,
    HWND hwnd,
    WPARAM wParam,
    LPARAM lParam)
{
    MessageBoxA(nullptr, "Hook!", "Petz", MB_OK);

    return RealDoWMDrawItemGotoMenu(self, hwnd, wParam, lParam);
}