using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

using MisaMinoNET;

namespace MisaMinoNET {
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
        public static extern void action(StringBuilder str, int len);
    }

    public static class MisaMino {
        /*public static byte[] Test() {
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
        }*/

        public static void Reset() {
            Interface.update_reset();
            Init();
        }

        public static void Init() {
            Interface.settings_level(10);
            Interface.settings_style(1);
        }

        static MisaMino() {
            Init();
        }

        private static readonly char[] MinoMap = new char[7] {
            'I', 'T', 'L', 'J', 'Z', 'S', 'O'
        };

        private static char MinoConvert(int piece) {
            if (piece < 5) {
                piece = 5 - piece;
            } else {
                piece = 6 * (6 - piece);
            }

            return MinoMap[piece];
        }

        private static void updateQueue(int[] queue) {
            char[] queueMinos = new char[queue.Length];

            for (int i = 0; i < queue.Length; i++)
                queueMinos[i] = MinoConvert(queue[i]);

            Interface.update_next(String.Join(",", queueMinos));
        }

        private static void updateCurrent(int current) {
            Interface.update_current(MinoConvert(current).ToString());
        }

        private static void updateField(int[,] field) {
            string[] rows = new string[20];

            for (int i = 0; i < 20; i++) {
                int[] row = new int[10];

                for (int j = 0; j < 10; j++) {
                    if (field[j, i] == -1) {
                        row[j] = 0;
                    } else {
                        row[j] = 2;
                    }
                }

                rows[19 - i] = String.Join(",", row);
            }

            Interface.update_field(String.Join(";", rows));
        }

        public static byte[] FindMove(int[] queue, int current, int[,] field, int combo, int garbage) {
            updateQueue(queue);
            updateCurrent(current);
            updateField(field);
            Interface.update_combo(combo);
            Interface.update_incoming(garbage);

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
