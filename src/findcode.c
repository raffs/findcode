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
#include <stdio.h>

#include "findcode.h"

int
main(int argc, char* argv[])
{
    FILE *file;
    char buffer[BUF_SIZE];
    char *codename;
    size_t n_read;
    unsigned int line_counter = 0;
    struct codestack_t *stack;
    struct codenode_t *node;

    if (argc < 2) {
        fprintf(stderr, "Error, please specify a <path>\n");
        fprintf(stderr, "Usage: %s <path> \n", argv[0]);
        return 1;
    }

    file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error when trying to fopen() file\n");
        return 2;
    }

    stack = stack_init();

    /* read until the end of the file */
    while (1) {
        n_read = fread(buffer, sizeof(char), (size_t)BUF_SIZE, file);

        if (n_read == 0)
            break; /* we reach the end of the file */

        line_counter = 1;
        for (int i = 0; i < n_read; i++) {

            /* increment the new line counter */
            if (buffer[i] == '\n')
                line_counter = line_counter + 1;

            /* when founds an open block, we should look behind and defines the words */
            if (buffer[i] == '{') {

                /* parse the previous defined words after finding the block name */
                codename = lookstr_behind(buffer, i, BUF_SIZE);

                /* let's push() the found code block to parsing stack */
                node = codenode_init();
                node->name = codename;
                node->starts = line_counter;

                codestack_push(stack, node);
            }

            /* we found a close block, let's pop the current block and finished it up */
            if (buffer[i] == '}') {
                node = codestack_pop(stack);
                node->ends = line_counter;
                fprintf(stdout, "%s:%d:%d: %s\n", argv[1], node->starts, node->ends, node->name);
            }
        }
    }

    stack_free(stack);
    fclose(file);

    return 0;
}
