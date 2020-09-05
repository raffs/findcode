/**
 * findcode helps you locate lines of code inside your code base
 * Copyright (C) 2020  Rafael Oliveira Silva <rafaeloliveira.cs@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include <string.h>

#include "findcode.h"

/*
 * Extra the last line from the buffer, given a specific
 * range of chars. The range are stored as simply start
 * and end indexes (aka: buffer[i]).
 *
 * This function is used to look behind the last line when
 * a new block code is open "{".
 */
char *
substr_lastline(char *buffer, int start, int end)
{
    char *str;
    size_t size;
    unsigned int idx;

    /*
     * transverse the buffer to find the previous newline
     * definition in order to identity where the previous
     * lines start so we can extract and return
     */
    for (idx = end; idx > 0 && buffer[idx] != '\n'; --idx);

    size = (end - idx) + 1;
    str = (char *) malloc(sizeof(char) * size);

    if (str == NULL)
        die("OutOfMemory when allocating substring()");

    memset(str, 0, size);

    for (int i = 0, j = idx; j <= end; ++i, ++j)
        str[i] = buffer[j];

    str[(size - 1)] = '\0';
    return str;
}
