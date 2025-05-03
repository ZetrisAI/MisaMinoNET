using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

namespace MisaMinoNET {
    /// <summary>
    /// The main MisaMino class.
    /// Contains all methods for performing actions with MisaMino.
    /// </summary>
    public static class MisaMino {
        /// <summary>
        /// Converts a piece's index to its regular string representation.
        /// </summary>
        public static readonly string[] ToChar = new string[7] {
            "S", "Z", "J", "L", "T", "O", "I"
        };

        /// <summary>
        /// Converts a piece's index to its MisaMino string representation.
        /// </summary>
        public static readonly char[] MisaMinoMap = new char[7] {
            'Z', 'S', 'L', 'J', 'T', 'O', 'I'
        };

        /// <summary>
        /// Converts a MisaMino piece's index to its regular index.
        /// </summary>
        public static readonly int[] FromMisaMino = new int[7] {
            6, 4, 2, 3, 0, 1, 5
        };

        /// <summary>
        /// Configures and resets all parameters inside of MisaMino.
        /// </summary>
        /// <param name="param">The MisaMino parameters to configure MisaMino with.</param>
        /// <param name="hold_allowed">Is holding allowed in the game.</param>
        /// <param name="allowedSpins">What pieces is game awarding spins with?</param>
        /// <param name="tsd_only">Are only TSDs allowed (this parameter should only be used for 20 TSD Sprint mode).</param>
        /// <param name="search_width">The width multiplier of the AI search per depth. The value 1000 means a regular search.</param>
        /// <param name="allow180">Are we allowed to use 180 rotations? (TETR.IO kicks)</param>
        /// <param name="tetrisGame">For PPT, uses regular SRS. For TETR.IO, uses SRS+.</param>
        /// <param name="tminioldbehavior">If false, new `tmini` MisaMino parameter is used for Tmini. If true, old `tspin` MisaMino parameter is used for Tmini.</param>
        /// <param name="log">Your favorite logging function</param>
        public static void Configure(MisaMinoParameters param, bool hold_allowed, AllowedSpins allowedSpins, bool tsd_only, int search_width, bool allow180, TetrisGame tetrisGame, bool tminioldbehavior, Action<string> log = null) {
            if (Interface.Running) {
                (log?? Console.WriteLine)("[MisaMinoNET] Tried to configure MisaMino while it was running. Scheduling a re-configure on finish.");

                FinishedEventHandler reconfigure = null;
                reconfigure = (bool success) => {
                    Finished -= reconfigure;
                    Configure(param, hold_allowed, allowedSpins, tsd_only, search_width, allow180, tetrisGame, tminioldbehavior, log);
                };
                Finished += reconfigure;
            }

            Interface.configure(param.Parameters, hold_allowed, (int)allowedSpins, tsd_only, search_width, allow180, (int)tetrisGame, tminioldbehavior);
            Reset();
        }

        /// <summary>
        /// Resets all temporary state variables inside of MisaMino.
        /// </summary>
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

        /// <param name="success">Whether the search was successful or not.</param>
        public delegate void FinishedEventHandler(bool success);

        /// <summary>
        /// Fires when MisaMino reaches the end of its search, whether naturally or after it was aborted.
        /// </summary>
        public static event FinishedEventHandler Finished;

        /// <summary>
        /// The latest search result from the most recent call to FindMove.
        /// </summary>
        public static Solution LastSolution = new Solution();

        /// <summary>
        /// Checks if MisaMino is currently searching for solutions.
        /// </summary>
        public static bool Running => Interface.Running;

        /// <summary>
        /// Aborts the currently running search, if there is one.
        /// </summary>
        public static void Abort() {
            if (!Interface.Running) return;

            var abortWait = AbortCoordinator.CreateWaiter();

            Interface.SetAbort();

            abortWait();
        }

        /// <summary>
        /// <para>Starts searching for a solution/decision for the given game state.</para>
        /// <para>Pieces should be formatted with numbers from 0 to 6 in the order of SZJLTOI. Empty state on the field should be formatted with 255.</para>
        /// <para>Since this method will begin a search in the background, it does not immediately return any data.</para>
        /// <para>When the search ends, the Finished event will fire and LastSolution will update.</para>
        /// <para>The search can be ended prematurely with the Abort method.</para>
        /// </summary>
        /// <param name="queue">The piece queue, can be of any size.</param>
        /// <param name="current">The current piece.</param>
        /// <param name="hold">The piece in hold. Should be null if empty.</param>
        /// <param name="height">The height of the board, on top of which the piece spawns. If your piece spawns inside row 20, the correct value would be 21.</param>
        /// <param name="field">A 2D array consisting of the field. Should be no smaller than int[10, height].</param>
        /// <param name="combo">The combo count.</param>
        /// <param name="b2b">The current number of consecutive back-to-back valid attacks. If you're lazy, you can pass 1 if the player has back-to-back, and 0 if they don't.</param>
        /// <param name="garbage">The amount of incoming garbage which will spawn after the piece is placed.</param>
        /// <param name="second_choice">Return the second best move rather than first. Useful to desync two near-identical bots playing each other.</param>
        public static async void FindMove(int[] queue, int current, int? hold, int height, int[,] field, int combo, int b2b, int garbage, bool second_choice) {
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
                    action = Interface.Move(second_choice, out long time);

                    LastSolution = new Solution(time);

                    bool solved = !action.Equals("-1");

                    if (solved) {
                        LastSolution = new Solution(action, time);
                    }

                    Finished?.Invoke(solved);

                    AbortCoordinator.WakeWaiters();
                });
            }
        }

        static List<Instruction> FindPathResult(string action, out bool spinUsed, out bool success) {
            List<Instruction> ret = new List<Instruction>();

            spinUsed = false;

            if (success = !action.Equals("0")) {
                string[] info = action.Split('|');

                foreach (string i in info[0].Split(',')) {
                    ret.Add((Instruction)int.Parse(i));
                }

                spinUsed = Convert.ToInt32(info[1]) != 0;
            }

            return ret;
        }

        /// <summary>
        /// <para>Finds the optimal path to a specified piece placement.</para>
        /// <para>Pieces should be formatted with numbers from 0 to 6 in the order of SZJLTOI. Empty state on the field should be formatted with 255.</para>
        /// <para>Returns the list of Instructions necessary to correctly place the piece in the desired position.</para>
        /// </summary>
        /// <param name="field">A 2D array consisting of the field. Should be no smaller than int[10, height].</param>
        /// <param name="height">The height of the board, on top of which the piece spawns. If your piece spawns inside row 20, the correct value would be 21.</param>
        /// <param name="piece">The piece.</param>
        /// <param name="x">The desired X position of the piece.</param>
        /// <param name="y">The desired Y position of the piece.</param>
        /// <param name="r">The desired rotation of the piece.</param>
        /// <param name="hold">Whether the player needs to hold before starting to move the piece.</param>
        /// <param name="spinUsed">This is set if a twist/spin was used to place the piece.</param>
        /// <param name="success">This is set if it's possible to place the piece in the desired position.</param>
        public static List<Instruction> FindPath(int[,] field, int height, int piece, int x, int y, int r, bool hold, out bool spinUsed, out bool success) {
            if (r == 3) r = 1;
            else if (r == 1) r = 3;

            string action = Interface.Path(
                encodeField(field, height),
                encodeCurrent(piece),
                x - 1, y - 3, r, hold
            );

            return FindPathResult(action, out spinUsed, out success);
        }

        /// <summary>
        /// <para>Finds the optimal path from any position to a specified piece placement.</para>
        /// <para>Pieces should be formatted with numbers from 0 to 6 in the order of SZJLTOI. Empty state on the field should be formatted with 255.</para>
        /// <para>Returns the list of Instructions necessary to correctly place the piece in the desired position.</para>
        /// </summary>
        /// <param name="field">A 2D array consisting of the field. Should be no smaller than int[10, height].</param>
        /// <param name="height">The height of the board, on top of which the piece spawns. If your piece spawns inside row 20, the correct value would be 21.</param>
        /// <param name="piece">The piece.</param>
        /// <param name="_x">The starting X position of the piece.</param>
        /// <param name="_y">The starting Y position of the piece.</param>
        /// <param name="_r">The starting rotation of the piece.</param>
        /// <param name="x">The desired X position of the piece.</param>
        /// <param name="y">The desired Y position of the piece.</param>
        /// <param name="r">The desired rotation of the piece.</param>
        /// <param name="spinUsed">This is set if a twist/spin was used to place the piece.</param>
        /// <param name="success">This is set if it's possible to place the piece in the desired position.</param>
        public static List<Instruction> FindPathFrom(int[,] field, int height, int piece, int _x, int _y, int _r, int x, int y, int r, out bool spinUsed, out bool success) {
            List<Instruction> ret = new List<Instruction>();

            if (_r == 3) _r = 1;
            else if (_r == 1) _r = 3;

            if (r == 3) r = 1;
            else if (r == 1) r = 3;

            string action = Interface.PathFrom(
                encodeField(field, height),
                encodeCurrent(piece),
                _x -1, _y - 3, _r,
                x - 1, y - 3, r
            );

            return FindPathResult(action, out spinUsed, out success);
        }
    }
}
