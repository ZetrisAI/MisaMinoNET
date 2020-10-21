#include "main.h"

#pragma unmanaged

std::map<char, int> m_gemMap;
Bot MisaBot;

DLL void set_abort(Callback handler) {
	Abort = handler;
}

DLL void configure(AI::AI_Param param, bool holdAllowed, bool allSpin, bool TSDonly, int search_width, bool allow180, bool srsplus) {
    MisaBot = Bot();
    MisaBot.updateStyle(param);
    MisaBot.updateHoldAllowed(holdAllowed);
    MisaBot.updateAllSpin(allSpin);
    TSD_only = TSDonly;
    sw_map_multiplier = search_width;
    AI::setSpin180(allow180);
    srs_plus = srsplus;
    MisaBot.setup();
};

DLL void update_next(const char* queue) {
    MisaBot.updateQueue(queue);
}
DLL void update_current(const char* piece) {
    MisaBot.updateCurrent(piece);
}
DLL void update_hold(const char* piece) {
    MisaBot.updateHold(piece);
}
DLL void update_incoming(int attack) {
    MisaBot.updateIncoming(attack);
}
DLL void update_combo(int combo) {
    MisaBot.updateCombo(combo);
}
DLL void update_b2b(int b2b) {
	MisaBot.updateB2B(b2b);
}
DLL void update_field(const char* field) {
    MisaBot.updateField(field);
}
DLL void update_reset() {
    MisaBot.updateReset();
    MisaBot.setup();
}

DLL void action(char* str, int len) {
    std::string a = MisaBot.outputAction(str, len);
    std::copy(a.c_str(), a.c_str() + a.length() + 1, str);
}
DLL bool alive() {
    return MisaBot.tetris.alive();
}

DLL void findpath(const char* _field, const char* _piece, int x, int y, int r, bool hold, char* str, int len) {
    std::string s = _field;
    std::vector<int> rows;
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
    
    AI::GameField field;
    field.reset(10, rows.size());
    for (auto &row : rows) {
        field.addRow(row);
    }
    
    std::string ps = _piece;
    AI::Gem piece = AI::getGem(m_gemMap[ps[0]], 0);
    
	AI::Moving result;
    AI::FindPathMoving(field, result, piece, AI::gem_beg_x, AI::gem_beg_y, hold, x, y, r, -1);
    
    std::stringstream out;
    
    for (int i = 0; i < result.movs.size(); i++) {
        out << result.movs[i] << ((i == result.movs.size() - 1)? "|" : ",");
    }
    
    out << ((int)result.wallkick_spin);
   
    std::string a = out.str();
    std::copy(a.c_str(), a.c_str() + a.length() + 1, str);
}

BOOL WINAPI DllMain(HANDLE handle, DWORD reason, LPVOID reserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            m_gemMap[' '] = AI::GEMTYPE_NULL;
            m_gemMap['I'] = AI::GEMTYPE_I;
            m_gemMap['T'] = AI::GEMTYPE_T;
            m_gemMap['L'] = AI::GEMTYPE_L;
            m_gemMap['J'] = AI::GEMTYPE_J;
            m_gemMap['Z'] = AI::GEMTYPE_Z;
            m_gemMap['S'] = AI::GEMTYPE_S;
            m_gemMap['O'] = AI::GEMTYPE_O;
    
            MisaBot.setup();
            break;

        case DLL_THREAD_ATTACH:
            break;        

        case DLL_THREAD_DETACH:       
            break;

        case DLL_PROCESS_DETACH:
            break;    
    }
    
    return TRUE;
}