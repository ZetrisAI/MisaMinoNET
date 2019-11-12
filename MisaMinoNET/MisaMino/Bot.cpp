#include "Bot.h"
#include <cassert>
#include <vector>

using namespace std;

Bot::Bot():m_hold(' '){
    m_gemMap[' '] = AI::GEMTYPE_NULL;
    m_gemMap['I'] = AI::GEMTYPE_I;
    m_gemMap['T'] = AI::GEMTYPE_T;
    m_gemMap['L'] = AI::GEMTYPE_L;
    m_gemMap['J'] = AI::GEMTYPE_J;
    m_gemMap['Z'] = AI::GEMTYPE_Z;
    m_gemMap['S'] = AI::GEMTYPE_S;
    m_gemMap['O'] = AI::GEMTYPE_O;
}

Bot::Bot(const Bot& orig) {
}

Bot::~Bot() {}

void Bot::updateField(const char* q) {
    std::string s = q;
    vector<int> rows;
    bool solidGarbage = false;
    int row = 0;
    int col = 0;
    for (const auto &c : s) {
        /* Read comma separated into integer, bitwise. 2 = filled. 1 is useless...? */
        switch (c) {
        case '0':
        case '1':
            ++col;
            break;
        case '2':
            ++col;
            row |= (1 << (10 - col));
            break;
        case '3':
            solidGarbage = true;
            break;
        default:
            break;
        }
        if (solidGarbage) {
            break;
        }
        if (col == 10) {
            rows.push_back(row);
            row = 0;
            col = 0;
        }
    }
    tetris.m_pool.reset(10, rows.size());
    for (auto &row : rows) {
        tetris.addRow(row);
    }
}

void Bot::updateQueue(const char* q) {
    std::string s = q;
    int i=1;
    for(const auto &c : s){
        if(c==',')continue;
        tetris.m_next[i++] = AI::getGem(m_gemMap[c], 0);
    }
    tetris.m_next_num=i;
    tetris.newpiece();
}

AI::AI_Param globalparam = {
	16, 9, 11, 17, 17, 25, 39, 2, 12, 19, 7, 24, 18, 7, 14, 19, 25, 30, 18, 19, 0 // 4w default = 200
};

void Bot::updateStyle(AI::AI_Param param) {
	globalparam = param;
}

bool holdallow = true;

void Bot::updateHoldAllowed(bool holdAllowed) {
	holdallow = holdAllowed;
}

bool allspin = true;

void Bot::updateAllSpin(bool allSpin) {
	allspin = allSpin;
}

void Bot::updateCurrent(const char* piece) {
    std::string s = piece;
    tetris.m_cur = AI::getGem(m_gemMap[s[0]], 0);
}

void Bot::updateHold(const char* piece) {
    std::string s = piece;
    tetris.m_pool.m_hold = m_gemMap[s[0]];
}

void Bot::updateIncoming(int attack) {
    m_upcomeAtt = attack;
}

void Bot::updateCombo(int combo) {
    tetris.m_pool.combo = combo;
}

void Bot::updateB2B(int b2b) {
	tetris.m_pool.b2b = b2b;
}

void Bot::updateReset() {
    tetris.reset(0);
    m_upcomeAtt = 0;
}

void Bot::setup() {
	tetris.m_ai_param = globalparam;
	tetris.hold = holdallow;
	AI::setAIsettings(0, "4w", tetris.m_ai_param.strategy_4w > 0? 1 : 0);

    if (rule.combo_table_style == 0) {
        int a[] = {0, 0, 0, 1, 1, 2};
        AI::setComboList(std::vector<int>(a, a + sizeof (a) / sizeof (*a)));
        tetris.m_ai_param.strategy_4w = 0;
        AI::setAIsettings(0, "4w", 0);
    } else if (rule.combo_table_style == 1) {
        int a[] = {0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4, 5};
        AI::setComboList(std::vector<int>(a, a + sizeof (a) / sizeof (*a)));
    } else if (rule.combo_table_style == 2) {
        int a[] = {0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 4, 5};
        AI::setComboList(std::vector<int>(a, a + sizeof (a) / sizeof (*a)));
    } else if (rule.combo_table_style == 3) {
        int a[] = {0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4, 5};
        AI::setComboList(std::vector<int>(a, a + sizeof (a) / sizeof (*a)));
    } else {
        int a[] = {0, 0, 0, 1, 1, 2};
        AI::setComboList(std::vector<int>(a, a + sizeof (a) / sizeof (*a)));
    }

	AI::setAllSpin(allspin);

    tetris.m_base = AI::point(0, 30);
    tetris.env_change = 1;
    tetris.n_pieces += 1;
    tetris.total_clears += tetris.m_clearLines;
    tetris.m_clearLines = 0;
    tetris.m_attack = 0;
}

void Bot::processMoves() {
    tetris.m_state = AI::Tetris::STATE_MOVING;
    while ( tetris.ai_movs_flag == -1 && !tetris.ai_movs.movs.empty() ){
        int mov = tetris.ai_movs.movs[0];
        tetris.ai_movs.movs.erase( tetris.ai_movs.movs.begin() );
        if (mov == AI::Moving::MOV_L) tetris.tryXMove(-1);
        else if (mov == AI::Moving::MOV_R) tetris.tryXMove( 1);
        else if (mov == AI::Moving::MOV_D) tetris.tryYMove( 1);
        else if (mov == AI::Moving::MOV_LSPIN) tetris.trySpin(1);
        else if (mov == AI::Moving::MOV_RSPIN) tetris.trySpin(3);
        else if (mov == AI::Moving::MOV_LL) { tetris.tryXXMove(-1); } //{ tetris.mov_llrr = AI::Moving::MOV_LL; }
        else if (mov == AI::Moving::MOV_RR) { tetris.tryXXMove( 1); } //{ tetris.mov_llrr = AI::Moving::MOV_RR; }
        else if (mov == AI::Moving::MOV_DD) tetris.tryYYMove( 1) ;
        else if (mov == AI::Moving::MOV_DROP) tetris.drop();
        else if (mov == AI::Moving::MOV_HOLD) {
            tetris.tryHold();
        } else if (mov == AI::Moving::MOV_SPIN2) {
            if ( AI::spin180Enable() ) {
                tetris.trySpin180();
            }
        } else if (mov == AI::Moving::MOV_REFRESH) {
            tetris.env_change = 1;
        }
    }
    tetris.clearLines();
}

std::string Bot::outputAction(char* str, int len) {
    std::vector<AI::Gem> next;
    for (int j = 0; j < tetris.m_next_num; ++j)
        next.push_back(tetris.m_next[j]);
    int deep = tetris.m_next_num + 1;
    bool canhold = tetris.hold;

    #if DEBUG_LEVEL>=5
    cerr << "[debug] RunAI: movs:" << tetris.ai_movs.movs.size() << ", flag:" << tetris.ai_movs_flag << ", combo:" <<tetris.m_pool.combo <<endl;
    cerr << "hold:" << tetris.m_pool.m_hold << ", cur:"<< tetris.m_cur.getLetter() << " x:"<< tetris.m_cur_x<< " y:"<<tetris.m_cur_y << endl << "Next:";
    for(auto &n : next){
        cerr << " " << n.getLetter();
    }
    cerr<< " " <<((canhold)?"canhold":"NOThold")<<", Deep:"<<deep<<",last:"<<tetris.ai_last_deep<<",level:"<<ai.level<<endl;
    #endif
    
    AI::Gem piece = AI::RunAI(tetris.ai_movs, tetris.ai_movs_flag, tetris.m_ai_param, tetris.m_pool, tetris.m_hold,
            tetris.m_cur,
            tetris.m_cur_x, tetris.m_cur_y, next, canhold, m_upcomeAtt,
            deep, tetris.ai_last_deep);

    std::stringstream out;
    
    if(/*tetris.alive() && */1 <= piece.num && piece.num <= 7){
        for (int i = 0; i < tetris.ai_movs.movs.size(); i++) {
            out << tetris.ai_movs.movs[i] << ((i == tetris.ai_movs.movs.size() - 1)? "|" : ",");
        }
        
        out << piece.num << "|";
        
        processMoves();
        out << ((int)tetris.wallkick_spin) << "|" << tetris.m_pool.b2b << "|" << last_nodes << "|" << last_depth << "|" << tetris.m_attack << "|";
        
        out << tetris.m_cur_x << "," << tetris.m_cur_y;
        
        
        tetris.m_state = AI::Tetris::STATE_READY;
    }else{
       out << "-1";
    }
    
    return out.str();
}
