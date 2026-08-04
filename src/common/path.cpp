#include <windows.h>
#include <string>
#include "path.h"

std::string GetExeDirectory()
{
    char path[MAX_PATH]; // Windows API variable

    GetModuleFileNameA( // gets working directory path
        NULL,
        path,
        MAX_PATH
    );

    std::string fullPath(path);

    size_t pos = fullPath.find_last_of("\\/");
    if (pos == std::string::npos)
        return "";

    return fullPath.substr(0, pos);
}