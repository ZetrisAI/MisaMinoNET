using System;
using System.Collections.Generic;
using System.Linq;
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
        public static extern void configure(int level, int style, int c4w);

        [DllImport("libtetris_ai.dll")]
        public static extern void update_next(string queue);

        [DllImport("libtetris_ai.dll")]
        public static extern void update_current(string piece);

        [DllImport("libtetris_ai.dll")]
        public static extern void update_hold(string piece);

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
        public static string Move() {
            StringBuilder sb = new StringBuilder(500);
            action(sb, sb.Capacity);
            return sb.ToString();
        }

        [DllImport("libtetris_ai.dll")]
        public static extern bool alive();

        [DllImport("libtetris_ai.dll")]
        private static extern void findpath(string field, string piece, int x, int y, int r, bool hold, StringBuilder str, int len);
        public static string Path(string field, string piece, int x, int y, int r, bool hold) {
            StringBuilder sb = new StringBuilder(500);
            findpath(field, piece, x, y, r, hold, sb, sb.Capacity);
            return sb.ToString();
        }
    }

    public static class MisaMino {
        public static void Configure(int level, int style, bool c4w) {
            if (level < 1) level = 1;
            if (level > 10) level = 10;

            if (style < 1) style = 1;
            if (style > 7) style = 7;

            Interface.configure(level, style, Convert.ToInt32(c4w));
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

        private static string encodeCurrent(int current) => MinoMap[current].ToString();

        private static string encodeField(int[,] field, int height) {
            string[] rows = new string[height];

            for (int i = 0; i < height; i++) {
                int[] row = new int[10];

                for (int j = 0; j < 10; j++)
                    row[9 - j] = (field[j, i] == 255)? 0 : 2; // Mirror for whatever reason. Blaming MisaMino.

                rows[height - i - 1] = String.Join(",", row);
            }

            return String.Join(";", rows);
        }

        public static List<Instruction> FindMove(int[] queue, int current, int? hold, int height, int[,] field, int combo, int garbage, ref int pieceUsed, ref bool spinUsed, ref int finalX, ref int finalY, ref int finalR) {
            List<Instruction> ret = new List<Instruction>();

            if (Interface.alive()) {
                updateQueue(queue);
                Interface.update_current(encodeCurrent(current));
                Interface.update_hold((hold == null)? " " : encodeCurrent(hold.Value));
                Interface.update_field(encodeField(field, height));
                Interface.update_combo(combo);
                Interface.update_incoming(garbage);
                string action = Interface.Move();

                if (!action.Equals("-1")) {
                    string[] info = action.Split('|');

                    foreach (string i in info[0].Split(',')) {
                        ret.Add((Instruction)int.Parse(i));
                    }

                    pieceUsed = RevMinoMap[Convert.ToInt32(info[1]) - 1];

                    spinUsed = Convert.ToInt32(info[2]) != 0;

                    int[] pieceInfo = info[3].Split(',').Select(s => int.Parse(s)).ToArray();
                    finalX = pieceInfo[0] + 1;
                    finalY = pieceInfo[1] + 3;
                    finalR = (ret.Count(i => i == Instruction.RSPIN) - ret.Count(i => i == Instruction.LSPIN) + 12) % 4;
                }
            }

            return ret;
        }

        public static List<Instruction> FindPath(int[,] field, int height, int piece, int x, int y, int r, bool hold, ref bool spinUsed, out bool success) {
            List<Instruction> ret = new List<Instruction>();

            if (r == 3) r = 1;
            else if (r == 1) r = 3;

            string action = Interface.Path(
                encodeField(field, height),
                encodeCurrent(piece),
                x - 1, y - 3, r, hold
            );

            if (success = !action.Equals("0")) {
                string[] info = action.Split('|');

                foreach (string i in info[0].Split(',')) {
                    ret.Add((Instruction)int.Parse(i));
                }

                spinUsed = Convert.ToInt32(info[1]) != 0;
            }

            return ret;
        }
    }
}
