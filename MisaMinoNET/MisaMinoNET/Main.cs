using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

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
        public static extern void configure(int level, int style);

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
        public static void Configure(int level, int style) {
            if (level < 1) level = 1;
            if (level > 10) level = 10;

            if (style < 1) style = 1;
            if (style > 6) style = 6;

            Interface.configure(level, style);
            Reset();
        }

        public static void Reset() {
            Interface.update_reset();
        }

        static MisaMino() {
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

        private static void updateField(int[,] field, int height) {
            string[] rows = new string[height];

            for (int i = 0; i < height; i++) {
                int[] row = new int[10];

                for (int j = 0; j < 10; j++)
                    row[9 - j] = (field[j, i] == 255)? 0 : 2; // Mirror for whatever reason. Blaming MisaMino.

                rows[height - i - 1] = String.Join(",", row);
            }

            Interface.update_field(String.Join(";", rows));
        }

        public static List<Instruction> FindMove(int[] queue, int current, int height, int[,] field, int combo, int garbage, ref int pieceUsed, ref bool spinUsed) {
            List<Instruction> ret = new List<Instruction>();

            if (Interface.alive()) {
                updateQueue(queue);
                updateCurrent(current);
                updateField(field, height);
                Interface.update_combo(combo);
                Interface.update_incoming(garbage);
                string action = Interface.process();

                if (!action.Equals("-1")) {
                    string[] info = action.Split('|');
                    pieceUsed = RevMinoMap[Convert.ToInt32(info[1]) - 1];
                    spinUsed = Convert.ToInt32(info[2]) != 0;

                    foreach (string i in info[0].Split(',')) {
                        ret.Add((Instruction)int.Parse(i));
                    }
                }
            }

            return ret;
        }
    }
}
