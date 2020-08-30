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

struct codenode_t *
codenode_init()
{
    struct codenode_t *node;

    node = (struct codenode_t *) malloc(sizeof(struct codenode_t));
    memset(node, 0x00, sizeof(struct codenode_t));

    node->name = NULL;
    node->starts = 0;
    node->ends = 0;

    return node;
}

void
codenode_free(struct codenode_t *n)
{
    if (n != NULL)
        free(n);
}
