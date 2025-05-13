#ifndef BOT_H
#define	BOT_H

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include "lastnodes.h"
#include "tetrisgame.h"

extern std::map<char, int> m_gemMap;

class Bot {
public:
    Bot();
    Bot(const Bot& orig);
    virtual ~Bot();
    
    void setup();
    std::string outputAction(bool second_choice, char* str, int len);
    void updateStyle(AI::AI_Param param);
	void updateHoldAllowed(bool holdAllowed);
	void updateAllowedSpins(int allowedSpins);
    void updateQueue(const char* s);
    void updateCurrent(const char* s);
    void updateHold(const char* s);
    void updateIncoming(int attack);
    void updateCombo(int combo);
	void updateB2B(int b2b);
    void updateField(const char* s);
    void updateReset();
    
    TetrisGame tetris;
    
private:
    void processMoves();
    
    int m_upcomeAtt;
};

#endif	/* BOT_H */
