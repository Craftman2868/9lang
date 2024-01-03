# Instructions

Here is the list of every instructions understood by the interpreter.

*(`.`s represent spaces and `X`s represent any other character)*

> - **PASS** ` ` (space)
>   ```
>   ...
>   ...
>   ...
>   ```
>
>   Do nothing.
>

> - **UP** `^`
>   ```
>   .X.
>   XXX
>   ...
>   ```
>
>   Change the [direction](direction.md) to **Up**.
>

> - **DOWN** `v`
>   ```
>   ...
>   XXX
>   .X.
>   ```
>
>   Change the [direction](direction.md) to **Down**.
>

> - **LEFT** `<`
>   ```
>   .X.
>   XX.
>   .X.
>   ```
>
>   Change the [direction](direction.md) to **Left**.
>

> - **RIGHT** `>`
>   ```
>   .X.
>   .XX
>   .X.
>   ```
>
>   Change the [direction](direction.md) to **Right**.
>

> - **EXIT** `X`
>   ```
>   X.X
>   .X.
>   X.X
>   ```
>
>   Exit the program.
>

> - **PRINT** `.`
>   ```
>   ...
>   .X.
>   ...
>   ```
>
>   [Unstack](stack.md) 1 character and print it to *stdout*.
>

> - **PRTALL** `.` (Print all)
>   ```
>   ...
>   .X.
>   ...
>   ```
>
>   [Unstack](stack.md) and print to *stdout* each character of the [stack](stack.md) until a NUL character or the [stack](stack.md) is empty.
>
>   *Note:* if you use it to print a string remember it will be reversed because the [interpreter](interpreter.md) will [unstack](stack.md) byte in the other direction than you stacked them.
>

> - **READ** `,`
>   ```
>   ...
>   .X.
>   .X.
>   ```
>
>   Read 1 character from **stdin** and [stack](stack.md) it.
>

> - **ASCII** `"`
>   ```
>   X.X
>   ...
>   ...
>   ```
>
>   Toggle [*ascii mode*](ascii_mode.md).
>

> - **ZERO** `0`
>   ```
>   .X.
>   X.X
>   .X.
>   ```
>
>   [Stack](stack.md) a NUL byte.
>
