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

/**
 * subscripts returns a NULL-terminated string given the
 * a buffer, and start and ending position.
 *
 * TODO: Validate the start/end boundaries to avoid
 * creation a substring that tries to access memory greater
 * or lower then the buffer size.
 */
char *
substring(char *buffer, int start, int end)
{
    char *newstr;
    size_t len;

    len = (end - start) + 1;
    newstr = (char *) malloc(sizeof(char) * (len));
    memset(newstr, 0, sizeof(len));

    for (int i = 0, bi = start; bi < end; ++i, bi++)
        newstr[i] = buffer[bi];

    newstr[end] = '\0';
    return newstr;
}

/**
 * freeing the memory allocated by a string. this is just wrapper
 * but the plan is to have string structure with more information
 * and this was safely handle the "de-allocation" of memory
 */
void
free_string(char* str)
{
    if (str != NULL)
        free(str);
}
