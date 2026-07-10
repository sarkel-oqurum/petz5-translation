#include <windows.h>
#include <iostream>
#include <string>
#include "launcher.h"
#include "injector.h"

std::string ResolveGamePath(const std::string& file);
std::string exePath = "C:\\Games\\Petz 5";

enum class PathMode
{
    DebugAbsolute,
    ReleaseRelative
};

static PathMode g_pathMode = PathMode::DebugAbsolute;

int main()
{
    PROCESS_INFORMATION pi = {};

    //auto dllPath = ResolveGamePath("font\\256Chuvash_test.ttf");
    BOOL ok = LaunchPetz(pi);

    if (!ok)
        return 1;

    auto dllPath = "C:\\Users\\Lighty\\Documents\\projects\\petzChuvash\\dll.dll";

    InjectDLL(pi.hProcess, dllPath);
    if (!InjectDLL(pi.hProcess, dllPath))
    {
        std::cout << "Injection failed: " << GetLastError() << '\n';
        return 1;
    }

    std::cout << "Injection succeeded.\n" << GetLastError();
    
    ResumeThread(pi.hThread);
    
    WaitForSingleObject(pi.hThread, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    DWORD exitCode = 0;
    GetExitCodeThread(pi.hThread, &exitCode);

    std::cout <<"ok "<< exitCode;
    return 0;
}

std::string ResolveGamePath(const std::string& file)
{
    if (g_pathMode == PathMode::DebugAbsolute)
    {
        return "C:\\Games\\Petz5\\" + file;
    }
    else
    {
        return file; // relative to working directory
    }
}