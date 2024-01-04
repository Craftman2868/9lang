# Errors

In *9lang*, the errors always follow the next format:
```
Error at <x>:<y> (<instruct>): <error message>
```
except the `Program didn't finish in normal mode` error that doesn't have any position.


## Error messages

|Error message|Description|Solution|
|-------------|-----------|--------|
|[Stack](stack.md) full (stack size = `<stack size>`)|You tried to [stack](stack.md) a byte but there is no more space in it|Try to use less space or increase stack size in the [`include/9lang.h`](../include/9lang.h) file
|[Stack](stack.md) empty|You tried to [unstack](stack.md) a byte but the stack was already empty|Don't [unstack](stack.md) a byte when the stack is empty|
|Unknown [instruction](instructions.md)|You asked the [interpreter](instructions.md) to execute an unknown [instruction](instructions.md)|Make the program never come to this instruction or look at the [list of the instructions](instructions.md) to known how to write the wanted one|
|Unknown [escape](ascii_mode.md) character|You tried to [escape](ascii_mode.md) an character that cannot be escaped|Look at the [list of escaped characters](ascii_mode.md)|
|Invalid [instruction](instructions.md) in [equal mode](modes.md)|Only hexadecimal characters are available in [equal mode](modes.md)|See [Equal mode](modes.md)|
|Escape used outside [ascii mode](ascii_mode.md)|You tried to escape a character but you were not in [ascii mode](ascii_mode.md)|Enter in [ascii mode](ascii_mode.md) using the **ASCII** `"` instruct or remove the **ESCAPE** `\` [instruction](instructions.md)|
|Can't go [`<direction>`](direction.md) anymore|You reached the program border|Use an **EXIT** `X` instruction to end the program (This error message can be disabled in [`include/9lang.h`](../include/9lang.h))|
|Program didn't finish in normal mode|Raised at the end of the program if it isn't in [normal mode](modes.md)|Return in normal mode (see [Modes](modes.md))|
<!--|Stack not initialized|Raised when you try to use the stack but it is not initialized|(Not possible)|-->
