#include <windows.h>
#include <string.h>
#include <stdio.h>

typedef BOOL (WINAPI* InsertMenuItemA_t)(
    HMENU, int, BOOL, LPCMENUITEMINFOA 
);

InsertMenuItemA_t RealInsertMenuItemA = nullptr;

InsertMenuItemA_t FindInsertMenuItemA()
{
    HMODULE user32 = GetModuleHandleA("user32.dll");

    if (!user32)
    {
        MessageBoxA(
            nullptr,
            "did not find user32",
            "Petz",
            MB_OK
        );
        return nullptr;
    }

    return (InsertMenuItemA_t)GetProcAddress(
        user32,
        "InsertMenuItemA"
    );
}

BOOL WINAPI MyInsertMenuItemA(HMENU hMenu, UINT uItem, BOOL fByPosition, LPCMENUITEMINFOA lpmii)
{
    // Check if the structure pointer exists and if it contains a string flag
   // if (lpmii && (lpmii->fMask & MIIM_STRING) && lpmii->dwTypeData)
    //{
        char msgBuf[512];
        
        // lpmii->dwTypeData holds the raw string pointer to the room name (e.g. "Family Room")      
    sprintf_s(
        msgBuf,
        sizeof(msgBuf),
        "fMask = %08X\n"
        "wID = %u\n"
        "dwItemData = %p\n"
        "dwTypeData = %s",
        lpmii->fMask,
        lpmii->wID,
        (void*)lpmii->dwItemData,
        lpmii->dwTypeData ? lpmii->dwTypeData : "(null)"
    );

        MessageBoxA(nullptr, msgBuf, "Captured Room Name!", MB_OK);
   // }

    // Call the original game function so it doesn't freeze or crash
    return RealInsertMenuItemA(hMenu, uItem, fByPosition, lpmii);
}