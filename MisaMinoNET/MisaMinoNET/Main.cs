using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

using MisaMinoNET;

namespace MisaMinoNET {
    public enum Instruction {
        NULL, // Don't use Hold piece
        L, // Tap Left
        R, // Tap Right
        LL, // DAS to Left wall
        RR, // DAS to Right wall
        D, // Soft drop tap
        DD, // Soft drop to ground
        LSPIN, // Rotate Left
        RSPIN, // Rotate Right
        DROP, // Hard drop
        HOLD, // Use Hold piece
        SPIN2, // Rotate 180
        REFRESH // ?
    };

    static class Interface {
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
        private static extern void action(StringBuilder str, int len);
        public static string process() {
            StringBuilder sb = new StringBuilder(500);
            Interface.action(sb, sb.Capacity);
            return sb.ToString();
        }

        [DllImport("libtetris_ai.dll")]
        public static extern bool alive();
    }

    public static class MisaMino {
        public static void updateLevel(int level) {
            if (level < 1) level = 1;
            if (level > 10) level = 10;

            Interface.settings_level(level);
            Reset();
        }

        public static void updateStyle(int style) {
            if (style < 1) style = 1;
            if (style > 5) style = 5;

            Interface.settings_style(style);
            Reset();
        }

        public static void Reset() {
            Interface.update_reset();
        }

        static MisaMino() {
            Interface.settings_level(10);
            Interface.settings_style(1);
            Reset();
        }

        private static readonly char[] MinoMap = new char[7] {
            'Z', 'S', 'L', 'J', 'T', 'O', 'I'
        };

        private static readonly int[] RevMinoMap = new int[7] {
            6, 4, 2, 3, 0, 1, 5
        };

        private static void updateQueue(int[] queue) {
            char[] queueMinos = new char[queue.Length];

            for (int i = 0; i < queue.Length; i++)
                queueMinos[i] = MinoMap[queue[i]];

            Interface.update_next(String.Join(",", queueMinos));
        }

        private static void updateCurrent(int current) {
            Interface.update_current(MinoMap[current].ToString());
        }

        private static void updateField(int[,] field) {
            string[] rows = new string[20];

            for (int i = 0; i < 20; i++) {
                int[] row = new int[10];

                for (int j = 0; j < 10; j++) {
                    if (field[j, i] == -1) {
                        // Mirror for whatever reason. Blaming MisaMino.
                        row[9 - j] = 0;
                    } else {
                        row[9 - j] = 2;
                    }
                }

                rows[19 - i] = String.Join(",", row);
            }

            Interface.update_field(String.Join(";", rows));
        }

        public static List<Instruction> FindMove(int[] queue, int current, int[,] field, int combo, int garbage, ref int pieceUsed) {
            List<Instruction> ret = new List<Instruction>();

            if (Interface.alive()) {
                updateQueue(queue);
                updateCurrent(current);
                updateField(field);
                Interface.update_combo(combo);
                Interface.update_incoming(garbage);
                string action = Interface.process();

                if (!action.Equals("-1")) {
                    string[] info = action.Split('|');
                    pieceUsed = RevMinoMap[int.Parse(info[1]) - 1];

                    foreach (string i in info[0].Split(',')) {
                        ret.Add((Instruction)int.Parse(i));
                    }
                }
            }

            return ret;
        }
    }
}
