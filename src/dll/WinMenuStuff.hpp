// Made by Reflet (Many thanks to them)

#pragma once
#include <WINDOWS.H>
#include <WINDOWSX.H>

#ifndef PFLIB_PETZ3
#define PFLIB_PETZ3 0
#endif

class mystring {
public:
    char* m_String;
};
template<typename T1, typename T2 = const char*>
class pfvector {
public:
    T1* m_Data;
    int m_Size;
    int m_Capacity;
};

class WinMenu {
public:
    HBRUSH m_ColorMenu;
    HBRUSH m_ColorHighlight;
    HBRUSH m_FemalePinkBrush;
    HBRUSH m_MaleBlueBrush;
    HBITMAP m_MonoDitheredMask;
    HBITMAP m_RunawayBitmap;
    bool m_PetMenuEnabled;
    RECT* m_PetMenuItemBounds;
    MENUITEMINFO* m_PetMenuItemInfos;
    int m_PetMenuItemCount;
    int m_PetMinIndex;
    int m_PetMaxIndex;
    int m_PetMenuWidth;
    HWND m_PetzWnd;
    HWND m_PetMenuWnd;
    bool m_PetMenuNotOpen;
    int m_CurrentPetIndex;
    int m_LastPetIndex;
    POINT m_PetMenuOrigin;
    SIZE m_PetMenuBorderSize;
    UINT m_PetMenuTimerID;  //  VS97-era nIDEvent was UINT instead of UINT_PTR
    HMENU m_GotoMenu;
    HMENU m_GotoMenuRoot;
#if !PFLIB_PETZ3
    HMENU m_CustomScenesMenu;
#endif
    int m_CurAreaGrayedOutTextY;
#if !PFLIB_PETZ3
    pfvector<mystring> m_GotoAreaNames;
#endif
    HHOOK m_MenuGetMessageHook;
    HHOOK m_MenuKeyboardHook;


    __declspec(dllimport) WinMenu(HINSTANCE hInstance);
    __declspec(dllimport) ~WinMenu();
    __declspec(dllimport) LRESULT DoHandleEvents(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    __declspec(dllimport) LRESULT DoWMMenuSelect(HWND hWnd, WPARAM wParam, LPARAM lParam);
    __declspec(dllimport) LRESULT DoWMMeasureItem(HWND hWnd, WPARAM wParam, LPARAM lParam) const;
    __declspec(dllimport) LRESULT DoWMDrawItem(HWND hWnd, WPARAM wParam, LPARAM lParam) const;
    __declspec(dllimport) LRESULT DoWMDrawItemGotoMenu(HWND hWnd, WPARAM wParam, LPARAM lParam) const;
    //__declspec(dllimport) int BitmapBlt(HDC hdc, int x, int y, HBITMAP hBmp, DWORD rop, int centerWidth, bool grayed) const;
    __declspec(dllimport) HBITMAP CreateMonoDitheredMask(int nWidth, int nHeight) const;
    __declspec(dllimport) void PFEnablePopupMenu();
    __declspec(dllimport) int PFTrackPopupMenu(HMENU hMenu, UINT uFlags, int x, int y, int nReserved, HWND hWndParent, const RECT* prc);
    __declspec(dllimport) void EndMenu(HWND hWnd);
    __declspec(dllimport) void MeasureMenu(HWND hWnd);
    __declspec(dllimport) void GetMenuMouseLoc(POINT& pt, HWND hWnd, UINT uMsg, LPARAM lParam);
    __declspec(dllimport) void FlashMenu(HWND hWnd);
    __declspec(dllimport) void PrepareMenuPaint(HDC hdc);
    __declspec(dllimport) LRESULT DoMenuWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    __declspec(dllimport) LRESULT DoMenuKeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);
    __declspec(dllimport) LRESULT DoMenuGetMessageHookProc(int nCode, WPARAM wParam, LPARAM lParam);
    __declspec(dllimport) void KillMenuIfNeeded();
    __declspec(dllimport) void BuildGotoMenu();
    __declspec(dllimport) void SetAreaByMenuText(int item);
    __declspec(dllimport) void SetGotoMenuEnableState();

    //__declspec(dllimport) static WinMenu* s_Self;
};


#define CUSTOM_SCENE_ID_FIRST 12000
#define CUSTOM_SCENE_ID_LAST 13999
#define GOTO_SEPARATOR_ID 11004
#define GOTO_SEPARATOR_SHADOW_X 1
#define GOTO_SEPARATOR_SHADOW_Y 3
#define GOTO_SEPARATOR_HIGHLIGHT_X 1
#define GOTO_SEPARATOR_HIGHLIGHT_Y 4
#define GOTO_ICON_X 3
#define GOTO_ICON_Y 2
#define GOTO_ICON_BORDER_SIZE 28
#define GOTO_ICON_COLUMN_WIDTH 30
#define GOTO_TEXT_X 32

class MyWinMenu : public WinMenu {
    public:
    LRESULT MyDoWMDrawItemGotoMenu(HWND hWnd, WPARAM wParam, LPARAM lParam) const;
};

LRESULT MyWinMenu::MyDoWMDrawItemGotoMenu(HWND hWnd, WPARAM wParam, LPARAM lParam) const {
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
        ? this->m_CustomScenesMenu
        : this->m_GotoMenu;
    GetMenuItemInfoA(hMenu, itemId, FALSE, &mii);

    const char* areaName = this->m_GotoAreaNames.m_Data[mii.dwItemData].m_String;

    RECT rc = dis->rcItem;

//  ID 11004: etched 3D line
    if (itemId == GOTO_SEPARATOR_ID) {
        FillRect(hdc, &rc, m_ColorMenu);

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
            FillRect(hdc, &rcIcon, m_ColorMenu);
            //BitmapBlt(hdc, rc.left + GOTO_ICON_X, rc.top + GOTO_ICON_Y,
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
            FillRect(hdc, &rc, m_ColorHighlight);

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
            FillRect(hdc, &rc, m_ColorMenu);
        };

        LONG textX = dis->rcItem.left + GOTO_TEXT_X;
        rc.left = textX;
        
//      draw text
        if (!isGrayed || isSelected){
            HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
                DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Impact"));

            DrawTextA(hdc, areaName, -1, &rc, DT_SINGLELINE | DT_VCENTER);
        }
        else {
            int textY = rc.top + (rc.bottom - this->m_CurAreaGrayedOutTextY - rc.top) / 2;
            DrawStateA(hdc, NULL, NULL, (LPARAM)areaName, strlen(areaName),
                       textX, textY, rc.right - textX, rc.bottom - rc.top,
                       DST_TEXT | DSS_DISABLED);
        };

//      when hovering the current area, a checkerboard mask is AND'd over the text portion for a subtle darkening effect
        if (isGrayed && isSelected) {
            HDC hdcMask = CreateCompatibleDC(hdc);
            HBITMAP hOldBmp = SelectBitmap(hdcMask, m_MonoDitheredMask);
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
};