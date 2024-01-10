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
#include <stdint.h>

struct {
    enum instruct ch;
    char shape[3*3];
} characters[] = {
    {' ', {0,0,0,  // Pass / space
           0,0,0,
           0,0,0}},
    {'^', {0,1,0,  // Up / caret
           1,1,1,
           0,0,0}},
    {'v', {0,0,0,  // Down / v
           1,1,1,
           0,1,0}},
    {'<', {0,1,0,  // Left / lower
           1,1,0,
           0,1,0}},
    {'>', {0,1,0,  // Right / higher
           0,1,1,
           0,1,0}},
    {'X', {1,0,1,  // Exit / X
           0,1,0,
           1,0,1}},
    {'.', {0,0,0,  // Print / dot
           0,1,0,  // Unstack and print 1 char
           0,0,0}},
    {',', {0,0,0,  // Read / comma
           0,1,0,  // Read and stack 1 char
           0,1,0}},
    {'"', {1,0,1,  // Enable/disable ascii mode: each next character will be read as ascii
           0,0,0,  //   character and added to the stack
           0,0,0}},
    {'\\',{1,0,0,  // Escape (only available in ascii mode)
           0,1,0,
           0,0,1}},
    {':', {0,1,0,  // Print all : Print until NUL character or stack empty
           0,0,0,
           0,1,0}},
    {'0', {0,1,0,  // Zero/null : add a NUL byte to the stack
           1,0,1,
           0,1,0}},

    // Ascii chars

 // {' ', {...}},
    {'!', {0,0,1,
           0,0,1,
           0,0,1}},
 // {'"', {...}},
    {'#', {1,1,1,
           1,1,1,
           1,1,1}},
    {'$', {0,1,1,
           1,1,1,
           1,1,0}},
    {'%', {1,0,0,
           1,1,1,
           0,0,1}},
    {'&', {0,1,1,
           1,1,0,
           1,1,1}},
    {'\'',{0,1,0,
           0,0,0,
           0,0,0}},
    {'(', {0,1,0,
           1,0,0,
           0,1,0}},
    {')', {0,1,0,
           0,0,1,
           0,1,0}},
    {'*', {1,1,1,
           1,1,1,
           0,0,0}},
    {'+', {0,1,0,
           1,1,1,
           0,1,0}},
 // {',', {...}},
    {'-', {0,0,0,
           1,1,1,
           0,0,0}},
 // {'.', {...}},
    {'/', {0,0,1,
           0,1,0,
           1,0,0}},
 // {'0', {...}},
    {'1', {1,0,0,
           1,0,0,
           1,0,0}},
    {'2', {0,1,0,
           0,0,1,
           1,1,1}},
    {'3', {1,1,1,
           0,1,1,
           1,1,1}},
    {'4', {1,0,1,
           1,1,1,
           0,0,1}},
    {'5', {1,1,1,
           1,1,0,
           1,1,0}},
    {'6', {0,1,1,
           1,1,1,
           0,1,0}},
    {'7', {1,1,1,
           0,0,1,
           0,0,1}},
    {'8', {1,0,1,
           0,0,0,
           1,0,1}},
    {'9', {0,1,0,
           1,1,1,
           1,1,0}},
    {':', {0,1,0,
           0,0,0,
           0,1,0}},
    {';', {0,1,0,
           0,0,0,
           1,1,0}},
 // {'<', {...}},
    {'=', {1,1,1,
           0,0,0,
           1,1,1}},
 // {'>', {...}},
    {'?', {1,1,1,
           0,0,1,
           0,1,0}},
    {'@', {0,1,0,
           1,1,1,
           0,1,1}},
    {'A', {0,1,0,
           1,1,1,
           1,0,1}},
    {'B', {1,0,0,
           1,1,0,
           1,1,0}},
    {'C', {1,1,1,
           1,0,0,
           1,1,1}},
    {'D', {1,1,0,
           1,0,1,
           1,1,0}},
    {'E', {1,1,1,
           1,1,0,
           1,1,1}},
    {'F', {1,1,1,
           1,1,0,
           1,0,0}},
    {'G', {0,1,0,
           1,0,1,
           0,1,1}},
    {'H', {1,0,1,
           1,1,1,
           1,0,1}},
    {'I', {1,1,1,
           0,1,0,
           1,1,1}},
    {'J', {1,1,1,
           0,1,0,
           1,1,0}},
    {'K', {1,0,1,
           1,1,0,
           1,0,1}},
    {'L', {1,0,0,
           1,0,0,
           1,1,1}},
    {'M', {1,1,1,
           1,1,1,
           1,0,1}},
    {'N', {0,0,0,
           1,1,0,
           1,0,1}},
    {'O', {1,1,1,
           1,0,1,
           1,1,1}},
    {'P', {1,1,0,
           1,1,0,
           1,0,0}},
    {'Q', {1,1,1,
           1,0,1,
           1,1,0}},
    {'R', {1,1,0,
           1,1,0,
           1,0,1}},
    {'S', {0,1,1,
           0,1,0,
           1,1,0}},
    {'T', {1,1,1,
           0,1,0,
           0,1,0}},
    {'U', {1,0,1,
           1,0,1,
           1,1,1}},
    {'V', {0,0,0,
           1,0,1,
           0,1,0}},
    {'W', {0,0,0,
           1,1,1,
           1,1,1}},
 // {'X', {...}},
    {'Y', {1,0,1,
           0,1,0,
           0,1,0}},
    {'Z', {1,1,0,
           0,1,0,
           0,1,1}},
    {'[', {1,1,0,
           1,0,0,
           1,1,0}},
 // {'\\',{...}},
    {']', {0,1,1,
           0,0,1,
           0,1,1}},
 // {'^', {...}},
    {'_', {0,0,0,
           0,0,0,
           1,1,1}},
    {'`', {1,0,0,
           0,1,0,
           0,0,0}},
    {'{', {0,1,1,
           1,1,0,
           0,1,1}},
    {'|', {0,1,0,
           0,1,0,
           0,1,0}},
    {'}', {1,1,0,
           0,1,1,
           1,1,0}},
    {'~', {0,0,0,
           1,0,1,
           0,0,0}},
};

enum instruct loadInstruct(char *instruct)
{
    for (size_t i = 0; i < sizeof (characters) / sizeof (*characters); i++)
        if (memcmp(instruct, characters[i].shape, 3*3) == 0)
            return characters[i].ch;

    return I_NULL;
}
