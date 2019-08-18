#ifndef CALLBACK_H
#define CALLBACK_H

typedef int(__stdcall * Callback)();

extern Callback Abort;

#endif