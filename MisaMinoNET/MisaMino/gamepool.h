#pragma once
#define _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH

#include <string.h>
#include <cstdint>
#include <iostream>
#include "tetris_gem.h"
#include "stuff.h"
#define AI_POOL_MAX_H 50

namespace AI {
    struct GameField;
    Gem& getGem( int number, int spin );
    int getComboAttack( int combo );
    void setAllowedSpins(int allowedSpins);
    int getAllowedSpins();
    void setSoftdrop( bool softdrop );
    bool softdropEnable();
    typedef uint64_t uint64;
    void InitHashTable();
    uint64 hash(const GameField & pool);
    const int gem_add_y = 20;
    const int gem_beg_x = 3;
    const int gem_beg_y = 1;

    struct GameField {
        signed char m_w, m_h;
        signed short combo;
        int b2b;
        unsigned long m_w_mask;
        unsigned long m_row[AI_POOL_MAX_H];
        int m_hold;
        int m_pc_att;
        uint64 hashval;
        unsigned long *row;
        GameField () {
            row = &m_row[gem_add_y];
        }
        GameField ( const GameField& field ) {
            row = &m_row[gem_add_y];
            *this = field;
        }
        GameField (signed char w, signed char h) {
            row = &m_row[gem_add_y];
            reset(w, h);
        }
        friend std::ostream& operator<<(std::ostream &os, const GameField &obj){
            os<<"\n";
            for (int i = AI::gem_add_y + 1; i < AI_POOL_MAX_H-5; ++i) {
            //for (int i = 0; i < AI_POOL_MAX_H; ++i) {
                os<<i<<"|";
                for (int k = obj.width()-1; k >=0; --k) {
                    if((obj.m_row[i] & (1<<k)) == 1<<k){
                        os<<"1";
                    }else{
                        os<<"0";
                    }
                }
                os<<"\n";
            }
        }
        int width() const { return m_w; }
        int height() const { return m_h; }
        void reset (signed char w, signed char h) {
            m_w = w;
            m_h = h;
            m_pc_att = 10;
            m_w_mask = ( 1 << w ) - 1;
            for (int i = 0; i < AI_POOL_MAX_H; ++i) {
                m_row[i] = 0;
            }
            for (int i = gem_add_y + m_h + 1; i < AI_POOL_MAX_H; ++i) {
                m_row[i] = (unsigned)-1;
            }
        }
        GameField& operator = (const GameField& field) {
            memcpy( this, &field, (size_t)&row - (size_t)this );
            row = m_row + ( field.row - field.m_row );
            return *this;
        }
        bool operator == (const GameField& field) const {
            if ( m_w != field.m_w || m_h != field.m_h ) return false;
            if ( m_hold != field.m_hold ) return false;
            if ( combo != field.combo ) return false;
            if ( b2b != field.b2b ) return false;
            if ( row - m_row != field.row - field.m_row ) return false;
            for ( int i = 0; i <= m_h + gem_add_y; ++i ) {
                if ( m_row[i] != field.m_row[i] ) return false;
            }
            return true;
        }

        inline
        bool isCollide(int y, const Gem & gem) const {
            for ( int h = 3; h >= 0; --h ) {
                if ( y + h > m_h && gem.bitmap[h] ) return true;
                if ( row[y + h] & gem.bitmap[h] ) return true;
            }
            return false;
        }

        inline
        bool isCollide(int x, int y, const Gem & gem) const {
            Gem _gem = gem;
            for ( int h = 0; h < 4; ++h ) {
                if ( x < 0 ) {
                    if (gem.bitmap[h] & ( ( 1 << (-x) ) - 1 ) ) return true;
                    _gem.bitmap[h] >>= -x;
                } else {
                    if ( (gem.bitmap[h] << x) & ~m_w_mask ) return true;
                    _gem.bitmap[h] <<= x;
                }
                if ( y + h > m_h && _gem.bitmap[h] ) return true;
                if ( row[y + h] & _gem.bitmap[h] ) return true;
            }
            return false; //isCollide(y, _gem);
        }
        /* WALLKICK TEST COMPARED TO TETRIO
            FLIP X VALUE SIGN.

            OR 01 = 30
            OL 03 = 10

            LO 30 = 01
            L2 32 = 21

            2L 23 = 12
            2R 21 = 32

            R2 12 = 23
            R0 10 = 03

            02 02 = 20
            LR 31 = 31
            20 20 = 02
            RL 13 = 13
        
        */
        bool wallkickTest(int& x, int& y, const Gem & gem, int spinclockwise) const {
            const int Iwallkickdata[4][2][4][2] = {
                { // O
                    { // R
                        { 2, 0},{-1, 0},{ 2,-1},{-1, 2},
                    },
                    { // L
                        { 1, 0},{-2, 0},{ 1, 2},{-2,-1},
                    },
                },
                { // L
                    { // O
                        {-1, 0},{ 2, 0},{-1,-2},{ 2, 1},
                    },
                    { // 2
                        { 2, 0},{-1, 0},{ 2,-1},{-1, 2},
                    },
                },
                { // 2
                    { // L
                        {-2, 0},{ 1, 0},{-2, 1},{ 1,-2},
                    },
                    { // R
                        {-1, 0},{ 2, 0},{-1,-2},{ 2, 1},
                    },
                },
                { // R
                    { // 2
                        { 1, 0},{-2, 0},{ 1, 2},{-2,-1},
                    },
                    { // O
                        {-2, 0},{ 1, 0},{-2, 1},{ 1,-2},
                    },
                },
            };
            const int srsplusIwallkickdata[4][2][4][2] = {
                { // O
                    { // R 1
                        {-1, 0},{ 2, 0},{-1, 2},{ 2,-1},
                    },
                    { // L 3
                        { 1, 0},{-2, 0},{ 1, 2},{-2,-1},
                    },
                },
                { // L 3
                    { // O
                        {-1, 0},{ 2, 0},{ 2, 1},{-1,-2},
                    },
                    { // 2
						{ 2, 0},{-1, 0},{ 2,-1},{-1, 2},
                    },
                },
                { // 2
                    { // L 3
                        { 1, 0},{-2, 0},{ 1,-2},{-2, 1},
                    },
                    { // R 1
                        {-1, 0},{ 2, 0},{-1,-2},{ 2, 1},
                    },
                },
                { // R 1
                    { // 2
                        {-2, 0},{ 1, 0},{-2,-1},{ 1, 2},
                    },
                    { // O
                        { 1, 0},{-2, 0},{-2, 1},{ 1,-2},
                    },
                },
            };
            const int wallkickdata[4][2][4][2] = {
                { // O
                    { // R
                        { 1, 0},{ 1, 1},{ 0,-2},{ 1,-2},
                    },
                    { // L
                        {-1, 0},{-1, 1},{ 0,-2},{-1,-2},
                    },
                },
                { // L
                    { // O
                        { 1, 0},{ 1,-1},{ 0, 2},{ 1, 2},
                    },
                    { // 2
                        { 1, 0},{ 1,-1},{ 0, 2},{ 1, 2},
                    },
                },
                { // 2
                    { // L
                        {-1, 0},{-1, 1},{ 0,-2},{-1,-2},
                    },
                    { // R
                        { 1, 0},{ 1, 1},{ 0,-2},{ 1,-2},
                    },
                },
                { // R
                    { // 2
                        {-1, 0},{-1,-1},{ 0, 2},{-1, 2},
                    },
                    { // O
                        {-1, 0},{-1,-1},{ 0, 2},{-1, 2},
                    },
                },
            };
			const int wallkick180data[4][5][2] = {
				{	//O -> 2
					{ 0, 1},{ 1, 1},{-1, 1},{ 1, 0},{-1, 0}
				},
                {	//L -> R
                    { 1, 0},{ 1,-2},{ 1,-1},{ 0,-2},{ 0,-1}
				},
                {	//2 -> O
                    { 0,-1},{-1,-1},{ 1,-1},{-1, 0},{ 1, 0}
				},
                {	//R -> L
                    {-1, 0},{-1,-2},{-1,-1},{ 0,-2},{ 0,-1}
				}
			};

			if (spinclockwise != 2) {
				const int (*pdata)[2][4][2] = wallkickdata;
				if ( gem.num == 1 ) pdata = tetris_game? srsplusIwallkickdata : Iwallkickdata;
				for (int itest = 0; itest < 4; ++itest) {
					int dx = x + pdata[gem.spin][spinclockwise][itest][0];
					int dy = y + pdata[gem.spin][spinclockwise][itest][1];
					if (!isCollide(dx, dy, gem)) {
						x = dx; y = dy;
						return true;
					}
				}
			} else {
                int kicks = gem.num == 1? 1 : 5;
				for (int i = 0; i < kicks; i++) {
					int dx = x + wallkick180data[gem.spin][i][0];
					int dy = y + wallkick180data[gem.spin][i][1];

					if (!isCollide(dx, dy, gem)) {
						x = dx; y = dy;
						return true;
					}
				}
			}
            return false;
        }
        void paste(int x, int y, const Gem & gem) {
            for ( int h = 0; h < gem.geth(); ++h ) {
                if (x >= 0)
                    row[y + h] |= gem.bitmap[h] << x;
                else
                    row[y + h] |= gem.bitmap[h] >> (-x);
            }
        }
        signed char isWallKickSpin(int x, int y, const Gem & gem) const {
            bool allspin = getAllowedSpins() == 2 || (getAllowedSpins() == 1 && gem.num != 2);
            if (allspin) {
                if ( isCollide( x - 1, y, gem )
                    && isCollide( x + 1, y, gem )
                    && isCollide( x, y - 1, gem )) {
                        return 1;
                }
            } else if ( gem.num == 2 ) { //T
                int cnt = 0;
                if ( x < 0 || (row[y] & (1 << x))) ++cnt;
                if ( x < 0 || y+2 > m_h || (row[y+2] & (1 << x))) ++cnt;
                if ( x+2 >= m_w || (row[y] & (1 << (x+2)))) ++cnt;
                if ( x+2 >= m_w || y+2 > m_h || (row[y+2] & (1 << (x+2)))) ++cnt;
                if ( cnt >= 3 ) return 1;
            }
            return 0;
        }
        signed char WallKickValue(int gem_num, int x, int y, int spin, signed char wallkick_spin) const {
            const Gem & gem = getGem(gem_num, spin);
            if ( ! isWallKickSpin( x, y, gem ) ) {
                return wallkick_spin = 0;
            }
            bool allspin = getAllowedSpins() == 2 || (getAllowedSpins() == 1 && gem.num != 2);
            if (allspin) {
                if ( wallkick_spin == 2) {
                    wallkick_spin = 1;
                    Gem g = getGem(gem_num, spin);
                    for ( int dy = 0; dy < 4; ++dy ) { //KOS mini test
                        if ( g.bitmap[dy] == 0 ) continue;
                        if ( ((g.bitmap[dy] << x) | row[y+dy]) == m_w_mask ) continue;
                        wallkick_spin = 2;
                        break;
                    }
                }
            } else {
                if ( wallkick_spin == 2 ) {
                    if ( ! isCollide( x, y, getGem(gem_num, spin^2) ) ) {
                        wallkick_spin = 1; // not t-mini
                    }
                }
            }
            return wallkick_spin;
        }
        int clearLines( signed char _wallkick_spin ) {
            int clearnum = 0;
            int h2 = m_h;
            for (int h = m_h; h >= -gem_add_y; --h) {
                if ( row[h] != m_w_mask) {
                    row[h2--] = row[h];
                } else {
                    ++ clearnum;
                }
            }
            for (int h = h2; h >= -gem_add_y; --h) {
                row[h] = 0;
            }
            if ( clearnum > 0 ) {
                ++combo;
                if ( clearnum == 4 ) {
                    ++b2b;
                } else if ( _wallkick_spin > 0 ) {
                    ++b2b;
                } else {
                    b2b = 0;
                }
            } else {
                combo = 0;
            }
            hashval = hash(*this);
            return clearnum;
        }
        int getPCAttack() const {
            return m_pc_att;
        }
        int getAttack( int clearfull, signed char wallkick ) {
            double attack = 0;

            if (tetris_game_is_PPT() || tetris_game_is_BotrisBattle()) { // PPT garbage logic
                if (clearfull > 1) {
                    if (clearfull < 4) {
                        attack = clearfull - 1;
                    }
                    else {
                        attack = clearfull;
                        if (b2b > 1) attack += 1;
                    }
                }
                if (clearfull > 0) {
                    if (wallkick) {
                        if (b2b > 1) attack += 1;
                        if (clearfull == 1) {
                            if (wallkick != 2) {
                                attack += 2;
                            }
                        }
                        else {
                            attack += clearfull + 1;
                        }
                    }

                    attack += getComboAttack(combo);

                    int i = gem_add_y + m_h;
                    for (; i >= 0; --i) {
                        if (m_row[i]) break;
                    }
                    if (i < 0) {
                        attack = m_pc_att; // pc
                    }
                }
            }
            else if (tetris_game_is_TETRIO()) { // TETR.IO garbage logic
                // TODO: TETR.IO S2
                if (clearfull > 1) {
                    if (clearfull < 4) {
                        attack = clearfull - 1;
                    }
                    else {
                        attack = clearfull;
                    }
                }
                if (clearfull > 0) {
                    if (wallkick) {
                        if (clearfull == 1) {
                            if (wallkick != 2) {
                                attack += 2;
                            }
                        }
                        else {
                            attack += clearfull + 1;
                        }
                    }

                    if (b2b > 1) {
                        double ln = log((b2b - 1) * 0.8 + 1);
                        attack += (int)(1 + ln) + (b2b == 2? 0 : ((1 + fmod(ln, 1)) / 3));
                    }

                    attack *= 1 + (combo - 1) / 4;
                    attack = fmax(log((combo - 1) * 1.25 + 1), attack);

                    int i = gem_add_y + m_h;
                    for (; i >= 0; --i) {
                        if (m_row[i]) break;
                    }
                    if (i < 0) {
                        attack += m_pc_att; // pc
                    }
                }
            }
            
            return (int)attack;
        }
        void addRow( int rowdata ) {
            for ( int h = -gem_add_y + 1; h <= m_h; ++h ) {
                row[h-1] = row[h];
            }
            row[m_h] = rowdata;
        }
        void minusRow( int lines ) {
            //row += lines;
            //m_h -= lines;
        }
    };
}
