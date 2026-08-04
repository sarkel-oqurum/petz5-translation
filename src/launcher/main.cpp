#include <windows.h>
#include <iostream>
#include <string>
#include "launcher.h"
#include "injector.h"
#include "path.h"

enum class PathMode
{
    DebugAbsolute,
    ReleaseRelative
};

static PathMode g_pathMode = PathMode::DebugAbsolute;

int main()
{
    PROCESS_INFORMATION pi = {};

    BOOL ok = LaunchPetz(pi);

    if (!ok)
        return 1;

    auto dllPath = GetExeDirectory() + "\\Chuvash.dll";
    std::cout << "DLL path: " << dllPath << '\n';
    InjectDLL(pi.hProcess, dllPath.c_str());
    if (!InjectDLL(pi.hProcess, dllPath.c_str()))
    {
        std::cout << "Injection failed: " << GetLastError() << '\n';
        return 1;
    }

    std::cout << "[main.cpp] Injection succeeded.";
    Sleep(500); // hook is losing the race sometimes and doesnt load?!
    
    ResumeThread(pi.hThread);
    
    WaitForSingleObject(pi.hThread, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    DWORD exitCode = 0;
    GetExitCodeThread(pi.hThread, &exitCode);

    std::cout <<"[main.cpp] Exit code: "<< exitCode;
    return 0;
}