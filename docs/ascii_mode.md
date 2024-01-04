# Ascii mode

When the program is in *ascii mode*, each instruction will be converted to a character and [stacked](stack.md). The *ascii mode* can be enabled or disabled using the `"` instruction:
```
X.X
...
...
```
*(`.`s represent spaces and `X`s represent any other character)*

*Note:* The characters are stacked when the [interpreter](interpreter.md) get them so strings are reversed. When you stack a string using the *ascii mode* and you print it, the characters will be printed in the other direction.

The list of every characters is in [characters.md](characters.md).


## Escaping

In *ascii mode*, characters can be escaped using the **ESCAPE** `\` instruction:
```
X..
.X.
..X
```

Characters that can be escaped

|Character|Hex   |Escaped character      |
|---------|:----:|-----------------------|
|`A`      |`0x07`|`\a` : Bell            |
|`B`      |`0x08`|`\n` : Backspace       |
|`E`      |`0x1B`|`\e` : Escape          |
|`F`      |`0x0C`|`\f` : Form feed       |
|`N`      |`0x0A`|`\n` : Line feed       |
|`R`      |`0x0D`|`\r` : Carriage Return |
|`T`      |`0x09`|`\t` : Horizontal Tab  |
|`V`      |`0x0B`|`\v` : Vertical Tab    |
|`\`      |-     |`\`                    |
|`"`      |-     |`"`                    |



## Uppercase / lowercase

In *ascii mode*, when they're escaped, **UP** `^` and **DOWN** `v` instructions enable and disable uppercase mode. Each time you disable and enable ascii mode, uppercase mode disables.

  - **UP** `^` : enable uppercase mode
    ```
    .X.
    XXX
    ...
    ```
  - **DOWN** `v` : disable uppercase mode
    ```
    ...
    XXX
    .X.
    ```
