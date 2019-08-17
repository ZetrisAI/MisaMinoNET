#ifndef MAIN_H
#define MAIN_H

#include "Bot.h"
#include "Windows.h"
#define DLL extern "C" __declspec(dllexport)

typedef int(__stdcall * Callback)();

extern Callback Abort;

#endif