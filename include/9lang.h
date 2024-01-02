#include <stdint.h>
#include <stdbool.h>

#define IW(prog) (prog->w / 3 + 1)
#define IH(prog) (prog->h / 3 + 1)

enum instruct {
    I_NULL,
    I_PASS,
    I_UP,
    I_DOWN,
    I_LEFT,
    I_RIGHT,
    I_EXIT,
} __attribute__((__packed__));

enum direction {
    D_UP,
    D_DOWN,
    D_LEFT,
    D_RIGHT,
};

#define DEFAULT_DIR D_RIGHT

struct program {
    uint16_t w, h;
    char *data;
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
