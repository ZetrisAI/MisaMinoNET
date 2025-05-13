#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "tetris.h"
#include "tetris_ai.h"

class TetrisGame : public AI::Tetris {
public:
    TetrisGame() {
        hold = true;
        ai_movs_flag = -1;
        reset();
        AI::AI_Param param = {
            26, 0, 20, 17, 45, 41, -8, 9, 10, 27, 15, 20, 19, 11, 44, 4, 300
        };
        m_ai_param = param;
    }
    void reset () {
        AI::Tetris::reset(10, 20);
        ai_movs.movs.clear();
        ai_last_deep = 0;
        ai_movs_flag = -1;
    }
    bool tryXMove(int dx) {
        bool ret = Tetris::tryXMove( dx );
        return ret;
    }
    bool tryXXMove(int dx) {
        bool ret = Tetris::tryXMove( dx );
        while ( Tetris::tryXMove(dx) ) ;
        return ret;
    }
    bool tryYMove(int dy) {
        bool ret = Tetris::tryYMove( dy );
        return ret;
    }
    bool tryYYMove(int dy) {
        bool ret = Tetris::tryYMove( dy );
        while ( Tetris::tryYMove(dy) ) ;
        return ret;
    }
    bool trySpin(int dSpin) {
        bool ret = Tetris::trySpin( dSpin );
        return ret;
    }
    bool trySpin180() {
        bool ret = Tetris::trySpin180( );
        return ret;
    }
    bool tryHold() {
        bool ret = Tetris::tryHold( );
        return ret;
    }
    bool drop() {
        bool ret = Tetris::drop( );
        return ret;
    }

public:
    bool hold;
    AI::Moving ai_movs;
    int ai_movs_flag;
    int ai_last_deep;
    AI::AI_Param m_ai_param;
};
