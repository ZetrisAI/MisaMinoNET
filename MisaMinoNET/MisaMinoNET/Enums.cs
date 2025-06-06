﻿namespace MisaMinoNET {
    public enum AllowedSpins {
        TSpins,
        AllSpinsRegularTSpins,
        AllSpins
    }

    public enum TetrisGame {
        PPT,
        TETRIO,
        TETRIOS2
    }

    public static class EnumExtensions {
        public static bool IsAllSpins(this AllowedSpins spins)
            => spins == AllowedSpins.AllSpinsRegularTSpins || spins == AllowedSpins.AllSpins;

        public static bool IsSRSPlus(this TetrisGame game)
            => game == TetrisGame.TETRIO || game == TetrisGame.TETRIOS2;
    }
}
