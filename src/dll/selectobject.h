#pragma once

#include <windows.h>

typedef HGDIOBJ (WINAPI* SelectObject_t)(
    HDC hdc,
    HGDIOBJ h
);

extern SelectObject_t RealSelectObject;

bool HookSelectObject();

SelectObject_t FindSelectObject();

HGDIOBJ WINAPI MySelectObject(HDC hdc, HGDIOBJ h);