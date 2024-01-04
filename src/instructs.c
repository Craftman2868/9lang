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

#include "instructs.h"

#include <string.h>

enum instruct loadInstruct(char *instruct)
{
    // Define_Instruct
#define D_I(...) if (memcmp(instruct, (char []) {__VA_ARGS__}, 3*3) == 0) return

    D_I(0,0,0,  // Pass / space
        0,0,0,
        0,0,0) I_PASS;
    D_I(0,1,0,  // Up / caret
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
        0,0,1,
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
 // D_I(...) 'X';
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
