#pragma once
#include <windows.h>

bool InjectDLL(HANDLE hProcess, const char* dllPath);