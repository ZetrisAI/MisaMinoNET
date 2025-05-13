#pragma once
#include "gamepool.h"
#include "tetris_ai.h"

namespace AI {
    class Tetris {
    public:
        enum {
            STATE_INIT,
            STATE_READY,
            STATE_MOVING,
            STATE_PASTED,
            STATE_OVER,
        };
        
        Tetris() : m_pool( 10, 20 ) {
            m_state = STATE_INIT;
            m_next.reserve(1024);
            reset ( 10, 20 );
        }

        void reset (signed char w, signed char h) {
            m_pool.reset( w, h );
            m_pool.combo = 0;
            m_pool.b2b = 0;
            m_pool.m_hold = 0;
            m_next.clear();
            m_state = STATE_READY;
            m_cur = AI::getGem(0, 0);
            m_cur_x = AI::gem_beg_x;
            m_cur_y = AI::gem_beg_y;
            m_attack = 0;
        }
        bool tryXMove(int dx) {
            if ( m_state != STATE_MOVING ) return false;
            if (m_pool.isCollide(m_cur_x + dx, m_cur_y, m_cur))
                return false;
            m_cur_x += dx;
            wallkick_spin = 0;
            return true;
        }
        bool tryYMove(int dy) {
            if ( m_state != STATE_MOVING ) return false;
            if (m_pool.isCollide(m_cur_x, m_cur_y + dy, m_cur))
                return false;
            m_cur_y += dy;
            wallkick_spin = 0;
            return true;
        }
        bool trySpin(int dSpin) {
            if ( m_state != STATE_MOVING ) return false;
            AI::Gem gem = AI::getGem(m_cur.num, (m_cur.spin + dSpin + 4) % 4);
            if (m_pool.isCollide(m_cur_x, m_cur_y, gem)) {
                int spin = 1;
                if ( dSpin == 1 ) spin = 0;
                if ( m_pool.wallkickTest(m_cur_x, m_cur_y, gem, spin) ) {
                    m_cur = gem;
                    wallkick_spin = 2;
                    return true;
                } else {
                    return false;
                }
            }
            m_cur = gem;
            wallkick_spin = 1;
            return true;
        }
        bool trySpin180() {
            if ( m_state != STATE_MOVING ) return false;
            AI::Gem gem = AI::getGem(m_cur.num, (m_cur.spin + 2) % 4);
            if (m_pool.isCollide(m_cur_x, m_cur_y, gem)) {
				if (m_pool.wallkickTest(m_cur_x, m_cur_y, gem, 2)) {
					m_cur = gem;
					wallkick_spin = 2;
					return true;
				}
				else {
					return false;
				}
            }
            m_cur = gem;
            wallkick_spin = 1;
            return true;
        }
        bool tryHold() {
            if ( m_state != STATE_MOVING ) return false;
            if ( m_hold ) return false;
            m_hold = true;
            int hold = m_pool.m_hold;
            m_pool.m_hold = m_cur.num;
            if ( hold == 0 ) {
                m_cur_x = AI::gem_beg_x;
                m_cur_y = AI::gem_beg_y;
                m_cur = AI::getGem(m_next[0], 0);
                removeNext();
            } else {
                m_cur_x = AI::gem_beg_x;
                m_cur_y = AI::gem_beg_y;
                m_cur = AI::getGem(hold, 0);
            }
            if ( m_pool.isCollide(m_cur_x, m_cur_y, m_cur)) {
                m_state = STATE_OVER;
                return true;
            }
            return true;
        }
        bool drop () {
            if ( m_state != STATE_MOVING ) return false;
            while ( tryYMove( 1 ) );
            wallkick_spin = m_pool.WallKickValue(m_cur.num, m_cur_x, m_cur_y, m_cur.spin, wallkick_spin);
            m_pool.paste( m_cur_x, m_cur_y, m_cur );
            m_cur = AI::getGem( 0, 0);
            m_state = STATE_PASTED;
            return true;
        }
        void clearLines() {
            if ( m_state != STATE_PASTED ) return;
            int clearLines = m_pool.clearLines( wallkick_spin );
            m_attack = m_pool.getAttack( clearLines, wallkick_spin );
            m_state = STATE_READY;
        }
        void addRow( int att ) {
            m_pool.addRow( att );
            if ( m_cur_y > 1 ) {
                m_cur_y -= 1;
            }
            if ( m_pool.m_row[0] ) {
                m_state = STATE_OVER;
            }
        }
        void removeNext() {
			if (m_next.size() > 0) {
				m_next.erase(m_next.begin());
			}
        }
        void newpiece() { // TODO see what the fuck this shit does
            m_cur_x = AI::gem_beg_x;
            m_cur_y = AI::gem_beg_y;
            m_hold = false;
            wallkick_spin = 0;
        }
        int poolw() const {
            return m_pool.width();
        }
        int poolh() const {
            return m_pool.height();
        }
        int curx() const {
            return m_cur_x;
        }
        int cury() const {
            return m_cur_y;
        }
        bool alive () const {
            return m_state != STATE_OVER;
        }
    public:
        int m_state;
        AI::GameField m_pool;
        AI::Gem m_cur;
        int m_hold;
        int m_cur_x, m_cur_y;
        signed char wallkick_spin;
        std::vector<int> m_next;
        int m_attack;
    };
}
