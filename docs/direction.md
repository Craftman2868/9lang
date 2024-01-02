# Direction

The program has a direction, it can change during the execution. When the [interpreter](interpreter.md) executes in instruction, the direction determine what instruction will be executed next. Direction can be:
  - **Up**
  - **Down**
  - **Left**
  - **Right**

The default direction (when the program starts) is **Right**.

*For example:* if the direction is **Right** and the [interpreter](interpreter.md) have just executed the instruction `3:2`, the next intruction will be `4:2`.

Direction can be modified using the following instructions:
  - `^` (go **up**)
    ```
    .X.
    XXX
    ...
    ```
    *(`.`s represent spaces and `X`s represent any other character)*

  - `v` (go **down**)
    ```
    ...
    XXX
    .X.
    ```
    *(`.`s represent spaces and `X`s represent any other character)*

  - `<` (go **left**)
    ```
    .X.
    XX.
    .X.
    ```
    *(`.`s represent spaces and `X`s represent any other character)*

  - `>` (go **right**)
    ```
    .X.
    .XX
    .X.
    ```
    *(`.`s represent spaces and `X`s represent any other character)*

