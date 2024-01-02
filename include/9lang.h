#include <stdint.h>
#include <stdbool.h>

#define DEBUG false

#if DEBUG
#define log(f, ...) printf(f "\n", ##__VA_ARGS__)
#else
#define log(...)
#endif

#define warn(prog, f, ...) printf("Warning at [%d:%d] ('%c'): " f "\n", prog->x, prog->y, prog->instructs[prog->y][prog->x], ##__VA_ARGS__)

#define STRINGIZE(x) #x

enum instruct {
    I_NULL  = '?',
    I_PASS  = ' ',
    I_UP    = '^',
    I_DOWN  = 'v',
    I_LEFT  = '<',
    I_RIGHT = '>',
    I_EXIT  = 'X',
    I_PRINT = '.',
    I_READ  = ',',
} __attribute__((__packed__));

enum direction {
    D_UP    = '^',
    D_DOWN  = 'v',
    D_LEFT  = '<',
    D_RIGHT = '>',
} __attribute__((__packed__));

#define DEFAULT_DIR D_RIGHT

struct program {
    uint16_t w, h;
    uint16_t x, y;
    enum direction direction;
    enum instruct **instructs;
    bool running;
    char *stack;
    char *stack_pointer;
};

#define STACK_SIZE 512

// enum instruct loadInstruct(char *instruct);
// void loadInstructs(struct program *prog);
struct program *loadProgram(char *path);
// int stack(struct program *prog, char b);
// int unstack(struct program *prog, char *b);
// void execInstruct(struct program *prog);
// void nextInstruct(struct program *prog);
// void initStack(struct program *prog);
int runProgram(struct program *prog);
void freeProgram(struct program *prog);

void progError(struct program *prog, char *message);
