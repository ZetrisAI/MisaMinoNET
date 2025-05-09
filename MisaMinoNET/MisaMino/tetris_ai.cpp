#include "tetris_ai.h"
#include <deque>
#include <map>
#include <set>

#include <assert.h>

namespace AI {
    struct _ai_settings {
        bool hash;
        bool combo;
        bool strategy_4w;
        _ai_settings() {
            hash = true;
            combo = true;
            strategy_4w = false;
        }
    } ai_settings;

    void setAIsettings(const char* key, int val) {
        if ( strcmp(key, "hash") == 0 ) {
            ai_settings.hash = val;
        } else if ( strcmp(key, "combo") == 0 ) {
            ai_settings.combo = val;
        } else if ( strcmp(key, "4w") == 0 ) {
            ai_settings.strategy_4w = val;
        }
    }

    int Evaluate( long long &clearScore, double &avg_height, const AI_Param& ai_param, const GameField& last_pool, const GameField& pool, int cur_num,
        int curdepth,
        int total_clear_att, int total_clears, int clear_att, int clears, signed char wallkick_spin,
        int lastCombo, int t_dis, int upcomeAtt
        ) {
        int score = 0;
        // 测高度 // Measure height
        int min_y[32] = {0};
        int emptys[32] = {0};
        int maxy_index = 31, maxy_cnt = 0;
        int maxy_flat_cnt = 0; // 最长平台 // Longest flat platform
        int miny_val = 31;
        int total_hole = 0;
        int beg_y = -5;
        const int pool_w = pool.width(), pool_h = pool.height();
        min_y[31] = -1;
        {
            beg_y = -5;
            while ( pool.row[beg_y] == 0 ) ++beg_y;
            for ( int x = 0; x < pool_w; ++x) {
                for ( int y = beg_y, ey = pool_h + 1; y <= ey; ++y) { // 要有底行保护（pool.h），否则会挂 // Must include bottom row protection (pool.h), or it will crash
                    if ( pool.row[y] & ( 1 << x ) ) {
                        min_y[x] = y;
                        miny_val = std::min(miny_val, y);
                        if ( y > min_y[maxy_index]) {
                            maxy_index = x;
                            maxy_cnt = 0;
                        } else if ( y == min_y[maxy_index]) {
                            ++maxy_cnt;
                        }
                        break;
                    }
                }
            }
            int transitions = 0;
            for ( int y = beg_y; y <= pool_h; ++y) {
                int last = 1;
                if ( pool.row[y] > 0 ) {
                    for ( int x = 0; x < pool_w; ++x) {
                        if ( pool.row[y] & ( 1 << x ) ) {
                            if ( last == 0 ) ++transitions;
                            last = 1;
                        } else {
                            if ( last == 1 ) ++transitions;
                            last = 0;
                            ++emptys[y];
                        }
                    }
                } else {
                    emptys[y] = pool_w;
                }
                transitions += !last;
            }
            score += ai_param.v_transitions * transitions / 10;
        }
        min_y[pool_w] = min_y[pool_w-2];

        if ( pool.m_hold == GEMTYPE_I ) {
            score -= ai_param.hold_I;
        }
        if ( pool.m_hold == GEMTYPE_T ) {
            score -= ai_param.hold_T;
        }
        if ( maxy_cnt > 0 ) {
            int ybeg = min_y[maxy_index];
            unsigned rowdata = pool.row[ybeg-1];
            int empty = ~rowdata & pool.m_w_mask;
            for ( int b = maxy_index; b < pool_w; ++b ) {
                if ( ybeg != min_y[b] ) continue;
                int cnt = 1;
                for ( int b1 = b + 1; empty & ( 1 << b1); ++b1) ++cnt;
                if ( maxy_flat_cnt < cnt ) {
                    maxy_flat_cnt = cnt;
                    maxy_index = b;
                }
            }
        }
        // 洞的数量 // Number of holes
        int x_holes[32] = {0}; // 水平方向洞的数量 // Number of holes in the horizontal direction
        int y_holes[32] = {0}; // 垂直方向洞的数量 // Number of holes in the vertical direction
        int x_op_holes[32] = {0}; // 水平方向洞的数量 // Number of holes in the horizontal direction
        int pool_hole_score;
        int pool_total_cell = 0;
        {   // pool
            int first_hole_y[32] = {0}; // 垂直方向最近的洞的y // Y-coordinate of the nearest vertical hole
            int x_renholes[32] = {0}; // 垂直连续洞的数量 // Number of vertically consecutive holes
            double hole_score = 0;
            const GameField& _pool = pool;
            for ( int x = 0; x < pool_w; ++x) {
                for ( int y = min_y[x]; y <= pool_h; ++y ) {
                    if ( ( _pool.row[y] & ( 1 << x ) ) == 0 ) {
                        pool_total_cell++;
                    }
                }
            }
            for ( int x = 0; x < pool_w; ++x) {
                int last = 0, next;
                first_hole_y[x] = pool_h + 1;
                int y = (x>0) ? std::min(min_y[x] + 1, std::max(min_y[x-1] + 6, min_y[x+1] + 6)) : min_y[x] + 1;
                for ( ; y <= pool_h; ++y, last = next) {
                    if ( ( _pool.row[y] & ( 1 << x ) ) == 0 ) {
                        double factor = ( y < 20 ? ( 1 + (20 - y) / 20.0 * 2 ) : 1.0);
                        y_holes[x]++;
                        next = 1;
                        if ( x > 1 ) {
                            if (min_y[x-1] > y && min_y[x-2] > y) {
                                hole_score += ai_param.open_hole * factor;
                                if ( y >= 0 ) ++x_op_holes[y];
                                continue;
                            }
                        }
                        if ( x < pool_w - 2 ) {
                            if (min_y[x+1] > y && min_y[x+2] > y) {
                                hole_score += ai_param.open_hole * factor;
                                if ( y >= 0 ) ++x_op_holes[y];
                                continue;
                            }
                        }
                        if ( y >= 0 ) ++x_holes[y];
                        if ( first_hole_y[x] > pool_h ) {
                            first_hole_y[x] = y;
                        }
                        int hs = 0;
                        if ( last ) {
                            hs += ai_param.hole / 2;
                            if ( y >= 0 ) ++x_renholes[y];
                        } else {
                            hs += ai_param.hole * 2;
                        }
                        {
                            ++total_hole;
                        }
                        hole_score += hs * factor;
                    } else {
                        next = 0;
                    }
                }
            }
            for ( int y = 0; y <= pool_h; ++y) {
                if ( x_holes[y] > 0 ) {
                    score += ai_param.hole_dis * (pool_h - y + 1);
                    break;
                }
            }
            if ( ai_param.hole_dis_factor ) {
                for ( int y = 0, cnt = 5, index = -1; y <= pool_h; ++y) {
                    if ( x_holes[y] > 0 ) {
                        if ( cnt > 0 ) --cnt, ++index;
                        else break;
                        for ( int x = 0; x <= pool_w; ++x) {
                            if ( ( _pool.row[y] & ( 1 << x ) ) == 0) {
                                int h = y - min_y[x];
                                if ( h > 4 - index ) h = 4 + (h - (4 - index)) * cnt / 4;
                                if ( h > 0 ) {
                                    if ( ( _pool.row[y - 1] & ( 1 << x ) ) != 0) {
                                        score += ai_param.hole_dis_factor * h * cnt / 5 / 2;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if ( ai_param.hole_dis_factor2 ) {
                int miny = pool_h;
                for ( int y = 0, cnt = 0; y <= pool_h; ++y) {
                    if ( x_holes[y] > 0 ) {
                        if ( cnt < 4 ) ++cnt;
                        else break;
                        for ( int x = 0; x <= pool_w; ++x) {
                            if ( ( _pool.row[y] & ( 1 << x ) ) == 0) {
                                int vy = min_y[x] + cnt * 1;
                                if ( vy < miny ) miny = vy;
                            }
                        }
                    }
                }
                for ( int x = 0; x <= pool_w; ++x) {
                    int vy = min_y[x] + 6;
                    if ( vy < miny ) miny = vy;
                }
                double total_emptys = 0;
                for ( int y = miny; y <= pool_h; ++y ) {
                    total_emptys += emptys[y] * ( y < 10 ? (10 + 10 - y ) / 10.0 : 1);
                }
                score += ai_param.hole_dis_factor2 * total_emptys / 4;
            }
            pool_hole_score = hole_score;
        }
        score += pool_hole_score;

        // 高度差 // Height difference
        {
            int last = min_y[1];
            for ( int x = 0; x <= pool_w; last = min_y[x], ++x) {
                int v = min_y[x] - last;
                if ( x == maxy_index ) {
                    x += maxy_flat_cnt;
                    continue;
                }
                int absv = abs(v);
                if ( x + 1 == maxy_index && v > 0 || x - 2 == maxy_index && v < 0 ) ; //
                else score += absv * ai_param.h_factor;
            }
        }
        int center = 10; // 摆楼警戒线 // Center warning line (used for tower height caution)
        double warning_factor = 1;
        int h_variance_score = 0;
        // 算方差 // Calculate variance (height irregularity)
        {
            int avg = 0;
            {
                int sum = 0;
                int sample_cnt = 0;
                for ( int x = 0; x < pool_w; ++x) {
                    avg += min_y[x];
                }
                int h = std::min(std::min(min_y[gem_beg_x], min_y[gem_beg_x+1]), std::min(min_y[gem_beg_x+2], min_y[gem_beg_x+3]));
                if ( h < 8 )
                {
                    score += int(ai_param.miny_factor * ( 8 - h ) * 2);
                }
                if ( avg < pool_w * center ) {
                    warning_factor = 0.0 + (double)avg / pool_w / center / 1;
                }
                // 偏差值 // Deviation value
                {
                    int dif_sum = 0;
                    for ( int x = 0; x < pool_w; ++x) {
                        dif_sum += abs( min_y[x] * pool_w - avg );
                    }
                    score += ai_param.dif_factor * dif_sum / pool_w / pool_w;
                }
            }
            // 攻击计算 // Attack calculation
            {
                int s = 0;
                int t_att = total_clear_att;
                double t_clear = total_clears; //+ total_clears / 4.0;
                avg_height = pool_h + 1 - (double)avg / pool_w;
                // Add tanked amount to avg_height
                if (upcomeAtt < 0) avg_height -= upcomeAtt;
                if ( pool.b2b ) {
                    double b2b_score = (double)ai_param.b2b * 5;
                    double b2b_height = avg_height;
                    // Don't drop off b2b_score if TSD_only because there is no opponent, there is no danger
                    if (!TSD_only) {
                        b2b_score /= 1 + pow(5, b2b_height - 6.5); // z
                    }
                    if (tetris_game == 2) {
                        // https://www.desmos.com/calculator/enes1mgnoo
                        // small extra bonus if we're low on the board
                        if (b2b_height < 6) {
                            b2b_score *= (66 - b2b_height) / 60;
                        }
                    }
                    b2b_score += 2; // b
                    s -= (int)b2b_score;
                }
                if ( t_clear > 0 ) {
                    s -= int( ((ai_param.clear_efficient) * ( t_att ) ) );
                }
                {
                    //if ( t_clear > t_att ) {
                        //int warning_factor = 0.5 + (double)avg / pool_w / center / 2;
                        s += int( warning_factor * t_clear * ai_param.clear_useless_factor);
                    //}
                }
                int cs = 0;
                // T消附加分，要比T1/T2形状基本分大一 // T-spin bonus scoring, should be higher than base score of T1/T2 shapes
                if ( cur_num == GEMTYPE_T && wallkick_spin && clears > 0 && ai_param.tspin > 0 ) {
                    s -= ai_param.hold_T;
                    if ( clears >= 3 ) {
                        if ( clear_att >= clears * 2 ) { // T3
                            cs -= int( warning_factor * (ai_param.tspin3 * 8 ) + ai_param.hole * 2 );
                        }
                    } else if ( clears >= 2 ) {
                        if ( clear_att >= clears * 2 ) { // T2
                            cs -= int( warning_factor * (ai_param.tspin * 5 + ai_param.open_hole / 2) );
                        }
                    } else if ( wallkick_spin == 1 ) { // T1
                        cs -= int( warning_factor * (ai_param.tspin * 1 + ai_param.open_hole / 2) );
                    } else if ( wallkick_spin == 2 ) { // Tmini
                        if (tmini_old_behavior) {
                            // Restore old misa mini behavior without use of tmini AI_Param
                            cs -= int(warning_factor * (ai_param.tspin / 2));
                        } else {
                            cs -= int(warning_factor * (ai_param.tmini / 2));
                            if (ai_param.tmini == 0) cs += 100000000;
                        }
                    }
                }
                clearScore += cs;

                if ( clears > 0 && upcomeAtt >= 4 && ai_param.upcomeAtt > 0 ) {
                    int cur_s = 0;
                    cur_s -= int( ((ai_param.clear_efficient) * ( clear_att ) ) )
                        - int( warning_factor * clears * ai_param.clear_useless_factor);
                    if ( avg - (12 + upcomeAtt) * pool_w > 0 && cur_s + cs < 0 )
                        s -= (cur_s + cs) * ( avg - (12 + upcomeAtt) * pool_w ) * ai_param.upcomeAtt / pool_w / 10 / 20;
                }

                score += s;
            }
        }

        // 特殊形状判定 // Special shape detection

        // 计算可攻击（Tetris和T2）// Evaluate potential attacks (Tetris and T-Spin Double)
        if ( maxy_cnt == 0 )
        {
            int ybeg = 0;
            if ( maxy_index > 0 && maxy_index < pool_w - 1 && ai_param.tspin > 0 ) { // T1/T2基本形状分 // Basic shape recognition for T1/T2
                ybeg = std::max( min_y[maxy_index - 1], min_y[maxy_index + 1] );
                if ( min_y[maxy_index - 1] == min_y[maxy_index + 1]
                    && x_holes[ybeg] == 0 && (!ybeg || x_holes[ybeg-1] == 0)
                    && x_op_holes[ybeg] == 0 && (!ybeg || x_op_holes[ybeg-1] == 0)
                    )
                { // T准备 // T-Spin setup detected
                    int cnt = 0;
                    if ( maxy_index > 1 && min_y[maxy_index - 2] >= min_y[maxy_index - 1] - 2 ) ++cnt;
                    if ( maxy_index < pool_w - 2 && min_y[maxy_index + 2] >= min_y[maxy_index + 1] - 2 ) ++cnt;
                    if ( cnt > 0 )
                    {
                        score -= int(warning_factor * ai_param.tspin);
                        if ( (~pool.row[ybeg] & pool.m_w_mask) == (1 << maxy_index) ) { // T1基础 // Valid T1 setup
                            score -= int(warning_factor * ai_param.tspin);
                            if ( (~pool.row[ybeg - 1] & pool.m_w_mask) == (7 << (maxy_index-1) ) ) { // 可T2完美坑 // Perfect T2 pit
                                score -= int( warning_factor * (ai_param.tspin * cnt) );
                            }
                        }
                    }
                } else if ( ybeg <= 6 && ybeg - t_dis > 1 || ybeg > 6 ) {
                    int row_data = pool.row[ybeg - 1];
                    if ( (row_data & ( 1 << (maxy_index-1) ) ) == 0 && (row_data & ( 1 << (maxy_index+1) ) ) == 0 // 坑的左右为空 // Left and right of the pit are empty
                         && x_holes[ybeg] == 0 && (!ybeg || x_holes[ybeg-1] == 0) // 其它位置无洞 // No holes in other nearby rows
                         && x_op_holes[ybeg] == 0 && (!ybeg || x_op_holes[ybeg-1] <= 1)
                         )
                    {
                        // T坑形状 // T-shaped hole detection
                        if ( ( pool.row[ybeg] & (1 << (maxy_index-1)) ) && ( pool.row[ybeg] & (1 << (maxy_index+1)) ) ) { // 坑的下面两块存在 // Pit base blocks are present
                            if ( !!( pool.row[ybeg-2] & (1 << (maxy_index-1)) ) + !!( pool.row[ybeg-2] & (1 << (maxy_index+1)) ) == 1 ) { // 坑的上面的块存在 // One side block above the pit
                                double s = 0;
                                double factor = ybeg > 6 ? 0.5 : 1 - t_dis / 6.0 * 0.5;
                                if ( warning_factor < 1 )
                                    factor = ybeg > 6 ? 1.0 / 5 : 1 / (1 + t_dis / 3.0);
                                s += ai_param.open_hole;
                                if ( (~pool.row[ybeg] & pool.m_w_mask) == (1 << maxy_index) ) { // 可T1 // Valid T1 setup
                                    s += ai_param.tspin + ai_param.tspin * 1 * factor;
                                    if ( (~row_data & pool.m_w_mask) == (7 << (maxy_index-1) ) ) { // 可T2完美坑 // Perfect T2 pit
                                        s += ai_param.tspin * 3 * factor;
                                    }
                                } else {
                                    s += ai_param.tspin * 1 + ai_param.tspin * 2 * factor / 2 ;
                                }
                                score -= int( warning_factor * s );
                            }
                        }
                    }
                }
            } else {
                if ( maxy_index == 0 ) {
                    ybeg = min_y[maxy_index + 1];
                } else {
                    ybeg = min_y[maxy_index - 1];
                }
            }
            int readatt = 0;
            int last = pool.row[ybeg];
            for ( int y = ybeg; y <= pool_h; ++y ) {
                if ( last != pool.row[y] ) break;
                int row_data = ~pool.row[y] & pool.m_w_mask;
                if ( (row_data & (row_data - 1)) != 0 ) break;
                ++readatt;
            }
            if ( readatt > 4 ) readatt = 4;
        }
        // T3 形状判定 // T-Spin Triple shape detection
        //3001	
        //2000	
        // 1101	
        // 1x01	
        // 1101	
        // Fumen visualization: https://tinyurl.com/y2ygrqtw 
        // 1003
        // 0002
        //1011 
        //10x1
        //1011
        if ( ai_param.tspin3 > 0 )
        {
            for ( int y = 3; y < pool_h; ++y ) {
                if ( x_holes[y] == 0 ) continue;
                for ( int x = 1; x < pool_w - 1; ++x ) {
                    if ( ( pool.row[y + 1] & ( 1 << x ) ) == 0 || ( pool.row[y + 1] & ( 1 << x ) ) == 0  ) {
                        continue; // 上下无洞 // No hole above and below
                    }
                    int row_y[5];
                    for ( int i = 0; i < 5; ++i ) {
                        row_y[i] = ( (pool.row[y - 3 + i] | (3 << pool_w)) << 2 ) | 3;
                    }
                    if ( ( (row_y[3] >> (x + 1)) & ( 7 ) ) == 1 /*100*/ ) { // 上图情况 // Match the target bit pattern (T3 center)
                        if ( x == pool_w - 2 ) continue;
                        // 所有空的地方先匹配 // Check if all empty regions match expected T3 pattern
                        if (   ( (row_y[2] >> (x + 1)) & ( 7 ) ) != 3 /*110*/
                            //|| ( (row_y[4] >> (x + 1)) & ( 15 ) ) != 11 /*1101*/
                            || ( (row_y[4] >> (x + 1)) & ( 13 ) ) != 9 /*1011mask=1001*/
                            || ( (row_y[1] >> (x + 1)) & ( 7 ) ) != 0 /*000*/
                            //|| ( (row_y[0] >> (x + 1)) & ( 3 ) ) != 0 /*00*/
                            ) {
                            continue;
                        }
                        if ( min_y[x] != y - 1 || min_y[x-1] != y - 1 ) {
                            continue;
                        }
                        if ( ( row_y[0] & ( 1 << (x) ) ) == 0 && ( row_y[1] & ( 1 << (x) ) ) ) {
                            continue; // 高处转角 // High-turn corner, not valid
                        }
                        if ( min_y[x + 1] > y ) { // 洞判定 // Hole detection when right column is taller
                            if ( x_holes[y - 1] > 0 || x_holes[y + 1] > 0 || x_holes[y] > 1
                                || x_op_holes[y - 1] > 0 || x_op_holes[y + 1] > 0 || x_op_holes[y] > 0)
                            {
                                continue;
                            }
                        } else {
                            if ( x_holes[y - 1] > 1 || x_holes[y + 1] > 1 || x_holes[y] > 2
                                || x_op_holes[y - 1] > 0 || x_op_holes[y + 1] > 0 || x_op_holes[y] > 0)
                            {
                                continue;
                            }
                        }
                        if ( ( (row_y[0] >> (x + 3)) & ( 1 ) ) == 0 && y - min_y[x + 2] > 3 ) continue;
                        int s = 0;
                        s -= int( warning_factor * ai_param.tspin3 );
                        score += s;
                        if ( y <= pool_h - 3 && ( pool.row[y + 3] & ( 1 << x ) ) == 0 ) {
                            int r = ~pool.row[y + 3] & pool.m_w_mask;
                            if ( ( r & ( r - 1 ) ) == 0 ) {
                                score -= int( warning_factor * (ai_param.tspin * 4 + ai_param.open_hole) );
                            }
                        }
                        //int full = 0;
                        {
                            // TODO: There's likely a small modification that can be made here to allow for STSDs
                            int e = ~(pool.row[y + 1] | (1<<x) ) & pool.m_w_mask;
                            e &= ( e-1); 
                            if ( e == 0 ) { // 最底只剩一空 // Bottom row (y+1) has only one empty cell left
                                //++full;
                            } else {
                                score -= s;
                                continue;
                            }
                        }
                        {
                            int e = ~(pool.row[y] | (1<<(x+2))) & pool.m_w_mask;
                            e &= ( e-1 );
                            if ( (e & ( e-1)) == 0 ) { // 底二只剩两空 // Second row from bottom (y) has only two empty cells
                                //++full;
                            } else {
                                if ( (pool.row[y] & (1<<(x+2))) == 0 ) {
                                    score -= int( warning_factor * ai_param.tspin3 * 3 );
                                }
                                score -= s;
                                score -= int( warning_factor * ai_param.tspin3 / 3 );
                                continue;
                            }
                        }
                        {
                            int e = ~pool.row[y - 1] & pool.m_w_mask;
                            e &= ( e-1 );
                            if ( e == 0 ) { // 底三只剩一空 // Third row from bottom (y-1) has only one empty cell left
                                //++full;
                            } else {
                                score -= s;
                                score -= int( warning_factor * ai_param.tspin3 );
                                continue;
                            }
                        }
                        score -= int( warning_factor * ai_param.tspin3 * 3 );
                        if ( pool.row[y-3] & ( 1 << (x + 1)) ) {
                            if ( t_dis < 7 ) {
                                score -= int( warning_factor * ( ai_param.tspin3 * 1 ) + ai_param.hole * 2);
                                score -= int( warning_factor * ai_param.tspin3 * 3 / ( t_dis + 1 ) );
                            }
                        }
                    } else if ( ( (row_y[3] >> (x+1) ) & ( 7 ) ) == 4 /*001*/ ) { // 镜像情况 // Mirrored T3 pattern (horizontal reflection)
                        if ( x == 1 ) continue;
                        // 所有空的地方先匹配 // Match all empty cells to expected T3 mirrored pattern
                        if (   ( (row_y[2] >> (x+1)) & ( 7 ) ) != 6 /*011*/
                            //|| ( (row_y[4] >> (x)) & ( 15 ) ) != 13 /*1011*/
                            || ( (row_y[4] >> (x)) & ( 11 ) ) != 9 /*1101mask=1001*/
                            || ( (row_y[1] >> (x + 1)) & ( 7 ) ) != 0 /*000*/
                            //|| ( (row_y[0] >> (x + 1)) & ( 3 ) ) != 0 /*00*/
                            ) {
                            continue;
                        }
                        if ( min_y[x] != y - 1 || min_y[x+1] != y - 1 ) {
                            continue;
                        }
                        if ( ( row_y[0] & ( 1 << (x + 4) ) ) == 0 && ( row_y[1] & ( 1 << (x + 4) ) ) ) {
                            continue; // 高处转角 // High-turn corner present — disqualifies this setup
                        }
                        if ( min_y[x - 1] > y ) { // 洞判定 // Hole check when left column is taller
                            if ( x_holes[y - 1] > 0 || x_holes[y + 1] > 0 || x_holes[y] > 1
                                || x_op_holes[y - 1] > 0 || x_op_holes[y + 1] > 0 || x_op_holes[y] > 0)
                            {
                                continue;
                            }
                        } else {
                            if ( x_holes[y - 1] > 1 || x_holes[y + 1] > 1 || x_holes[y] > 2
                                || x_op_holes[y - 1] > 0 || x_op_holes[y + 1] > 0 || x_op_holes[y] > 0)
                            {
                                continue;
                            }
                        }
                        if ( ( (row_y[0] >> (x + 1)) & ( 1 ) ) == 0 && y - min_y[x - 2] > 3 ) continue;
                        int s = 0;
                        s -= int( warning_factor * ai_param.tspin3 );
                        score += s;
                        if ( y <= pool_h - 3 && ( pool.row[y + 3] & ( 1 << x ) ) == 0 ) {
                            int r = ~pool.row[y + 3] & pool.m_w_mask;
                            if ( ( r & ( r - 1 ) ) == 0 ) {
                                score -= int( warning_factor * (ai_param.tspin * 4 + ai_param.open_hole) );
                            }
                        }
                        //int full = 0;
                        {
                            int e = ~(pool.row[y + 1] | (1<<x) ) & pool.m_w_mask;
                            e &= ( e-1);
                            if ( e == 0 ) { // 最底只剩一空 // Bottom row (y + 1) has only one empty cell
                                //++full;
                            } else {
                                score -= s;
                                continue;
                            }
                        }
                        {
                            int e = ~(pool.row[y] | (1<<(x-2))) & pool.m_w_mask;
                            e &= ( e-1);
                            if ( (e & ( e-1)) == 0 ) { // 底二只剩两空 // Second row from bottom (y) has only two empty cells
                                //++full;
                            } else {
                                if ( (pool.row[y] & (1<<(x-2))) == 0 ) {
                                    score -= int( warning_factor * ai_param.tspin3 * 3 );
                                }
                                score -= s;
                                score -= int( warning_factor * ai_param.tspin3 / 3 );
                                continue;
                            }
                        }
                        {
                            int e = ~pool.row[y - 1] & pool.m_w_mask;
                            e &= ( e-1);
                            if ( e == 0 ) { // 底三只剩一空 // Third row from bottom (y - 1) has only one empty cell
                                //++full;
                            } else {
                                score -= s;
                                score -= int( warning_factor * ai_param.tspin3 );
                                continue;
                            }
                        }
                        score -= int( warning_factor * ai_param.tspin3 * 3 );
                        if ( pool.row[y-3] & ( 1 << (x - 1)) ) {
                            if ( t_dis < 7 ) {
                                score -= int( warning_factor * ( ai_param.tspin3 * 1 ) + ai_param.hole * 2);
                                score -= int( warning_factor * ai_param.tspin3 * 3 / ( t_dis + 1 ) );
                            }
                        }
                    }
                }
            }
        }
        // 4W形状判定 // 4-wide pattern detection
        if ( ai_param.strategy_4w > 0 && total_clears < 1 )
        {
            int maxy_4w = min_y[3];
            maxy_4w = std::max(maxy_4w, min_y[4] );
            maxy_4w = std::max(maxy_4w, min_y[5] );
            maxy_4w = std::max(maxy_4w, min_y[6] );
            int maxy_4w_combo = min_y[0];
            maxy_4w_combo = std::max(maxy_4w_combo, min_y[1] );
            maxy_4w_combo = std::max(maxy_4w_combo, min_y[2] );
            maxy_4w_combo = std::max(maxy_4w_combo, min_y[pool_w-3] );
            maxy_4w_combo = std::max(maxy_4w_combo, min_y[pool_w-2] );
            maxy_4w_combo = std::max(maxy_4w_combo, min_y[pool_w-1] );
            if ( (min_y[4] < min_y[3] && min_y[4] <= min_y[5])
                || (min_y[5] < min_y[6] && min_y[5] <= min_y[4]) )
            {
                maxy_4w = -10;
            } else
            for ( int x = 0; x < pool_w; ++x ) {
                if ( min_y[x] > maxy_4w ) {
                    maxy_4w = -10;
                    break;
                }
            }
            while ( maxy_4w > 0 ) {
                if ( (pool_h - maxy_4w) * 2 >= maxy_4w - maxy_4w_combo ) {
                    maxy_4w = -10;
                    break;
                }
                break;
            }
            if ( maxy_4w <= pool_h - 4 ) { // If there are more than 4 garbage lines, don't build 4-wide
                maxy_4w = -10;
            }
            if ( maxy_4w - maxy_4w_combo < 9 && pool_hole_score > ai_param.hole * (maxy_4w - maxy_4w_combo) / 2 ) {
                maxy_4w = -10;
            }

            if ( maxy_4w > 8 ) {
                bool has_hole = false;
                for ( int y = maxy_4w - 1; y >= 0; --y ) {
                    if ( x_holes[y] || x_op_holes[y] ) {
                        has_hole = true;
                        break;
                    }
                }
                if ( ! has_hole && maxy_4w < pool_h ) {
                    if ( x_holes[maxy_4w]>1 || x_op_holes[maxy_4w]>1 ) {
                        has_hole = true;
                    }
                }

                if ( ! has_hole )
                {
                    int sum = maxy_4w - min_y[3];
                    sum += maxy_4w - min_y[4];
                    sum += maxy_4w - min_y[5];
                    sum += maxy_4w - min_y[6];
                    int s = 0;
                    if ( sum == 3 || sum == 0 || sum == 4 )
                    {
                        int hv = (maxy_4w - maxy_4w_combo + 1) * 1 + pool.combo;
                        s += ai_param.strategy_4w * ( hv ) + (ai_param.hole * 2 + ai_param.tspin * 4);
                        if ( sum > 0 ) {
                            s -= ai_param.strategy_4w / 3;
                        }
                    }
                    if ( s > 0 ) {
                        score -= s;
                    }
                }
            }
        }

        int i = 0;
        int highest_row = -1;
        for (; pool.m_row[i] != -1; i++) {
            if (highest_row < 0 && pool.m_row[i]) highest_row = i;
        }
        int boardheight = highest_row < 0 ? 0 : (i - highest_row);

        // Perfect Clear scoring (no detection - that's sfinder's job)
        // In TETR.IO Season 2, PCs from misa aren't that great
        if (tetris_game < 2 && boardheight <= 0) {
            clearScore -= 1000000;
        }

        // In TETR.IO Season 2, prioritize All-Spin Singles the lower we are on the board
        if (ai_param.ass_bonus && tetris_game == 2 && getAllowedSpins() >= 1 &&
            ((wallkick_spin != 0 && clears == 1) || (cur_num == AI::GEMTYPE_T && wallkick_spin != 0 && clears == 2)) // is ASS or TSD
        ) {
            const int ass_bonus_lookup[] = {
                40, 40, 40, 40,
                30, 20, 20, 10,
                10, 10,  9,  8,
                 7,  6,  4,  2
            };
            if (boardheight < (sizeof(ass_bonus_lookup) / sizeof(ass_bonus_lookup[0]))) {
                if (wallkick_spin != 0 && clears == 1) {
                    clearScore -= ass_bonus_lookup[boardheight] * ai_param.ass_bonus;
                }
            }
        }

		if (TSD_only) {
            // Force ASS only clears
            if (getAllowedSpins() >= 1) {
                if (wallkick_spin != 0 && clears == 1) clearScore -= 100000000;
                else if (clears != 0) clearScore += 100000000;

            // Force TSD only clears
            } else {
                if (cur_num == AI::GEMTYPE_T) {
                    if (wallkick_spin != 0 && clears == 2) clearScore -= 100000000;
                    else clearScore += 100000000;

                } else if (clears != 0) clearScore += 100000000;
            }
        }

        // 累积分 // Accumulate score
        score += clearScore;
        return score;
    }
    struct MovsState {
        MovingSimple first;
        GameField pool_last;
        int att, clear;
        signed short max_combo, max_att, combo;
        signed short upcomeAtt;
        MovsState() { upcomeAtt = max_combo = combo = att = clear = 0; }
        bool operator < (const MovsState& m) const {
			if ( ai_settings.strategy_4w )
			{
				if ( ( max_combo > 6 * 32 || m.max_combo > 6 * 32 ) ) {
					if ( max_combo != m.max_combo ) {
						return max_combo < m.max_combo;
					}
				}
				if ( (combo >= 32 * 3 || m.combo >= 32 * 3) && combo != m.combo) {
					return combo < m.combo;
				}
			}
			else
			{
				if ( ai_settings.combo ) {
					if ( ( max_combo > 6 * 32 || m.max_combo > 6 * 32 ) ) {
						if ( max_combo != m.max_combo ) {
							return max_combo < m.max_combo;
						}
					}
					if ( max_combo > combo && m.max_combo > m.combo && (m.max_combo > 4 * 32 || max_combo > 4 * 32) ) {
						if ( (combo <= 2 * 32 && m.combo <= 2 * 32) ) {
							long long diff = first.score - m.first.score;
							if ( -1000 < diff && diff < 1000 ) {
								if ( att != m.att )
									return att < m.att;
							} else {
								return first < m.first;
							}
						}
					}
				}
				if ( (combo > 32 * 2 || m.combo > 32 * 2) && combo != m.combo) {
					return combo < m.combo;
				}
			}
            return first < m.first;
        }
    };
    struct GameState{
        uint64 hash;
        signed short hold, att, clear, combo, b2b;
        GameState(uint64 _hash
            ,signed short _hold
            ,signed short _att
            ,signed short _clear
            ,signed short _combo
            ,signed short _b2b
            )
            :hash(_hash)
            ,hold(_hold)
            ,att(_att)
            ,combo(_combo)
            ,b2b(_b2b)
        {
        }
        bool operator < ( const GameState& gs) const {
            if ( hash != gs.hash ) return hash < gs.hash;
            if ( hold != gs.hold ) return hold < gs.hold;
            if ( att != gs.att ) return att < gs.att;
            if ( clear != gs.clear ) return clear < gs.clear;
            if ( combo != gs.combo ) return combo < gs.combo;
            if ( b2b != gs.b2b ) return b2b < gs.b2b;
            return false;
        }
        bool operator == ( const GameState& gs) const {
            if ( hash != gs.hash ) return false;
            if ( hold != gs.hold ) return false;
            if ( att != gs.att ) return false;
            if ( clear != gs.clear ) return false;
            if ( combo != gs.combo ) return false;
            if ( b2b != gs.b2b ) return false;
            return true;
        };
    };
    MovingSimple AISearch(AI_Param ai_param, const GameField& pool, int hold, Gem cur, int x, int y, const std::vector<int>& next, bool canhold, int upcomeAtt, int maxDeep, int & searchDeep, bool second_choice) {
		if (cur.num == 0) { // rare race condition, we're dead already if this happens
			assert(true); // debug break
			cur = AI::getGem(AI::GEMTYPE_I, 0);
		}
		int level = 10;
        typedef std::vector<MovingSimple> MovingList;
        MovingList movs;
        MovQueue<MovsState> que(16384);
        MovQueue<MovsState> que2(16384);
        movs.reserve(128);
        int search_nodes = 0;
        const int combo_step_max = 32;
        searchDeep = 0;
        upcomeAtt = std::min(upcomeAtt, pool.height() - gem_beg_y - 1);
        if ( pool.combo > 0 && (pool.row[10] || pool.combo > 1) ) ai_param.strategy_4w = 0;
        if ( ai_param.hole < 0 ) ai_param.hole = 0;
        ai_param.hole += ai_param.open_hole;
        
        int next_add = 0;
        if ( pool.m_hold == 0 ) {
            next_add = 1;
            if ( next.empty() ) {
                //return MovingSimple();
            }
        }

        {
            const GameField& _pool = pool;
            int t_dis = 14;
            if ( _pool.m_hold != GEMTYPE_T ) {
                for ( size_t i = 0; i < next.size(); ++i ) {
                    if ( next[i] == GEMTYPE_T ) {
                        t_dis = i;
                        break;
                    }
                }
            } else {
                t_dis = 0;
            }
            GenMoving(_pool, movs, cur, x, y, 0, false);
            for (MovingList::iterator it = movs.begin(); it != movs.end() && !Abort(); ++it) {
                ++search_nodes;
                MovsState &ms = que.append();
                ms.pool_last = _pool;
                signed char wallkick_spin = it->wallkick_spin;
                wallkick_spin = ms.pool_last.WallKickValue(cur.num, (*it).x, (*it).y, (*it).spin, wallkick_spin);
                ms.pool_last.paste((*it).x, (*it).y, getGem(cur.num, (*it).spin));
                int clear = ms.pool_last.clearLines( wallkick_spin );
                int att = ms.pool_last.getAttack( clear, wallkick_spin );
                ms.clear = clear;
                ms.att = att;
                if ( clear > 0 ) {
                    ms.combo = (_pool.combo + 1) * combo_step_max * ai_param.combo / 30;
                    ms.upcomeAtt = std::max(0, upcomeAtt - att);
                } else {
                    ms.combo = 0;
                    ms.upcomeAtt = -upcomeAtt;
                }
                ms.max_att = att;
                ms.max_combo = ms.combo;
                ms.first = *it;
                ms.first.score2 = 0;
				double h = 0;
                ms.first.score = Evaluate(ms.first.score2, h, ai_param, pool, ms.pool_last, cur.num, 0, ms.att, ms.clear, att, clear, wallkick_spin, _pool.combo, t_dis, upcomeAtt);
                if ( wallkick_spin == 0 && it->wallkick_spin ) ms.first.score += 1;

				ms.first.score += score_avoid_softdrop(ai_param.avoid_softdrop, it->softdrop, cur.num, it->wallkick_spin, h);
                que.push_back();
            }
        }
        if ( canhold && ! hold &&
            (
                pool.m_hold == 0
                && !next.empty() && ! pool.isCollide(gem_beg_x, gem_beg_y, getGem( next[0], 0 ) )
                || ! pool.isCollide(gem_beg_x, gem_beg_y, getGem( pool.m_hold, 0 ) )
            )
            )
        if (next.size() > 0){
            int cur_num;
            if ( pool.m_hold ) {
                cur_num = pool.m_hold;
            } else {
                cur_num = next[0];
            }
            if ( cur_num != cur.num ) {
                GameField _pool = pool;
                _pool.m_hold = cur.num;
                int t_dis = 14;
                if ( _pool.m_hold != GEMTYPE_T ) {
                    for ( size_t i = 0; i + next_add < next.size(); ++i ) {
                        if ( next[i + next_add] == GEMTYPE_T ) {
                            t_dis = i;
                            break;
                        }
                    }
                } else {
                    t_dis = 0;
                }
                int x = gem_beg_x, y = gem_beg_y;
                Gem cur = getGem( cur_num, 0 );
                GenMoving(_pool, movs, cur, x, y, 1, false);
                for (MovingList::iterator it = movs.begin(); it != movs.end() && !Abort(); ++it) {
                    ++search_nodes;
                    MovsState &ms = que.append();
                    ms.pool_last = _pool;
                    signed char wallkick_spin = it->wallkick_spin;
                    wallkick_spin = ms.pool_last.WallKickValue(cur_num, (*it).x, (*it).y, (*it).spin, wallkick_spin);
                    ms.pool_last.paste((*it).x, (*it).y, getGem(cur_num, (*it).spin));
                    int clear = ms.pool_last.clearLines( wallkick_spin );
                    int att = ms.pool_last.getAttack( clear, wallkick_spin );
                    ms.clear = clear;
                    ms.att = att;
                    if ( clear > 0 ) {
						ms.combo = (_pool.combo + 1) * combo_step_max * ai_param.combo / 30;
                        ms.upcomeAtt = std::max(0, upcomeAtt - att);
                    } else {
                        ms.combo = 0;
                        ms.upcomeAtt = -upcomeAtt;
                    }
                    ms.max_att = att;
                    ms.max_combo = ms.combo;
                    ms.first = *it;
                    ms.first.score2 = 0;
                    double h = 0;
                    ms.first.score = Evaluate(ms.first.score2, h, ai_param, pool, ms.pool_last, cur_num, 0, ms.att, ms.clear, att, clear, wallkick_spin, _pool.combo, t_dis, upcomeAtt);
                    if ( wallkick_spin == 0 && it->wallkick_spin ) ms.first.score += 1;

					ms.first.score += score_avoid_softdrop(ai_param.avoid_softdrop, it->softdrop, cur_num, it->wallkick_spin, h);
                    que.push_back();
                }
            }
        }
        if ( que.empty() ) {
            return MovingSimple();
        }

		int sw_map[5] = {40, 300, 200, 150, 100};
		for (int i = 0; i < 5; i++) {
			sw_map[i] = sw_map[i] * sw_map_multiplier / 100;
		}

        MovQueue<MovsState> * pq_last = &que2, * pq = &que;
        searchDeep = 1;
		int final_depth = 65535;

		int depth = 0;
        for (; depth < maxDeep; searchDeep = ++depth ) {
			if (Abort()) break;

            std::swap(pq_last, pq);

            int search_base_width = sw_map[0];
            int search_wide = sw_map[(depth > 4)? 4 : depth];
            
            int search_select_best = std::min(search_wide - search_wide / 4, search_base_width);
            if ( level <= 3 ) {
                search_select_best = search_wide - search_wide / 4;
            }
            if ( search_select_best < search_base_width ) {
                search_select_best = std::min(search_base_width, std::max(15, search_wide) );
            }
            pq->clear();
            int max_combo = 3;
            long long max_search_score = pq_last->back().first.score;
            {
                for ( int s = pq_last->size(), i = s / 2; i < s; ++i ) {
                    max_search_score = std::max((long long)max_search_score, pq_last->queue[i].first.score );
                }
                max_search_score = (max_search_score * 2 + pq_last->front().first.score) / 3;
            }
            std::set<GameState> gsSet;
            // TODO: Multithread here.
            for ( int pqmax_size = (int)pq_last->size(),
                pq_size = (int)pq_last->size(),
                stop_size = std::max(0, (int)pq_size - search_wide);
                pq_size > stop_size;

                --pq_size, pq_last->dec_size())
            {

                if ( pq_size > 1 ) pq_last->pop_back();

                const MovsState &ms_last = pq_last->back();
                if ( pq_size != pqmax_size && ms_last.first.score > 50000000 ) { // 超高分剪枝 // Prune extremely high-score paths
                    break;
                }
                if (Abort()) {
					if (final_depth > depth) final_depth = depth;

                    pq->push(ms_last);
                    break;
                }
                max_combo = std::max( max_combo, (int)ms_last.pool_last.combo );
                if ( ai_settings.hash )
                {
                    GameState gs(ms_last.pool_last.hashval, ms_last.pool_last.m_hold, ms_last.att, ms_last.clear, ms_last.combo, ms_last.pool_last.b2b);
                    if ( gsSet.find(gs) == gsSet.end() ) {
                        gsSet.insert(gs);
                    } else {
                        continue;
                    }
                }
                int t_dis = 14;
                int d_pos = depth;
                if ( next_add && ms_last.pool_last.m_hold ) d_pos = depth + 1;
                if ( d_pos >= next.size() ) {
                    pq->push(ms_last);
                    continue;
                }
                int cur_num = next[d_pos];
                if ( ms_last.pool_last.m_hold != GEMTYPE_T ) {
                    for ( size_t i = 0; d_pos + 1 + i < next.size(); ++i ) {
                        if ( next[d_pos + 1 + i] == GEMTYPE_T ) {
                            t_dis = i;
                            break;
                        }
                    }
                } else {
                    t_dis = 0;
                }
                if ( ms_last.upcomeAtt < 0 )
                    GenMoving(ms_last.pool_last, movs, getGem( cur_num, 0 ), AI::gem_beg_x, AI::gem_beg_y - ms_last.upcomeAtt, 0, ms_last.combo > 0);
                else
                    GenMoving(ms_last.pool_last, movs, getGem( cur_num, 0 ), AI::gem_beg_x, AI::gem_beg_y, 0, ms_last.combo > 0);
                if ( movs.empty() ) {
                    MovsState ms = ms_last;
                    ms.first.score += 100000000;
                    pq->push(ms);
                    continue; // 出现就挂的话使用hold的情况不能跑 // If it crashes immediately, fallback to using hold (can't proceed directly)
                } else {
                    MovQueue<MovsState> p(movs.size());
                    for (size_t i = 0; (i < movs.size()) && !Abort(); ++i) {
                        ++search_nodes;
                        MovsState &ms = p.append();
                        {
                            ms.first = ms_last.first;
                            ms.pool_last = ms_last.pool_last;
                            signed char wallkick_spin = movs[i].wallkick_spin;
                            wallkick_spin = ms.pool_last.WallKickValue(cur_num, movs[i].x, movs[i].y, movs[i].spin, wallkick_spin);
                            ms.pool_last.paste(movs[i].x, movs[i].y, getGem(cur_num, movs[i].spin));
                            int clear = ms.pool_last.clearLines( wallkick_spin );
                            int att = ms.pool_last.getAttack( clear, wallkick_spin );
                            ms.clear = clear + ms_last.clear;
                            ms.att = att + ms_last.att;
                            ms.upcomeAtt = ms_last.upcomeAtt;
                            if ( clear > 0 ) {
                                ms.combo = ms_last.combo + (combo_step_max + 1 - clear) * ai_param.combo / 30;
                                if ( ms_last.upcomeAtt > 0 )
                                    ms.upcomeAtt = std::max(0, ms_last.upcomeAtt - att);
                            } else {
                                ms.combo = 0;
                                if ( ms_last.upcomeAtt > 0 ) {
                                    ms.upcomeAtt = -ms_last.upcomeAtt;
                                }
                            }
                            ms.max_att = std::max((int)ms_last.max_att, ms_last.att + att);
                            ms.max_combo = std::max(ms_last.max_combo, ms.combo);
                            ms.first.score2 = ms_last.first.score2;
                            double h = 0;
                            ms.first.score = Evaluate(ms.first.score2, h, ai_param,
                                pool,
                                ms.pool_last, cur_num, depth + 1, ms.att, ms.clear, att, clear, wallkick_spin, ms_last.pool_last.combo, t_dis, ms_last.upcomeAtt);
                            if ( wallkick_spin == 0 && movs[i].wallkick_spin ) ms.first.score += 1;

							ms.first.score += score_avoid_softdrop(ai_param.avoid_softdrop, movs[i].softdrop, cur_num, movs[i].wallkick_spin, h);
                        }
                        p.push_back();
                    }
                    for ( int i = 0; i < search_select_best && ! p.empty(); ++i) {
                        pq->push(p.front());
                        p.pop_back();
                        p.dec_size();
                    }
                }
                if ( canhold && depth + next_add < next.size() && !Abort())
                {
                    MovsState ms_last = pq_last->back();
                    int cur_num;
                    int d_pos = depth + next_add;
                    if ( ms_last.pool_last.m_hold != next[d_pos] ) {
                        if ( ms_last.pool_last.m_hold ) {
                            cur_num = ms_last.pool_last.m_hold;
                        } else {
                            cur_num = next[d_pos];
                        }
                        ms_last.pool_last.m_hold = next[d_pos];
                        if ( ms_last.pool_last.m_hold != GEMTYPE_T ) {
                            t_dis -= next_add;
                            if ( t_dis < 0 ) {
                                for ( size_t i = 0; d_pos + 1 + i < next.size(); ++i ) {
                                    if ( next[i + 1 + d_pos] == GEMTYPE_T ) {
                                        t_dis = i;
                                        break;
                                    }
                                }
                            }
                        } else {
                            t_dis = 0;
                        }
                        if ( ms_last.upcomeAtt < 0 )
                            GenMoving(ms_last.pool_last, movs, getGem( cur_num, 0 ), AI::gem_beg_x, AI::gem_beg_y - ms_last.upcomeAtt, 1, ms_last.combo > 0);
                        else
                            GenMoving(ms_last.pool_last, movs, getGem( cur_num, 0 ), AI::gem_beg_x, AI::gem_beg_y, 1, ms_last.combo > 0);
                        if ( movs.empty() ) {
                            MovsState ms = ms_last;
                            ms.first.score += 100000000;
                            pq->push(ms);
                        } else {
                            MovQueue<MovsState> p(movs.size());
                            for (size_t i = 0; i < movs.size() && !Abort(); ++i) {
                                ++search_nodes;
                                MovsState &ms = p.append();
                                {
                                    ms.first = ms_last.first;
                                    ms.pool_last = ms_last.pool_last;
                                    signed char wallkick_spin = movs[i].wallkick_spin;
                                    wallkick_spin = ms.pool_last.WallKickValue(cur_num, movs[i].x, movs[i].y, movs[i].spin, wallkick_spin);
                                    ms.pool_last.paste(movs[i].x, movs[i].y, getGem(cur_num, movs[i].spin));
                                    int clear = ms.pool_last.clearLines( wallkick_spin );
                                    int att = ms.pool_last.getAttack( clear, wallkick_spin );
                                    ms.clear = clear + ms_last.clear;
                                    ms.att = att + ms_last.att;
                                    ms.upcomeAtt = ms_last.upcomeAtt;
                                    if ( clear > 0 ) {
										ms.combo = ms_last.combo + (combo_step_max + 1 - clear) * ai_param.combo / 30;
                                        if ( ms_last.upcomeAtt > 0 )
                                            ms.upcomeAtt = std::max(0, ms_last.upcomeAtt - att);
                                    } else {
                                        ms.combo = 0;
                                        if ( ms_last.upcomeAtt > 0 ) {
                                            ms.upcomeAtt = -ms_last.upcomeAtt;
                                        }
                                    }
                                    ms.max_att = std::max((int)ms_last.max_att, ms_last.att + att);
                                    ms.max_combo = std::max(ms_last.max_combo, ms.combo);
                                    ms.first.score2 = ms_last.first.score2;
                                    double h = 0;
                                    ms.first.score = Evaluate(ms.first.score2, h, ai_param,
                                        pool,
                                        ms.pool_last, cur_num, depth + 1, ms.att, ms.clear, att, clear, wallkick_spin, ms_last.pool_last.combo, t_dis, ms_last.upcomeAtt);

									ms.first.score += score_avoid_softdrop(ai_param.avoid_softdrop, movs[i].softdrop, cur_num, movs[i].wallkick_spin, h);
                                    if ( wallkick_spin == 0 && movs[i].wallkick_spin ) ms.first.score += 1;
                                }
                                p.push_back();
                            }
                            for ( int i = 0; i < search_select_best && ! p.empty(); ++i) {
                                pq->push(p.front());
                                p.pop_back();
                                p.dec_size();
                            }
                        }
                    }
                }
            }
            if ( pq->empty() ) {
                return MovingSimple();
            }
        }
		if (final_depth > depth) final_depth = depth;

        if ( ai_settings.hash && canhold && !Abort() ) { // extra search the leftover held piece
            std::swap(pq_last, pq);
            pq->clear();
            int depth = searchDeep - 1;
            
            int search_base_width = sw_map[0];
			int search_wide = sw_map[(depth > 4)? 4 : depth];
            
            int search_select_best = std::min(search_wide - search_wide / 4, search_base_width);
            if ( level <= 3 ) {
                search_select_best = search_wide - search_wide / 4;
            }
            if ( search_select_best < search_base_width ) {
                search_select_best = std::min(search_base_width, std::max(15, search_wide) );
            }

            std::set<GameState> gsSet;
            // TODO: Is this ever reachable?
            for ( int pqmax_size = (int)pq_last->size(),
                pq_size = (int)pq_last->size(),
                stop_size = std::max(0, (int)pq_size - search_wide);
                pq_size > stop_size;

                --pq_size, pq_last->dec_size())
            {

                if ( pq_size > 1 ) pq_last->pop_back();

                const MovsState &ms_last = pq_last->back();
                if ( pq_size != pqmax_size && ms_last.first.score > 50000000 ) { // 超高分剪枝 // Prune extremely high-score paths
                    break;
                }
                pq->push(ms_last);
                if ( Abort() ) {
					break;
                }
                {
                    GameState gs(ms_last.pool_last.hashval, ms_last.pool_last.m_hold, ms_last.att, ms_last.clear, ms_last.combo, ms_last.pool_last.b2b);
                    if ( gsSet.find(gs) == gsSet.end() ) {
                        gsSet.insert(gs);
                    } else {
                        continue;
                    }
                }
                if ( !ms_last.first.hold ) {
                    continue;
                }
                int t_dis = 14;
                int d_pos = depth + next_add;
                int cur_num = ms_last.pool_last.m_hold;
                for ( size_t i = 0; d_pos + 1 + i < next.size(); ++i ) {
                    if ( next[d_pos + 1 + i] == GEMTYPE_T ) {
                        t_dis = i;
                        break;
                    }
                }
                if ( ms_last.upcomeAtt < 0 )
                    GenMoving(ms_last.pool_last, movs, getGem( cur_num, 0 ), AI::gem_beg_x, AI::gem_beg_y - ms_last.upcomeAtt, 1, ms_last.combo > 0);
                else
                    GenMoving(ms_last.pool_last, movs, getGem( cur_num, 0 ), AI::gem_beg_x, AI::gem_beg_y, 1, ms_last.combo > 0);
                if ( movs.empty() ) {
                    MovsState ms = ms_last;
                    ms.first.score += 100000000;
                    pq->push(ms);
                } else {
                    MovQueue<MovsState> p;
                    for (size_t i = 0; i < movs.size() && !Abort(); ++i) {
                        ++search_nodes;
                        MovsState &ms = p.append();
                        {
                            ms.first = ms_last.first;
                            ms.pool_last = ms_last.pool_last;
                            signed char wallkick_spin = movs[i].wallkick_spin;
                            wallkick_spin = ms.pool_last.WallKickValue(cur_num, movs[i].x, movs[i].y, movs[i].spin, wallkick_spin);
                            ms.pool_last.paste(movs[i].x, movs[i].y, getGem(cur_num, movs[i].spin));
                            int clear = ms.pool_last.clearLines( wallkick_spin );
                            int att = ms.pool_last.getAttack( clear, wallkick_spin );
                            ms.clear = clear + ms_last.clear;
                            ms.att = att + ms_last.att;
                            ms.upcomeAtt = ms_last.upcomeAtt;
                            if ( clear > 0 ) {
								ms.combo = ms_last.combo + (combo_step_max + 1 - clear) * ai_param.combo / 30;
                                if ( ms_last.upcomeAtt > 0 )
                                    ms.upcomeAtt = std::max(0, ms_last.upcomeAtt - att);
                            } else {
                                ms.combo = 0;
                                if ( ms_last.upcomeAtt > 0 ) {
                                    ms.upcomeAtt = -ms_last.upcomeAtt;
                                }
                            }
                            ms.max_att = std::max((int)ms_last.max_att, ms_last.att + att);
                            ms.max_combo = std::max(ms_last.max_combo, ms.combo);
                            ms.first.score2 = ms_last.first.score2;
                            double h = 0;
                            ms.first.score = Evaluate(ms.first.score2, h, ai_param,
                                pool,
                                ms.pool_last, cur_num, depth + 1, ms.att, ms.clear, att, clear, wallkick_spin, ms_last.pool_last.combo, t_dis, ms_last.upcomeAtt);

							ms.first.score += score_avoid_softdrop(ai_param.avoid_softdrop, movs[i].softdrop, cur_num, movs[i].wallkick_spin, h);
                        }
                        p.push_back();
                    }
                    for ( int i = 0; i < search_select_best && ! p.empty(); ++i) {
                        pq->push(p.front());
                        p.pop_back();
                        p.dec_size();
                    }
                }
            }
            if ( pq->empty() ) {
                return MovingSimple();
            }
        }
        // Return best result
        {
            MovingSimple* m;
            std::swap(pq_last, pq);
            m = &pq_last->queue[0].first;

            for (int i = 1; i < pq_last->size(); i++) {
                MovingSimple* c = &pq_last->queue[i].first;
                if (m->score > c->score) {
                    m = c;
                }
            }

            if (second_choice) {
                MovingSimple* m2 = nullptr;

                for (int i = 0; i < pq_last->size(); i++) {
                    MovingSimple* c = &pq_last->queue[i].first;
					if (*c == *m) continue;

                    // prevent AI from just swapping piece order lol
                    if (c->hold != m->hold) continue;

                    if (m2 == nullptr || m2->score > c->score) {
                        m2 = c;
                    }
                }

				if (m2 != nullptr) {
					m = m2;
                } else {
                    // All other first moves had been pruned. Just move the piece its whatever
                    if (m->x < 5) m->x++;
                    else m->x--;
                }
            }

			last_nodes = search_nodes;
			last_depth = final_depth;
            return *m;
        }
    }
	int score_avoid_softdrop(int param, bool sd, int cur, bool wk, double h) {
        return TSD_only ? 0 : (int)((double)((sd && !((cur == AI::GEMTYPE_T || getAllowedSpins() >= 1) && wk)) ? param * 5 : 0) / (1 + pow(5, h - 6.5)));
	}
    struct AI_THREAD_PARAM {
        Moving* ret_mov;
        int* flag;
        AI_Param ai_param;
        GameField pool;
        int hold;
        Gem cur;
        int x;
        int y;
        std::vector<int> next;
        bool canhold;
        int upcomeAtt;
        int maxDeep;
        int *searchDeep;
        bool second_choice;
        AI_THREAD_PARAM(Moving& _ret_mov, int& _flag, const AI_Param& _ai_param, const GameField& _pool, int _hold, Gem _cur, int _x, int _y, const std::vector<int>& _next, bool _canhold, int _upcomeAtt, int _maxDeep, int & _searchDeep, bool _second_choice) {
            ret_mov = &_ret_mov;
            flag = &_flag;
            ai_param = _ai_param;
            pool = _pool;
            hold = _hold;
            cur = _cur;
            x = _x;
            y = _y;
            next = _next;
            canhold = _canhold;
            upcomeAtt = _upcomeAtt;
            maxDeep = _maxDeep;
            searchDeep = &_searchDeep;
            second_choice = _second_choice;
        }
        ~AI_THREAD_PARAM() {
        }
    };
    AI::Gem AI_Thread( void* lpParam ) {
		AI::Gem cur = AI::getGem(AI::GEMTYPE_NULL, 0);
        
        AI_THREAD_PARAM* p = (AI_THREAD_PARAM*)lpParam;
        int searchDeep = 0;
        *p->flag = 1;

        AI::MovingSimple best = AISearch(p->ai_param, p->pool, p->hold, p->cur, p->x, p->y, p->next, p->canhold, p->upcomeAtt, p->maxDeep, searchDeep, p->second_choice);
        AI::Moving mov;
        const AI::GameField &gamefield = p->pool;
        std::vector<int> &gemNext = p->next;
        mov.movs.push_back(Moving::MOV_DROP);
        if ( best.x != AI::MovingSimple::INVALID_POS ) { // find path
            int hold_num = gamefield.m_hold;
            if ( gamefield.m_hold == 0 && ! gemNext.empty()) {
                hold_num = gemNext[0];
            }
            if ( best.hold ) {
                cur = AI::getGem(hold_num, 0);
				FindPathMoving(gamefield, mov, cur, AI::gem_beg_x, AI::gem_beg_y, true, best.x, best.y, best.spin, best.wallkick_spin);
            } else {
                cur = p->cur;
				FindPathMoving(gamefield, mov, cur, p->x, p->y, false, best.x, best.y, best.spin, best.wallkick_spin);
            }
        }
        if ( mov.movs.empty() ) {
            mov.movs.clear();
            mov.movs.push_back( AI::Moving::MOV_NULL );
            mov.movs.push_back( AI::Moving::MOV_DROP );
        }
        *p->ret_mov = mov;

        *p->searchDeep = searchDeep;
        *p->flag = -1;
        delete p;
        
        return cur;
    }
    AI::Gem RunAI(Moving& ret_mov, int& flag, const AI_Param& ai_param, const GameField& pool, int hold, Gem cur, int x, int y, const std::vector<int>& next, bool canhold, int upcomeAtt, int maxDeep, int & searchDeep, bool second_choice) {
        flag = 0;
        return AI_Thread(new AI_THREAD_PARAM(ret_mov, flag, ai_param, pool, hold, cur, x, y, next, canhold, upcomeAtt, maxDeep, searchDeep, second_choice));
    }
}
