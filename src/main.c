#include <stdio.h>
#include <stdlib.h>

#include "9lang.h"

struct program *prog;
int retval;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s program", argv[0]);
        return EXIT_FAILURE;
    }

    prog = loadProgram(argv[1]);

    if (prog == NULL)
    {
        printf("%s: cannot load '%s'", argv[0], argv[1]);
        return EXIT_FAILURE;
    }

    retval = runProgram(prog);

    freeProgram(prog);

    return retval;
}
