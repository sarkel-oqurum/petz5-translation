#include <windows.h>
#include <stdio.h>
#include "WinMenuStuff.hpp"

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
            //self->BitmapBlt(hdc, rc.left + GOTO_ICON_X, rc.top + GOTO_ICON_Y,
            //          mii.hbmpChecked, SRCCOPY, 0, false);
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
        if (!isGrayed || isSelected){
            HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
                DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Chuvash256"));
            HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

            DrawTextA(hdc, areaName, -1, &rc, DT_SINGLELINE | DT_VCENTER);
            
            SelectObject(hdc, oldFont);
        }
        else {
            int textY = rc.top + (rc.bottom - self->m_CurAreaGrayedOutTextY - rc.top) / 2;
            DrawStateA(hdc, NULL, NULL, (LPARAM)areaName, strlen(areaName),
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