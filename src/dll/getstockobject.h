#pragma once

#include <windows.h>

typedef HGDIOBJ (WINAPI* GetStockObject_t)(int fnObject);

extern GetStockObject_t RealGetStockObject;

GetStockObject_t FindGetStockObject();

HGDIOBJ WINAPI MyGetStockObject(int fnObject);