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

#define BUF_SIZE 1024

struct cblock_t
{
    char *name;
    int start_at;
    int end_at;
};

struct cqueue_t
{
    struct cblock_t *cblock;

    /* points to the next object on the queue */
    struct cqueue_t *head;
    struct cqueue_t *next;
};

struct ctree_t
{
    /* holds a pointer to the child block */
    struct ctree_t *child;

    /* points to the next block same-level block */
    struct ctree_t *next;
};

struct cqueue_t * init_cqueue();
struct cblock_t * cqueue_pop(struct cqueue_t *cq);
int cqueue_push(struct cqueue_t *cq, struct cblock_t *cbk);
void free_cqueue(struct cqueue_t *cq);
struct cblock_t * init_cblock();
void free_cblock(struct cblock_t *cbk);

int process_file(char* filepath, char* pattern, size_t filesize);

#endif
