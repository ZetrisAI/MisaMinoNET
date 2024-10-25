#include "stuff.h"

bool TSD_only = false;
int sw_map_multiplier = 100;

// 0 => PPT
// 1 => TETR.IO
//			enables SRS+, disables LL/RR instructions and enables TETRIO multiplier code
// 2 => TETR.IO S2
//			TODO: additionally enables S2 damage calculation
int tetris_game = 0;

bool tmini_old_behavior = false;
