#include "main.h"
#include "tetris_ai.h"
#include <assert.h>
#define USING_MOV_D     0
#define GENMOV_W_MASK   15
#define SWITCH_USING_HEIGHT_OPT

#define _MACRO_CREATE_MOVINGSIMPLE(arg_action_name,arg_wkspin,arg_sd) \
    MovingSimple nm = m; \
    nm.x = nx; \
    nm.y = ny; \
    nm.spin = ns; \
    nm.lastmove = Moving::arg_action_name; \
    nm.wallkick_spin = arg_wkspin; \
	nm.softdrop = arg_sd;

#define _MACRO_CREATE_MOVING(arg_action_name,arg_wkspin) \
    Moving nm = m; \
    nm.x = nx; \
    nm.y = ny; \
    nm.spin = ns; \
    nm.movs.push_back(Moving::arg_action_name); \
    nm.wallkick_spin = arg_wkspin
#define _MACRO_HASH_POS(arg_hash_table,arg_prefix) \
    arg_hash_table[arg_prefix##y][arg_prefix##s][arg_prefix##x & GENMOV_W_MASK]

namespace AI {
    bool g_spin180 = false;
    std::vector<int> g_combo_attack;
    bool g_allSpin = false;
    bool g_softdrop = true;
    //bool g_softdrop = false;

    void setSpin180( bool enable ) {
        g_spin180 = enable;
    }
    bool spin180Enable() {
        return g_spin180;
    }

    void setComboList( std::vector<int> combolist ) {
        g_combo_attack = combolist;
    }
    int getComboAttack( int combo ) {
		if (g_combo_attack.empty()) return 0;
		return (int) ((double)((combo >= (int)g_combo_attack.size()) ? g_combo_attack.back() : g_combo_attack[combo]));
    }
    void setAllSpin(bool allSpin) {
        g_allSpin = allSpin;
    }
    bool isEnableAllSpin() {
        return g_allSpin;
    }
    void setSoftdrop( bool softdrop ) {
        g_softdrop = softdrop;
    }
    bool softdropEnable() {
        return g_softdrop;
    }
    //enum {
    //    MOV_SCORE_DROP = 1,
    //    MOV_SCORE_LR = 10,
    //    MOV_SCORE_LR2 = 19,
    //    MOV_SCORE_LLRR = 100,
    //    MOV_SCORE_D = 1000,
    //    MOV_SCORE_DD = 3000,
    //    MOV_SCORE_SPIN = 20,
    //};
    enum {
        MOV_SCORE_DROP = 1,
        MOV_SCORE_LR = 80,
        MOV_SCORE_LR2 = 200,
        MOV_SCORE_LLRR = 100,
        MOV_SCORE_D = 1000,
        MOV_SCORE_DD = 3000,
        MOV_SCORE_SPIN = 150,
    };

    void GenMoving(const GameField& field, std::vector<MovingSimple> & movs, Gem cur, int x, int y, bool hold) {
        movs.clear();
		if (cur.num == 0) { // rare race condition, we're dead already if this happens
			assert(true); // debug break
			cur = AI::getGem(AI::GEMTYPE_I, 0);
		}
        if ( field.isCollide(x, y, getGem(cur.num, cur.spin) ) ) {
            return ;
        }
        //if ( field.isCollide(x, y + 1, getGem(cur.num, cur.spin) ) ) {
        //    return ;
        //}
        char _hash[64][4][GENMOV_W_MASK+1] = {0};
        char _hash_drop[64][4][GENMOV_W_MASK+1] = {0};
        char (*hash)[4][GENMOV_W_MASK+1] = &_hash[gem_add_y];
        char (*hash_drop)[4][GENMOV_W_MASK+1] = &_hash_drop[gem_add_y];
        MovList<MovingSimple> q(1024);

#ifdef SWITCH_USING_HEIGHT_OPT
        // height of every column
        int field_w = field.width(), field_h = field.height();
        int min_y[32] = {0};
        {
            int beg_y = -5;
            while ( field.row[beg_y] == 0 ) ++beg_y;
            for ( int x = 0; x < field_w; ++x ) {
                for ( int y = beg_y, ey = field_h + 1; y <= ey; ++y ) {
                    if ( field.row[y] & ( 1 << x ) ) {
                        min_y[x] = y;
                        break;
                    }
                }
            }
        }
#endif
		if ( 1 || field.row[y+3] & field.m_w_mask ) // �ǿ����еĻ�
        {
            MovingSimple m;
            m.x = x;
            m.y = y;
            m.spin = cur.spin;
            m.wallkick_spin = 0;
            if ( hold ) {
                m.lastmove = MovingSimple::MOV_HOLD;
                m.hold = true;
            } else {
                m.lastmove = MovingSimple::MOV_NULL;
                m.hold = false;
            }
            q.push(m);
            hash[m.y][m.spin][m.x & GENMOV_W_MASK] = 1;
        }
		else
		{
			for ( int spin = 0; spin < 4; ++spin )
			{
				int ns = (cur.spin + spin) % cur.mod;
				int dx = 0;
				for ( ; ; ++dx )
				{
					MovingSimple m;
					int nx = x + dx, ny = y;
					if ( field.isCollide(nx, ny, getGem(cur.num, ns) ) )
						break;
					m.x = nx;
					m.y = ny;
					m.spin = ns;
					m.wallkick_spin = 0;
					int dist_min = 0x7fffffff;
					for ( int x = 0; x < 4; ++x ) {
						if ( getGemColH(cur.num, ns, x) ) { // 0 = empty column
							int dist_cur_col = min_y[nx + x] - (ny + getGemColH(cur.num, ns, x));
							if ( dist_cur_col < dist_min ) {
								dist_min = dist_cur_col;
							}
						}
					}
					if ( dist_min < 0 ) { // underground
						while ( ! field.isCollide(nx, ny + 1, getGem(cur.num, ns) ) ) {
							if ( !USING_MOV_D && (_MACRO_HASH_POS(hash,n) & 1) == 0) {
								_MACRO_HASH_POS(hash,n) |= 1;
							}
							++ny; //wallkick_spin = 0;
						}
					} else { // under the sun
						ny = ny + dist_min;
						//if ( dist_min > 0 ) wallkick_spin = 0;
						//for ( int y = m.y + 1; y < ny; ++y ) {
						//	if ( !USING_MOV_D && (hash[y][ns][nx & GENMOV_W_MASK] & 1) == 0) {
						//		hash[y][ns][nx & GENMOV_W_MASK] |= 1;
						//	}
						//}
					}
					m.y = ny;
					m.lastmove = MovingSimple::MOV_NULL;
					if ( hold ) {
						m.hold = true;
					} else {
						m.hold = false;
					}
					q.push(m);
					_MACRO_HASH_POS(hash,n) = 1;
					//hash[ny][ns][nx & GENMOV_W_MASK] |= 1;
				}
				if ( dx > 0 )
				for ( dx = -1; ; --dx )
				{
					MovingSimple m;
					int nx = x + dx, ny = y;
					if ( field.isCollide(nx, ny, getGem(cur.num, ns) ) )
						break;
					m.x = nx;
					m.y = y;
					m.spin = ns;
					m.wallkick_spin = 0;
					int dist_min = 0x7fffffff;
					for ( int x = 0; x < 4; ++x ) {
						if ( getGemColH(cur.num, ns, x) ) { // 0 = empty column
							int dist_cur_col = min_y[nx + x] - (ny + getGemColH(cur.num, ns, x));
							if ( dist_cur_col < dist_min ) {
								dist_min = dist_cur_col;
							}
						}
					}
					if ( dist_min < 0 ) { // underground
						while ( ! field.isCollide(nx, ny + 1, getGem(cur.num, ns) ) ) {
							if ( !USING_MOV_D && (_MACRO_HASH_POS(hash,n) & 1) == 0) {
								_MACRO_HASH_POS(hash,n) = 1;
							}
							++ny; //wallkick_spin = 0;
						}
					} else { // under the sun
						ny = ny + dist_min;
						//if ( dist_min > 0 ) wallkick_spin = 0;
						//for ( int y = m.y + 1; y < ny; ++y ) {
						//	if ( !USING_MOV_D && (hash[y][ns][nx & GENMOV_W_MASK] & 1) == 0) {
						//		hash[y][ns][nx & GENMOV_W_MASK] |= 1;
						//	}
						//}
					}
					m.y = ny;
					m.lastmove = MovingSimple::MOV_NULL;
					if ( hold ) {
						m.hold = true;
					} else {
						m.hold = false;
					}
					q.push(m);
					_MACRO_HASH_POS(hash,n) = 1;
					//hash[ny][ns][nx & GENMOV_W_MASK] |= 1;
				}
			}
		}
        while ( ! q.empty() ) {
            MovingSimple m;
            q.pop(m);
            //if ( m.y < -1 ) continue;
            if ( m.lastmove == MovingSimple::MOV_DROP ) {
                if ( getGemMaxH(cur.num, m.spin) + m.y <= 2 ) //lockout
                    continue;
                movs.push_back(m);
                continue;
            }

            if ( m.lastmove != MovingSimple::MOV_DD && m.lastmove != MovingSimple::MOV_D )
            {
                int nx = m.x, ny = m.y, ns = m.spin;
                int wallkick_spin = m.wallkick_spin;
#ifndef SWITCH_USING_HEIGHT_OPT
                while ( field.row[ny + cur.geth()] == 0 && ny + cur.geth() <= field.height() ) { // �ǿ����в���ʹ�õ��Ż�
                    ++ny; wallkick_spin = 0;
                }
                while ( ! field.isCollide(nx, ny + 1, getGem(cur.num, ns) ) ) {
                    if ( !USING_MOV_D && ( _MACRO_HASH_POS(hash, n) & 1 ) == 0) {
                        _MACRO_HASH_POS(hash, n) |= 1;
                    }
                    ++ny; wallkick_spin = 0;
                }
#endif
#ifdef SWITCH_USING_HEIGHT_OPT
                {
                    int dist_min = 0x7fffffff;
                    for ( int x = 0; x < 4; ++x ) {
                        if ( getGemColH(cur.num, ns, x) ) { // 0 = empty column
                            int dist_cur_col = min_y[nx + x] - (ny + getGemColH(cur.num, ns, x));
                            if ( dist_cur_col < dist_min ) {
                                dist_min = dist_cur_col;
                            }
                        }
                    }
                    if ( dist_min < 0 ) { // underground
                        while ( ! field.isCollide(nx, ny + 1, getGem(cur.num, ns) ) ) {
                            if ( !USING_MOV_D && (_MACRO_HASH_POS(hash,n) & 1) == 0) {
                                _MACRO_HASH_POS(hash,n) |= 1;
                            }
                            ++ny; wallkick_spin = 0;
                        }
                    } else { // under the sun
                        ny = ny + dist_min;
                        if ( dist_min > 0 ) wallkick_spin = 0;
                        for ( int y = m.y + 1; y < ny; ++y ) {
                            if ( !USING_MOV_D && (hash[y][ns][nx & GENMOV_W_MASK] & 1) == 0) {
                                hash[y][ns][nx & GENMOV_W_MASK] |= 1;
                            }
                        }
                    }
                }
#endif
                {
                    int v_spin = (isEnableAllSpin() || cur.num == GEMTYPE_T) ? wallkick_spin : 0;
                    if ( (_MACRO_HASH_POS(hash_drop, n) & ( 1 << v_spin)) == 0 )
                    {

                        int _nx = nx, _ny = ny, _ns = ns;

                        //if ( cur.num == GEMTYPE_I || cur.num == GEMTYPE_Z || cur.num == GEMTYPE_S ) {
                        //    if ( ns == 2 ) {
                        //        _ny = ny + 1;
                        //        _ns = 0;
                        //    }
                        //    if ( ns == 3 ) {
                        //        _nx = nx + 1;
                        //        _ns = 1;
                        //    }
                        //}

                        //if ( (_MACRO_HASH_POS(hash_drop, _n) & ( 1 << v_spin)) == 0 )
                        {
                                _MACRO_CREATE_MOVINGSIMPLE(MOV_DROP, v_spin, m.softdrop);
                                _MACRO_HASH_POS(hash_drop, _n) |= 1 << v_spin;
                                q.push(nm);
                        }
                    }
                    if ( softdropEnable() ) // DD
                    {
                        if ( ny != y ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & 1 ) == 0) {
                                _MACRO_CREATE_MOVINGSIMPLE(MOV_DD, 0, m.softdrop);
                                _MACRO_HASH_POS(hash, n) |= 1;
								nm.softdrop = true;
                                q.push(nm);
                            }
                        }
                    }
                }
            }
            {
                int nx = m.x, ny = m.y, ns = m.spin;
                --nx;
                if ( ( _MACRO_HASH_POS(hash, n) & 1 ) == 0) {
                    if ( ! field.isCollide(nx, ny, getGem(cur.num, ns) ) ) {
                        _MACRO_CREATE_MOVINGSIMPLE(MOV_L, 0, m.softdrop);
                        _MACRO_HASH_POS(hash, n) |= 1;
                        q.push(nm);
                        if ( m.lastmove != MovingSimple::MOV_L && m.lastmove != MovingSimple::MOV_R
                            && m.lastmove != MovingSimple::MOV_LL && m.lastmove != MovingSimple::MOV_RR )
                        {
                            int nx = m.x - 1, ny = m.y, ns = m.spin;
                            while ( ! field.isCollide(nx - 1, ny, getGem(cur.num, ns) ) ) {
                                --nx;
                            }
                            if ( nx != x && ( _MACRO_HASH_POS(hash, n) & 1 ) == 0) {
                                _MACRO_CREATE_MOVINGSIMPLE(MOV_LL, 0, m.softdrop);
                                _MACRO_HASH_POS(hash, n) |= 1;
                                q.push(nm);
                            }
                        }
                    }
                }
            }
            {
                int nx = m.x, ny = m.y, ns = m.spin;
                ++nx;
                if ( ( _MACRO_HASH_POS(hash, n) & 1 ) == 0) {
                    if ( ! field.isCollide(nx, ny, getGem(cur.num, ns) ) ) {
                        _MACRO_CREATE_MOVINGSIMPLE(MOV_R, 0, m.softdrop);
                        _MACRO_HASH_POS(hash, n) |= 1;
                        q.push(nm);
                        if ( m.lastmove != MovingSimple::MOV_L && m.lastmove != MovingSimple::MOV_R
                            && m.lastmove != MovingSimple::MOV_LL && m.lastmove != MovingSimple::MOV_RR )
                        {
                            int nx = m.x + 1, ny = m.y, ns = m.spin;
                            while ( ! field.isCollide(nx + 1, ny, getGem(cur.num, ns) ) ) {
                                ++nx;
                            }
                            if ( nx != x && ( _MACRO_HASH_POS(hash, n) & 1 ) == 0) {
                                _MACRO_CREATE_MOVINGSIMPLE(MOV_RR, 0, m.softdrop);
                                _MACRO_HASH_POS(hash, n) |= 1;
                                q.push(nm);
                            }
                        }
                    }
                }
            }
#if USING_MOV_D > 0
            if ( m.lastmove != MovingSimple::MOV_DD )
            {
                int nx = m.x, ny = m.y, ns = m.spin;
                ++ny;
                MovingSimple nm = m;
                while ( field.row[ny + cur.geth()] == 0 && ny + cur.geth() <= field.height() ) { // �ǿ����в���ʹ�õ��Ż�
                    ++ny;
                    nm.lastmove = MovingSimple::MOV_D;
                }
                if ( ( _MACRO_HASH_POS(hash, n) & 1 ) == 0) {
                    if ( ! field.isCollide(nx, ny, getGem(cur.num, ns) ) ) {
                        _MACRO_CREATE_MOVINGSIMPLE(MOV_D, 0);
                        _MACRO_HASH_POS(hash, n) |= 1;
                        q.push(nm);
                    }
                }
            }
#endif
            {
                int nx = m.x, ny = m.y, ns = (m.spin + 1) % cur.mod;
                if ( ns != m.spin ) {
                    if ( (isEnableAllSpin() || cur.num == GEMTYPE_T) ) {
                        if ( ! field.isCollide(nx, ny, getGem(cur.num, ns) ) ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & ( 1 << 1 ) ) == 0 ) {
                                _MACRO_CREATE_MOVINGSIMPLE(MOV_LSPIN, 1, m.softdrop);
                                _MACRO_HASH_POS(hash, n) |= 1 << 1;
                                q.push(nm);
                            }
                        } else if ( field.wallkickTest(nx, ny, getGem(cur.num, ns), 0 ) ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & ( 1 << 2 ) ) == 0 ) {
                                _MACRO_CREATE_MOVINGSIMPLE(MOV_LSPIN, 2, m.softdrop);
                                _MACRO_HASH_POS(hash, n) |= 1 << 2;
                                q.push(nm);
                            }
                        }
                    } else {
                        if ( ! field.isCollide(nx, ny, getGem(cur.num, ns) ) 
                            || field.wallkickTest(nx, ny, getGem(cur.num, ns), 0 ) ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & 1 ) == 0 ) {
                                _MACRO_CREATE_MOVINGSIMPLE(MOV_LSPIN, 0, m.softdrop);
                                _MACRO_HASH_POS(hash, n) |= 1;
                                q.push(nm);
                            }
                        }
                    }
                }
            }
            {
                int nx = m.x, ny = m.y, ns = (m.spin + 3) % cur.mod;
                if ( ns != m.spin ) {
                    if ( (isEnableAllSpin() || cur.num == GEMTYPE_T) ) {
                        if ( ! field.isCollide(nx, ny, getGem(cur.num, ns) ) ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & ( 1 << 1 ) ) == 0 ) {
                                _MACRO_CREATE_MOVINGSIMPLE(MOV_RSPIN, 1, m.softdrop);
                                _MACRO_HASH_POS(hash, n) |= 1 << 1;
                                q.push(nm);
                            }
                        } else if ( field.wallkickTest(nx, ny, getGem(cur.num, ns), 1 ) ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & ( 1 << 2 ) ) == 0 ) {
                                _MACRO_CREATE_MOVINGSIMPLE(MOV_RSPIN, 2, m.softdrop);
                                _MACRO_HASH_POS(hash, n) |= 1 << 2;
                                q.push(nm);
                            }
                        }
                    } else {
                        if ( ! field.isCollide(nx, ny, getGem(cur.num, ns) )
                            || field.wallkickTest(nx, ny, getGem(cur.num, ns), 1 ) ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & 1 ) == 0 ) {
                                _MACRO_CREATE_MOVINGSIMPLE(MOV_RSPIN, 0, m.softdrop);
                                _MACRO_HASH_POS(hash, n) |= 1;
                                q.push(nm);
                            }
                        }
                    }
                }
            }
            if ( spin180Enable() ) // no 180 wallkick only
            {
                int nx = m.x, ny = m.y, ns = (m.spin + 2) % cur.mod;
                if ( ns != m.spin ) {
                    if ( (isEnableAllSpin() || cur.num == GEMTYPE_T) ) {
                        if ( ! field.isCollide(nx, ny, getGem(cur.num, ns) ) ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & ( 1 << 1 ) ) == 0 ) {
                                _MACRO_CREATE_MOVINGSIMPLE(MOV_SPIN2, 1, m.softdrop);
                                _MACRO_HASH_POS(hash, n) |= 1 << 1;
                                q.push(nm);
                            }
                        } else if ( field.wallkickTest(nx, ny, getGem(cur.num, ns), 2 ) ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & ( 1 << 2 ) ) == 0 ) {
                                _MACRO_CREATE_MOVINGSIMPLE(MOV_SPIN2, 1, m.softdrop); // because it thinks its tmini
                                _MACRO_HASH_POS(hash, n) |= 1 << 2;
                                q.push(nm);
                            }
                        }
                    } else {
                        if ( ! field.isCollide(nx, ny, getGem(cur.num, ns) )
                            || field.wallkickTest(nx, ny, getGem(cur.num, ns), 2 ) ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & 1 ) == 0 ) {
                                _MACRO_CREATE_MOVINGSIMPLE(MOV_SPIN2, 0, m.softdrop);
                                _MACRO_HASH_POS(hash, n) |= 1;
                                q.push(nm);
                            }
                        }
                    }
                }
            }
        }
    }
	
	void FindPathMoving(const GameField& field, Moving& mov, Gem cur, int x, int y, bool hold, int goalx, int goaly, int goalr, int goalwk) {
        if ( field.isCollide(x, y, getGem(cur.num, cur.spin) ) ) {
            return ;
        }
        char _hash[64][4][GENMOV_W_MASK+1] = {0};
        char _hash_drop[64][4][GENMOV_W_MASK+1] = {0};
        char (*hash)[4][GENMOV_W_MASK+1] = &_hash[gem_add_y];
        char (*hash_drop)[4][GENMOV_W_MASK+1] = &_hash_drop[gem_add_y];
		goalwk = (goalwk < 0)? 0xFFFFFF : (1 << goalwk);
        MovQueue<Moving> q(1024);
        {
            Moving m;
            m.x = x;
            m.y = y;
            m.spin = cur.spin;
            m.wallkick_spin = 0;
            if ( hold ) {
                m.movs.push_back(Moving::MOV_HOLD);
            } else {
                m.movs.push_back(Moving::MOV_NULL);
            }
            m.score = 0;
            q.push(m);
            //hash[m.y][m.spin][m.x & GENMOV_W_MASK] = 1;
        }
        while ( ! q.empty() ) {
            Moving m;
            q.pop(m);
            if ( m.movs.back() == Moving::MOV_DROP) {
				if (m.x == goalx && m.y == goaly && m.spin == goalr && (1 << m.wallkick_spin & goalwk)) {
					mov = m;
					return;
				}
				else if (cur.num == AI::GEMTYPE_I || cur.num == AI::GEMTYPE_Z || cur.num == AI::GEMTYPE_S) {
					if ((goalr + 2) % 4 == m.spin) {
						if (goalr == 0) {
							if (m.x == goalx && m.y == goaly - 1 && (1 << m.wallkick_spin & goalwk)) {
								mov = m;
								return;
							}
						}
						else if (goalr == 1) {
							if (m.x == goalx - 1 && m.y == goaly && (1 << m.wallkick_spin & goalwk)) {
								mov = m;
								return;
							}
						}
						else if (goalr == 2) {
							if (m.x == goalx && m.y == goaly + 1 && (1 << m.wallkick_spin & goalwk)) {
								mov = m;
								return;
							}
						}
						else if (goalr == 3) {
							if (m.x == goalx + 1 && m.y == goaly && (1 << m.wallkick_spin & goalwk)) {
								mov = m;
								return;
							}
						}
					}
				}
				continue;
            }
            {
                if ( (isEnableAllSpin() || cur.num == GEMTYPE_T) ) {
                    if ( hash[m.y][m.spin][m.x & GENMOV_W_MASK] & ( 1 << m.wallkick_spin ) )
                        continue;
                    hash[m.y][m.spin][m.x & GENMOV_W_MASK] |= 1 << m.wallkick_spin;
                } else {
                    if ( hash[m.y][m.spin][m.x & GENMOV_W_MASK] & 1 )
                        continue;
                    hash[m.y][m.spin][m.x & GENMOV_W_MASK] |= 1;
                }
            }

            if ( m.movs.back() != Moving::MOV_DD && m.movs.back() != Moving::MOV_D)
            {
                int nx = m.x, ny = m.y, ns = m.spin;
                int wallkick_spin = m.wallkick_spin;
                //while ( field.row[ny + cur.geth()] == 0 && ny + cur.geth() <= field.height() ) { // �ǿ����в���ʹ�õ��Ż�
                //    ++ny; wallkick_spin = 0;
                //}
                while ( ! field.isCollide(nx, ny + 1, getGem(cur.num, ns) ) ) {
                    //if ( !USING_MOV_D && ( _MACRO_HASH_POS(hash, n) & 1 ) == 0) {
                    //    _MACRO_HASH_POS(hash, n) |= 1;
                    //}
                    ++ny; wallkick_spin = 0;
                }
                {
                    int v_spin = (isEnableAllSpin() || cur.num == GEMTYPE_T) ? wallkick_spin : 0;
                    if ( (_MACRO_HASH_POS(hash_drop, n) & ( 1 << v_spin )) == 0 )
                    {
                        int _nx = nx, _ny = ny, _ns = ns;
                        //if ( (_MACRO_HASH_POS(hash_drop, _n) & ( 1 << v_spin)) == 0 )
                        {
                                _MACRO_CREATE_MOVING(MOV_DROP, v_spin);
                                _MACRO_HASH_POS(hash_drop, _n) |= 1 << v_spin;
                                q.push(nm);
                        }
                    }
                    if ( softdropEnable() ) {
                        if ( ny != y ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & 1 ) == 0) {
                                _MACRO_CREATE_MOVING(MOV_DD, 0);
                                //_MACRO_HASH_POS(hash, n) |= 1;
                                nm.score += MOV_SCORE_DD - nm.movs.size();
                                q.push(nm);
                            }
                        }
                    }
                }
            }
            {
                int nx = m.x, ny = m.y, ns = m.spin;
                --nx;
                if ( ( _MACRO_HASH_POS(hash, n) & 1 ) == 0) {
                    if ( ! field.isCollide(nx, ny, getGem(cur.num, ns) ) ) {
                        _MACRO_CREATE_MOVING(MOV_L, 0);
                        //_MACRO_HASH_POS(hash, n) = 1;
                        if ( m.movs.back() != Moving::MOV_L )
                            nm.score += MOV_SCORE_LR;
                        else
                            nm.score += MOV_SCORE_LR2;
                        q.push(nm);
                        if (m.movs.back() != Moving::MOV_L && m.movs.back() != Moving::MOV_R
                            && m.movs.back() != Moving::MOV_LL && m.movs.back() != Moving::MOV_RR )
                        {
                            int cnt = 0;
                            int nx = m.x - 1, ny = m.y, ns = m.spin;
                            while ( ! field.isCollide(nx - 1, ny, getGem(cur.num, ns) ) ) {
                                --nx;
                                cnt++;
                            }
                            if ( nx != x && ( _MACRO_HASH_POS(hash, n) & 1 ) == 0) {
                                #ifdef GENERATE_LL_RR
                                    _MACRO_CREATE_MOVING(MOV_LL, 0);
                                #else
                                    _MACRO_CREATE_MOVING(MOV_L, 0);
                                    for (int i = 0; i < cnt; i++) nm.movs.push_back(Moving::MOV_L);
                                #endif
                                //_MACRO_HASH_POS(hash, n) |= 1;
                                nm.score += MOV_SCORE_LLRR;
                                q.push(nm);
                            }
                        }
                    }
                }
            }
            {
                int nx = m.x, ny = m.y, ns = m.spin;
                ++nx;
                if ( ( _MACRO_HASH_POS(hash, n) & 1 ) == 0) {
                    if ( ! field.isCollide(nx, ny, getGem(cur.num, ns) ) ) {
                        _MACRO_CREATE_MOVING(MOV_R, 0);
                        //_MACRO_HASH_POS(hash, n) |= 1;
                        if ( m.movs.back() != Moving::MOV_R )
                            nm.score += MOV_SCORE_LR;
                        else
                            nm.score += MOV_SCORE_LR2;
                        q.push(nm);
                        if (m.movs.back() != Moving::MOV_L && m.movs.back() != Moving::MOV_R
                            && m.movs.back() != Moving::MOV_LL && m.movs.back() != Moving::MOV_RR )
                        {
                            int cnt = 0;
                            int nx = m.x + 1, ny = m.y, ns = m.spin;
                            while ( ! field.isCollide(nx + 1, ny, getGem(cur.num, ns) ) ) {
                                ++nx;
                                cnt++;
                            }
                            if ( nx != x && ( _MACRO_HASH_POS(hash, n) & 1 ) == 0) {
                                #ifdef GENERATE_LL_RR
                                    _MACRO_CREATE_MOVING(MOV_RR, 0);
                                #else
                                    _MACRO_CREATE_MOVING(MOV_R, 0);
                                    for (int i = 0; i < cnt; i++) nm.movs.push_back(Moving::MOV_R);
                                #endif
                                //_MACRO_HASH_POS(hash, n) |= 1;
                                nm.score += MOV_SCORE_LLRR;
                                q.push(nm);
                            }
                        }
                    }
                }
            }
            //if (USING_MOV_D)
            if ( m.movs.back() != Moving::MOV_DD )
            {
                int nx = m.x, ny = m.y, ns = m.spin;
                ++ny;
                if ( ( _MACRO_HASH_POS(hash, n) & 1 ) == 0) {
                    if ( ! field.isCollide(nx, ny, getGem(cur.num, ns) ) ) {
                        _MACRO_CREATE_MOVING(MOV_D, 0);
                        //_MACRO_HASH_POS(hash, n) |= 1;
                        nm.score += MOV_SCORE_D;
                        q.push(nm);
                    }
                }
            }
            {
                int nx = m.x, ny = m.y, ns = (m.spin + 1) % cur.mod;
                if ( ns != m.spin ) {
                    if ( (isEnableAllSpin() || cur.num == GEMTYPE_T) ) {
                        if ( ! field.isCollide(nx, ny, getGem(cur.num, ns) ) ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & ( 1 << 1 ) ) == 0 ) {
                                _MACRO_CREATE_MOVING(MOV_LSPIN, 1);
                                //_MACRO_HASH_POS(hash, n) |= 1 << 1;
                                if ( m.movs.back() != Moving::MOV_LSPIN )
                                    nm.score += MOV_SCORE_LR;
                                else
                                    nm.score += MOV_SCORE_SPIN;
                                q.push(nm);
                            }
                        } else if ( field.wallkickTest(nx, ny, getGem(cur.num, ns), 0 ) ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & ( 1 << 2 ) ) == 0 ) {
                                _MACRO_CREATE_MOVING(MOV_LSPIN, 2);
                                //_MACRO_HASH_POS(hash, n) |= 1 << 2;
                                if ( m.movs.back() != Moving::MOV_LSPIN )
                                    nm.score += MOV_SCORE_LR;
                                else
                                    nm.score += MOV_SCORE_SPIN;
                                q.push(nm);
                            }
                        }
                    } else {
                        if ( ! field.isCollide(nx, ny, getGem(cur.num, ns) )
                            || field.wallkickTest(nx, ny, getGem(cur.num, ns), 0 ) ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & 1 ) == 0 ) {
                                _MACRO_CREATE_MOVING(MOV_LSPIN, 0);
                                //_MACRO_HASH_POS(hash, n) |= 1;
                                if ( m.movs.back() != Moving::MOV_LSPIN )
                                    nm.score += MOV_SCORE_LR;
                                else
                                    nm.score += MOV_SCORE_SPIN;
                                q.push(nm);
                            }
                        }
                    }
                }
            }
            {
                int nx = m.x, ny = m.y, ns = (m.spin + 3) % cur.mod;
                if ( ns != m.spin ) {
                    if ( (isEnableAllSpin() || cur.num == GEMTYPE_T) ) {
                        if ( ! field.isCollide(nx, ny, getGem(cur.num, ns) ) ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & ( 1 << 1 ) ) == 0 ) {
                                _MACRO_CREATE_MOVING(MOV_RSPIN, 1);
                                //_MACRO_HASH_POS(hash, n) |= 1 << 1;
                                if ( m.movs.back() != Moving::MOV_RSPIN )
                                    nm.score += MOV_SCORE_LR;
                                else
                                    nm.score += MOV_SCORE_SPIN;
                                q.push(nm);
                            }
                        } else if ( field.wallkickTest(nx, ny, getGem(cur.num, ns), 1 ) ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & ( 1 << 2 ) ) == 0 ) {
                                _MACRO_CREATE_MOVING(MOV_RSPIN, 2);
                                //_MACRO_HASH_POS(hash, n) |= 1 << 2;
                                if ( m.movs.back() != Moving::MOV_RSPIN )
                                    nm.score += MOV_SCORE_LR;
                                else
                                    nm.score += MOV_SCORE_SPIN;
                                q.push(nm);
                            }
                        }
                    } else {
                        if ( ! field.isCollide(nx, ny, getGem(cur.num, ns) )
                            || field.wallkickTest(nx, ny, getGem(cur.num, ns), 1 ) ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & 1 ) == 0 ) {
                                _MACRO_CREATE_MOVING(MOV_RSPIN, 0);
                                //_MACRO_HASH_POS(hash, n) |= 1;
                                if ( m.movs.back() != Moving::MOV_RSPIN )
                                    nm.score += MOV_SCORE_LR;
                                else
                                    nm.score += MOV_SCORE_SPIN;
                                q.push(nm);
                            }
                        }
                    }
                }
            }
            if ( spin180Enable() )
            {
                int nx = m.x, ny = m.y, ns = (m.spin + 2) % cur.mod;
                if ( ns != m.spin ) {
                    if ( (isEnableAllSpin() || cur.num == GEMTYPE_T) ) {
                        if ( ! field.isCollide(nx, ny, getGem(cur.num, ns) ) ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & ( 1 << 1 ) ) == 0 ) {
                                _MACRO_CREATE_MOVING(MOV_SPIN2, 1);
                                //_MACRO_HASH_POS(hash, n) |= 1 << 1;
                                if ( m.movs.back() != Moving::MOV_SPIN2)
                                    nm.score += MOV_SCORE_LR;
                                else
                                    nm.score += MOV_SCORE_SPIN;
                                q.push(nm);
                            }
                        } else if ( field.wallkickTest(nx, ny, getGem(cur.num, ns), 2 ) ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & ( 1 << 2 ) ) == 0 ) {
                                _MACRO_CREATE_MOVING(MOV_SPIN2, 1);  // because it thinks its tmini
                                //_MACRO_HASH_POS(hash, n) |= 1 << 2;
                                if ( m.movs.back() != Moving::MOV_SPIN2)
                                    nm.score += MOV_SCORE_LR;
                                else
                                    nm.score += MOV_SCORE_SPIN;
                                q.push(nm);
                            }
                        }
                    } else {
                        if ( ! field.isCollide(nx, ny, getGem(cur.num, ns) )
                            || field.wallkickTest(nx, ny, getGem(cur.num, ns), 2 ) ) {
                            if ( ( _MACRO_HASH_POS(hash, n) & 1 ) == 0 ) {
                                _MACRO_CREATE_MOVING(MOV_SPIN2, 0);
                                //_MACRO_HASH_POS(hash, n) |= 1;
                                if ( m.movs.back() != Moving::MOV_SPIN2)
                                    nm.score += MOV_SCORE_LR;
                                else
                                    nm.score += MOV_SCORE_SPIN;
                                q.push(nm);
                            }
                        }
                    }
                }
            }
        }
    }
}
