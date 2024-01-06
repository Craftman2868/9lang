// 9lang is an esoteric programming language.
// Copyright (C) 2024 Craftman2868

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include <stdio.h>
#include <stdlib.h>

#include "9lang.h"

struct program *prog;
int retval;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s program\n", argv[0]);
        return EXIT_FAILURE;
    }

    prog = loadProgram(argv[1]);

    if (prog == NULL)
    {
        switch (errorN)
        {
        case 1:  // Error opening the file
            printf("%s: cannot load '%s'\n", argv[0], argv[1]);
            break;
        case 2:  // Not enough memory
            printf("%s: not enough memory to load the program\n", argv[0]);
            break;
        default:
            printf("%s: unknown error while program loading\n", argv[0]);
            break;
        }
        return EXIT_FAILURE;
    }

    retval = runProgram(prog);

    freeProgram(prog);

    return retval;
}
