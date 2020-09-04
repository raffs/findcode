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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "findcode.h"

/*
 * foreach macros provides an easy way of walking through all
 * the item for a cqueue_t structure. Usually used on for debug
 * process as right now no feature of the program requires a cqueue_t
 * full items walk
 */
#define foreach_queue(item, cq) \
    for (struct cqueue_t *item = cq->head; item != NULL; item = item->next)

/*
 * Initialize a cqueue_t struct and returns the memory pointer
 */
struct cqueue_t *
init_cqueue()
{
    struct cqueue_t *cq;

    cq = (struct cqueue_t *) malloc(sizeof(struct cqueue_t));
    if (cq == NULL)
        die("OutOfMemory: could not allocate memory for init_cqueue()");

    memset(cq, 0, sizeof(struct cqueue_t));

    cq->cblock = NULL;
    cq->next = NULL;

    return cq;
}

/*
 * Pushes a initialized 'cblock_t' struct into the current
 * queue head.
 *
 * If the 'cq' or 'cbk' points to NULL we should return a
 * error code -1 or -2 respectively.
 */
int
cqueue_push(struct cqueue_t *cq, struct cblock_t *cbk)
{
    struct cqueue_t *temp_cq;

    if (cbk == NULL)
        return -2;

    temp_cq = init_cqueue();
    if (temp_cq == NULL)
        return -1;

    temp_cq->cblock = cbk;
    temp_cq->next = cq->head;

    cq->head = temp_cq;

    return 0;
}

/*
 * Pops a 'cblock' from the given 'cq' queue.
 */
struct cblock_t *
cqueue_pop(struct cqueue_t *cq)
{
    struct cblock_t *cbk;
    struct cqueue_t *temp_cq;

    if (cq->head == NULL)
        return NULL;

    cbk = cq->head->cblock;
    temp_cq = cq->head;

    cq->head = cq->head->next;

    free(temp_cq);
    return cbk;
}

/*
 * free/dealloc/release (whatever you want to call it) memory from a
 * initialized cqueue_t struct.
 *
 * If the 'cq' pointers to NULL this function should ignore it
 */
void
free_cqueue(struct cqueue_t *cq)
{
    if (cq != NULL) {
        foreach_queue(item, cq) {
            if (item != NULL)
                free_cblock(item->cblock);
        }
    }

    free(cq);
}
