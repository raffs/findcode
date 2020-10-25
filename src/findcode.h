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
#ifndef __FINDCODE_H
#define __FINDCODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

/*
 * cblock_t structure holds information about a code block such as
 * code block name, statistics about on which line and column the code
 * started and ended
 */
struct cblock_t
{
    /* name of the code block including all spaces, parameters */
    char *name;

    /* keeps track on which line and column the codeblock started */
    int start_line;
    int start_column;

    /* keeps track on which line and column the code ended */
    int end_line;
    int end_column;
};

/*
 * cqueue_t is LIFO data structure that is used to store
 * information about the 'cblock_t' that are being parsed
 * from the file_parser()
 *
 * This provides the ability to store nested block code by
 * only walking through the buffer once and pushing/popping
 * the code blocks information as they are open and closed
 */
struct cqueue_t
{
    struct cblock_t *cblock;

    /* should always points to HEAD of the queue */
    struct cqueue_t *head;

    /* points to the next object on the queue */
    struct cqueue_t *next;
};

/*
 * die() is used to suddenly exit the program, when critical
 * errors are found like when malloc() returns null (aka: OOM)
 */
static void inline die(char *errmsg)
{
    fprintf(stderr, "ERROR: %s\n", errmsg);
    exit(128);
}

/*
 * prototype for string.c helpers functions
 */
char *substr_lastline(char *buffer, int start, int end);

/*
 * prototype for managing the cqueue (codequeu) struct
 */
struct cqueue_t * init_cqueue();
struct cblock_t * cqueue_pop(struct cqueue_t *cq);
int cqueue_push(struct cqueue_t *cq, struct cblock_t *cbk);
void free_cqueue(struct cqueue_t *cq);

/*
 * prototypes for the cblock (codeblock) managed functions
 */
struct cblock_t * init_cblock();
void print_block(struct cblock_t *cbk, char *filepath, int flags);
void free_cblock(struct cblock_t *cbk);

/*
 * main function that will process the buffer from the open file
 * and parse the characers looking for the code block and storing
 * the statistics
 */
int process_file(char* filepath, size_t filesize);

#endif
