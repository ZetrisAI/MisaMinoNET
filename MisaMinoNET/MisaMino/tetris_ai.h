#pragma once
#define _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH

#include "callback.h"
#include "gamepool.h"
#include "lastnodes.h"
#include "stuff.h"

#include <algorithm>
#include <vector>

namespace AI {
    struct Moving
    {
        enum {
            MOV_NULL,
            MOV_L,
            MOV_R,
            MOV_LL,
            MOV_RR,
            MOV_D,
            MOV_DD,
            MOV_LSPIN,
            MOV_RSPIN,
            MOV_DROP,
            MOV_HOLD,
            MOV_SPIN2,
        };
        std::vector<int> movs;
        int x, y;
        int score, score2;
        signed char spin;
        signed char wallkick_spin;
        Moving () { wallkick_spin = 0; movs.reserve (16); }
        Moving ( const Moving & m ) {
            //movs.reserve (16);
            movs = m.movs;
            x = m.x;
            y = m.y;
            spin = m.spin;
            score = m.score;
            score2 = m.score2;
            wallkick_spin = m.wallkick_spin;
        }
        Moving ( const Moving & m, int _spin ) {
            movs.reserve (16);
            movs = m.movs;
            spin = _spin;
        }
        bool operator < (const Moving& _m) const {
            return score > _m.score;
        }
        bool operator == (const Moving& _m) const {
            if ( x != _m.x || y != _m.y ) return false;
            if ( spin != _m.spin ) return false;
            if ( wallkick_spin != _m.wallkick_spin ) return false;
            return true;
        }
    };
    struct MovingSimple
    {
        enum {
            MOV_NULL,
            MOV_L,
            MOV_R,
            MOV_LL,
            MOV_RR,
            MOV_D,
            MOV_DD,
            MOV_LSPIN,
            MOV_RSPIN,
            MOV_DROP,
            MOV_HOLD,
            MOV_SPIN2,
        };
        enum {
            INVALID_POS = -64,
        };
        int x, y;
        int lastmove;
        long long score, score2;
        signed char spin;
        signed char wallkick_spin;
        bool hold;
		bool softdrop;
		MovingSimple() { x = INVALID_POS; wallkick_spin = 0; lastmove = MovingSimple::MOV_NULL; softdrop = false; }
        MovingSimple ( const Moving & m ) {
            x = m.x;
            y = m.y;
            spin = m.spin;
            score = m.score;
            score2 = m.score2;
            wallkick_spin = m.wallkick_spin;
            if (m.movs.empty()) hold = false;
            else hold = (m.movs[0] == MovingSimple::MOV_HOLD);
            if (m.movs.empty()) lastmove = MovingSimple::MOV_NULL;
            else lastmove = m.movs.back();
        }
        MovingSimple ( const MovingSimple & m ) {
            x = m.x;
            y = m.y;
            spin = m.spin;
            score = m.score;
            score2 = m.score2;
            wallkick_spin = m.wallkick_spin;
			softdrop = m.softdrop;
            hold = m.hold;
            lastmove = m.lastmove;
        }
        MovingSimple ( const MovingSimple & m, int _spin ) {
            lastmove = m.lastmove;
            hold = m.hold;
            spin = (signed char)_spin;
        }
        bool operator == (const MovingSimple& _m) const {
            if ( x != _m.x || y != _m.y ) return false;
            if ( spin != _m.spin ) return false;
            if ( lastmove != _m.lastmove ) return false;
            if ( hold != _m.hold ) return false;
			if ( softdrop != _m.softdrop) return false;
            if ( wallkick_spin != _m.wallkick_spin ) return false;
            return true;
        }
        bool operator < (const MovingSimple& _m) const {
            return score > _m.score;
        }
    };
    template <class T>
    struct MovList
    {
        std::vector<T> queue;
        int beg, end;
        MovList() {
            beg = end = 0;
        }
        MovList( size_t size ) {
            beg = end = 0;
            queue.reserve( size );
        }
        void clear() {
            beg = end = 0;
            queue.clear();
        }
        size_t size() const {
            return end - beg;
        }
        T& back() {
            return queue[end-1];
        }
        void push(const T& mov) {
            queue.push_back(mov);
            ++end;
        }
        bool empty () const {
            return beg == end;
        }
        void pop(T& mov) {
            mov = queue[beg++];
        }
    };
    template <class T>
    struct MovQueue
    {
        std::vector<T> queue;
        MovQueue() {
        }
        MovQueue( size_t size ) {
            queue.reserve( size );
        }
        void clear() {
            queue.clear();
        }
        size_t size() const {
            return queue.size();
        }
        T& front() {
            return queue.front();
        }
        T& back() {
            return queue.back();
        }
        T& append() {
            queue.push_back(T());
            return back();
        }
        void push_back() {
            std::push_heap(queue.begin(), queue.end());
        }
        void pop_back() {
            std::pop_heap(queue.begin(), queue.end());
        }
        void dec_size() {
            queue.pop_back();
        }
        void push(const T& mov) {
            queue.push_back(mov);
            std::push_heap(queue.begin(), queue.end());
        }
        bool empty () const {
            return queue.empty();
        }
        void pop(T& mov) {
            std::pop_heap(queue.begin(), queue.end());
            mov = queue.back();
            queue.pop_back();
        }
        void sort () {
            std::sort( queue.begin(), queue.end() );
        }
    };
    struct AI_Param {
        int miny_factor; // 最高高度分 // Score based on maximum height
        int hole; // -洞分 // Hole penalty
        int open_hole; // -开放洞，可能插块 // Open hole penalty, potential for block insertion
        int v_transitions; // -水平转换系数 // Horizontal transition coefficient
        int tspin3; // T3基本分 // Basic score for T-Spin Triple

        int clear_efficient; // 消行效率系数 // Line clear efficiency coefficient
        int upcomeAtt; // -预备攻击基本系数 // Upcoming attack base coefficient
        int h_factor; // -高度差系数 // Height difference coefficient
        int hole_dis_factor2; // -洞距离系数 // Hole distance factor 2
        int hole_dis; // -洞的距离分 // Hole distance score

        int hole_dis_factor; // -洞距离系数 // Hole distance coefficient
        int tspin; // tspin系数 // T-Spin coefficient
        int hold_T; // hold T和I系数 // Hold T and I coefficient
        int hold_I; // hold T和I系数 // Hold T and I coefficient
        int clear_useless_factor; // 无效行系数 // Useless line clear penalty

        int dif_factor; //偏差值 // Deviation factor
		int b2b;
		int combo;
		int avoid_softdrop;
		int tmini;

        int ass_bonus;
		int strategy_4w;
    };
    void setComboList( std::vector<int> combolist );
    int getComboAttack( int combo );
    void setSpin180( bool enable );
    bool spin180Enable();
    void setAIsettings(const char* key, int val);

    void GenMoving(const GameField& field, std::vector<MovingSimple> & movs, Gem cur, int x, int y, bool hold, bool wasLineClear);
	void FindPathMoving(const GameField& field, Moving& mov, Gem cur, int x, int y, bool hold, int goalx, int goaly, int goalr, int goalwk);
    MovingSimple AISearch(AI_Param ai_param, const GameField& pool, int hold, Gem cur, int x, int y, const std::vector<int>& next, bool canhold, int upcomeAtt, int maxDeep, int & searchDeep, bool second_choice);
	int score_avoid_softdrop(int param, bool sd, int cur, bool wk, double h);
    Gem RunAI(Moving& ret_mov, int& flag, const AI_Param& ai_param, const GameField& pool, int hold, Gem cur, int x, int y, const std::vector<int>& next, bool canhold, int upcomeAtt, int maxDeep, int & searchDeep, bool second_choice);
}
