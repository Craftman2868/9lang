# The interpreter

The interpreter is named `9`. You can call it just using the `9 <program path>` command (if the command path contains spaces put `"` around it). The interpreter is really simple, it just takes 1 argument: the program path. It doesn't have any help command. If you run it without argument, it will show `Usage: 9 program` (or the path to the executable instead of `9`). If any error happen while loading the program (e.g. the file is not found), it will show the error `9: cannot load '<program path>'`. If the given file is empty, the interpreter won't do anything.

Errors might occur during the program execution. For more informations, see [Errors](errors.md).

If the debug mode is enabled (in [`include/9lang.h`](../include/9lang.h)), some debug data will be printed in *stderr*.
