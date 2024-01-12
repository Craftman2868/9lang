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
#include <stdio.h>

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

#ifndef STACK_COUNT
#define STACK_COUNT 128
#endif  // STACK_COUNT

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
    M_NO_MOVE,
} __attribute__((__packed__));

struct program;

struct stack {
    char data[STACK_SIZE];
    char *top;
    struct program *prog;
};

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

    // Stacks
    struct stack *stacks[STACK_COUNT];
    struct stack **cur_stack;
#define STACK_N(prog) ((prog->stacks - prog->cur_stack) / sizeof (*prog->stacks))
};

char renderInstruct(enum instruct instruct);

int loadInstructs(struct program *prog, FILE *f);

struct program *loadProgram(char *path);

int initStack(struct program *prog, struct stack **stack_ptr);

int nextStack(struct program *prog);
int prevStack(struct program *prog);

int stack(struct stack *stack, char b);
int stack_prog(struct program *prog, char b);
int unstack(struct stack *stack, char *b);
int unstack_prog(struct program *prog, char *b);
int peekstack(struct stack *stack, char *b);
int peekstack_prog(struct program *prog, char *b);

int execInstruct(struct program *prog);
void nextInstruct(struct program *prog);

int runProgram(struct program *prog);

void freeProgram(struct program *prog);

void progError(struct program *prog, char *message);
void stackError(struct stack *stack, char *message);

#endif  // _9LANG_H
