#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "createfontindirecta.h"
#include "fonttypes.h"

void* g_trampoline = nullptr;

//CreateFontIndirectA_t RealCreateFontIndirectA = nullptr;

bool InstallFontHook()
{
    CreateFontIndirectA_t target = FindCreateFontIndirectA();

    if (!target)
        return false;


    BYTE original[5];

    memcpy(
        original,
        target,
        5
    );


    //
    // Create trampoline
    //

    g_trampoline = VirtualAlloc(
        nullptr,
        10,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE
    );

    if (!g_trampoline)
        return false;


    // copy original bytes

    memcpy(
        g_trampoline,
        original,
        5
    );


    // jump back

    BYTE* trampolineBytes =
        (BYTE*)g_trampoline;

    trampolineBytes[5] = 0xE9;


    DWORD jumpBack =
        ((DWORD)target + 5)
        - ((DWORD)g_trampoline + 5)
        - 5;


    *(DWORD*)(trampolineBytes + 6) =
        jumpBack;



    RealCreateFontIndirectA =
        (CreateFontIndirectA_t)g_trampoline;



    //
    // Patch original function
    //

    DWORD oldProtect;

    VirtualProtect(
        target,
        5,
        PAGE_EXECUTE_READWRITE,
        &oldProtect
    );


    BYTE patch[5];

    patch[0] = 0xE9;


    DWORD jump =
        (DWORD)MyCreateFontIndirectA
        - (DWORD)target
        - 5;


    *(DWORD*)(patch + 1) = jump;


    memcpy(
        target,
        patch,
        5
    );


    VirtualProtect(
        target,
        5,
        oldProtect,
        &oldProtect
    );


    return true;
}