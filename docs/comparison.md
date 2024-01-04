# Comparison

There are 3 comparison operators in *9lang*:
  - **LOWER THAN** `(` *(because `<` was already used)*
    ```
    .X.
    X..
    .X.
    ```
    *(`.`s represents spaces and `X`s represents any other character)*

  - **HIGHER THAN** `)` *(because `>` was already used)*
    ```
    .X.
    ..X
    .X.
    ```
    *(`.`s represents spaces and `X`s represents any other character)*

  - **EQUAL TO** `==` (**WARNING:** There is 2 equals, if you put only 1 it enable [equal mode](modes.md))
    ```
    XXXXXX
    ......
    XXXXXX
    ```
    *(`.`s represents spaces and `X`s represents any other character)*

When the [interpreter](interpreter.md) see a comparison operator it [unstack](stack.md) first 1 byte `b` then 1 byte `a`. It compare them (e.g. `a` < `b`) and it [stack](stack.md) the result (`1` for *true*, `0` for *false*). You can now use the [conditional operator `?`](modes.md).
