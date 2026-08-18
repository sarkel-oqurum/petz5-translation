#define STRICT
#include <windows.h>
#include <stdio.h>
#include <string>
#include "WinMenuStuff.hpp"

struct WinMenu;

static void Log(const char* text)
{
    FILE* f = fopen("C:\\Temp\\menu.txt", "a");
    if (f)
    {
        fputs(text, f);
        fclose(f);
    }
}

typedef LRESULT (__thiscall* DoWMDrawItemGotoMenu_t)(
    WinMenu*,
    HWND,
    WPARAM,
    LPARAM
);

DoWMDrawItemGotoMenu_t RealDoWMDrawItemGotoMenu = nullptr;

DoWMDrawItemGotoMenu_t FindDoWMDrawItemGotoMenu()
{
    printf("mydowmdrawitem");
    return reinterpret_cast<DoWMDrawItemGotoMenu_t>(0x00407540);
}

bool MyBitmapBlt (HDC hdc, int x, int y, HBITMAP hBmp, DWORD rop, int centerWidth, bool grayed) {

    //https://doxygen.reactos.org/d3/d89/win32ss_2gdi_2gdi32_2objects_2gdiobj_8c.html
    BITMAP bmp;

    int obj = GetObjectA(hBmp, 0x18, &bmp);
    if (obj == 0) {
        MessageBoxA(
            nullptr,
            "bitmap",
            "Petz",
            MB_OK
        );
        return 0;
    }

    //if ((centerWidth != 0) && (iVar3 = (centerWidth - local_14) / 2, iVar3 < 0)) {
    //    iVar3 = 0;
    //}

    if (grayed) {
    //    BVar2 = DrawStateA(param_1,*(HBRUSH *)this,(DRAWSTATEPROC)0x0,(LPARAM)param_4,0,iVar3 + param_2,
    //                   param_3,local_14,local_10,0x24);
    //    return BVar2;
    }

    HDC newHdc = CreateCompatibleDC(hdc);
    HGDIOBJ h = SelectObject(newHdc, hBmp);
    bool BVar2 = BitBlt(hdc, /*iVar3 + */x, y, bmp.bmWidth, bmp.bmHeight, newHdc, 0, 0, rop);
    SelectObject(newHdc, h);
    DeleteDC(newHdc);
    return BVar2;
}

LRESULT __fastcall MyDoWMDrawItemGotoMenu(
    WinMenu* self,
    void*,
    HWND hwnd,
    WPARAM wParam,
    LPARAM lParam)
{
    DRAWITEMSTRUCT* const dis = (DRAWITEMSTRUCT*)lParam;
    HDC hdc= dis->hDC;
    UINT itemId = dis->itemID;

//  palette management stuff
    HPALETTE hOldPal = NULL;
    HPALETTE theirPalette = (HPALETTE)GetProcAddress(NULL, "?theirPalette@XDrawPort@@2PAUHPALETTE__@@A");
    if (theirPalette) {
        hOldPal = SelectPalette(hdc, theirPalette, FALSE);
        RealizePalette(hdc);
    };

//  MENUITEMINFO stuff
    MENUITEMINFOA mii;
    memset(&mii, 0, sizeof(mii));
    mii.cbSize = sizeof(MENUITEMINFOA);
    mii.fMask  = MIIM_STATE | MIIM_ID | MIIM_CHECKMARKS | MIIM_DATA;

    HMENU hMenu = (itemId >= CUSTOM_SCENE_ID_FIRST && itemId <= CUSTOM_SCENE_ID_LAST)
        ? self->m_CustomScenesMenu
        : self->m_GotoMenu;
    GetMenuItemInfoA(hMenu, itemId, FALSE, &mii);

    const char* areaName = self->m_GotoAreaNames.m_Data[mii.dwItemData].m_String;

    RECT rc = dis->rcItem;

//  ID 11004: etched 3D line
    if (itemId == GOTO_SEPARATOR_ID) {
        FillRect(hdc, &rc, self->m_ColorMenu);

//      shadow
        HPEN hPenDark = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNSHADOW));
        HPEN hOldPen = SelectPen(hdc, hPenDark);
        MoveToEx(hdc, rc.left + GOTO_SEPARATOR_SHADOW_X, rc.top + GOTO_SEPARATOR_SHADOW_Y, NULL);
        LineTo(hdc, rc.right - GOTO_SEPARATOR_SHADOW_X, rc.top + GOTO_SEPARATOR_SHADOW_Y);

//      highlight
        HPEN hPenLight = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_HIGHLIGHTTEXT));
        HPEN hOldPenDark = SelectPen(hdc, hPenLight);
        DeleteObject(hOldPenDark);  // delete dark pen
        MoveToEx(hdc, rc.left + GOTO_SEPARATOR_HIGHLIGHT_X, rc.top + GOTO_SEPARATOR_HIGHLIGHT_Y, NULL);
        LineTo(hdc, rc.right - GOTO_SEPARATOR_HIGHLIGHT_X, rc.top + GOTO_SEPARATOR_HIGHLIGHT_Y);

//      cleanup
        HPEN hOldPenLight = SelectPen(hdc, hOldPen);
        DeleteObject(hOldPenLight); // delete light pen
    }

//  area icon + text
    else {
//      draw icon
        if (mii.hbmpChecked) {
            RECT rcIcon = rc;
            rcIcon.right = rcIcon.left + GOTO_ICON_COLUMN_WIDTH;
            FillRect(hdc, &rcIcon, self->m_ColorMenu);
            MyBitmapBlt(hdc, rc.left + GOTO_ICON_X, rc.top + GOTO_ICON_Y,
                      mii.hbmpChecked, SRCCOPY, 0, false);
        };

        rc = dis->rcItem;
        rc.left += GOTO_ICON_COLUMN_WIDTH;
        bool isSelected = (dis->itemState & ODS_SELECTED) != 0;
        bool isGrayed = (dis->itemState & ODS_GRAYED) != 0;

//      colors
        if (isSelected) {
            SetTextColor(hdc, GetSysColor(COLOR_HIGHLIGHTTEXT));
            SetBkColor(hdc, GetSysColor(COLOR_HIGHLIGHT));
            FillRect(hdc, &rc, self->m_ColorHighlight);

//          selected gets an etched 3D border
            POINT border;
            border.x = dis->rcItem.left + 1;
            border.y = dis->rcItem.top;

//          top-left: highlight
            HPEN hPenLight = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_HIGHLIGHTTEXT));
            HPEN hOldPen = SelectPen(hdc, hPenLight);
            MoveToEx(hdc, border.x, border.y, NULL);
            LineTo(hdc, border.x + GOTO_ICON_BORDER_SIZE, border.y);
            MoveToEx(hdc, border.x, border.y, NULL);
            LineTo(hdc, border.x, border.y + GOTO_ICON_BORDER_SIZE);
//          bottom-right: shadow
            HPEN hPenDark = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_HIGHLIGHT));
            HPEN hOldPenLight = SelectPen(hdc, hPenDark);
            DeleteObject(hOldPenLight); // delete light pen
            LineTo(hdc, border.x + GOTO_ICON_BORDER_SIZE, border.y + GOTO_ICON_BORDER_SIZE);
            LineTo(hdc, border.x + GOTO_ICON_BORDER_SIZE, border.y - 1);
            
//          cleanup
            HPEN hOldPenDark = SelectPen(hdc, hOldPen);
            DeleteObject(hOldPenDark);  // delete dark pen
        }
        else {
            SetTextColor(hdc, GetSysColor(COLOR_MENUTEXT));
            SetBkColor(hdc, GetSysColor(COLOR_MENU));
            FillRect(hdc, &rc, self->m_ColorMenu);
        };

        LONG textX = dis->rcItem.left + GOTO_TEXT_X;
        rc.left = textX;
        
//      draw text
        Log(areaName);
        
        if (!isGrayed || isSelected){
            HFONT hFont = CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
                DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Chuvash256"));
            HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

            //shitty bandaid solution bear with me. have to write bytes directly
            if (!lstrcmpA(areaName, "Adoption Center")) {
                const char* name = "\xD3" "\xF1" "\xF0" "\xE0" "\xE2" "\x20" "\x87" "\xF3" "\xF0" "\xF7" "\x84";
                // Усрав çурчĕ
                DrawTextA(hdc, name, -1, &rc, DT_SINGLELINE | DT_VCENTER);
            }            
            else if (!lstrcmpA(areaName, "Exit")) {
                // Тух
                DrawTextA(hdc, "\xD2" "\xF3" "\xF5", -1, &rc, DT_SINGLELINE | DT_VCENTER);
            }
            else if (!lstrcmpA(areaName, "Editor")) DrawTextA(hdc, "\xD0" "\xE5" "\xE4" "\xE0" "\xEA" "\xF2" "\xEE" "\xF0", -1, &rc, DT_SINGLELINE | DT_VCENTER);
            else if (!lstrcmpA(areaName, "Desktop")) DrawTextA(hdc, "\xD1" "\x84" "\xF2" "\xE5" "\xEB", -1, &rc, DT_SINGLELINE | DT_VCENTER);
            else if (!lstrcmpA(areaName, "Custom Scenes")) DrawTextA(hdc, "\xCA" "\xE0" "\xF1" "\xF2" "\xEE" "\xEC" "\xD1" "\xF6" "\xE5" "\xED" "\xE0" "\xF1" "\xE5" "\xEC", -1, &rc, DT_SINGLELINE | DT_VCENTER);
            else if (!lstrcmpA(areaName, "Petz Publisher")) DrawTextA(hdc, "Petz " "\xE8" "\xE7" "\xE4" "\xE0" "\xF2" "\xE5" "\xEB" "\x84", -1, &rc, DT_SINGLELINE | DT_VCENTER);
            else DrawTextA(hdc, areaName, -1, &rc, DT_SINGLELINE | DT_VCENTER);
            
            SelectObject(hdc, oldFont);
        }
        else {
            int textY = rc.top + (rc.bottom - self->m_CurAreaGrayedOutTextY - rc.top) / 2;
             if (!lstrcmpA(areaName, "Adoption Center")) {
                const char* name = "\xD3" "\xF1" "\xF0" "\xE0" "\xE2" "\x20" "\x87" "\xF3" "\xF0" "\xF7" "\x84";
                DrawTextA(hdc, name, -1, &rc, DT_SINGLELINE | DT_VCENTER);
            }
            else DrawStateA(hdc, NULL, NULL, (LPARAM)areaName, strlen(areaName),
                       textX, textY, rc.right - textX, rc.bottom - rc.top,
                       DST_TEXT | DSS_DISABLED);
        };

//      when hovering the current area, a checkerboard mask is AND'd over the text portion for a subtle darkening effect
        if (isGrayed && isSelected) {
            HDC hdcMask = CreateCompatibleDC(hdc);
            HBITMAP hOldBmp = SelectBitmap(hdcMask, self->m_MonoDitheredMask);
            SetTextColor(hdc, RGB(0, 0, 0));
            SetBkColor(hdc, RGB(255, 255, 255));

//          dither mask is 256×256; tile horizontally if needed
            RECT rcDither = dis->rcItem;
            rcDither.left += (GOTO_ICON_COLUMN_WIDTH - 1);

            if (rcDither.right - rcDither.left > 256)
                rcDither.right = rcDither.left + 256;

            do {
                BitBlt(hdc, rcDither.left, rcDither.top,
                       rcDither.right - rcDither.left,
                       rcDither.bottom - rcDither.top,
                       hdcMask, 0, 0, SRCAND);
                OffsetRect(&rcDither, 256, 0);
            } while (rcDither.left < dis->rcItem.right);

//          cleanup
            SelectBitmap(hdcMask, hOldBmp);
            DeleteDC(hdcMask);
        };
    };

    if (hOldPal) SelectPalette(hdc, hOldPal, FALSE);
    return 1;
}

