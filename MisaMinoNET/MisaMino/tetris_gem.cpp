#include "tetris_gem.h"

namespace AI {
    Gem gems[8][4];
    int GEM_COL_H[8][4][4];
    int GEM_MAXH[8][4];

    class init_obj {
    public:
        init_obj() {
            Gem _gems[8][4] = {
                {
                    { 0, 0, 0, 0},
                    { 0, 0, 0, 0},
                    { 0, 0, 0, 0},
                    { 0, 0, 0, 0},
                },
                { // I 1
                    { 0,15, 0, 0},
                    { 2, 2, 2, 2},
                    { 0, 0,15, 0},
                    { 4, 4, 4, 4},
                },
                { // T 2
                    { 2, 7, 0, 0},
                    { 2, 3, 2, 0},
                    { 0, 7, 2, 0},
                    { 2, 6, 2, 0},
                },
                { // L 3
                    { 1, 7, 0, 0},
                    { 2, 2, 3, 0},
                    { 0, 7, 4, 0},
                    { 6, 2, 2, 0},
                },
                { // J 4
                    { 4, 7, 0, 0},
                    { 3, 2, 2, 0},
                    { 0, 7, 1, 0},
                    { 2, 2, 6, 0},
                },
                { // Z 5
                    { 6, 3, 0, 0},
                    { 1, 3, 2, 0},
                    { 0, 6, 3, 0},
                    { 2, 6, 4, 0},
                },
                { // S 6
                    { 3, 6, 0, 0},
                    { 2, 3, 1, 0},
                    { 0, 3, 6, 0},
                    { 4, 6, 2, 0},
                },
                { // O 7
                    { 6, 6, 0, 0},
                    { 6, 6, 0, 0},
                    { 6, 6, 0, 0},
                    { 6, 6, 0, 0},
                },
            };
            int m[] = {1, 4, 4, 4, 4, 4, 4, 1}; // TOP SRS
            for ( int n = 0; n < 8; ++n) {
                for ( int s = 0; s < 4; ++s) {
                    gems[n][s] = _gems[n][s];
                    gems[n][s].num = n;
                    gems[n][s].mod = m[n];
                    gems[n][s].spin = s % m[n];
                    //GEM_COL_H Initialization
                    GEM_MAXH[n][s] = 0;
                    for ( int x = 0; x < 4; ++x) {
                        GEM_COL_H[n][s][x] = 0;
                        for ( int y = 0; y < 4; ++y) {
                            if(gems[n][s].bitmap[y] & (1 << x)) {
                                GEM_COL_H[n][s][x] = y + 1;
                                if ( GEM_MAXH[n][s] < y ) GEM_MAXH[n][s] = y;
                                //I vertical: 0 4 0 0
                            }
                        }
                    }
                }
            }
        }
    } _o;
}
