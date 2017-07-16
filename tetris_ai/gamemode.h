#pragma once
#include <deque>

#ifndef XP_RELEASE

class EvE {
    struct ai_info {
        AI::AI_Param ai;
        int score;
        ai_info () {
            memset(&ai, 0, sizeof(ai));
            score = -1;
        }
        ai_info( const AI::AI_Param& _ai ) : ai(_ai) {
            score = -1;
        }
        bool operator < (const ai_info& info) const {
            return score < info.score;
        }
    };
public:
    EvE() {
        round = TRAINING_ROUND;
        nround = 0;
        state_find_best = false;
        find_best = false;
        //find_best = true;
        m_init = false;
    }
    void init(std::deque<ai_info> _ai) {
        ai = _ai;
        state_find_best = false;
        find_best = false;
        //find_best = true;
        m_init = false;
    }
    void result(int p1_win, int p1_score, int p2_win, int p2_score) {
        ai[0].score = p1_score;
        ai[1].score = p2_score;
        if ( !state_find_best && abs(p1_win - p2_win) < (p1_win + p2_win) / 3 ) {
            ai.push_back(ai.front());
            ai.pop_front();
            ai.push_back(ai.front());
            ai.pop_front();
        } else if ( p1_win > p2_win ) {
            ai.push_back(ai.front());
            ai.pop_front();
            ai.pop_front();
        } else if ( p1_win < p2_win ) {
            ai.pop_front();
            ai.push_back(ai.front());
            ai.pop_front();
        }
    }
    void result(int score) {
        ai[0].score = score;
        ai.push_back(ai.front());
        ai.pop_front();
    }

    volatile int state_find_best;
    volatile bool find_best;
    bool m_init;
    std::deque<ai_info> ai;
    std::deque<ai_info> best_ai;
    int round;
    int nround;
    int m_p1, m_p2;
    std::vector<unsigned> m_p2_seed;
    int m_p1_score;
    int m_p2_score;
    int m_p2_sqr_score;
    int m_p2_last_pieces;
    int m_p2_att_val;
};
#endif
