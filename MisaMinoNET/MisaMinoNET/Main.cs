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
    }

    public class MisaMino {
        public static void Test() {
            Interface.update_next("T,L,I,O,Z");
            Interface.update_current("J");
        }
    }
}
