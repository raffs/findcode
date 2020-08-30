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
#ifndef __FINDCODE_H
#define __FINDCODE_H

#define BUF_SIZE 2048

/**
 * codenode_t holds information about a code block definition,
 * such as code block name, lines where starts and ends.
 */
struct codenode_t
{
    /* the name of the code block */
    char *name;

    /* the line where the block starts and ends */
    int starts;
    int ends;
};

/**
 * codestack_t is FILO data structure used to store the open blocks
 * being parsed.
 *
 * By using a FILO we can avoid any recusrive call, and process the block
 * at one cycle of reading through the file. When a new code block is open
 * we store the block on the stack, and any other inner block is store at
 * the top of the stack, once the blocks is closed we start to pop the code
 * from the stack and finishes the code writing them into the main tree.
 *
 * Technically the stack is linked list with a head pointer to knows what
 * objects are on the top of the list.
 */
struct codestack_t
{
    /* holds the length of the stack */
    int length;

    /* holds information about a code block */
    struct codenode_t *cblock;

    /* pointer to the stack head and next object on the stack */
    struct codestack_t *head;
    struct codestack_t *next;
};

struct codenode_t *codenode_init();
void codenode_free();

struct codestack_t *stack_init();
struct codenode_t *codestack_pop(struct codestack_t *q);
void codestack_push(struct codestack_t *q, struct codenode_t *n);
void stack_free(struct codestack_t *q);

char *substring(char *buffer, int start, int end);
char *lookstr_behind(char *buffer, int curidx, size_t maxbuffer);
void free_string(char* str);

#endif
