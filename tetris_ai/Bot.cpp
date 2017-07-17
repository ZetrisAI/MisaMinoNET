#include "Bot.h"

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
    tetris.m_pool.reset(10,20);
    int y=1;
    int x=9;
    int r=0;
    for(const auto &c : s){
        if(c==',')continue;
        if(c==';'){
            tetris.addRow(r);
            //tetris.m_pool.m_row[ AI::gem_add_y + y ]=r;
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
    //tetris.m_pool.m_row[ AI::gem_add_y + y ]=r;
    tetris.addRow(r);
    //cerr << "[debug] pool:" << tetris.m_pool << endl;
}

void Bot::updateQueue(const std::string& s) {
    int i=1;
    for(const auto &c : s){
        if(c==',')continue;
        tetris.m_next[i++] = AI::getGem(m_gemMap[c], 0);
    }
    m_queueLen=i;
    tetris.newpiece();
}

void Bot::updateState(const std::string& p1, const std::string& p2, const std::string& p3) {
    if(p2=="next_pieces")
        updateQueue(p3);
    else if(p2=="combo")
        m_combo=std::atoi(p3.c_str());
    else if(p2=="field")
        updateField(p3);
    else if(p2=="this_piece_type")
        tetris.m_next[0]=AI::getGem(m_gemMap[p3[0]], 0);
    else if(p2=="round" && p3=="1")
        tetris.reset(0);
}

//todo: hold
//tetris.m_pool.m_hold = gemMap['L'];

void Bot::setup() {
    int ai_4w = 1;

    ai.style = 2;
    ai.level = 10;
    ai.PieceMul = 1;
    
    EvE eve;
    eve.ai.push_back(tetris.m_ai_param);

    int ai_eve = 1;

    AI::AI_Param param = {
        //  47,  26,  70,   4,  46,  16,  26,  21,   7,  31,  14,  17,  69,  11,  53, 300

        //  33,  25,  57,  19,  37,  11,  33,  10,   9,  38,  12,  13,  63,  11,  51, 300
        //  37,  24,  50,  29,  53,  15,  26,  12,  13,  36,  11,   7,  69,  12,  53, 300
        //  36,  25,  50,  20,  55,  15,  28,  12,  15,  36,  12,  10,  70,  12,  53, 300

        //  40,  15,  50,  20,  56,  16,  17,  12,   7,  55,  99,  23,  78,  16,  67, 300
        //  37,  16,  51,  18,  50,  16,  32,  11,   5,  32,  99,  21,  68,   8,  41,   0 //lv1 s lv2 b
        //  26,  18,  46,  30,  53,  22,  29,  17,   9,  33,   3,  11,  84,   6,  50, -16 //lv1 b lv2 a
        //  28,  21,  57,  28,  48,  17,  18,  12,   8,  29,  17,  22,  78,  10,  65, -10 //lv2 c

        //  21,  20,  66,  40,  27,  22,  48,  26,   8,  71,  13,  24,  92,   7,  69, 300 // a
        //  19,  24,  99,  35,  24,  20,  52,  30,   9,  77,  13,  32,  91,   9,  69,  83 // b
        //  19,  22,  87,  37,  16,  13,  42,  19,   6,  73,  10,  33,  93,   5,  73,  83 // b+
        //  21,  24,  54,  33,  23,  24,  36,  16,   8,  70,  12,  25,  73,   8,  67,  92 // c
        //  23,  20,  66,  36,  24,  21,  46,  27,  14,  77,  15,  32,  93,   5,  67,  85 // s
        //  20,  21,  71,  35,  16,  22,  44,  20,   6,  79,   9,  28,  74,   4,  67,  82 // s

        //  49,  18,  76,  33,  39,  27,  32,  25,  22,  99,  41,  29,  96,  14,  60, 300 //s
        //  31,  17,  69,  32,  21,  27,  49,  24,  18,  99,  78,  28,  99,   8,  62,  91 //c
        //  52,  19,  69,  34,  35,  30,  34,  20,  17,  89,  37,  31,  83,   9,  55,  97 //c
        //  53,  16,  73,  33,  26,  28,  30,  22,  22,  79,  18,  28,  93,   5,  63,  95 //a
        //  60,  16,  72,  35,  29,  30,  41,  20,  20,  79,  55,  22,  97,   4,  60,  95 // b
        //  40,  13,  80,  37,  16,  24,  38,  19,  21,  67,  99,  24,  96,   6,  42,  83 // a

        //  49,  18,  76,  40,  39,  27,  32,  50,  22,  99,   0,  29,  96,  14,  60, 300 //s ogn

        //  45,  28,  84,  21,  35,  27,  56,  30,   9,  64,  13,  18,  97,  11,  29,  300 // cmp
        //  43,  16,  80,  20,  38,  26,  53,  30,   3,  70, -17,  19,  96,  18,  30,  300 // b
        //  40,  18,  97,  25,  39,  18,  59,  30,   4,  64,  -9,  17,  97,  17,  31,  300 // b+
        //  39,  28,  94,  24,  42,  23,  56,  31,   5,  78,  -8,  14,  96,  13,  33,  300 //b
        //  38,  21,  94,  28,  48,  25,  54,  34,   8,  88, -21,  19,  80,  31,  32,  300 //a
        //  43,  27,  87,  34,  50,  32,  68,  26, -10,  83,  -2,  14,  89,   6,  27,  300 //a
        //  40,  20,  98,  13,  35,  22,  63,  29,   5,  68, -11,  15,  98,  14,  32,  300 //b

        //  48,  27,  88,  25,  34,  23,  52,  26,   3,  63, -14,  19,  34,   5,  33, 300 // a+
        //  47,  27,  92,  31,  38,  28,  52,  29,   5,  61,  -6,  25,  92,   9,  33, 300 // b
        //  50,  26,  84,  29,  46,  25,  35,  29,   0,  68, -14,  17,  99,   3,  40, 300 // b-
        //  37,  30,  95,  34,  32,  26,  44,  33,  11,  56, -11,  22,  37,  12,  35, 300 // a
        //  44,  32,  96,  28,  42,  24,  49,  25,  -6,  58,  17,  20,  51,  10,  32, 300 // s
        //  48,  27,  97,  22,  41,  29,  53,  27,   3,  60, -10,  19,  42,   6,  31, 300 // a+

        //new param

        //  41,  13,  99,  28,  42,  32,  47,  24,   6,  61,  19,  30,  41,  12,  27, 300 // b
        //  41,  25,  91,  28,  41,  26,  54,  31,   7,  43,  16,  35,   8,  12,  24, 300 // b
        //  35,   5,  98,  26,  43,  32,  52,  18,  16,  57,  24,  22,  38,  10,  28, 300 // b
        //  41,  13,  98,  27,  45,  24,  51,  28,  13,  65,  27,  21,  39,  13,  39, 300 // s
        //  39,  15,  99,  26,  41,  28,  50,  26,   8,  68,  23,  22,  36,  14,  28, 300 // a
        //  36,  20,  99,  27,  41,  32,  28,  24,  11,  56,  26,  24,  43,  14,  27, 300 //s

        //*
        //  41,  13,  99,  25,  46,  27,  49,  27,   9,  73,  23,  26,  25,   9,  42,  300 // a
        //  44,  13,  98,  31,  51,  30,  53,  27,  16,  56,  29,  27,  34,  16,  24,  300 // s
        //  39,  13,  98,  29,  30,  34,  54,  28,  18,  58,  20,  28,  35,  11,  33,  300 // s-
        //  36,  11,  90,  29,  31,  34,  52,  22,  20,  66,  26,  26,  38,  11,  30,  300 // a
        //  34,  29,  97,  35,  24,  23,  55,  27,  14,  75,  19,  41,  32,   0,  46,  300 // a
        //  39,  12,  98,  30,  32,  27,  55,  25,  15,  68,  22,  25,  30,   9,  33,  300 // s-
        //*/
        //  46,  10,  99,  86,  80,  29,  30,  24, -22,  71,  50,  36,  47,  37,  44,  83 // a
        //  74,  39,  88,  99,  57,  33,  30,  23, -14,  73,  60,  43,  58,  39,  38,  96 // a
        //  46,  38,  96,  98,  63,  37,  28,  26, -17,  69,  64,  43,  45,  42,  42,  99 // s
        //  68,  34,  99,  99,  75,  38,  33,  20, -16,  80,  56,  31,  61,  36,  43,  88
        //  42,  48,  99,  95,  63,  27,  -2,  29, -22,  60,  54,  31,  57,  38,  37,  99 // b
        //  70,  30,  96,  99,  73,  41,  41,  18, -20,  82,  51,  35,  55,  50,  41,  89 // a
        //  36,  30,  71,  67,  72,  48,  22,  16,  34,  60,   0,  34,  46,  35,  16,  33 //test
        //  31,  43,  78,  80,  63,  46,  48,  14,   0,  99,  59,  24,  29,  30,  33,   7 //test2
        //  77,  42,  92,  98,  98,  28,   1,  19,  -1,  75,  52,  50,  99,  27,  49,  65 //test3
        //  70,  16,  99,  50,  95,  33,  21,  29,  38,  98,  10,  54,  91,  26,  42,  65
        13, 9, 17, 10, 29, 25, 39, 2, 12, 19, 7, 24, 21, 16, 14, 19, 200

    };
    tetris.m_ai_param = param;

    if (ai.level <= 0) {
        AI::AI_Param param[2] = {
            {

                //  43,  47,  84,  62,  60,  47,  53,  21,   2,  98,  85,  13,  21,  37,  38,  0
                //  47,  66,  86,  66, -79,  42,  38,  23,  -3,  95,  74,  13,  27,  36,  37,  0
                //  45,  61,  99,  49,  63,  40,  42,  31, -27,  88,  80,  28,  28,  41,  33,  0
                //  58,  61,  90,  82,  19,  27,  44,  17,  -4,  75,  47,  20,  38,  32,  41,  0
                47, 62, 94, 90, 11, 35, 48, 19, -21, 78, 64, 20, 42, 42, 39, 300
                //  48,  65,  84,  59, -75,  39,  43,  23, -17,  92,  64,  20,  29,  37,  36,  0

            },
            {

                //  43,  47,  84,  62,  60,  47,  53,  21,   2,  98,  85,  13,  21,  37,  38,  0
                //  47,  66,  86,  66, -79,  42,  38,  23,  -3,  95,  74,  13,  27,  36,  37,  0
                //  45,  61,  99,  49,  63,  40,  42,  31, -27,  88,  80,  28,  28,  41,  33,  0 // a
                //  58,  61,  90,  82,  19,  27,  44,  17,  -4,  75,  47,  20,  38,  32,  41,  0 // b
                //  47,  62,  94,  90,  11,  35,  48,  19, -21,  78,  64,  20,  42,  42,  39,  0 // s
                //  48,  65,  84,  59, -75,  39,  43,  23, -17,  92,  64,  20,  29,  37,  36,  0 // s
                71, 12, 78, 52, 96, 37, 14, 24, 40, 99, 44, 49, 93, 25, 44, 380
            }
        };
        tetris.m_ai_param = param[0];
    }

    std::string ai_name = "T-spin AI";
    if (ai.style == 1) {
        ai_name = "T-spin+ AI";
    } else if (ai.style == 2) {
        AI::setAIsettings(0, "hash", 0);
        //
    } else if (ai.style == 3) {
        tetris.m_ai_param.tspin = tetris.m_ai_param.tspin3 = -300;
        tetris.m_ai_param.clear_useless_factor *= 0.8;
        ai_name = "Ren AI";
    } else if (ai.style == 4) {
        tetris.hold = false;
        tetris.m_ai_param.clear_useless_factor *= 0.7;
        ai_name = "non-Hold";
        // no 4w
        tetris.m_ai_param.strategy_4w = 0;
        AI::setAIsettings(0, "4w", 0);
    } else if (ai.style != -1) { //if ( ai.style == 5 ) {
        AI::AI_Param param[2] = {
            {49, 918, 176, 33, -300, -0, 0, 25, 22, 99, 41, -300, 0, 14, 290, 0}, // defence AI
            {21, 920, 66, 40, -300, -2, 0, 26, 8, 71, 13, -300, 0, 7, 269, 0},
        };
        AI::setAIsettings(0, "combo", 0);
        tetris.m_ai_param = param[0];
        ai_name = "Downstack";
    }

    if (ai.style) {
        char name[200];
        sprintf(name, "%s LV%d", ai_name.c_str(), ai.level);
        tetris.m_name = name;
        cerr << "[debug] Name:" << tetris.m_name << std::endl;
    }
    if (tetris.pAIName) {
        tetris.m_name = tetris.pAIName(ai.level);
    }
    if (!ai_4w || ai_eve || ai.level < 6) {
        tetris.m_ai_param.strategy_4w = 0;
    }
    if (tetris.m_ai_param.strategy_4w > 0) {
        AI::setAIsettings(0, "4w", 1);
    }

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
    } else {
        int a[] = {0, 0, 0, 1, 1, 2};
        AI::setComboList(std::vector<int>(a, a + sizeof (a) / sizeof (*a)));
    }
    if (rule.GarbageBlocking == 0 || rule.GarbageBuffer == 0 || rule.GarbageCancel == 0) {
        tetris.m_ai_param.strategy_4w = 0;
        AI::setAIsettings(0, "4w", 0);
    }

    tetris.m_base = AI::point(0, 30);
    tetris.env_change = 1;
    tetris.n_pieces += 1;
    tetris.total_clears += tetris.m_clearLines; //tetris.m_attack;
    tetris.m_clearLines = 0;
    tetris.m_attack = 0;
}

void Bot::processMoves() {
    cerr << "[debug] movs:" <<  tetris.ai_movs.movs.size() << endl;
    tetris.m_state = AI::Tetris::STATE_MOVING;
    while ( tetris.ai_movs_flag == -1 && !tetris.ai_movs.movs.empty() ){
        int mov = tetris.ai_movs.movs[0];
        tetris.ai_movs.movs.erase( tetris.ai_movs.movs.begin() );
        if (0) ;
        else if (mov == AI::Moving::MOV_L) tetris.tryXMove(-1);
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

void Bot::outputAction() {
    std::vector<AI::Gem> next;
    for (int j = 0; j < 5; ++j) //NEXT size
        next.push_back(tetris.m_next[j]);
    int deep = AI_TRAINING_DEEP;
    int upcomeAtt = 0;
    int level = ai.level;
    bool canhold = tetris.hold;

    
    AI::RunAI(tetris.ai_movs, tetris.ai_movs_flag, tetris.m_ai_param, tetris.m_pool, tetris.m_hold,
            tetris.m_cur,
            tetris.m_cur_x, tetris.m_cur_y, next, canhold, upcomeAtt,
            deep, tetris.ai_last_deep, level, 0);
    
    cerr << "[debug] pool:" << tetris.m_pool << endl;
    
    std::stringstream out;
    
    if(tetris.alive()){
        processMoves();
        out << tetris.m_clearLines << "|"<<((int)tetris.wallkick_spin)<<"|";
        tetris.m_state = AI::Tetris::STATE_READY;
    }else{
        out << "-1|0|";
    }
   
    int bottom=AI_POOL_MAX_H-5;
    for(int i=AI::gem_add_y + 1;i<bottom;i++){
        unsigned long mask=512u; //(2^WIDTH-1)
        for(;mask!=0;mask=mask>>1){
            out<<( ((tetris.m_pool.m_row[i] & mask) == mask)? 2 : 0 );
            if(mask!=1)out<<',';
        }
        if(i!=bottom-1)out<<';';
    }
    std::cout<<out.str()<<std::endl;    

}
