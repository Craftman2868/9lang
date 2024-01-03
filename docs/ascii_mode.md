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

In *ascii mode*, characters can be escaped using the **ESCAPE** `\` instruction:
```
X..
.X.
..X
```

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
