using System;
using System.Collections.Generic;
using System.Linq;

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

    public class Solution {
#pragma warning disable 0169
        public int PieceUsed { get; private set; }
        public bool SpinUsed { get; private set; }
        public int B2B { get; private set; }
        public int Nodes { get; private set; }
        public int Depth { get; private set; }
        public long Time { get; private set; }
        public int Attack { get; private set; }
        public int FinalX { get; private set; }
        public int FinalY { get; private set; }
        public int FinalR { get; private set; }
        public List<Instruction> Instructions { get; private set; } = new List<Instruction>();
#pragma warning restore 0169

        public bool Empty => Instructions.Count == 0;

        public Solution(long time = 0) => Time = time;

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

        public override string ToString() => $"{SpinUsed} {MisaMino.ToChar[PieceUsed]}={FinalX},{FinalY},{FinalR}: {String.Join(", ", Instructions)}";
    }
}