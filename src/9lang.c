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

#include "9lang.h"

#define __STDC_WANT_LIB_EXT2__  1  // To enable 'asprintf'

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

#include "instructs.h"

int errorN = 0;

char renderInstruct(enum instruct instruct)
{
    if (instruct == I_NULL)
        return '?';

    return (char) instruct;
}

int loadInstructs(struct program *prog, FILE *f)
{
    char instructs[prog->h][prog->w][3*3];
    memset(instructs, 0, sizeof (instructs));
    int c;

    prog->x = prog->y = 0;
    do {
        c = fgetc(f);

        if (c == '\n' || c == '\r' || c == EOF)
        {
            memset(&instructs[prog->y / 3][prog->x / 3][(prog->y % 3) * 3 + (prog->x % 3)], 0, 3 - (prog->x % 3));
            prog->y++;
            prog->x = 0;
        }
        else
        {
            instructs[prog->y / 3][prog->x / 3][(prog->y % 3) * 3 + (prog->x % 3)] = (c == ' ') ? 0 : 1;
            prog->x++;
        }
    } while (c != EOF);

    prog->instructs = malloc(prog->h * sizeof (enum instruct *));

    if (prog->instructs == NULL)
    {
        log("Warning: Error allocating %lu byte (for prog->instructs)", prog->h * sizeof (enum instruct *));
        return 1;  // Malloc error
    }

    for (prog->y = 0; prog->y < prog->h; prog->y++)
    {
        prog->instructs[prog->y] = malloc(prog->w * sizeof (enum instruct));
        if (prog->instructs[prog->y] == NULL)
        {
            log("Warning: Error allocating %lu byte (for prog->instructs[%d])", prog->w * sizeof (enum instruct), prog->y);
            return 1;  // Malloc error
        }
    }

    for (prog->y = 0; prog->y < prog->h; prog->y++)
    {
        for (prog->x = 0; prog->x < prog->w; prog->x++)
        {  
            prog->instructs[prog->y][prog->x] = loadInstruct(instructs[prog->y][prog->x]);
        }
    }

    prog->x = prog->y = 0;

    return 0;  // Success
}

struct program *loadProgram(char *path)
{
    struct program *prog;
    int c;
    uint16_t curLineLength = 0;
    uint16_t w = 0, h = 0;

    FILE *f = fopen(path, "r");

    if (f == NULL)
    {
        errorN = 1;  // Error opening the file
        return NULL;
    }

    prog = malloc(sizeof (struct program));

    if (prog == NULL)
    {
        log("Warning: Error allocating %lu byte (for prog)", sizeof (struct program));
        errorN = 2;  // Not enough memory
        return NULL;
    }

    memset(prog, 0, sizeof (struct program));

    prog->direction = DEFAULT_DIRECTION;

    do {
        c = fgetc(f);

        if (c == '\n' || c == '\r' || c == EOF)
        {
            h++;
            if (curLineLength > w)
                w = curLineLength;
            curLineLength = 0;
        }
        else
        {
            curLineLength++;
        }
    } while (c != EOF);

    if (w * h == 0)
        return prog;  // Nothing to load

    prog->w = w / 3 + (w % 3 ? 1 : 0);
    prog->h = h / 3 + (h % 3 ? 1 : 0);

    rewind(f);

    if (loadInstructs(prog, f) != 0)
    {
        freeProgram(prog);
        errorN = 2;  // Not enough memory
        return NULL;
    }

    fclose(f);

    log("%d, %d", prog->w, prog->h);

    return prog;
}

int initStack(struct program *prog, struct stack **stack_ptr)
{
    *stack_ptr = malloc(sizeof (struct stack));

    if (*stack_ptr == NULL)
    {
        log("Warning: Error allocating %lu byte (for prog->stacks[%u])", sizeof (struct stack), (unsigned int) STACK_N(prog));
        return 1;
    }

    (*stack_ptr)->top = (*stack_ptr)->data;
    (*stack_ptr)->prog = prog;

    return 0;
}

int nextStack(struct program *prog)
{
    if (prog->cur_stack != &prog->stacks[STACK_COUNT-1])
        prog->cur_stack += sizeof (prog->cur_stack);
    else
        prog->cur_stack = &prog->stacks[0];

    if (*prog->cur_stack == NULL)
        return initStack(prog, prog->cur_stack);

    return 0;
}

int prevStack(struct program *prog)
{
    if (prog->cur_stack != &prog->stacks[0])
        prog->cur_stack -= sizeof (prog->cur_stack);
    else
        prog->cur_stack = &prog->stacks[STACK_COUNT-1];

    if (*prog->cur_stack == NULL)
        return initStack(prog, prog->cur_stack);

    return 0;
}

int stack(struct stack *stack, char b)
{
    if (stack->top - stack->data >= STACK_SIZE)
    {
        stackError(stack, "Stack full (stack size = " STRINGIZE(STACK_SIZE) ")");
        return 1;
    }

    log("Stack byte %d", b);

    *stack->top = b;

    stack->top++;

    return 0;  // Success
}

int unstack(struct stack *stack, char *b)
{
    if (stack->top == stack->data)
    {
        stackError(stack, "Stack empty");
        return 1;
    }

    stack->top--;

    if (b != NULL)
        *b = *stack->top;

    log("Unstack byte %d", *stack->top);

    return 0;
}

int peekstack(struct stack *stack, char *b)
{
    if (stack->top == stack->data)
    {
        stackError(stack, "Stack empty");
        return 1;
    }

    *b = *(stack->top-1);

    log("Peeked byte %d from the stack", *stack->top);

    return 0;
}

int execInstruct(struct program *prog)
{
    char a, b, c;
    enum instruct instruct = prog->instructs[prog->y][prog->x];

    if (instruct == I_NULL)
        log("Executing ? at %d:%d", prog->x, prog->y);
    else
        log("Executing '%c' at %d:%d", renderInstruct(instruct), prog->x, prog->y);

    if (instruct == I_NULL)
    {
        progError(prog, "Unknown instruction");
        return 1;  // Error
    }

    switch (prog->mode)
    {
    case M_ASCII:
        if (getFlag(prog, F_ESCAPE))
        {
            switch (instruct)
            {
            case I_UP:
                setFlag(prog, F_UPPERCASE, true);
                break;
            case I_DOWN:
                setFlag(prog, F_UPPERCASE, false);
                break;
            case I_ZERO:
                stack(*prog->cur_stack, 0);
                break;
            case 'A':
                stack(*prog->cur_stack, '\a');
                break;
            case 'B':
                stack(*prog->cur_stack, '\b');
                break;
            case 'E':
                stack(*prog->cur_stack, '\033');
                break;
            case 'F':
                stack(*prog->cur_stack, '\f');
                break;
            case 'N':
                stack(*prog->cur_stack, '\n');
                break;
            case 'R':
                stack(*prog->cur_stack, '\r');
                break;
            case 'T':
                stack(*prog->cur_stack, '\t');
                break;
            case 'V':
                stack(*prog->cur_stack, '\v');
                break;
            case '\\':
            case '"':
                stack(*prog->cur_stack, (char) instruct);
                break;
            default:
                progError(prog, "Unknown escape character");
                return 1;  // Error
            }
            setFlag(prog, F_ESCAPE, false);
            return 0;  // Success
        }

        switch (instruct)
        {
        case I_ASCII:
            resetMode(prog);
            break;
        case I_ESCAPE:
            setFlag(prog, F_ESCAPE, true);
            break;
        default:
            stack(*prog->cur_stack, (getFlag(prog, F_UPPERCASE)?toupper:tolower)(instruct));
            break;
        }

        return 0;  // Success

    case M_EQUAL:
        if (getFlag(prog, F_HIGHPART) && instruct == I_EQUAL)  // '=='  (is equal)
        {
            resetMode(prog);
            if (unstack(*prog->cur_stack, &b) == 0 && unstack(*prog->cur_stack, &a) == 0)
                return stack(*prog->cur_stack, a == b) != 0;  // Succeess / error (stack full, not possible)
            else
                return 1;  // Error (stack empty)
        }

        if ((instruct < '0' || instruct > '9') && (instruct < 'A' || instruct > 'F'))
        {
            progError(prog, "Invalid instruction in equal mode");
            return 1;
        }

        a = (char) instruct;

        if (a > 'A')
            a = a - 'A' + 10;
        else
            a = a - '0';

        if (getFlag(prog, F_HIGHPART))
        {
            setFlag(prog, F_HIGHPART, false);
            stack(*prog->cur_stack, a * 16);
        }
        else
        {
            resetMode(prog);

            if (unstack(*prog->cur_stack, &b) == 0)
                stack(*prog->cur_stack, b | a);
            else
                return 1;  // Error (stack empty, not possible)
        }

        return 0;  // Success
    case M_COND:
        if (unstack(*prog->cur_stack, &b) != 0)
            return 1;  // Error (stack empty)

        resetMode(prog);
        if (b == 0)
            return 0;  // Success (don't execute the instruction)
     // else
        break;  // execute the instruction...
    case M_NORMAL:
    default:
        break;
    }

    // Normal mode
    switch (instruct)
    {
    case I_PASS:
        break;

    // Moves
    //   Directions
    case I_UP:      // '^'
    case I_DOWN:    // 'v'
    case I_LEFT:    // '<'
    case I_RIGHT:   // '>'
        prog->direction = (enum direction) instruct;
        break;
    //   Set pos
   case I_AT:
        if (unstack(*prog->cur_stack, &b) != 0)
            return 1;  // Error (stack empty)
        if (unstack(*prog->cur_stack, &a) != 0)
            return 1;  // Error (stack empty)
        prog->x = a;
        prog->y = b;
        setMode(prog, M_NO_MOVE);
        break;

    // Exit
    case I_EXIT:    // 'X'
        prog->running = false;
        break;

    // IO
    case I_PRINT:   // "."
        if (unstack(*prog->cur_stack, &c) == 0)
            putc(c, stdout);
        else
            return 1;  // Error (stack empty)
    case I_PRTALL:  // ':'
        while ((*prog->cur_stack)->top != (*prog->cur_stack)->data && unstack(*prog->cur_stack, &c) == 0)
        {
            if (c == 0)
                break;
            putc(c, stdout);
        }
        break;
    case I_READ:    // ','
        c = getchar();
        return stack(*prog->cur_stack, c) != 0;  // Success / error (stack full)

    // Stack / Ascii mode
    case I_OBRCKT:  // switch to the next stack
        if (nextStack(prog) != 0)
            return 1;
        break;
    case I_CBRCKT:  // switch to the previous stack
        if (prevStack(prog) != 0)
            return 1;
        break;
    case I_ASCII:   // '"'
        setMode(prog, M_ASCII);
        break;
    case I_ESCAPE:  // '\'
        progError(prog, "Escape used outside ascii mode");
        return 1;   // Error
    case I_ZERO:    // '0'
        return stack(*prog->cur_stack, 0) != 0;  // Success / error (stack full)
    case I_SHARP:   // '#'
        if (peekstack(*prog->cur_stack, &b) == 0)
            return stack(*prog->cur_stack, b) != 0;  // Success / error (stack full)
        else
            return 1;  // Error (stack empty)
    case I_EQUAL:   // '='
        setMode(prog, M_EQUAL);
        setFlag(prog, F_HIGHPART, true);
        break;
    case I_EXCL:   // '!'
        return unstack(*prog->cur_stack, NULL) != 0;  // Success / error (stack empty)

    // Operators
    case I_PLUS:    // '+'
        if (unstack(*prog->cur_stack, &b) == 0 && unstack(*prog->cur_stack, &a) == 0)
            return stack(*prog->cur_stack, a + b) != 0;  // Success / error (stack full, not possible)
        else
            return 1;  // Error (stack empty)
    case I_MINUS:   // '-'
        if (unstack(*prog->cur_stack, &b) == 0 && unstack(*prog->cur_stack, &a) == 0)
            return stack(*prog->cur_stack, a - b) != 0;  // Success / error (stack full, not possible)
        else
            return 1;  // Error (stack empty)
    case I_MUL:     // '*'
        if (unstack(*prog->cur_stack, &b) == 0 && unstack(*prog->cur_stack, &a) == 0)
            return stack(*prog->cur_stack, a * b) != 0;  // Success / error (stack full, not possible)
        else
            return 1;  // Error (stack empty)
    case I_DIV:     // '/'
        if (unstack(*prog->cur_stack, &b) == 0 && unstack(*prog->cur_stack, &a) == 0)
            return stack(*prog->cur_stack, a / b) != 0;  // Success / error (stack full, not possible)
        else
            return 1;  // Error (stack empty)
    case I_AND:     // '&'
        if (unstack(*prog->cur_stack, &b) == 0 && unstack(*prog->cur_stack, &a) == 0)
            return stack(*prog->cur_stack, a & b) != 0;  // Success / error (stack full, not possible)
        else
            return 1;  // Error (stack empty)
    case I_VBAR:    // '|'
        if (unstack(*prog->cur_stack, &b) == 0 && unstack(*prog->cur_stack, &a) == 0)
            return stack(*prog->cur_stack, a | b) != 0;  // Success / error (stack full, not possible)
        else
            return 1;  // Error (stack empty)


    // Conditions
    //   Comparison operators
    case I_OPAR:    // '('  (used as '<')
        if (unstack(*prog->cur_stack, &b) == 0 && unstack(*prog->cur_stack, &a) == 0)
            return stack(*prog->cur_stack, a < b) != 0;  // Succeess / error (stack full, not possible)
        else
            return 1;  // Error (stack empty)
    case I_CPAR:    // ')'  (used as '>')
        if (unstack(*prog->cur_stack, &b) == 0 && unstack(*prog->cur_stack, &a) == 0)
            return stack(*prog->cur_stack, a > b) != 0;
        else
            return 1;  // Error (stack empty)
 // case I_EQUAL:  // See in 'if (prog->equal_mode)' above
 //     ...
    //   Conditional mode
     case I_QMARK:  // '?'
        setMode(prog, M_COND)
        break;

    // Unknown / not implemented
 // case I_NULL:  // Unknown instruction  // Cached below
 //     progError(prog, "Unknown instruction");
 //     return 1;  // Error
    default:
        warn(prog, "Not implemented");
        break;  // Not implemented
    }

    return 0;  // Success
}
void nextInstruct(struct program *prog)
{
    if (prog->mode == M_NO_MOVE)
    {
        resetMode(prog);
        return;
    }

#if OUT_IS_ERROR
#define CURSOR_OUT(dir) progError(prog, "Can't go " dir " anymore")
#else
#define CURSOR_OUT(dir) warn(prog, "Can't go " dir " anymore"); prog->running = false
#endif

    switch (prog->direction)
    {
    case D_UP:
        if (prog->y <= 0)
        {
            CURSOR_OUT("up");
            return;
        }
        prog->y--;
        break;
    case D_DOWN:
        if (prog->y >= prog->h - 1)
        {
            CURSOR_OUT("down");
            return;
        }
        prog->y++;
        break;
    case D_LEFT:
        if (prog->x <= 0)
        {
            CURSOR_OUT("left");
            return;
        }
        prog->x--;
        break;
    case D_RIGHT:
        if (prog->x >= prog->w - 1)
        {
            CURSOR_OUT("right");
            return;
        }
        prog->x++;
        break;
    }
#undef CURSOR_OUT
}

int runProgram(struct program *prog)
{
    if (prog->instructs == NULL)  // If the program is empty
        return 0;  // Nothing to do

#if DEBUG
    for (uint16_t y = 0; y < prog->h; y++)
    {
        for (uint16_t x = 0; x < prog->w; x++)
            fprintf(stderr, "[%c]", renderInstruct(prog->instructs[y][x]));
        fprintf(stderr, "\n");
    }
#endif

    prog->cur_stack = &prog->stacks[0];

    if (initStack(prog, prog->cur_stack) != 0)
    {
        return 2;  // Not enough memory
    }

    prog->running = true;
    while (prog->running && execInstruct(prog) == 0 && prog->running)
        nextInstruct(prog);

    if (!prog->error && prog->mode != M_NORMAL)
    {
        printf("Error: Program didn't finish in normal mode\n");
        prog->error = true;
    }

    return (int) prog->error;
}

void freeProgram(struct program *prog)
{
    if (prog->instructs != NULL)
    {
        for (prog->y = 0; prog->y < prog->h; prog->y++)
            free(prog->instructs[prog->y]);

        free(prog->instructs);
    }

    for (unsigned int i = 0; i < STACK_COUNT; i++)
    {
        if (prog->stacks[i] != NULL)
        {
#if DEBUG
            if (!prog->error && prog->stacks[i]->top != prog->stacks[i]->data)
                log("Warning: %lu bytes remaining on the stack after the program end.", prog->stacks[i]->top - prog->stacks[i]->data);
#endif  // DEBUG
            free(prog->stacks[i]);
        }
    }

    free(prog);
}

void progError(struct program *prog, char *message)
{
    if (prog->instructs[prog->y][prog->x] == I_NULL)
        printf("Error at %d:%d (?): %s\n", prog->x, prog->y, message);
    else
        printf("Error at %d:%d ('%c'): %s\n", prog->x, prog->y, renderInstruct(prog->instructs[prog->y][prog->x]), message);
    prog->running = false;
    prog->error = true;
}

void stackError(struct stack *stack, char *message)
{
    if (stack->prog->instructs[stack->prog->y][stack->prog->x] == I_NULL)
        printf("Error at %d:%d (?) (Stack %u): %s\n", stack->prog->x, stack->prog->y, (unsigned int) STACK_N(stack->prog) + 1, message);
    else
        printf("Error at %d:%d ('%c') (Stack %u): %s\n", stack->prog->x, stack->prog->y, renderInstruct(stack->prog->instructs[stack->prog->y][stack->prog->x]), (unsigned int) STACK_N(stack->prog) + 1, message);
    stack->prog->running = false;
    stack->prog->error = true;
}
