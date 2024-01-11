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

#ifndef INSTRUCTS_H
#define INSTRUCTS_H

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
    I_EQUAL = '=',
    I_SHARP = '#',

    // Operators
    I_PLUS  = '+',
    I_MINUS = '-',
    I_MUL   = '*',
    I_DIV   = '/',
    I_AND   = '&',
    I_VBAR  = '|',  // vertical bar

    // Comparison operators
    I_OPAR  = '(',  // open parenthesis  (used as <)
    I_CPAR  = ')',  // closed parenthesis  (used as >)

    // Every ascii char
    I_SPACE = ' ',
    I_EXCL  = '!',  // exclamation mark
    I_QUOTE = '"',
 // I_SHARP = '#',
    I_DOLLAR= '$',
    I_PERCNT= '%',  // percent
 // I_AND   = '&',
    I_SQUOTE= '\'',  // simple quote
 // I_OPAR  = '(',  // open parenthesis
 // I_CPAR  = ')',  // closed parenthesis
    I_STAR  = '*',
 // I_PLUS  = '+',   // (already defined)
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
 // I_EQUAL = '=',
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
 // I_VBAR  = '|',  // vertical bar
    I_CBRACE= '}',  // closed brace
    I_TILE  = '~',

} __attribute__((__packed__));

enum instruct loadInstruct(char *instruct);

#endif  // INSTRUCTS_H
