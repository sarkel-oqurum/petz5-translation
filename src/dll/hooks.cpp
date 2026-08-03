#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "createfontindirecta.h"
#include "createfonta.h"
#include "DoWMDrawItemGotoMenu.h"
//#include "getstockobject.h"
//#include "selectobject.h"
#include "fonttypes.h"

void* g_trampolineIndirectA = nullptr;
void* g_trampolineA = nullptr;
//void* g_trampolineStockObjectA = nullptr;
void* g_trampolineSelectObjectA = nullptr;

void* HookFunction(void* targetAddress, void* hookDestination, size_t hookSize)
{
    if (!targetAddress || !hookDestination || hookSize < 5)
        return nullptr;

    // trampoline = original bytes + JMP back
    void* trampoline = VirtualAlloc(
        nullptr,
        hookSize + 5,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE);

    if (!trampoline)
        return nullptr;

    // copy original bytes
    memcpy(trampoline, targetAddress, hookSize);

    BYTE* trampBytes = (BYTE*)trampoline;

    // JMP back
    trampBytes[hookSize] = 0xE9;

    DWORD jumpBackOffset =
        ((DWORD)targetAddress + hookSize)
        - ((DWORD)trampoline + hookSize)
        - 5;

    *(DWORD*)(trampBytes + hookSize + 1) = jumpBackOffset;

    DWORD oldProtect;
    VirtualProtect(targetAddress, hookSize, PAGE_EXECUTE_READWRITE, &oldProtect);

    BYTE* patch = new BYTE[hookSize];
    memset(patch, 0x90, hookSize);   // fill with NOPs

    patch[0] = 0xE9;

    DWORD jumpToHookOffset =
        (DWORD)hookDestination
        - (DWORD)targetAddress
        - 5;

    *(DWORD*)(patch + 1) = jumpToHookOffset;

    memcpy(targetAddress, patch, hookSize);

    delete[] patch;

    VirtualProtect(targetAddress, hookSize, oldProtect, &oldProtect);

    return trampoline;
}

bool InstallFontHook()
{
    // 1. Hook CreateFontIndirectA
    void* targetIndirectA = (void*)FindCreateFontIndirectA();
    if (targetIndirectA) {
        RealCreateFontIndirectA =
            (CreateFontIndirectA_t)HookFunction(
            targetIndirectA,
            (void*)MyCreateFontIndirectA,
            5);
    }

    // 2. Hook CreateFontA
    void* targetFontA = (void*)FindCreateFontA();
    if (targetFontA) {
        RealCreateFontA =
            (CreateFontA_t)HookFunction(
            targetFontA,
            (void*)MyCreateFontA,
            5);
    }

    //void* targetStock = FindGetStockObject();
    //if (targetStock)
    //{
    //    RealGetStockObject =
    //        (GetStockObject_t)HookFunction(targetStock, (void*)MyGetStockObject);
    //}

    void* targetMenu = FindDoWMDrawItemGotoMenu();
    if (targetMenu)
    {
        RealDoWMDrawItemGotoMenu =
            (DoWMDrawItemGotoMenu_t)HookFunction(
            targetMenu,
            (void*)MyDoWMDrawItemGotoMenu,
            8); 
    }
    //std::cout << "GetLastError:\n" << RealDrawTextA;
    // Verify all hooks initialized successfully
    return (RealCreateFontIndirectA != nullptr) && 
           (RealCreateFontA != nullptr) && 
           //(RealGetStockObject != nullptr) &&
           (RealDoWMDrawItemGotoMenu != nullptr);
}
