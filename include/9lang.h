#include <stdint.h>
#include <stdbool.h>

enum instruct {
    I_NULL  = '?',
    I_PASS  = ' ',
    I_UP    = '^',
    I_DOWN  = 'v',
    I_LEFT  = '<',
    I_RIGHT = '>',
    I_EXIT  = 'X',
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
};

// enum instruct loadInstruct(char *instruct);
// void loadInstructs(struct program *prog);
struct program *loadProgram(char *path);
// void execInstruct(struct program *prog);
// void nextInstruct(struct program *prog);
int runProgram(struct program *prog);
void freeProgram(struct program *prog);

void progError(struct program *prog, char *message);
