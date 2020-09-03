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
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "findcode.h"

#define foreach_queue(item, cq) \
    for (struct cqueue_t *item = cq->head; item != NULL; item = item->next)

struct cblock_t *
init_cblock()
{
    struct cblock_t *cbk;

    cbk = (struct cblock_t *) malloc(sizeof(struct cblock_t));
    if (cbk == NULL) {
        return NULL;
    }

    cbk->name = NULL;
    cbk->start_at = -1;
    cbk->end_at = -1;

    return cbk;
}

struct cqueue_t *
init_cqueue()
{
    struct cqueue_t *cq;

    cq = (struct cqueue_t *) malloc(sizeof(struct cqueue_t));
    if (cq == NULL) {
        return NULL;
    }

    memset(cq, 0, sizeof(struct cqueue_t));

    cq->cblock = NULL;
    cq->next = NULL;

    return cq;
}

void
free_cblock(struct cblock_t *cbk)
{
    if (cbk != NULL) {
        free(cbk->name);
    }
    free(cbk);
}

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

int
cqueue_push(struct cqueue_t *cq, struct cblock_t *cbk)
{
    struct cqueue_t *temp_cq;

    temp_cq = init_cqueue();
    if (temp_cq == NULL)
        return -1;

    temp_cq->cblock = cbk;
    temp_cq->next = cq->head;
    cq->head = temp_cq;

    //DEBUG
    //fprintf(stdout, "QUEUE(PUSH): ");
    //foreach_queue(item, cq) {
    //    fprintf(stdout, "[%p/%p] %p |", item, &item->cblock, &item->cblock->name);
    //} 
    //fprintf(stdout, "\n");

    return 0;
}

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

    //DEBUG
    //fprintf(stdout, "QUEUE(POP): ");
    //foreach_queue(item, cq) {
    //    fprintf(stdout, "[%p/%p] %p |", item, &item->cblock, &item->cblock->name);
    //} 
    //fprintf(stdout, "\n");

    //fprintf(stdout, "POPED: %p\n", &cbk);
    return cbk;
}

int
process_file(char* filepath, char* pattern, size_t filesize)
{
    FILE *fp;
    int n_read;
    int cur_line = 1;
    char *buffer;
    struct cblock_t *cblock;
    struct cqueue_t *cqueue;

    fp = fopen(filepath, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error when open file: %s\n", filepath);
        return 1;
    }

    cqueue = init_cqueue();

    buffer = (char *) malloc(sizeof(char) * (filesize + 1));
    n_read = fread(buffer, sizeof(char), filesize, fp);

    if (n_read == 0 || n_read != filesize)
        goto exit;

    /* process each line */
    for (int i = 0; i < n_read; i++) {

        if (buffer[i] == '\n') {
            cur_line += 1;
        }

        else if (buffer[i] == '{') {
            cblock = init_cblock();

            //cblock->name = parse_getprev_word(buffer, cur_line); 
            cblock->start_at = cur_line;

            cqueue_push(cqueue, cblock);
        }

        else if (buffer[i] == '}') {
            cblock = cqueue_pop(cqueue);

            if (cblock != NULL) {
                cblock->end_at = cur_line;
                fprintf(stdout, "blocks [#%p]%s:%d:%d\n", 
                    &cblock->name, cblock->name, cblock->start_at, cblock->end_at);

                free_cblock(cblock);
            }
        }
    }

exit:
    free_cqueue(cqueue);
    fclose(fp);
    free(buffer);

    return 0;
}

int
main(const int argc, char* argv[])
{
    char* pattern;
    struct stat file_sb;

    if (argc < 3) {
        fprintf(stderr, "Usage pattern: <pattern> <path>\n");
        return 1;
    }

    pattern = argv[1];
    for (int i = 2; i < argc; i++) { /* for each file */

        if (lstat(argv[i], &file_sb) == -1) {
            fprintf(stderr, "no able to stat() file %s\n", argv[i]);
            continue;
        }

        if (!(file_sb.st_mode & S_IFREG)) {
            fprintf(stderr, "file '%s' not a regular file\n", argv[i]);
            continue;
        }

        fprintf(stdout, "File %ld\n", file_sb.st_size);
        process_file(argv[i], pattern, (size_t)file_sb.st_size);
    }

    return 0;
}
