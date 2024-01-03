#include <stdint.h>
#include <stdbool.h>

// Configuration

#define DEBUG false
#define OUT_IS_ERROR false  // true  : raise an error when cursor get out
                            // false : just exit

#define STACK_SIZE 512

// End of the configuration

#if DEBUG
#define log(f, ...) fprintf(stderr, f "\n", ##__VA_ARGS__)
#else
#define log(...)
#endif

#define warn(prog, f, ...) log("Warning at [%d:%d] ('%c'): " f, prog->x, prog->y, prog->instructs[prog->y][prog->x], ##__VA_ARGS__)

#define STRINGIZE(x) #x

enum instruct {
    I_NULL  = '\0',
    I_PASS  = ' ',
    I_UP    = '^',
    I_DOWN  = 'v',
    I_LEFT  = '<',
    I_RIGHT = '>',
    I_EXIT  = 'X',
    I_PRINT = '.',
    I_PRTALL= ':',  // print all
    I_READ  = ',',
    I_ASCII = '"',
    I_ESCAPE= '\\',
    I_ZERO  = '0',

    // Operators    TODO
    I_PLUS  = '+',
    I_MINUS = '-',
    I_MUL   = '*',
    I_DIV   = '/',

    // Every ascii char
    I_SPACE = ' ',
    I_EXCL  = '!',  // exclamation mark
    I_QUOTE = '"',
    I_SHARP = '#',
    I_DOLLAR= '$',
    I_PERCNT= '%',  // percent
    I_AND   = '&',
    I_SQUOTE= '\'',  // simple quote
    I_OPAR  = '(',  // open parenthesis
    I_CPAR  = ')',  // closed parenthesis
    I_STAR  = '*',
  // I_PLUS = '+',  // (already defined)
    I_COMMA = ',',
    I_DASH  = '-',
    I_DOT   = '.',
    I_SLASH = '/',
    I_0     = '0',
    I_1     = '1',
    I_2     = '2',
    I_3     = '3',
    I_4     = '4',
    I_5     = '5',
    I_6     = '6',
    I_7     = '7',
    I_8     = '8',
    I_9     = '9',
    I_COLON = ':',
    I_SMCLN = ';',  // Semi-colon
    I_LOWER = '<',
    I_EQUAL = '=',
    I_HIGHER= '>',
    I_QMARK = '?',  // Question mark
    I_AT    = '@',
    I_A     = 'A',
    I_B     = 'B',
    I_C     = 'C',
    I_D     = 'D',
    I_E     = 'E',
    I_F     = 'F',
    I_G     = 'G',
    I_H     = 'H',
    I_I     = 'I',
    I_J     = 'J',
    I_K     = 'K',
    I_L     = 'L',
    I_M     = 'M',
    I_N     = 'N',
    I_O     = 'O',
    I_P     = 'P',
    I_Q     = 'Q',
    I_R     = 'R',
    I_S     = 'S',
    I_T     = 'T',
    I_U     = 'U',
    I_V     = 'V',
    I_W     = 'W',
    I_X     = 'X',
    I_Y     = 'Y',
    I_Z     = 'Z',
    I_OBRCKT= '[',  // open bracket
    I_BKSLSH= '\\',  // backslash
    I_CBRCKT= ']',  // closed bracket
    I_CARET = '^',
    I_UDRSCR= '_',  // underscore
    I_BCKTCK= '`',  // backtrick
    I_OBRACE= '{',  // open brace
    I_VBAR  = '|',  // vertical bar
    I_CBRACE= '}',  // closed brace

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
    bool ascii_mode;
    bool escape;
    bool uppercase;
    char *stack;
    char *stack_pointer;
};

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
