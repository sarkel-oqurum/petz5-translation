#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "createfontindirecta.h"
#include "createfonta.h"
//#include "drawtexta.h"
#include "DoWMDrawItemGotoMenu.h"
#include "fonttypes.h"

void* g_trampolineIndirectA = nullptr;
void* g_trampolineA = nullptr;
void* g_trampolineDrawA = nullptr;
void* g_trampolineMenuA = nullptr;
void* g_trampolineWMDrawA = nullptr;

void* HookFunction(void* targetAddress, void* hookDestination) {
    if (!targetAddress|| !hookDestination) return nullptr;

    // what this does
    void* trampoline = VirtualAlloc(nullptr, 10, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!trampoline) return nullptr;

    // copy original 5 bytes to trampoline
    memcpy(trampoline, targetAddress, 5);

    BYTE* trampBytes = (BYTE*)trampoline;
    trampBytes[5] = 0xE9; // JMP opcode

    DWORD jumpBackOffset = ((DWORD)targetAddress + 5) - ((DWORD)trampoline + 5) - 5;
    *(DWORD*)(trampBytes + 6) = jumpBackOffset;

    // patch  the target function
    DWORD oldProtect;
    VirtualProtect(targetAddress, 5, PAGE_EXECUTE_READWRITE, &oldProtect);

    BYTE patch[5];
    patch[0] = 0xE9; // JMP opcode

    DWORD jumpToHookOffset = (DWORD)hookDestination - (DWORD)targetAddress - 5;
    *(DWORD*)(patch + 1) = jumpToHookOffset;

    memcpy(targetAddress, patch, 5);
    VirtualProtect(targetAddress, 5, oldProtect, &oldProtect);

    return trampoline;
}


bool InstallFontHook()
{
    // 1. Hook CreateFontIndirectA
    void* targetIndirectA = (void*)FindCreateFontIndirectA();
    if (targetIndirectA) {
        RealCreateFontIndirectA = (CreateFontIndirectA_t)HookFunction(targetIndirectA, (void*)MyCreateFontIndirectA);
    }

    // 2. Hook CreateFontA
    void* targetFontA = (void*)FindCreateFontA();
    if (targetFontA) {
        RealCreateFontA = (CreateFontA_t)HookFunction(targetFontA, (void*)MyCreateFontA);
    }

    // 3. Hook DrawItemGotoMenu
    void* targetDrawA = (void*)FindDoWMDrawItemGotoMenu();
    if (targetDrawA) {
        RealDoWMDrawItemGotoMenu = (DoWMDrawItemGotoMenu_t)HookFunction(targetDrawA, (void*)MyDoWMDrawItemGotoMenu);
    }
    //std::cout << "GetLastError:\n" << RealDrawTextA;
    // Verify all hooks initialized successfully
    return (RealCreateFontIndirectA != nullptr) && 
           (RealCreateFontA != nullptr) && 
           (RealDoWMDrawItemGotoMenu != nullptr);
}
