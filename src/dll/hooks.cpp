#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "createfontindirecta.h"
#include "createfonta.h"
#include "fonttypes.h"

void* g_trampolineIndirectA = nullptr;
void* g_trampolineA = nullptr;

//CreateFontIndirectA_t RealCreateFontIndirectA = nullptr;

bool FontHookIndirectA()
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

    g_trampolineIndirectA = VirtualAlloc(
        nullptr,
        10,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE
    );

    if (!g_trampolineIndirectA)
        return false;


    // copy original bytes

    memcpy(
        g_trampolineIndirectA,
        original,
        5
    );


    // jump back

    BYTE* trampolineBytes =
        (BYTE*)g_trampolineIndirectA;

    trampolineBytes[5] = 0xE9;


    DWORD jumpBack =
        ((DWORD)target + 5)
        - ((DWORD)g_trampolineIndirectA + 5)
        - 5;


    *(DWORD*)(trampolineBytes + 6) =
        jumpBack;



    RealCreateFontIndirectA =
        (CreateFontIndirectA_t)g_trampolineIndirectA;



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

bool FontHookA()
{
    CreateFontA_t target = FindCreateFontA();

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

    g_trampolineA = VirtualAlloc(
        nullptr,
        10,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE
    );

    if (!g_trampolineA)
        return false;


    // copy original bytes

    memcpy(
        g_trampolineA,
        original,
        5
    );


    // jump back

    BYTE* trampolineBytes =
        (BYTE*)g_trampolineA;

    trampolineBytes[5] = 0xE9;


    DWORD jumpBack =
        ((DWORD)target + 5)
        - ((DWORD)g_trampolineA + 5)
        - 5;


    *(DWORD*)(trampolineBytes + 6) =
        jumpBack;



    RealCreateFontA =
        (CreateFontA_t)g_trampolineA;

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
        (DWORD)MyCreateFontA
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

bool InstallFontHook()
{
    bool a = FontHookIndirectA();
    bool b = FontHookA();

    return a && b;
}