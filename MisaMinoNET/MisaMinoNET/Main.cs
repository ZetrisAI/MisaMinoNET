using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

namespace MisaMinoNET {
    public static class MisaMino {
        public static readonly string[] ToChar = new string[7] {
            "S", "Z", "J", "L", "T", "O", "I"
        };

        public static readonly char[] MisaMinoMap = new char[7] {
            'Z', 'S', 'L', 'J', 'T', 'O', 'I'
        };

        public static readonly int[] FromMisaMino = new int[7] {
            6, 4, 2, 3, 0, 1, 5
        };

        public static void Configure(MisaMinoParameters param, bool hold_allowed, bool tsd_only) {
            Interface.configure(param.Parameters, hold_allowed, tsd_only);
            Reset();
        }

        public static void Reset() {
            Abort();
            Interface.update_reset();
        }

        static MisaMino() {
            Reset();
        }

        private static void updateQueue(int[] queue) {
            char[] queueMinos = new char[queue.Length];

            for (int i = 0; i < queue.Length; i++)
                queueMinos[i] = MisaMinoMap[queue[i]];

            Interface.update_next(String.Join(",", queueMinos));
        }

        private static string encodeCurrent(int current) => MisaMinoMap[current].ToString();

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
        
        public delegate void FinishedEventHandler(bool success);
        public static event FinishedEventHandler Finished;

        public static Solution LastSolution = new Solution();

        public static bool Running { get => Interface.Running; }

        static ManualResetEvent abortWait;

        public static void Abort() {
            if (Interface.Running) {
                abortWait = new ManualResetEvent(false);

                Interface.SetAbort();

                abortWait.WaitOne();
            }
        }

        public static async void FindMove(int[] queue, int current, int? hold, int height, int[,] field, int combo, int b2b, int garbage) {
            if (Interface.alive()) {
                updateQueue(queue);
                Interface.update_current(encodeCurrent(current));
                Interface.update_hold((hold == null)? " " : encodeCurrent(hold.Value));
                Interface.update_field(encodeField(field, height));
                Interface.update_combo(combo);
                Interface.update_b2b(b2b);
                Interface.update_incoming(garbage);

                string action;

                await Task.Run(() => {
                    action = Interface.Move(out long time);

                    LastSolution = new Solution(time);

                    bool solved = !action.Equals("-1");

                    if (solved) {
                        LastSolution = new Solution(action, time);
                    }

                    Finished?.Invoke(solved);

                    abortWait?.Set();
                });
            }
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
