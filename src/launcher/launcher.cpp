#include "injector.h"
#include "path.h"

BOOL LaunchPetz(PROCESS_INFORMATION& pi) { 
    // start the program up
    STARTUPINFOA si = {};
    si.cb = sizeof(si);

    BOOL ok = CreateProcess( "C:\\Games\\Petz 5\\Petz 5.exe",   // the path
    NULL,               // Command line
    NULL,               // Process handle not inheritable
    NULL,               // Thread handle not inheritable
    FALSE,              // Set handle inheritance to FALSE
    CREATE_SUSPENDED,   // Create suspended so we can inject DLL
    NULL,               // Use parent's environment block
    NULL,               // Use parent's starting directory 
    &si,                // required by CreateProcess (window/startup config)
    &pi                 // receives process + thread handles from Windows
    );
    
    return ok;
}
