#include "Bot.h"
#include "Windows.h"
#define DLL extern "C" __declspec(dllexport)
using namespace std;

Bot MisaBot;



BOOL WINAPI DllMain(HANDLE handle, DWORD reason, LPVOID reserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            MisaBot.setup();     
            // MisaBot.startParser();
            break;

        case DLL_THREAD_ATTACH:
            break;        

        case DLL_THREAD_DETACH:       
            break;

        case DLL_PROCESS_DETACH:
            break;    
    }
    
    return TRUE;
}