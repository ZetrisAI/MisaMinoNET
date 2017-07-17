# MisaMinoBot
Command line version of the MisaMino bot

Misamino is located in the `tetris_ai` directory. `BakaBot` contains a weak fool bot which is also distributed with the MisaMino game as dllai - this bot has its own README file and its options are limited.

## Example

### Input
```
update game round 1
update game this_piece_type T
update game next_pieces Z,I,L,J,L
update bot1 combo 0
update bot 1 inAtt 4
update bot1 field 0,0,0,1,1,1,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0
action2 moves 10000
```
* `combo`, `inAtt`, `round` are not required
* If `combo` is not present, AI uses its internal value
* If `inAtt` is not present, `0` is used
* `round` is only used to reset the game, which happens when `round==1`

### Output
```
0|0|0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,0,0,0,0,0,0,0;0,0,0,2,0,0,0,0,0,0;0,0,2,2,2,0,0,0,0,0
```

1. Number of cleared lines, `-1` if no moves were found
2. `>1` if spins were used to clear the lines
3. Game field after line clears

## MisaMino settings
The following commands are available to customize AI settings:
### Level change
```
settings level N
```
* `level` in range 0-10

### Style change
```
settings style N
```
* `style` from the following table:

| Value | AI style    |
|  ---  |    ---      |
| 1     | T-spin+ AI  |
| 2     | T-spin      |
| 3     | Ren         |
| 4     | non-Hold    |
| other | Downstack   |

### Settings response
After both `level` and `style` commands the current name of the AI is printed in the following json format:
```
{"name":"Ren AI LV4"}
```
