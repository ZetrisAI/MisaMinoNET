using System;
using System.Collections.Generic;
using System.Linq;

namespace MisaMinoNET {
    /// <summary>
    /// A performable in-game movement described as an instruction.
    /// </summary>
    public enum Instruction {
        /// <summary>
        /// "Don't do anything" instruction. Usually skippable. Often found at the start of an instruction list where using the held piece isn't required.
        /// </summary>
        NULL,

        /// <summary>
        /// Move left one tile.
        /// </summary>
        L,

        /// <summary>
        /// Move right one tile.
        /// </summary>
        R,

        /// <summary>
        /// DAS to left wall.
        /// </summary>
        LL,

        /// <summary>
        /// DAS to right wall.
        /// </summary>
        RR,

        /// <summary>
        /// Soft drop one tile.
        /// </summary>
        D,

        /// <summary>
        /// Soft drop to the ground.
        /// </summary>
        DD,

        /// <summary>
        /// Rotate left (counter-clockwise).
        /// </summary>
        LSPIN,

        /// <summary>
        /// Rotate right (clockwise).
        /// </summary>
        RSPIN,

        /// <summary>
        /// Hard drop piece.
        /// </summary>
        DROP, 

        /// <summary>
        /// Take the held piece.
        /// </summary>
        HOLD,

        /// <summary>
        /// Rotate by 180 degrees.
        /// </summary>
        SPIN2,

        /// <summary>
        /// Refresh (unused).
        /// </summary>
        REFRESH
    };

    /// <summary>
    /// The Solution class describes the results of a MisaMino search.
    /// </summary>
    public class Solution {
#pragma warning disable 0169
        /// <summary>
        /// Gets which piece was used in the Solution.
        /// </summary>
        public int PieceUsed { get; private set; }

        /// <summary>
        /// Gets if a spin was used to move the piece into position.
        /// </summary>
        public bool SpinUsed { get; private set; }

        /// <summary>
        /// Gets the value of back-to-back after executing the Solution.
        /// </summary>
        public int B2B { get; private set; }

        /// <summary>
        /// Gets the number of nodes used in the search. This generally represents how confident MisaMino is in its decision.
        /// </summary>
        public int Nodes { get; private set; }

        /// <summary>
        /// Gets how deep the search was able to expand. This represents how many pieces from the queue MisaMino was able to consider in its search.
        /// </summary>
        public int Depth { get; private set; }

        /// <summary>
        /// Gets the amount of time, in milliseconds, that the search took to complete.
        /// </summary>
        public long Time { get; private set; }

        /// <summary>
        /// Gets the amount of attack garbage executing the decision would send.
        /// </summary>
        public int Attack { get; private set; }

        /// <summary>
        /// Gets the final X position of the piece.
        /// </summary>
        public int FinalX { get; private set; }

        /// <summary>
        /// Gets the final Y position of the piece.
        /// </summary>
        public int FinalY { get; private set; }

        /// <summary>
        /// Gets the final rotation of the piece.
        /// </summary>
        public int FinalR { get; private set; }

        /// <summary>
        /// Gets the list of Instructions necessary to correctly place the piece in the desired position.
        /// </summary>
        public List<Instruction> Instructions { get; private set; } = new List<Instruction>();
#pragma warning restore 0169

        /// <summary>
        /// Checks if the list of Instructions is empty.
        /// </summary>
        public bool Empty => Instructions.Count == 0;

        /// <summary>
        /// Creates an empty Solution.
        /// </summary>
        /// <param name="time">The amount of time the search took to complete.</param>
        public Solution(long time = 0) => Time = time;

        /// <summary>
        /// Creates a custom Solution from raw MisaMino output.
        /// </summary>
        /// <param name="input">The raw MisaMino output string which resulted from the search.</param>
        /// <param name="time">The amount of time the search took to complete.</param>
        public Solution(string input, long time) {
            string[] info = input.Split('|');

            foreach (string i in info[0].Split(',')) {
                Instructions.Add((Instruction)int.Parse(i));
            }

            PieceUsed = MisaMino.FromMisaMino[Convert.ToInt32(info[1]) - 1];
            SpinUsed = Convert.ToInt32(info[2]) != 0;
            B2B = Convert.ToInt32(info[3]);
            Nodes = Convert.ToInt32(info[4]);
            Depth = Convert.ToInt32(info[5]);
            Time = time;
            Attack = Convert.ToInt32(info[6]);

            int[] pieceInfo = info[7].Split(',').Select(s => int.Parse(s)).ToArray();
            FinalX = pieceInfo[0] + 1;
            FinalY = pieceInfo[1] + 3;
            FinalR = (Instructions.Count(i => i == Instruction.RSPIN) - Instructions.Count(i => i == Instruction.LSPIN) + 100) % 4;
        }

        /// <summary>
        /// Returns a human-readable string representation of the Solution.
        /// </summary>
        public override string ToString() => $"{SpinUsed} {MisaMino.ToChar[PieceUsed]}={FinalX},{FinalY},{FinalR}: {String.Join(", ", Instructions)}";
    }
}