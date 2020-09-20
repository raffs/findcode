/*
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "findcode.h"

/*
 * Initialize a cblock_t struct and returns the memory pointer
 */
struct cblock_t *
init_cblock()
{
    struct cblock_t *cbk;

    cbk = (struct cblock_t *) malloc(sizeof(struct cblock_t));
    if (cbk == NULL)
        die("OutOfMemory: could not allocate memory for cblock()");

    memset(cbk, 0, sizeof(struct cblock_t));

    cbk->name = NULL;
    cbk->start_line = -1;
    cbk->start_column = -1;
    cbk->end_line = -1;
    cbk->end_column = -1;

    return cbk;
}

/*
 * Helper to print information about a code block with on the format:
 *
 *    <filepath>:<start_line>:<endd_line>: <block_name>
 */
void
print_block(struct cblock_t *cbk, char *filepath, int flags)
{
    fprintf(stdout, "%s:%d:%d: ", filepath, cbk->start_line, cbk->end_line);

    for (char *c = cbk->name; *c != '\0'; ++c) {
        if (*c == '\n')
            continue;

        fprintf(stdout, "%c", *c);
    }

    fprintf(stdout, "\n");
}

/*
 * free/dealloc/release (whatever you want to call it) memory from a
 * initialized cblock_t struct.
 *
 * If the 'cblock' pointers to NULL this function should ignore it
 */
void
free_cblock(struct cblock_t *cbk)
{
    if (cbk != NULL) {
        free(cbk->name);
    }

    free(cbk);
}
