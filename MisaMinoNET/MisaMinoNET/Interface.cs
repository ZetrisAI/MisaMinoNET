using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;

namespace MisaMinoNET {
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
        public static extern void configure(AI_Param param, bool holdAllowed, bool allSpin, bool TSDonly, int search_width, bool allow180);

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
        public static string Move(out long time) {
            StringBuilder sb = new StringBuilder(500);

            abort = true;

            lock (locker) {
                abort = false;

                Stopwatch stopwatch = new Stopwatch();
                stopwatch.Start();

                Running = true;

                action(sb, sb.Capacity);

                Running = false;

                stopwatch.Stop();
                time = stopwatch.ElapsedMilliseconds;
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
}