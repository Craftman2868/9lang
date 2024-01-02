# The stack

The stack is composed of `512` bytes (the size of the stack can be modified in the [`include/9lang.h`](../include/9lang.h) file). It is the only memory area you can use.

You can stack bytes with the following instructions:
  - **ASCII** `"` : toggle [*ascii mode*](ascii_mode.md)
    ```
    X.X
    ...
    ...
    ```
    *(`.`s represent spaces and `X`s represent any other character)*

  - **ZERO** `0` : add a null byte
    ```
    .X.
    X.X
    .X.
    ```
    *(`.`s represent spaces and `X`s represent any other character)*
    
  - **READ** `,` : read a char from *stdin* and stack it
    ```
    ...
    .X.
    .X.
    ```
    *(`.`s represent spaces and `X`s represent any other character)*

You can unstack bytes with the following instructions:
  - **PRINT** `.` : unstack 1 char and print it in *stdout*
    ```
    ...
    .X.
    ...
    ```
    *(`.`s represent spaces and `X`s represent any other character)*
  
  - **PRTALL** `:` : unstack and print to *stdout* each char until a `NUL` byte or the end of the stack.
    ```
    .X.
    ...
    .X.
    ```
    *(`.`s represent spaces and `X`s represent any other character)*

