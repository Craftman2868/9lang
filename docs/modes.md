# Modes

The [interpreter](interpreter.md) has 4 modes:
  - the normal mode
  - the [*ascii mode*](ascii_mode.md)
  - the ["equal mode"](equal_mode.md)
  - the [conditional mode](cond_mode.md)

The program starts in normal mode and **must** end in normal mode too.

## The normal mode

When the program starts, it is in normal mode.
When the program is in normal mode, the [interpreter](interpreter.md) will just execute the current [instruction](instructions.md) and go to the next one (see [direction](direction.md)).


## The ascii mode

See [*Ascii mode*](ascii_mode.md)


## The "equal mode"

The equal mode allow you to add any byte to the [stack](stack.md). To enter in equal mode use the `=` [instruction](instructions.md).

In equal mode, only 16 [instructions](instructions.md) (the hexadecimal characters) are available: `0`, `1`, `2`, `3`, `4`, `5`, `6`, `7`, `8`, `9`, `A`, `B`, `C`, `D`, `E`, `F` (see [Characters](characters.md) to know how to write it in *9lang*).

A special case is when the `=` instruction is followed by another `=` instruction, it cancel the equal mode and it become an [equality operator](comparison.md)

An `=` **must** always be followed by hexadecimal characters.


### Example

```
XXXX.XX..
...XXXX..
XXX..XX..

 =  4  1
```
*(`.`s represents spaces and `X`s represents any other character)*

This *code* means: *[stack](stack.md) the byte `0x41`*.


## The conditional mode

To enter in conditional mode use the `?` [instruction](instructions.md).
When you enter in conditional mode, the [interpreter](interpreter.md) [unstack](stack.md) 1 byte.
If the byte is different from 0, the [interpreter](interpreter.md) execute the next [instruction](instruction.md), else, it does not.
The conditional mode is very useful with the [comparison operators](comparison.md).


### Example

```
...X.X.X.X.XXXXXXXXXX...X.XXXX...X.X.X.
.X....XXX...........XXXX...X.XXX.......
.X....X.X...XXXXXX.X..X....XXXX.X....X.
.X.X.XXXX.X.X..X.XX.X.X................
......X.XXXX.X.XX....XX................
.X....XXX.....XX.X....X................

 ,  "  a  "  =  =  ?  v  "  O  N  "  :
 :  "  O  ^  \  K  "  <
```
*(`.`s represents spaces and `X`s represents any other character)*

This program read 1 character from *stdin* and check if it is the letter `a` (in lowercase). If it is, the program print `Ok`, else it print `no`.
