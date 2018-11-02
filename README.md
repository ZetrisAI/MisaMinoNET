# MisaMinoNET
.NET version of the MisaMino bot (via DLL Importing).

MisaMino is located in the `tetris_ai` directory.

### MisaMino Commands
* settings
    * level (int 0 to 10)
    * style (int 1 = t-spin+, 2 = t-spin, 3 = ren, 4 = no hold, 5 = downstack)
* update
    * next_pieces (string comma-separated I,T,L,J,Z,S,O)
    * combo (int)
    * field (array of int comma-separated 0 = empty, 2 = filled)
    * this_piece_type (string I,T,L,J,Z,S,O)
    * inAtt (int)
    * round (int, 1 = reset bot)
* action (process info and returns move)

### Output

1. Number of cleared lines, `-1` if no moves were found
2. `>1` if spins were used to clear the lines
3. Game field after line clears
