#ifndef BOT_H
#define	BOT_H

#include <string>

class Bot {
public:
    Bot();
    Bot(const Bot& orig);
    virtual ~Bot();
    
    void startParser();
    void outputAction();
private:
    void updateState(const std::string & p1,const std::string & p2,const std::string & p3);
    void updateQueue(const std::string & s);
    void updateField(const std::string & s);
    
    char m_currentPiece;
    char m_hold;
    char m_queue[8];
    int *m_field;
    int m_queueLen;
    int m_combo;
};

#endif	/* BOT_H */