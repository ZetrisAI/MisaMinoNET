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
    std::string outputAction(char* str, int len);
    void updateLevel(int level);
    void updateStyle(int style);
    void updateC4W(int c4w);
    void updateQueue(const char* s);
    void updateCurrent(const char* s);
    void updateHold(const char* s);
    void updateIncoming(int attack);
    void updateCombo(int combo);
    void updateField(const char* s);
    void updateReset();
    
    TetrisGame tetris;
    
    struct tetris_ai {
        int style;
        int level;
        int c4w;
        tetris_ai() {
            style = 1;
            level = 10;
            c4w = 0;
        }
    };

    struct tetris_rule {
        int spin180;
        int GarbageCancel;
        int GarbageBuffer;
        int GarbageBlocking;
        int combo_table_style;
        tetris_rule() {
            spin180 = 0;
            GarbageCancel = 1;
            GarbageBuffer = 0;
            GarbageBlocking = 0;
            combo_table_style = 3; //1=TOJ, 2=TF, 3=PPT
        }
    };
    
private:
    void processMoves();
    
    char m_hold;
    char m_queue[8];
    int *m_field;
    int m_queueLen;
    int m_upcomeAtt;
    std::map<char, int> m_gemMap;
    
    tetris_rule rule;
    tetris_ai ai;
};

#endif	/* BOT_H */