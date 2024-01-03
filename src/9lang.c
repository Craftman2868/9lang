#include "9lang.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>


char renderInstruct(enum instruct instruct)
{
    if (instruct == I_NULL)
        return '?';

    return (char) instruct;
}

enum instruct loadInstruct(char *instruct)
{
    // Define_Instruct
#define D_I(...) if (memcmp(instruct, (char []) {__VA_ARGS__}, 3*3) == 0) return

    D_I(0,0,0,  // Pass / space
        0,0,0,
        0,0,0) I_PASS;
    D_I(0,1,0,  // Up / Caret
        1,1,1,
        0,0,0)  I_UP;
    D_I(0,0,0,  // Down / v
        1,1,1,
        0,1,0) I_DOWN;
    D_I(0,1,0,  // Left / lower
        1,1,0,
        0,1,0) I_LEFT;
    D_I(0,1,0,  // Right / higher
        0,1,1,
        0,1,0) I_RIGHT;
    D_I(1,0,1,  // Exit / X
        0,1,0,
        1,0,1) I_EXIT;
    D_I(0,0,0,  // Print / dot
        0,1,0,  // Unstack and print 1 char
        0,0,0) I_PRINT;
    D_I(0,0,0,  // Read / comma
        0,1,0,  // Read and stack 1 char
        0,1,0) I_READ;
    D_I(1,0,1,  // Enable/disable ascii mode: each next character will be read as ascii
        0,0,0,  //   character and added to the stack
        0,0,0) I_ASCII;
    D_I(1,0,0,  // Escape (only available in ascii mode)
        0,1,0,
        0,0,1) I_ESCAPE;
    D_I(0,1,0,  // Print all : Print until NUL character or stack empty
        0,0,0,
        0,1,0) I_PRTALL;
    D_I(0,1,0,  // Zero/null : add a NUL byte to the stack
        1,0,1,
        0,1,0) I_ZERO;

    // Ascii chars

 // D_I(...) ' ';
    D_I(0,0,1,
        0,0,1,
        0,0,1) '!';
 // D_I(...) '"';
    D_I(1,1,1,
        1,1,1,
        1,1,1) '#';
    D_I(0,1,1,
        1,1,1,
        1,1,0) '$';
    D_I(1,0,0,
        1,1,1,
        0,0,1) '%';
    D_I(0,1,1,
        1,1,0,
        1,1,1) '&';
    D_I(0,1,0,
        0,0,0,
        0,0,0) '\'';
    D_I(0,1,0,
        1,0,0,
        0,1,0) '(';
    D_I(0,1,0,
        0,0,1,
        0,1,0) ')';
    D_I(1,1,1,
        1,1,1,
        0,0,0) '*';
    D_I(0,1,0,
        1,1,1,
        0,1,0) '+';
 // D_I(...) ',';
    D_I(0,0,0,
        1,1,1,
        0,0,0) '-';
 // D_I(...) '.';
    D_I(0,0,1,
        0,1,0,
        1,0,0) '/';
 // D_I(...) '0';
    D_I(1,0,0,
        1,0,0,
        1,0,0) '1';
    D_I(0,1,0,
        0,0,1,
        1,1,1) '2';
    D_I(1,1,1,
        0,1,1,
        1,1,1) '3';
    D_I(1,0,1,
        1,1,1,
        0,0,1) '4';
    D_I(1,1,1,
        1,1,0,
        1,1,0) '5';
    D_I(0,1,1,
        1,1,1,
        0,1,0) '6';
    D_I(1,1,1,
        0,0,1,
        0,0,1) '7';
    D_I(1,0,1,
        0,0,0,
        1,0,1) '8';
    D_I(0,1,0,
        1,1,1,
        1,1,0) '9';
    D_I(0,1,0,
        0,0,0,
        0,1,0) ':';
    D_I(0,1,0,
        0,0,0,
        1,1,0) ';';
 // D_I(...) '<';
    D_I(1,1,1,
        0,0,0,
        1,1,1) '=';
 // D_I(...) '>';
    D_I(1,1,1,
        1,0,1,
        0,1,0) '?';
    D_I(0,1,0,
        1,1,1,
        0,1,1) '@';
    D_I(0,1,0,
        1,1,1,
        1,0,1) 'A';
    D_I(1,0,0,
        1,1,0,
        1,1,0) 'B';
    D_I(1,1,1,
        1,0,0,
        1,1,1) 'C';
    D_I(1,1,0,
        1,0,1,
        1,1,0) 'D';
    D_I(1,1,1,
        1,1,0,
        1,1,1) 'E';
    D_I(1,1,1,
        1,1,0,
        1,0,0) 'F';
    D_I(0,1,0,
        1,0,1,
        0,1,1) 'G';
    D_I(1,0,1,
        1,1,1,
        1,0,1) 'H';
    D_I(1,1,1,
        0,1,0,
        1,1,1) 'I';
    D_I(1,1,1,
        0,1,0,
        1,1,0) 'J';
    D_I(1,0,1,
        1,1,0,
        1,0,1) 'K';
    D_I(1,0,0,
        1,0,0,
        1,1,1) 'L';
    D_I(1,1,1,
        1,1,1,
        1,0,1) 'M';
    D_I(0,0,0,
        1,1,0,
        1,0,1) 'N';
    D_I(1,1,1,
        1,0,1,
        1,1,1) 'O';
    D_I(1,1,0,
        1,1,0,
        1,0,0) 'P';
    D_I(1,1,1,
        1,0,1,
        1,1,0) 'Q';
    D_I(1,1,0,
        1,1,0,
        1,0,1) 'R';
    D_I(0,1,1,
        0,1,0,
        1,1,0) 'S';
    D_I(1,1,1,
        0,1,0,
        0,1,0) 'T';
    D_I(1,0,1,
        1,0,1,
        1,1,1) 'U';
    D_I(0,0,0,
        1,0,1,
        0,1,0) 'V';
    D_I(0,0,0,
        1,1,1,
        1,1,1) 'W';
    D_I(1,0,1,
        0,1,0,
        1,0,1) 'X';
    D_I(1,0,1,
        0,1,0,
        0,1,0) 'Y';
    D_I(1,1,0,
        0,1,0,
        0,1,1) 'Z';
    D_I(1,1,0,
        1,0,0,
        1,1,0) '[';
 // D_I(...) '\\';
    D_I(0,1,1,
        0,0,1,
        0,1,1) ']';
 // D_I(...) '^';
    D_I(0,0,0,
        0,0,0,
        1,1,1) '_';
    D_I(1,0,0,
        0,1,0,
        0,0,0) '`';
    D_I(0,1,1,
        1,1,0,
        0,1,1) '{';
    D_I(0,1,0,
        0,1,0,
        0,1,0) '|';
    D_I(1,1,0,
        0,1,1,
        1,1,0) '}';
    D_I(0,0,0,
        1,0,1,
        0,0,0) '~';
    // Unknown instruct
    return I_NULL;

#undef D_I
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
    FILE *f = fopen(path, "r");
    struct program *prog;
    uint16_t curLineLength = 0;
    char c;
    uint16_t w = 0, h = 0;
    char *data;

    if (f == NULL)
        return NULL;  // Error opening file

    prog = malloc(sizeof (struct program));
    memset(prog, 0, sizeof (struct program));

    prog->direction = DEFAULT_DIR;

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

    *(prog->stack_pointer++) = b;

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

    *b = *(--prog->stack_pointer);

    return 0;
}

void initStack(struct program *prog)
{
    prog->stack = malloc(STACK_SIZE);
    prog->stack_pointer = prog->stack;
}

int execInstruct(struct program *prog)
{
    char c;
    int retval;
    enum instruct instruct = prog->instructs[prog->y][prog->x];

    log("Executing '%c' at %d:%d", renderInstruct(instruct), prog->x, prog->y);

    if (prog->ascii_mode)
    {
        if (prog->escape)
        {
            switch (instruct)
            {
            case I_0:
                stack(prog, 0);
                break;
            case I_UP:
                prog->uppercase = true;
                break;
            case I_DOWN:
                prog->uppercase = false;
                break;
            default:
                stack(prog, (prog->uppercase?toupper:tolower)(instruct));
                break;
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

    switch (instruct)
    {
    case I_PASS:
        break;
    case I_UP:
    case I_DOWN:
    case I_LEFT:
    case I_RIGHT:
        prog->direction = instruct;
        break;
    case I_EXIT:
        prog->running = false;
        break;
    case I_PRINT:
        retval = unstack(prog, &c);

        if (retval == 0)
            putc(c, stdout);

        return retval != 0;
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
        stack(prog, c);
        break;
    case I_ASCII:
        prog->ascii_mode = !prog->ascii_mode;
        break;
    case I_ESCAPE:
        progError(prog, "Escape used outside ascii mode");
        return 1;  // Error
    case I_0:
        stack(prog, 0);
        break;
    case I_NULL:  // Unknown instruction
        progError(prog, "Unknown instruction\n");
        return 1;  // Error
    default:
        warn(prog, "Not implemented");
        break;  // Not implemented
    }

    return 0;  // Success
}

#if OUT_IS_ERROR
#define CURSOR_OUT(dir) progError(prog, "Can't go " dir " anymore")
#else
#define CURSOR_OUT(dir) prog->running = false
#endif

void nextInstruct(struct program *prog)
{
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
}

int runProgram(struct program *prog)
{
    if (prog->instructs == NULL)  // If the program is empty
        return 0;  // Nothing to do

#if DEBUG
    for (uint16_t y = 0; y < prog->h; y++)
    {
        for (uint16_t x = 0; x < prog->w; x++)
            printf("[%c]", renderInstruct(prog->instructs[y][x]));
        printf("\n");
    }
#endif

    initStack(prog);

    prog->running = true;
    while (prog->running && execInstruct(prog) == 0 && prog->running)
        nextInstruct(prog);

    if (prog->ascii_mode)
        printf("Error: program finished in ascii mode\n");

    return 0;  // TODO
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
        log("Warning: %d bytes remaining on the stack after the program end.", prog->stack_pointer - prog->stack);

    if (prog->stack)
        free(prog->stack);

    free(prog);
}

void progError(struct program *prog, char *message)
{
    printf("Error at %d:%d ('%c'): %s\n", prog->x, prog->y, renderInstruct(prog->instructs[prog->y][prog->x]), message);
    prog->running = false;
}
