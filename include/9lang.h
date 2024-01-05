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

#ifndef _9LANG_H
#define _9LANG_H

#include <stdint.h>
#include <stdbool.h>

#include "instructs.h"

// Configuration

#ifndef DEBUG
#define DEBUG false
#endif  // DEBUG

#ifndef OUT_IS_ERROR
#define OUT_IS_ERROR false  // true  : raise an error when the cursor get out
                            // false : just exit
#endif  // OUT_IS_ERROR

#ifndef STACK_SIZE
#define STACK_SIZE 512
#endif  // STACK_SIZE

// End of the configuration

#if DEBUG
#define log(f, ...) fprintf(stderr, f "\n", ##__VA_ARGS__)
#else  // !DEBUG
#define log(...) NULL
#endif  // DEBUG

#define warn(prog, f, ...) {\
    if (prog->instructs[prog->y][prog->x] == I_NULL)\
        log("Warning at [%d:%d] (?): " f, prog->x, prog->y, ##__VA_ARGS__);\
    else\
        log("Warning at [%d:%d] ('%c'): " f, prog->x, prog->y, renderInstruct(prog->instructs[prog->y][prog->x]), ##__VA_ARGS__);\
}

#define _STRINGIZE(x) #x
#define STRINGIZE(x) _STRINGIZE(x)

extern int errorN;

enum direction {
    D_UP    = '^',
    D_DOWN  = 'v',
    D_LEFT  = '<',
    D_RIGHT = '>',
} __attribute__((__packed__));

#define DEFAULT_DIRECTION D_RIGHT

enum mode {
    M_NORMAL,
    M_ASCII,
    M_EQUAL,
    M_COND,
} __attribute__((__packed__));

struct program {
    // Instructs
    uint16_t w, h;
    uint16_t x, y;
    enum direction direction;
    enum instruct **instructs;

    // Running
    bool running;
    bool error;

    // Modes
    enum mode mode;
    char mode_flags;
// Flags:
//   Normal mode
// (no flags)
//   Ascii mode
#define F_ESCAPE    0b00000001
#define F_UPPERCASE 0x00000010
//   Equal mode
#define F_HIGHPART  0x00000001
//   Conditional mode
// (no flags)
#define getFlag(prog, flag) (prog->mode_flags & flag)
#define setFlag(prog, flag, val) {if (val) prog->mode_flags |= flag; else prog->mode_flags &= ~flag;}
#define resetFlags(prog) prog->mode_flags = 0

#define setMode(prog, m) {prog->mode = m; resetFlags(prog);}
#define resetMode(prog) setMode(prog, M_NORMAL)

    // Stack
    char *stack;
    char *stack_pointer;
} __attribute__((__packed__));

// int loadInstructs(struct program *prog);
struct program *loadProgram(char *path);
// int stack(struct program *prog, char b);
// int unstack(struct program *prog, char *b);
// int peekstack(struct program *prog, char *b);
// void initStack(struct program *prog);
// void execInstruct(struct program *prog);
// void nextInstruct(struct program *prog);
int runProgram(struct program *prog);
void freeProgram(struct program *prog);

char renderInstruct(enum instruct instruct);
void progError(struct program *prog, char *message);

#endif  // _9LANG_H
