#ifndef BOT_H
#define	BOT_H

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include "lastnodes.h"
#include "tetrisgame.h"

class Bot {
public:
    Bot();
    Bot(const Bot& orig);
    virtual ~Bot();
    
    //void startParser();
    void setup();
    std::string outputAction(char* str, int len);
    void updateStyle(AI::AI_Param param);
	void updateHoldAllowed(bool holdAllowed);
	void updateAllSpin(bool allSpin);
    void updateQueue(const char* s);
    void updateCurrent(const char* s);
    void updateHold(const char* s);
    void updateIncoming(int attack);
    void updateCombo(int combo);
	void updateB2B(int b2b);
    void updateField(const char* s);
    void updateReset();
    
    TetrisGame tetris;

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
};

#endif	/* BOT_H */