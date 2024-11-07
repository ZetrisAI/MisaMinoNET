#ifndef STUFF_H
#define STUFF_H

extern bool TSD_only;
extern int sw_map_multiplier;
extern int tetris_game;
extern bool tmini_old_behavior;

inline bool tetris_game_is_PPT() {
	return tetris_game == 0;
}

inline bool tetris_game_is_TETRIO_S1() {
	return tetris_game == 1;
}

inline bool tetris_game_is_TETRIO_S2() {
	return tetris_game == 2;
}

inline bool tetris_game_is_TETRIO() {
	return tetris_game_is_TETRIO_S1() || tetris_game_is_TETRIO_S2();
}

inline bool tetris_game_is_BotrisBattle() {
	return tetris_game == 3;
}
#endif