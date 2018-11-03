using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

using MisaMinoNET;

namespace MisaMinoNET {
    public class Interface {
        [DllImport("libtetris_ai.dll")]
        public static extern void settings_level(int level);

        [DllImport("libtetris_ai.dll")]
        public static extern void settings_style(int style);

        [DllImport("libtetris_ai.dll")]
        public static extern void update_next(string queue);

        [DllImport("libtetris_ai.dll")]
        public static extern void update_current(string piece);

        [DllImport("libtetris_ai.dll")]
        public static extern void update_incoming(int attack);

        [DllImport("libtetris_ai.dll")]
        public static extern void update_combo(int combo);

        [DllImport("libtetris_ai.dll")]
        public static extern void update_field(string field);

        [DllImport("libtetris_ai.dll")]
        public static extern void update_reset();

        [DllImport("libtetris_ai.dll")]
        public static extern void action(StringBuilder str, int len);
    }

    public class MisaMino {
        public static byte[] Test() {
            Interface.settings_level(10);
            Interface.settings_style(1);
            Interface.update_next("T,L,I,O,Z");
            Interface.update_current("J");
            Interface.update_combo(0);
            Interface.update_incoming(0);
            Interface.update_field("0,0,0,1,1,1,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0");

            StringBuilder sb = new StringBuilder(500);
            Interface.action(sb, sb.Capacity);

            byte[] ret = Encoding.ASCII.GetBytes(sb.ToString());
            for (int i = 0; i < ret.Length; i++) {
                ret[i] -= 70;
            }

            return ret;
        }
    }
}
