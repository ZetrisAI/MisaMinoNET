#include "Bot.h"
#include "Windows.h"
#define DLL extern "C" __declspec(dllexport)
using namespace std;

Bot MisaBot;

DLL void settings_level(int level) {
    MisaBot.updateLevel(level);
}
DLL void settings_style(int style) {
    MisaBot.updateStyle(style);
};

DLL void update_next(const std::string& queue);
DLL void update_current(const std::string& piece);
DLL void update_incoming(int attack);
DLL void update_combo(int combo);
DLL void update_field(const std::string& field);
DLL void update_reset();

DLL void action();

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