#include <iostream>
#include "Bot.h"
#include "dllai.h"

using namespace std;

Bot::Bot():m_hold(' '){
    m_field=new int[21];
}

Bot::Bot(const Bot& orig) {
}

Bot::~Bot() {
    delete m_field;
}

void Bot::startParser() {
    while (true) {
        string command;
        cin >> command;
        if (command == "settings") {
            string part1, part2;
            cin >> part1 >> part2;
            //those are ignored
        } else if (command == "update") {
            string part1, part2, part3;
            cin >> part1 >> part2 >> part3;
            updateState(part1, part2, part3);
        } else if (command == "action2") {
            string part1, part2;
            cin >> part1 >> part2;
            outputAction();
        } else if (command.size() == 0) {
            // no more commands, exit.
            break;
        } else {
            cerr << "Unable to parse command: " << command << endl;
        }
    }
}

void Bot::updateField(const std::string& s) {
    m_field[0]=0;
    int y=1;
    int x=9;
    int r=0;
    for(const auto &c : s){
        if(c==',')continue;
        if(c==';'){
            m_field[y]=r;
            y++;
            r=0;
            x=9;
            continue;
        }
        if(c=='2'){
            r|=(1<<x);
        }
        x--;
    }
    m_field[y]=r;  //because the last semicilon is ommited
}

void Bot::updateQueue(const std::string& s) {
    int i=0;
    for(const auto &c : s){
        if(c==',')continue;
        m_queue[i]=c;
        i++;
    }
    m_queueLen=i;
}

void Bot::updateState(const std::string& p1, const std::string& p2, const std::string& p3) {
    if(p2=="this_piece_type")
        m_currentPiece=p3.at(0);
    else if(p2=="next_pieces")
        updateQueue(p3);
    else if(p2=="combo")
        m_combo=std::atoi(p3.c_str());
    else if(p2=="field")
        updateField(p3);
}

void Bot::outputAction() {
    int overfield[10]={0,0,0,0,0,0,0,0,0,0};
    int comboTable[14]={0,0,1,1,1,2,2,3,3,4,4,4,5,-1};
    char * res=TetrisAI(overfield,m_field,10,20,0,m_combo,m_queue,m_hold,true,m_currentPiece,3,0,0,true,true,0,comboTable,m_queueLen,5,0);
    int i=0;
    while(res[i]){
        if(res[i]=='v'){ //HOLD was used
            m_hold=m_currentPiece;
        }
        i++;
    }
}
