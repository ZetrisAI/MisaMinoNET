#ifndef DLLAI_H
#define DLLAI_H

#include "ai.h"
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>

char* result(int player, std::string r);
char* AIName( int level );

char* TetrisAI(int overfield[], int field[], int field_w, int field_h, int b2b, int combo,
               char next[], char hold, bool curCanHold, char active, int x, int y, int spin,
               bool canhold, bool can180spin, int upcomeAtt, int comboTable[], int maxDepth, int level, int player);

#endif /* DLLAI_H */

