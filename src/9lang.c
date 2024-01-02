#include "9lang.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>


char renderInstruct(enum instruct instruct)
{
    return (char) instruct;
}

enum instruct loadInstruct(char *instruct)
{
#define IF_INSTRUCT(...) if (memcmp(instruct, (char []) {__VA_ARGS__}, 3*3) == 0)

    IF_INSTRUCT(0,0,0,  // Pass
                0,0,0,
                0,0,0)
        return I_PASS;
    IF_INSTRUCT(0,1,0,  // Up
                1,1,1,
                0,0,0)
        return I_UP;
    IF_INSTRUCT(0,0,0,  // Down
                1,1,1,
                0,1,0)
        return I_DOWN;
    IF_INSTRUCT(0,1,0,  // Left
                1,1,0,
                0,1,0)
        return I_LEFT;
    IF_INSTRUCT(0,1,0,  // Right
                0,1,1,
                0,1,0)
        return I_RIGHT;
    IF_INSTRUCT(1,0,1,  // Exit
                0,1,0,
                1,0,1)
        return I_EXIT;
    IF_INSTRUCT(0,0,0,  // Unstack and print 1 char
                0,1,0,
                0,0,0)
        return I_PRINT;
    IF_INSTRUCT(0,0,0,  // Read and stack 1 char
                0,1,0,
                0,1,0)
        return I_READ;

    // Unknown instruct
    return I_NULL;

#undef IF_INSTRUCT
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
        return -1;  // Error: stack not initialized

    if (prog->stack_pointer - prog->stack >= STACK_SIZE)
        return 1;  // Error: stack full

    *(prog->stack_pointer++) = b;

    return 0;  // Success
}

int unstack(struct program *prog, char *b)
{
    if (prog->stack == NULL)
        return -1;  // Error: stack not initialized

    if (prog->stack_pointer == prog->stack)
        return 1;  // Error: stack empty

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
    case I_NULL:  // unknown
        progError(prog, "Unknown instruction\n");
        break;
    case I_PRINT:
        retval = unstack(prog, &c);

        if (retval == -1)
            progError(prog, "Stack not initialized");  // Not possible
        else if (retval == 1)
            progError(prog, "Stack full (stack size = " STRINGIZE(STACK_SIZE) ")");
        else // retval == 0
            putc(c, stdout);
        break;
    case I_READ:
        c = getchar();

        retval = stack(prog, c);

        if (retval == -1)
            progError(prog, "Stack not initialized");  // Not possible
        else if (retval == 1)
            progError(prog, "Stack empty");

        break;
    default:
        warn(prog, "Not implemented");
        break;  // Not implemented
    }
}

void nextInstruct(struct program *prog)
{
    switch (prog->direction)
    {
    case D_UP:
        if (prog->y <= 0)
            return progError(prog, "Can't go up anymore");
        prog->y--;
        break;
    case D_DOWN:
        if (prog->y >= prog->h - 1)
            return progError(prog, "Can't go down anymore");
        prog->y++;
        break;
    case D_LEFT:
        if (prog->x <= 0)
            return progError(prog, "Can't go left anymore");
        prog->x--;
        break;
    case D_RIGHT:
        if (prog->x >= prog->w - 1)
            return progError(prog, "Can't go right anymore");
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
    while (prog->running)
    {
        execInstruct(prog);
        if (prog->running)
            nextInstruct(prog);
    }

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
        log("Warning: %d bytes remaining on the stack after program end.", prog->stack_pointer - prog->stack);

    if (prog->stack)
        free(prog->stack);

    free(prog);
}

void progError(struct program *prog, char *message)
{
    printf("Error at %d:%d ('%c'): %s\n", prog->x, prog->y, renderInstruct(prog->instructs[prog->y][prog->x]), message);
    prog->running = false;
}
