#include "Bot.h"
#include "Windows.h"

using namespace std;

extern "C" __declspec(dllexport) int test();

__declspec(dllexport) int test() {
    return 123;
}

BOOL WINAPI DllMain(  
         HANDLE handle,  // handle to DLL module
         DWORD reason,     // reason for calling function
         LPVOID reserved )  // reserved
{
    // Perform actions based on the reason for calling.
    switch( reason )
    {
    case DLL_PROCESS_ATTACH:
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.            
        break;

    case DLL_THREAD_ATTACH:        
        // Do thread-specific initialization.
        break;        
   
    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.            
        break;
   
    case DLL_PROCESS_DETACH:        
        // Perform any necessary cleanup.
        break;    
    }
        return TRUE;
}

int main() {
    Bot MisaBot;
    MisaBot.setup();
    MisaBot.startParser();
    return 0;
}
