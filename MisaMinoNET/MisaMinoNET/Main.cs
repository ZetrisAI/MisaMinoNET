using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

// Suppresses readonly suggestion
[assembly: System.Diagnostics.CodeAnalysis.SuppressMessage("Style", "IDE0044")]

// Suppresses naming rule violation
[assembly: System.Diagnostics.CodeAnalysis.SuppressMessage("Style", "IDE1006")]

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
            int miny_factor = 16, int hole = 9, int open_hole = 11, int v_transitions = 23, int tspin3 = 29,
            int clear_efficient = 25, int upcomeAtt = 39, int h_factor = 2, int hole_dis_factor2 = 12, int hole_dis = 19,
            int hole_dis_factor = 7, int tspin = 24, int hold_T = 32, int hold_I = 16, int clear_useless_factor = 14,
            int dif_factor = 19, int b2b = 32, int combo = 25, int avoid_softdrop = 18, int tmini = 0,
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

    static class Interface {
        private static bool abort = false;

        private delegate bool Callback();
        private static Callback AbortCallback;

        [DllImport("MisaMino.dll")]
        private static extern void set_abort(Callback func);

        private static object locker = new object();
        public static bool Running { get; private set; } = false;

        static Interface() {
            AbortCallback = new Callback(Abort);
            set_abort(AbortCallback);
        }

        public static bool Abort() => abort;
        public static void SetAbort() {
            if (Running) abort = true;
        }

        [DllImport("MisaMino.dll")]
        public static extern void configure(AI_Param param, bool holdAllowed, bool TSDonly);

        [DllImport("MisaMino.dll")]
        public static extern void update_next(string queue);

        [DllImport("MisaMino.dll")]
        public static extern void update_current(string piece);

        [DllImport("MisaMino.dll")]
        public static extern void update_hold(string piece);

        [DllImport("MisaMino.dll")]
        public static extern void update_incoming(int attack);

        [DllImport("MisaMino.dll")]
        public static extern void update_combo(int combo);

        [DllImport("MisaMino.dll")]
        public static extern void update_b2b(int b2b);

        [DllImport("MisaMino.dll")]
        public static extern void update_field(string field);

        [DllImport("MisaMino.dll")]
        public static extern void update_reset();

        [DllImport("MisaMino.dll")]
        private static extern void action(StringBuilder str, int len);
        public static string Move() {
            StringBuilder sb = new StringBuilder(500);

            abort = true;

            lock (locker) {
                abort = false;

                Running = true;

                action(sb, sb.Capacity);

                Running = false;
            }

            return sb.ToString();
        }

        [DllImport("MisaMino.dll")]
        public static extern bool alive();

        [DllImport("MisaMino.dll")]
        private static extern void findpath(string field, string piece, int x, int y, int r, bool hold, StringBuilder str, int len);
        public static string Path(string field, string piece, int x, int y, int r, bool hold) {
            StringBuilder sb = new StringBuilder(500);
            findpath(field, piece, x, y, r, hold, sb, sb.Capacity);
            return sb.ToString();
        }
    }

    public static class Mino {
        public static readonly string[] ToChar = new string[7] {
            "S", "Z", "J", "L", "T", "O", "I"
        };

        public static readonly char[] MisaMinoMap = new char[7] {
            'Z', 'S', 'L', 'J', 'T', 'O', 'I'
        };

        public static readonly int[] FromMisaMino = new int[7] {
            6, 4, 2, 3, 0, 1, 5
        };
    }

    public class Solution {
        #pragma warning disable 0169
        public int PieceUsed { get; private set; }
        public bool SpinUsed { get; private set; }
        public int B2B { get; private set; }
        public int Nodes { get; private set; }
        public int Attack { get; private set; }
        public int FinalX { get; private set; }
        public int FinalY { get; private set; }
        public int FinalR { get; private set; }
        public List<Instruction> Instructions { get; private set; } = new List<Instruction>();
        #pragma warning restore 0169

        public bool Empty => Instructions.Count == 0;

        public Solution() {}

        public Solution(string input) {
            string[] info = input.Split('|');

            foreach (string i in info[0].Split(',')) {
                Instructions.Add((Instruction)int.Parse(i));
            }

            PieceUsed = Mino.FromMisaMino[Convert.ToInt32(info[1]) - 1];
            SpinUsed = Convert.ToInt32(info[2]) != 0;
            B2B = Convert.ToInt32(info[3]);
            Nodes = Convert.ToInt32(info[4]);
            Attack = Convert.ToInt32(info[5]);

            int[] pieceInfo = info[6].Split(',').Select(s => int.Parse(s)).ToArray();
            FinalX = pieceInfo[0] + 1;
            FinalY = pieceInfo[1] + 3;
            FinalR = (Instructions.Count(i => i == Instruction.RSPIN) - Instructions.Count(i => i == Instruction.LSPIN) + 100) % 4;
        }

        public override string ToString() => $"{SpinUsed} {Mino.ToChar[PieceUsed]}={FinalX},{FinalY},{FinalR}: {String.Join(", ", Instructions)}";
    }

    public static class MisaMino {
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
                queueMinos[i] = Mino.MisaMinoMap[queue[i]];

            Interface.update_next(String.Join(",", queueMinos));
        }

        private static string encodeCurrent(int current) => Mino.MisaMinoMap[current].ToString();

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
                    action = Interface.Move();

                    LastSolution = new Solution();

                    bool solved = !action.Equals("-1");

                    if (solved) {
                        LastSolution = new Solution(action);
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
