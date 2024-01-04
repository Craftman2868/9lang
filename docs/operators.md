# Operators

There are 4 operators in *9lang*:
  - **PLUS** `+` (addition)
    ```
    .X.
    XXX
    .X.
    ```

  - **MINUS** `-` (substaction)
    ```
    ...
    XXX
    ...
    ```

  - **MUL** `*` (multiplication)
    ```
    XXX
    XXX
    ...
    ```

  - **DIV** `/` (division)
    ```
    ..X
    .X.
    X..
    ```

*(`.`s represent spaces and `X`s represent any other character)*


When the [interpreter](interpreter.md) execute an operator it [unstack](stack.md) first 1 byte `b` then 1 byte `a`. It operate them (e.g. `a` - `b` or `a` / `b`) and it [stack](stack.md) the result (**WARNING:** It is stored on 1 byte so the result is *modulo* 256). You can then reuse the result in other operation.
