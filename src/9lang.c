#include "9lang.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>


char renderInstruct(enum instruct instruct)
{
    switch (instruct)
    {
    case I_PASS:
        return ' ';
    case I_UP:
        return '^';
    case I_DOWN:
        return 'v';
    case I_LEFT:
        return '<';
    case I_RIGHT:
        return '>';
    case I_EXIT:
        return 'X';
    case I_NULL:
        return '?';
    default:
        return 0;  // Not implemented
    }
}

enum instruct loadInstruct(char *instruct)
{
#define IF_INSTRUCT(...) if (memcmp(instruct, (char []) {__VA_ARGS__}, 3*3) == 0)

    IF_INSTRUCT(0,0,0,  // PASS
                0,0,0,
                0,0,0)
        return I_PASS;
    IF_INSTRUCT(0,1,0,  // UP
                1,1,1,
                0,0,0)
        return I_UP;
    IF_INSTRUCT(0,0,0,  // DOWN
                1,1,1,
                0,1,0)
        return I_DOWN;
    IF_INSTRUCT(0,1,0,  // LEFT
                1,1,0,
                0,1,0)
        return I_LEFT;
    IF_INSTRUCT(0,1,0,  // RIGHT
                0,1,1,
                0,1,0)
        return I_RIGHT;
    IF_INSTRUCT(1,0,1,  // EXIT
                0,1,0,
                1,0,1)
        return I_EXIT;

    // Unknown instruct
    return I_NULL;

#undef IF_INSTRUCT
}

void loadInstructs(struct program *prog)
{
    char instruct[3*3];

    prog->instructs = malloc(IH(prog) * sizeof (char *));

    for (prog->y = 0; prog->y < IH(prog); prog->y++)
        prog->instructs[prog->y] = malloc(IW(prog) * sizeof (char));

    for (prog->x = 0; prog->x < prog->w + 3; prog->x+=3)
    {
        for (prog->y = 0; prog->y < prog->h + 3; prog->y+=3)
        {
            memcpy(instruct + 0 * 3, &prog->data[prog->w * (prog->y + 0) + prog->x], 3);
            memcpy(instruct + 1 * 3, &prog->data[prog->w * (prog->y + 1) + prog->x], 3);
            memcpy(instruct + 2 * 3, &prog->data[prog->w * (prog->y + 2) + prog->x], 3);            
            prog->instructs[prog->y / 3][prog->x / 3] = loadInstruct(instruct);
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

    if (f == NULL)
        return NULL;  // Error opening file

    prog = malloc(sizeof (struct program));
    memset(prog, 0, sizeof (struct program));

    prog->direction = DEFAULT_DIR;

    do {
        c = fgetc(f);

        if (c == '\n' || c == '\r' || c == EOF)
        {
            prog->h++;
            if (curLineLength > prog->w)
                prog->w = curLineLength;
            curLineLength = 0;
        }
        else
        {
            curLineLength++;
        }
    } while (c != EOF);

    if (prog->w * prog->h == 0)
        return prog;  // Nothing to load

    prog->data = malloc(prog->w * prog->h * sizeof (char));

    rewind(f);

    prog->x = prog->y = 0;

    do {
        c = fgetc(f);

        if (c == '\n' || c == '\r' || c == EOF)
        {
            memset(&prog->data[prog->w * prog->y + prog->x], 0, prog->w - prog->x);
            prog->y++;
            prog->x = 0;
        }
        else
        {
            prog->data[prog->w * prog->y + prog->x] = (c == ' ' | c == '\t') ? 0 : 1;
            prog->x++;
        }
    } while (c != EOF);

    fclose(f);

    prog->x = prog->y = 0;

    loadInstructs(prog);

    return prog;
}

int execInstruct(struct program *prog)
{
    enum instruct instruct = prog->instructs[prog->y][prog->x];

    // printf("Executing %c at %d:%d\n", renderInstruct(instruct), prog->x, prog->y);

    switch (instruct)
    {
    case I_PASS:
        break;
    case I_UP:
    case I_DOWN:
    case I_LEFT:
    case I_RIGHT:
        prog->direction = instruct - I_UP;
        break;
    case I_EXIT:
        prog->running = false;
        break;
    case I_NULL:  // unknown
        progError(prog, "Unknown instruction\n");
        break;
    default:
        break;  // Not implemented
    }
}

void nextInstruct(struct program *prog)
{
    switch (prog->direction)
    {
    case D_UP:
        if (prog->y == 0)
            return progError(prog, "Can't go up anymore");
        prog->y--;
        break;
    case D_DOWN:
        if (prog->y >= IH(prog))
            return progError(prog, "Can't go down anymore");
        prog->y++;
        break;
    case D_LEFT:
        if (prog->x == 0)
            return progError(prog, "Can't go left anymore");
        prog->x--;
        break;
    case D_RIGHT:
        if (prog->x >= IW(prog))
            return progError(prog, "Can't go right anymore");
        prog->x++;
        break;
    }
}

int runProgram(struct program *prog)
{
    if (prog->instructs == NULL)  // If the program is empty
        return 0;  // Nothing to do

    for (uint16_t y = 0; y < IH(prog); y++)
    {
        for (uint16_t x = 0; x < IW(prog); x++)
            printf("[%c]", renderInstruct(prog->instructs[y][x]));
        printf("\n");
    }

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
    if (prog->data != NULL)
        free(prog->data);

    if (prog->instructs != NULL)
        free(prog->instructs);

    free(prog);
}

void progError(struct program *prog, char *message)
{
    printf("Error at %d:%d ('%c'): %s\n", prog->x, prog->y, renderInstruct(prog->instructs[prog->y][prog->x]), message);
    prog->running = false;
}
