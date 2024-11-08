#include "stuff.h"

bool TSD_only = false;
int sw_map_multiplier = 100;

// 0 => PPT
// 1 => TETR.IO
//			enables SRS+ and TETRIO multiplier code
//			TODO: should also disable LL/RR instructions PROPERLY in MisaMino
//				  ctrl+F for #ifdef GENERATE_LL_RR
// 2 => TETR.IO S2
//			TODO: additionally enables S2 damage calculation
// 3 => BotrisBattle
int tetris_game = 0;

bool tmini_old_behavior = false;
