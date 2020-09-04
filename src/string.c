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
 * Returns a string buffer pointing to a substring of the given
 * 'buffer' variable. This is helper and initialy used to extract
 * the name of the codeblock
 */
char *
substring(char *buffer, int start, int end)
{
    char *str;
    size_t size;

    size = (end - start) + 1;

    str = (char *) malloc(sizeof(char) * size);
    if (str == NULL)
        die("OutOfMemory when allocating substring()");

    memset(str, 0, size);

    for (int i = 0, j = start; j <= end; ++i, ++j)
        str[i] = buffer[j];

    str[(size - 1)] = '\0';
    return str;
}
