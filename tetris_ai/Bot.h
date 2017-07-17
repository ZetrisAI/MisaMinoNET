#ifndef BOT_H
#define	BOT_H

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include "tetrisgame.h"

class Bot {
public:
    Bot();
    Bot(const Bot& orig);
    virtual ~Bot();
    
    void startParser();
    void setup();
    void outputAction();
    
    struct tetris_ai {
        int style;
        int level;
        int PieceMul;
        tetris_ai() {
            style = 2;
            level = 4;
            PieceMul = 0;
        }
    };

    struct tetris_rule {
        int turnbase;
        int garbage;
        int spin180;
        int GarbageCancel;
        int GarbageBuffer;
        int GarbageBlocking;
        int combo_table_style;
        int samesequence;
        int turn;
        tetris_rule() {
            turnbase = 1;
            garbage = 0;
            spin180 = 0;
            GarbageCancel = 1;
            GarbageBuffer = 1;
            GarbageBlocking = 1;
            combo_table_style = 0;
            samesequence = 1;
            turn = 1;
        }
    };
    
private:
    void updateState(const std::string & p1,const std::string & p2,const std::string & p3);
    void updateQueue(const std::string & s);
    void updateField(const std::string & s);
    
    void processMoves();
    
    char m_hold;
    char m_queue[8];
    int *m_field;
    int m_queueLen;
    int m_combo;
    std::map<char, int> m_gemMap;
    
    TetrisGame tetris;
    tetris_rule rule;
    tetris_ai ai;
};

#endif	/* BOT_H */