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

struct codestack_t *
stack_init()
{
    struct codestack_t *stack;

    stack = (struct codestack_t *) malloc(sizeof(struct codestack_t));
    memset(stack, 0x00, sizeof(struct codenode_t));

    stack->head = NULL;
    stack->next = NULL;
    stack->length = 0;

    return stack;
}

struct codenode_t *
codestack_pop(struct codestack_t *q)
{
    struct codestack_t *temp;
    struct codenode_t *item;

    /* holds temp for freeing later */
    temp = q->head;

    item = q->head->cblock;
    q->head = q->head->next;
    q->length -= 1;

    free(temp);
    return item;
}

void
codestack_push(struct codestack_t *q, struct codenode_t *node)
{
    struct codestack_t *new_item;

    new_item = (struct codestack_t *) malloc(sizeof(struct codestack_t));
    memset(new_item, 0, sizeof(struct codestack_t));

    new_item->cblock = node;
    new_item->next = q->head;
    q->head = new_item;

    q->length += 1;
}

void
stack_free(struct codestack_t *stack)
{
    for (struct codestack_t *q = stack; q->next != NULL; q = q->next)
        free(q->next);

    free(stack);
}
