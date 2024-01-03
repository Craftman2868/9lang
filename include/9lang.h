#ifndef _9LANG_H
#define _9LANG_H

#include <stdint.h>
#include <stdbool.h>

#include "instructs.h"

// Configuration

#define DEBUG true
#define OUT_IS_ERROR false  // true  : raise an error when the cursor get out
                            // false : just exit

#define STACK_SIZE 512

// End of the configuration

#if DEBUG
#define log(f, ...) fprintf(stderr, f "\n", ##__VA_ARGS__)
#else
#define log(...) {}
#endif

#define warn(prog, f, ...) log("Warning at [%d:%d] ('%c'): " f, prog->x, prog->y, prog->instructs[prog->y][prog->x], ##__VA_ARGS__)

#define STRINGIZE(x) #x

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

// void loadInstructs(struct program *prog);
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
