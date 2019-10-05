using System;

namespace MisaMinoNET {
    public struct AI_Param {
        public int miny_factor;
        public int hole;
        public int open_hole;
        public int v_transitions;
        public int tspin3;

        public int clear_efficient;
        public int upcomeAtt;
        public int h_factor;
        public int hole_dis_factor2;
        public int hole_dis;

        public int hole_dis_factor;
        public int tspin;
        public int hold_T;
        public int hold_I;
        public int clear_useless_factor;

        public int dif_factor;
        public int b2b;
        public int combo;
        public int avoid_softdrop;
        public int tmini;

        public int strategy_4w;
    };

    public class MisaMinoParameters {
        public AI_Param Parameters;

        public MisaMinoParameters(
            int miny_factor = 16, int hole = 9, int open_hole = 11, int v_transitions = 17, int tspin3 = 17,
            int clear_efficient = 25, int upcomeAtt = 39, int h_factor = 2, int hole_dis_factor2 = 12, int hole_dis = 19,
            int hole_dis_factor = 7, int tspin = 24, int hold_T = 18, int hold_I = 7, int clear_useless_factor = 14,
            int dif_factor = 19, int b2b = 25, int combo = 30, int avoid_softdrop = 19, int tmini = 0,
            int strategy_4w = 0
        ) {
            Parameters.miny_factor = miny_factor;
            Parameters.hole = hole;
            Parameters.open_hole = open_hole;
            Parameters.v_transitions = v_transitions;
            Parameters.tspin3 = tspin3;

            Parameters.clear_efficient = clear_efficient;
            Parameters.upcomeAtt = upcomeAtt;
            Parameters.h_factor = h_factor;
            Parameters.hole_dis_factor2 = hole_dis_factor2;
            Parameters.hole_dis = hole_dis;

            Parameters.hole_dis_factor = hole_dis_factor;
            Parameters.tspin = tspin;
            Parameters.hold_T = hold_T;
            Parameters.hold_I = hold_I;
            Parameters.clear_useless_factor = clear_useless_factor;

            Parameters.dif_factor = dif_factor;
            Parameters.b2b = b2b;
            Parameters.combo = combo;
            Parameters.avoid_softdrop = avoid_softdrop;
            Parameters.tmini = tmini;

            Parameters.strategy_4w = strategy_4w;
        }

        public static MisaMinoParameters FromArray(int[] arr) {
            if (arr.Length != 21) throw new ArgumentException();

            return new MisaMinoParameters(
                arr[0], arr[1], arr[2], arr[3], arr[4],
                arr[5], arr[6], arr[7], arr[8], arr[9],
                arr[10], arr[11], arr[12], arr[13], arr[14],
                arr[15], arr[16], arr[17], arr[18], arr[19],
                arr[20]
            );
        }

        public int[] ToArray() => new int[] {
            Parameters.miny_factor,
            Parameters.hole,
            Parameters.open_hole,
            Parameters.v_transitions,
            Parameters.tspin3,

            Parameters.clear_efficient,
            Parameters.upcomeAtt,
            Parameters.h_factor,
            Parameters.hole_dis_factor2,
            Parameters.hole_dis,

            Parameters.hole_dis_factor,
            Parameters.tspin,
            Parameters.hold_T,
            Parameters.hold_I,
            Parameters.clear_useless_factor,

            Parameters.dif_factor,
            Parameters.b2b,
            Parameters.combo,
            Parameters.avoid_softdrop,
            Parameters.tmini,

            Parameters.strategy_4w
        };
    };
}