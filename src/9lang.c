#include "9lang.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

#include "instructs.h"


char renderInstruct(enum instruct instruct)
{
    if (instruct == I_NULL)
        return '?';

    return (char) instruct;
}

void loadInstructs(struct program *prog, FILE *f)
{
    char instructs[prog->h][prog->w][3*3] = {};
    char c;

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

    for (prog->y = 0; prog->y < prog->h; prog->y++)
        prog->instructs[prog->y] = malloc(prog->w * sizeof (enum instruct));

    for (prog->y = 0; prog->y < prog->h; prog->y++)
    {
        for (prog->x = 0; prog->x < prog->w; prog->x++)
        {  
            prog->instructs[prog->y][prog->x] = loadInstruct(instructs[prog->y][prog->x]);
        }
    }

    prog->x = prog->y = 0;
}

struct program *loadProgram(char *path)
{
    struct program *prog;
    char c;
    uint16_t curLineLength = 0;
    uint16_t w = 0, h = 0;

    FILE *f = fopen(path, "r");

    if (f == NULL)
        return NULL;  // Error opening file

    prog = malloc(sizeof (struct program));
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

    loadInstructs(prog, f);

    fclose(f);

    log("%d, %d", prog->w, prog->h);

    return prog;
}

int stack(struct program *prog, char b)
{
    if (prog->stack == NULL)
    {
        progError(prog, "Stack not initialized");  // Not possible
        return -1;
    }

    if (prog->stack_pointer - prog->stack >= STACK_SIZE)
    {
        progError(prog, "Stack full (stack size = " STRINGIZE(STACK_SIZE) ")");
        return 1;
    }

    log("Stack byte %d", b);

    *prog->stack_pointer = b;

    prog->stack_pointer++;

    return 0;  // Success
}

int unstack(struct program *prog, char *b)
{
    if (prog->stack == NULL)
    {
        progError(prog, "Stack not initialized");  // Not possible
        return -1;
    }

    if (prog->stack_pointer == prog->stack)
    {
        progError(prog, "Stack empty");
        return 1;
    }

    prog->stack_pointer--;

    if (b != NULL)
        *b = *prog->stack_pointer;

    log("Unstack byte %d", *prog->stack_pointer);

    return 0;
}

int peekstack(struct program *prog, char *b)
{
    if (prog->stack == NULL)
    {
        progError(prog, "Stack not initialized");  // Not possible
        return -1;
    }

    if (prog->stack_pointer == prog->stack)
    {
        progError(prog, "Stack empty");
        return 1;
    }

    *b = *(prog->stack_pointer);

    return 0;
}

void initStack(struct program *prog)
{
    prog->stack = malloc(STACK_SIZE);
    prog->stack_pointer = prog->stack;
}

int execInstruct(struct program *prog)
{
    char a, b, c;
    enum instruct instruct = prog->instructs[prog->y][prog->x];

    log("Executing '%c' at %d:%d", renderInstruct(instruct), prog->x, prog->y);

    if (prog->ascii_mode)
    {
        if (prog->escape)
        {
            switch (instruct)
            {
            case I_UP:
                prog->uppercase = true;
                break;
            case I_DOWN:
                prog->uppercase = false;
                break;
            case I_ZERO:
                stack(prog, 0);
                break;
            case 'A':
                stack(prog, '\a');
                break;
            case 'B':
                stack(prog, '\b');
                break;
            case 'E':
                stack(prog, '\033');
                break;
            case 'F':
                stack(prog, '\f');
                break;
            case 'N':
                stack(prog, '\n');
                break;
            case 'R':
                stack(prog, '\r');
                break;
            case 'T':
                stack(prog, '\t');
                break;
            case 'V':
                stack(prog, '\v');
                break;
            case '\\':
            case '"':
                stack(prog, (char) instruct);
                break;
            default:
                progError(prog, "Unknown escape character");
                return 1;  // Error
            }
            prog->escape = false;
            return 0;  // Success
        }

        switch (instruct)
        {
        case I_ASCII:
            prog->uppercase = false;
            prog->ascii_mode = false;
            break;
        case I_ESCAPE:
            prog->escape = true;
            break;
        default:
            stack(prog, (prog->uppercase?toupper:tolower)(instruct));
            break;
        }

        return 0;  // Success
    }

    if (prog->equal_mode)
    {
        if (prog->high_part && instruct == I_EQUAL)  // '=='  (is equal)
        {
            prog->equal_mode = prog->high_part = false;
            if (unstack(prog, &b) == 0 && unstack(prog, &a) == 0)
                return stack(prog, a == b) != 0;  // Succeess / error (stack full, not possible)
            else
                return 1;  // Error (stack empty)
        }

        if ((instruct < '0' || instruct > '9') && (instruct < 'A' || instruct > 'F'))
        {
            progError(prog, "Invalid instruction in equal mode");
        }

        a = (char) instruct;

        if (a > 'A')
            a = a - 'A' + 10;
        else
            a = a - '0';

        if (prog->high_part)
        {
            prog->high_part = false;
            stack(prog, a * 16);
        }
        else
        {
            prog->equal_mode = prog->high_part = false;

            if (unstack(prog, &b) == 0)
                stack(prog, b | a);
            else
                return 1;  // Error (stack empty, not possible)
        }

        return 0;  // Success
    }

    switch (instruct)
    {
    case I_PASS:
        break;

    // Directions
    case I_UP:
    case I_DOWN:
    case I_LEFT:
    case I_RIGHT:
        prog->direction = (enum direction) instruct;
        break;

    // Exit
    case I_EXIT:
        prog->running = false;
        break;

    // IO
    case I_PRINT:
        if (unstack(prog, &c) == 0)
            putc(c, stdout);
        else
            return 1;  // Error (stack empty)
    case I_PRTALL:
        while (prog->stack_pointer != prog->stack && unstack(prog, &c) == 0)
        {
            if (c == 0)
                break;
            putc(c, stdout);
        }
        break;
    case I_READ:
        c = getchar();
        return stack(prog, c) != 0;  // Success / error (stack full)

    // Stack / Ascii mode
    case I_ASCII:
        prog->ascii_mode = !prog->ascii_mode;
        break;
    case I_ESCAPE:
        progError(prog, "Escape used outside ascii mode");
        return 1;  // Error
    case I_ZERO:
        return stack(prog, 0) != 0;  // Success / error (stack full)
    case I_SHARP:
        if (peekstack(prog, &b) == 0)
            return stack(prog, b) != 0;  // Success / error (stack full)
        else
            return 1;  // Error (stack empty)

    // Operators
    case I_PLUS:
        if (unstack(prog, &b) == 0 && unstack(prog, &a) == 0)
            return stack(prog, a + b) != 0;  // Success / error (stack full, not possible)
        else
            return 1;  // Error (stack empty)
    case I_MINUS:
        if (unstack(prog, &b) == 0 && unstack(prog, &a) == 0)
            return stack(prog, a - b) != 0;  // Success / error (stack full, not possible)
        else
            return 1;  // Error (stack empty)
    case I_MUL:
        if (unstack(prog, &b) == 0 && unstack(prog, &a) == 0)
            return stack(prog, a * b) != 0;  // Success / error (stack full, not possible)
        else
            return 1;  // Error (stack empty)
    case I_DIV:
        if (unstack(prog, &b) == 0 && unstack(prog, &a) == 0)
            return stack(prog, a / b) != 0;  // Success / error (stack full, not possible)
        else
            return 1;  // Error (stack empty)
    case I_EQUAL:
        prog->equal_mode = true;
        prog->high_part = true;
        break;

    // Comparison operators
    case I_OPAR:  // '('  (used as '<')
        if (unstack(prog, &b) == 0 && unstack(prog, &a) == 0)
            return stack(prog, a < b) != 0;  // Succeess / error (stack full, not possible)
        else
            return 1;  // Error (stack empty)
    case I_CPAR:  // ')'  (used as '>')
        if (unstack(prog, &b) == 0 && unstack(prog, &a) == 0)
            return stack(prog, a > b) != 0;
        else
            return 1;  // Error (stack empty)
 // case I_EQUAL:  // See in 'if (prog->equal_mode)' above
 //     ...

    // Unknown / not implemented
    case I_NULL:  // Unknown instruction
        progError(prog, "Unknown instruction\n");
        return 1;  // Error    
    default:
        warn(prog, "Not implemented");
        break;  // Not implemented
    }

    return 0;  // Success
}
void nextInstruct(struct program *prog)
{
#if OUT_IS_ERROR
#define CURSOR_OUT(dir) progError(prog, "Can't go " dir " anymore")
#else
#define CURSOR_OUT(dir) prog->running = false
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

    initStack(prog);

    prog->running = true;
    while (prog->running && execInstruct(prog) == 0 && prog->running)
        nextInstruct(prog);

    if (prog->ascii_mode)
        printf("Error: program finished in ascii mode\n");

    return 0;
}

void freeProgram(struct program *prog)
{
    if (prog->instructs != NULL)
    {
        for (prog->y = 0; prog->y < prog->h; prog->y++)
            free(prog->instructs[prog->y]);

        free(prog->instructs);
    }

    if (prog->stack_pointer != prog->stack)
        log("Warning: %ld bytes remaining on the stack after the program end.", prog->stack_pointer - prog->stack);

    if (prog->stack)
        free(prog->stack);

    free(prog);
}

void progError(struct program *prog, char *message)
{
    printf("Error at %d:%d ('%c'): %s\n", prog->x, prog->y, renderInstruct(prog->instructs[prog->y][prog->x]), message);
    prog->running = false;
}
